#include "../../includes/tester.hpp"

static void launch_tests(struct tester_client_s tc, char **test_files)
{
	std::string	msg;

	// Clean buffers:
	std::string		test_msg;

	for (int i = 1; test_files[i] != NULL; i++) {
		std::string		file = test_files[i];
		std::ifstream	infile(file.c_str());
		if (infile.is_open()) {
			while (std::getline(infile, test_msg)) {
				if (!test_msg.empty() && test_msg.at(0) != '#') {
					if (*(test_msg.end() - 1) == '\n')
						test_msg.erase(test_msg.size() - 1); //remove trailing \n
					replace_tokens(test_msg, tc.tci);
					std::cout << "======== TESTER: Sending command [" << test_msg << "] =========\n"; 
					// test_msg += IRC_MSG_SEPARATOR;
					send_command(tc.socket_fd, test_msg);
				}
			}
		} else {
			std::cout << "Invalid test file " << file << std::endl;
		}
	}
}

static void replace_tokens(std::string &str, struct tester_client_info_s &tci)
{
	size_t ret;
	while ((ret = str.find("$PASSWORD")) != str.npos)
		str.replace(ret, 9, tci.password);
	while ((ret = str.find("$NICK")) != str.npos)
		str.replace(ret, 5, tci.nick);
	while ((ret = str.find("$USERNAME")) != str.npos)
		str.replace(ret, 9, tci.username);
	while ((ret = str.find("$FULLNAME")) != str.npos)
		str.replace(ret, 9, tci.fullname);
}

static void	send_command(int socket_fd, std::string command)
{
	char	server_response[BUFFER_LEN];
	memset(server_response, '\0', BUFFER_LEN);
	std::cout << "sending " << command << " on socket " << socket_fd << std::endl;
	command += IRC_MSG_SEPARATOR;
    // Send the message to server:
	if (write(socket_fd, command.c_str(), command.length()) == -1) {
        std::cout << "Unable to send message\n";
        return;
	}
	
	usleep(1000);
    // Receive the server's response:
	int read_ret = 0;
	while ((read_ret = read(socket_fd, server_response, BUFFER_LEN)) >= 0) {
        ;
	}
	std::cout << "Server's response: " << server_response << std::endl;
}

static bool	init_tester_config(struct tester_client_info_s &tci)
{
	std::string		rl_str;
	std::ifstream	infile(CONFIG_FILE_PATH);
	unsigned int	init_count = 0;

	while (std::getline(infile, rl_str)) {
		if (!rl_str.empty() && rl_str.at(0) != '#') {
			if (init_count == 0)
				tci.port = std::atoi(rl_str.c_str());
			else if (init_count == 1)
				tci.password = rl_str;
			else if (init_count == 2)
				tci.nick = rl_str;
			else if (init_count == 3)
				tci.username = rl_str;
			else if (init_count == 4)
				tci.fullname = rl_str;
			init_count++;
		}
	}
	return (init_count == 5);
}

struct tester_client_s	init_client(int i, struct tester_client_info_s tci)
{
	struct tester_client_s	tc;

	// init client info
	tc.tci = tci;
	// modify names to be test + index for stress test
	if (i >= 0) {
		std::string	client_name = "test";
		while (i > 0) {
			client_name += (i % 10) + 48;
			i /= 10;
		}
		tc.tci.nick = client_name;
		tc.tci.fullname = "FN"+client_name;
		tc.tci.username = "U"+client_name;
	}

	// init client socket
	try
	{
		// Create socket:
		tc.socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

		if (tc.socket_fd < 0)
		{
			std::cout << "Unable to create socket\n";
			return tc;
		}

		std::cout << "Socket created successfully\n";

		// Set port and IP the same as server-side:
		tc.server_addr.sin_family		= AF_INET;
		tc.server_addr.sin_port		= htons(tc.tci.port);
		tc.server_addr.sin_addr.s_addr	= inet_addr("127.0.0.1");

		// Send connection request to server:
		while (connect(tc.socket_fd, (struct sockaddr *)&tc.server_addr, sizeof(tc.server_addr)) < 0)
		{
			std::cout << "Unable to connect\n";
		}
		std::cout << "Connected with server successfully\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (tc);
}

static void	login_client(struct tester_client_s tc)
{
	std::cout << "login " << tc.tci.nick << std::endl;
	send_command(tc.socket_fd, "PASS " + tc.tci.password);
	send_command(tc.socket_fd, "NICK " + tc.tci.nick);
	send_command(tc.socket_fd, "USER " + tc.tci.nick + " "+tc.tci.username + " 0 :" + tc.tci.fullname);
	std::cout << "done\n";
}

int main(int ac, char **av)
{
	if (ac < 2 || std::strncmp(av[1], "-help", 6) == 0) {
		std::cout << "USAGE: ./tester {-help} ...[test_files] OR ./tester -st\n"
			<< "Config file is ./tester_files/config.test\n";
		return (0);
	}

	struct tester_client_info_s	tci;

	if (init_tester_config(tci) == false) {
		std::cout << "Config error\n";
		return (EXIT_FAILURE);
	}

	// Stress test mode
	if (std::strncmp(av[1], "-st", 3) == false) {
		client_list_t	client_list;
		// create clients
		for (size_t i = 0; i < STRESSTEST_CLIENTS; i++)
			client_list.push_back(init_client(i, tci));
		// make them login
		for (client_list_t::iterator it = client_list.begin(); it != client_list.end(); it++)
			login_client(*it);
		// make them join #test channel
		for (client_list_t::iterator it = client_list.begin(); it != client_list.end(); it++)
			send_command(it->socket_fd, "JOIN #test");
		// make each spam 10 times
		for (client_list_t::iterator it = client_list.begin(); it != client_list.end(); it++)
			for (size_t i = 0; i < STRESSTEST_MSG_N; i++) {
				send_command(it->socket_fd, "PRIVMSG #test :spam!!");
			}
		usleep(10000);
		for (client_list_t::iterator it = client_list.begin(); it != client_list.end(); it++) {
			close(it->socket_fd);
		}
	}
	// Manual tests mode
	else {
		struct tester_client_s	tc = init_client(-1, tci);
		launch_tests(tc, av);
		close(tc.socket_fd);
	}
}

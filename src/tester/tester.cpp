#include "common.hpp"
#include "Scheduler.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include "functionMap.hpp"
#include <arpa/inet.h>
#include <fstream>

#define BUFFER_LEN			1024
#define CONFIG_FILE_PATH	"./tester_files/config.test"

static void	test_command(int socket_fd, std::string test_str);
static void replace_tokens(std::string &str,
							const std::string &password,
							const std::string &nick,
							const std::string &username,
							const std::string &fullname);


static void launch_tests(int socket_fd, const std::string &password,
	const std::string &nick, const std::string &username, const std::string &fullname, char **test_files)
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
					replace_tokens(test_msg, password, nick, username, fullname);
					std::cout << "======== TESTER: Sending command [" << test_msg << "] =========\n"; 
					test_msg += IRC_MSG_SEPARATOR;
					test_command(socket_fd, test_msg);
				}
			}
		} else {
			std::cout << "Invalid test file " << file << std::endl;
		}
	}
}

static void replace_tokens(std::string &str,
							const std::string &password,
							const std::string &nick,
							const std::string &username,
							const std::string &fullname)
{
	size_t ret;
	while ((ret = str.find("$PASSWORD")) != str.npos)
		str.replace(ret, 9, password);
	while ((ret = str.find("$NICK")) != str.npos)
		str.replace(ret, 5, nick);
	while ((ret = str.find("$USERNAME")) != str.npos)
		str.replace(ret, 9, username);
	while ((ret = str.find("$FULLNAME")) != str.npos)
		str.replace(ret, 9, fullname);
}

static void	test_command(int socket_fd, std::string test_str)
{
	char	server_response[BUFFER_LEN];
	memset(server_response, '\0', BUFFER_LEN);
    // Send the message to server:
	if (write(socket_fd, test_str.c_str(), test_str.length()) == -1) {
        std::cout << "Unable to send message\n";
        return;
	}
	
	usleep(100000);
    // Receive the server's response:
	int read_ret = 0;
	while ((read_ret = read(socket_fd, server_response, BUFFER_LEN)) >= 0) {
        ;
	}
	std::cout << "Server's response: " << server_response << std::endl;
}

static bool	init_tester_config(int &port, std::string &password,
	std::string &nick, std::string &username, std::string &fullname)
{
	std::string		rl_str;
	std::ifstream	infile(CONFIG_FILE_PATH);
	unsigned int	init_count = 0;

	while (std::getline(infile, rl_str)) {
		if (!rl_str.empty() && rl_str.at(0) != '#') {
			if (init_count == 0)
				port = std::atoi(rl_str.c_str());
			else if (init_count == 1)
				password = rl_str;
			else if (init_count == 2)
				nick = rl_str;
			else if (init_count == 3)
				username = rl_str;
			else if (init_count == 4)
				fullname = rl_str;
			init_count++;
		}
	}
	return (init_count == 5);
}

int main(int ac, char **av)
{
	if (ac < 2) {
		std::cout << "USAGE: ./tester {-help} ...[test_files]\n";
		return (EXIT_FAILURE);
	}

	int	port;
	std::string	password, nick, username, fullname;

	if (std::strncmp(av[1], "-help", 6) == 0) {
		std::cout << "USAGE: ./tester {-help} ...[test_files]\n"
			<< "Config file is ./tester_files/config.test\n";
	}

	if (init_tester_config(port, password, nick, username, fullname) == false) {
		std::cout << "Config error\n";
		return (EXIT_FAILURE);
	}

	int					socket_fd;
	struct sockaddr_in	server_addr;

	try
	{
		// Create socket:
		socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

		if (socket_fd < 0)
		{
			std::cout << "Unable to create socket\n";
			return -1;
		}

		std::cout << "Socket created successfully\n";

		// Set port and IP the same as server-side:
		server_addr.sin_family		= AF_INET;
		server_addr.sin_port		= htons(port);
		server_addr.sin_addr.s_addr	= inet_addr("127.0.0.1");

		// Send connection request to server:
		while (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{
			std::cout << "Unable to connect\n";
		}
		std::cout << "Connected with server successfully\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	launch_tests(socket_fd, password, nick, username, fullname, av);
		// Close the socket:
	std::cout << "Tests end, closing server connexion\n";
	close(socket_fd);
}

#include "common.hpp"
#include "Scheduler.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include "functionMap.hpp"
#include <arpa/inet.h>

#define BUFFER_LEN 2048
#include <fstream>

static void	test_command(int socket_fd, std::string test_str, char *server_reponse);
static void replace_tokens(std::string &str, const std::string &password,
									const std::string &nick, const std::string &username);


static void launch_tests(int socket_fd, const std::string &password,
	const std::string &nick, const std::string &username, char **test_files)
{
	std::string	msg;
	char server_response[BUFFER_LEN];

	// Clean buffers:
	memset(server_response, '\0', BUFFER_LEN);
	std::string		test_msg;

	for (int i = 0; test_files[i] != NULL; i++) {
		std::ifstream	infile(test_files[i]);
		while (std::getline(infile, test_msg)) {
			if (!test_msg.empty() && test_msg.at(0) != '#') {
				replace_tokens(test_msg, password, nick, username);
				std::cout << "======== TESTER: Sending command [" << test_msg << "] =========\n"; 
				test_msg += IRC_MSG_SEPARATOR;
				test_command(socket_fd, test_msg, server_response);
			}
		}
	}
	

}

static void replace_tokens(std::string &str, const std::string &password,
									const std::string &nick, const std::string &username)
{
	size_t ret;
	while ((ret = str.find("$PASSWORD")) != str.npos)
		str.replace(ret, 9, password);
	while ((ret = str.find("$NICK")) != str.npos)
		str.replace(ret, 5, nick);
	while ((ret = str.find("$USERNAME")) != str.npos)
		str.replace(ret, 9, username);
}

static void	test_command(int socket_fd, std::string test_str, char *server_response)
{
	// Get input from the user:
	test_str += IRC_MSG_SEPARATOR;

    // Send the message to server:
	if (write(socket_fd, test_str.c_str(), test_str.length()) == -1) {
        std::cout << "Unable to send message\n";
        return;
	}
	
	usleep(1000);
    // Receive the server's response:
	int read_ret = 0;
	if ((read_ret = read(socket_fd, server_response, BUFFER_LEN)) == -1) {
        std::cout << "No server response\n";
	}
    else
    	std::cout << "Server's response: " << server_response << std::endl;
}

int main(int ac, char **av)
{
	(void)av;
	if (ac < 5)
	{
		std::cout << "Usage: ./irc_tester [port] [password] [nick] [username] {... [test_file(s)]}\n";
		return (EXIT_FAILURE);
	}

	const int			port = std::atoi(av[1]);
	const std::string	password = av[2];
	const std::string	nick = av[3];
	const std::string	username = av[4];

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

	launch_tests(socket_fd, password, nick, username, &av[5]);
		// Close the socket:
	close(socket_fd);
}
#include "common.hpp"
#include "Scheduler.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include "functionMap.hpp"
#include <arpa/inet.h>

#define BUFFER_LEN 2048

static void	test_command(int socket_fd, std::string test_str);


static void launch_tests(int socket_fd, const std::string &password,
	const std::string &nick, const std::string &username)
{
	std::string	msg;

	// VALID LOGIN
	msg = "PASS " + password;
	test_command(socket_fd, msg);
	msg = "NICK " + nick;
	test_command(socket_fd, msg);
	msg = "USER " + nick + " 0 0 :" + username;
	test_command(socket_fd, msg);
}

static void	test_command(int socket_fd, std::string test_str)
{
	// Get input from the user:
	std::cout << "TESTER: Sending command [" << test_str << "] =========\n"; 
	test_str += IRC_MSG_SEPARATOR;
	char server_response[BUFFER_LEN];

	// Clean buffers:
	memset(server_response, '\0', BUFFER_LEN);

    // Send the message to server:
	if (write(socket_fd, test_str.c_str(), test_str.length()) == -1) {
        std::cout << "Unable to send message\n";
        return;
	}
	
	usleep(100);
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
	if (ac != 5)
	{
		std::cout << "Usage: ./irc_tester [port] [password] [nick] [username]\n";
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

	launch_tests(socket_fd, password, nick, username);
		// Close the socket:
	close(socket_fd);
}
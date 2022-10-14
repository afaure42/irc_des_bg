#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.hpp"
#include "custom_exceptions.hpp"
#include "Client.hpp"




class Server
{
public:
	Server(int port);
	~Server(){};

	/** @brief adds the fd to the epoll interest list
	 * May throw a syscall_error exception
	*/
	void add_socket_epoll(int fd);

	/** @brief removes fd from the epoll interset list
	 * May throw a syscall_error exception
	*/
	void rmv_socket_epoll(int fd);

	/**
	 * @brief accepts a new tcp connection
	 * ( call if EPOLLIN event on the server socket)
	 * May Throw a syscall_error exception
	 * @return a Client instance containing information about the connection
	 */
	Client	accept_new_client(void);

	/**
	 * @brief Closes the tcp connection with the client
	 * No condition regarding the server socket
	 * May Throw a syscall_error exception
	 * @param client The client you want to disconnect
	 */
	void	disconnect_client(Client & client);

	/**
	 * @brief getter for the Clients
	 * 
	 * @param fd fd of the client you want
	 * @return A Reference to the client class corresponding to that fd
	 */
	Client & get_client(int fd);
	
	/**
	 * @brief will wait for new event using epoll
	 * and accept new connections
	 * 
	 * @param scheduler A ref to the scheduler, the server will
	 * add to the update queue every fd that received data
	 */
	void wait_and_accept(Scheduler & scheduler);

private:
//SOCKET INFORMATIONS
	Server();
	Server(const Server &);
	int _port;
	int _sockfd;
	sockaddr_in _sockaddr;
	socklen_t	_socklen;

//EPOLL RELATED THINGS
	int _epfd;
	epoll_event _events[EVENTS_SIZE];

// Current_connections
	std::map<int, Client> _connected_clients;
};

#endif

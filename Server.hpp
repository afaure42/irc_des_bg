#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.hpp"
#include "custom_exceptions.hpp"
#include "Client.hpp"
#include "Scheduler.hpp"

class Scheduler;
class Client;

class Server
{
public:
	Server(int port);
	~Server();


	/**
	 * @brief Closes the tcp connection with the client
	 * No condition regarding the server socket
	 * May Throw a syscall_error exception
	 * @param client The client you want to disconnect
	 */
	void	disconnect_client(Client & client);
	/**
	 * @brief Closes the tcp connection with the client
	 * No condition regarding the server socket
	 * May Throw a syscall_error exception
	 * @param connection_id The id of the client you want to disconnect
	 */
	void	disconnect_client(unsigned int connection_id);

	/**
	 * @brief Function to know if a client is still connected
	 * 
	 * @param connection_id id of the client
	 * @return true if client is connected
	 * @return false if cilent is not connected
	 */
	bool	is_client_connected(unsigned int connection_id);

	/**
	 * @brief getter for the Clients
	 * 
	 * @param connection_id of the client you want
	 * @return A Pointer to the client class corresponding to that
	 * 	connection_id
	 */
	Client & get_client(unsigned int connection_id);
	
	/**
	 * @brief will wait for new event using epoll
	 * and accept new connections
	 * 
	 * @param scheduler A ref to the scheduler, the server will
	 * add to the update queue every fd that received data
	 */
	void wait_and_accept(Scheduler & scheduler);

private:
//Private methods:

	//no default nor copy construct
	Server();
	Server(const Server &);

	/** @brief adds the fd to the epoll interest list
	 * May throw a syscall_error exception
	*/
	void add_socket_epoll(int fd);

	/** @brief removes fd from the epoll interset list
	 * May throw a syscall_error exception
	*/
	void rmv_socket_epoll(int fd);

	/**
	 * @brief will accept new clients until no connection is pending
	 * ( call if EPOLLIN event on the server socket)
	 * May Throw a syscall_error exception
	 */
	void	accept_new_clients(void);

	/**
	 * @brief conversion from connection id 
	 *  to fd
	 * @param id 
	 * @return fd of that connection
	 */
	int	to_fd(unsigned int id);

	/**
	 * @brief conversion from fd to 
	 * connection id
	 * @param fd 
	 * @return connection id of this client
	 */
	unsigned int to_id(int fd);


//SOCKET INFORMATIONS
	int _port;
	int _sockfd;
	sockaddr_in _sockaddr;
	socklen_t	_socklen;

//EPOLL RELATED THINGS
	int _epfd;
	epoll_event _events[EVENTS_SIZE];

// Current_connections
	std::map<unsigned int, Client>	_connected_clients;
	unsigned int			_connection_counter;

	/**
	 * @brief this map is used to convert a
	 * fd to a connection_id 
	 */
	std::map<int, unsigned int> _connection_index;
};

#endif

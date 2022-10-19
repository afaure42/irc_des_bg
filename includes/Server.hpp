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
	void	disconnectClient(Client & client);
	/**
	 * @brief Closes the tcp connection with the client
	 * No condition regarding the server socket
	 * May Throw a syscall_error exception
	 * @param connection_id The id of the client you want to disconnect
	 */
	void	disconnectClient(unsigned int connection_id);

	/**
	 * @brief Function to know if a client is still connected
	 * 
	 * @param connection_id id of the client
	 * @return true if client is connected
	 * @return false if cilent is not connected
	 */
	bool	isClientConnected(unsigned int connection_id);

	/**
	 * @brief getter for the Clients
	 * 
	 * @param connection_id of the client you want
	 * @return A Pointer to the client class corresponding to that
	 * 	connection_id
	 */
	Client & getClient(unsigned int connection_id);
	
	/**
	 * @brief will wait for new event using epoll
	 * and accept new connections
	 * 
	 * @param scheduler A ref to the scheduler, the server will
	 * add to the update queue every fd that received data
	 */
	void waitAndAccept(Scheduler & scheduler);

private:
//Private methods:

	//no default nor copy construct
	Server();
	Server(const Server &);

	/** @brief adds the fd to the epoll interest list
	 * May throw a syscall_error exception
	*/
	void _addSocketEpoll(int fd);

	/** @brief removes fd from the epoll interset list
	 * May throw a syscall_error exception
	*/
	void _rmvSocketEpoll(int fd);

	/**
	 * @brief will accept new clients until no connection is pending
	 * ( call if EPOLLIN event on the server socket)
	 * May Throw a syscall_error exception
	 */
	void	_acceptNewClients(void);

	/**
	 * @brief conversion from connection id 
	 *  to fd
	 * @param id 
	 * @return fd of that connection
	 */
	int	_toFd(unsigned int id);

	/**
	 * @brief conversion from fd to 
	 * connection id
	 * @param fd 
	 * @return connection id of this client
	 */
	unsigned int _toId(int fd);


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

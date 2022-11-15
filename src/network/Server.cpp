#include "Server.hpp"

Server::Server(int port, std::string & pass)
:_port(port), _sockfd(-1), _sockaddr(), _socklen(),
	_epfd(-1), _events(), _pass(pass), _is_on(true), _connection_counter()
{
	//CREATING SOCKET
	this->_sockfd = socket(AF_INET,
					SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (this->_sockfd < 0)
		throw(syscall_error(errno, "socket:"));
	
	//SETUPPING VALUES FOR SOCKADDR STRUCT (MAN IP, MAN SOCKET)
	this->_sockaddr.sin_family = AF_INET;
	this->_sockaddr.sin_port = htons(this->_port);
	this->_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	this->_socklen = sizeof(sockaddr_in);

	//BINDING SOCKET TO SOCKADDR
	if (bind(this->_sockfd, (sockaddr *)&this->_sockaddr,
		this->_socklen) < 0)
	{
		close(this->_sockfd);
		throw(syscall_error(errno, "bind:"));
	}

	//SETTING SOCKET ON LISTENING MODE
	if (listen(this->_sockfd, MAX_CONNECTIONS_LISTN) < 0)
	{
		close(this->_sockfd);
		throw(syscall_error(errno, "listen:"));
	}

	//EPOLL INIT

	epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));

	//CREATING EPOLL INSTANCE
	this->_epfd = epoll_create(42);
	if (this->_epfd < 0)
	{
		close(this->_sockfd);
		throw(syscall_error(errno, "epoll_create:"));
	}

	//ADDING SERVER SOCKET TO EPOLL INSTANCE
	ev.events = EPOLLIN;
	ev.data.fd = this->_sockfd;
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, this->_sockfd, &ev) < 0)
	{
		close(this->_sockfd);
		close(this->_epfd);
		throw(syscall_error(errno, "epoll_ctl"));
	}

	//just for good conscience...
	memset(this->_events, 0, sizeof(epoll_event) * EVENTS_SIZE);
}

int Server::_toFd(unsigned int id)
{
	return this->getClient(id).getFd();
}

unsigned int Server::_toId(int fd)
{
	return this->_connection_index.at(fd);
}

void Server::disconnectClient(unsigned int id)
{
	this->_rmvSocketEpoll(this->_toFd(id));
	close(this->_toFd(id));
	this->_connection_index.erase(this->_toFd(id));
	this->_connected_clients.erase(id);
}

void Server::disconnectClient(Client & client)
{
	this->disconnectClient(client.getId());
}

bool Server::isClientConnected(unsigned int connection_id)
{
	return (this->_connected_clients.find(connection_id) 
		!= this->_connected_clients.end());
}

Client & Server::getClient(unsigned int connection_id)
{
		return this->_connected_clients
		.at(connection_id);
}

const std::string & Server::getPass(void) const {
	return this->_pass;
}

void Server::waitAndAccept(Scheduler & scheduler)
{
	int nb_fds = epoll_wait(this->_epfd,
			this->_events, EVENTS_SIZE, -1);
	if (nb_fds < 0)
		throw(syscall_error(errno, "waitAndAccept: epoll_wait:"));

	epoll_event * ev = this->_events;
	for(int i = 0; i < nb_fds; i++)
	{
		int fd = ev[i].data.fd;
		if (fd == this->_sockfd)
		{
			std::cout << "NEW CONNECTION DETECTED\n";
			this->_acceptNewClients();
			continue;
		}

		Client & current_client = this->getClient(this->_toId(fd));
		//IF deconnected, set client in a passive state, processing will have to tell the
		//server when the deconnection has been processed and the server can free the resources
		if (ev[i].events & EPOLLRDHUP
			|| ev[i].events & EPOLLERR
			|| ev[i].events & EPOLLHUP)
		{
			std::cout << "EPOLLHUP for id:" << current_client.getId() << '\n';
			scheduler.addToUpdates(Update(current_client.getId(), NULL, false));
			current_client.setConnected(false);
			current_client.setReadable(false);
			current_client.setWriteable(false);
			continue;
		}

		//IF POLLIN, add to the read queue and set read ok flag
		if (ev[i].events & EPOLLIN)
		{
			this->getClient(this->_toId(fd)).setReadable(true);

			scheduler.addToRead(this->_toId(fd));
		}

		//if POLLOUT just set write ok flag
		if (ev[i].events & EPOLLOUT)
			this->getClient(this->_toId(fd)).setWriteable(true);
	}
}

std::string	Server::getClientHost(unsigned int connection_id)
{
	char buff[1024];
	sockaddr * sa = (sockaddr *)this->getClient(connection_id).getSockAddr();

	int ret = getnameinfo(sa, sizeof(sockaddr_in), buff, 1024, NULL, 0, 0);

	if (ret != 0)
		throw(syscall_error(errno, "getClientHost"));

	return (std::string(buff));	
}

void Server::_addSocketEpoll(int fd)
{
	epoll_event ev;

	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = fd;
	ev.events = USR_EVENT_FLAGS;
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, fd, &ev) < 0)
		throw(syscall_error(errno, "epoll_ctl_add:"));
}

void Server::_rmvSocketEpoll(int fd)
{
	if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, NULL) < 0)
		throw(syscall_error(errno, "epoll_ctl_del:"));
}


void Server::_acceptNewClients(void)
{
	while (1)
	{
		struct sockaddr_in	addr;
		socklen_t			len;
		int					new_sock;
		int					ret;

		memset(&addr, 0, sizeof(addr));
		len = sizeof(addr);

		new_sock = accept(this->_sockfd, (sockaddr *)&addr, &len);
		if (new_sock < 0)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN)
				break;
			else
				throw (syscall_error(errno, "_acceptNewClients: accept:"));
		}

		ret = fcntl(new_sock, F_SETFL, O_NONBLOCK);
		if (ret < 0)
			throw(syscall_error(errno, "_acceptNewClients: fcntl:"));
		
		_addSocketEpoll(new_sock);
		this->_connection_counter++;
		this->_connection_index[new_sock] = this->_connection_counter;
		this->_connected_clients[this->_connection_counter] = 
			Client(new_sock, this->_connection_counter, addr, len, true);
	}
}

void Server::setIsOn(bool status) {
	this->_is_on = status;
}

bool Server::getIsOn(void) const {
	return this->_is_on;
}

Server::~Server()
{
	//add little disconnect all maybe
	for(std::map<unsigned int, Client>::iterator it = this->_connected_clients.begin();
		it != this->_connected_clients.end(); it++)
		close(it->second.getFd());

	close(this->_epfd);
	close(this->_sockfd);
	//easier than init... innit ?
}

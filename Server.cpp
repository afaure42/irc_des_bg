#include "Server.hpp"

Server::Server(int port)
:_port(port), _sockfd(-1), _sockaddr(), _socklen(),
	_epfd(-1), _events()
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

void Server::add_socket_epoll(int fd)
{
	epoll_event ev;

	ev.data.fd = fd;
	ev.events = USR_EVENT_FLAGS;
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, fd, &ev) < 0)
		throw(syscall_error(errno, "epoll_ctl_add:"));
}

void Server::rmv_socket_epoll(int fd)
{
	if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, NULL) < 0)
		throw(syscall_error(errno, "epoll_ctl_del:"));
}

Server::~Server()
{
	close(this->_epfd);
	close(this->_sockfd);
	//easier than init... innit ?
}

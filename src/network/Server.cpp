#include "Server.hpp"

Server::Server(int port)
:_port(port), _sockfd(-1), _sockaddr(), _socklen(),
	_epfd(-1), _events(), _connection_counter()
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

int Server::to_fd(unsigned int id)
{
	return this->get_client(id).get_fd();
}

unsigned int Server::to_id(int fd)
{
	return this->_connection_index.at(fd);
}

void Server::disconnect_client(unsigned int id)
{
	this->rmv_socket_epoll(this->to_fd(id));
	close(this->to_fd(id));
	this->_connection_index.erase(this->to_fd(id));
	this->_connected_clients.erase(id);
}

void Server::disconnect_client(Client & client)
{
	this->disconnect_client(client.get_id());
}

bool Server::is_client_connected(unsigned int connection_id)
{
	return (this->_connected_clients.find(connection_id) 
		!= this->_connected_clients.end());
}

Client & Server::get_client(unsigned int connection_id)
{
		return this->_connected_clients
		.at(connection_id);
}

void Server::wait_and_accept(Scheduler & scheduler)
{
	int nb_fds = epoll_wait(this->_epfd,
			this->_events, EVENTS_SIZE, -1);
	if (nb_fds < 0)
		throw(syscall_error(errno, "wait_and_accept: epoll_wait:"));

	epoll_event * ev = this->_events;
	for(int i = 0; i < nb_fds; i++)
	{
		int fd = ev[i].data.fd;
		if (fd == this->_sockfd)
		{
			std::cout << "NEW CONNECTION DETECTED\n";
			this->accept_new_clients();
			continue;
		}
		//IF deconnected, disconnect client and remove from different
		//queues
		if (ev[i].events & EPOLLRDHUP
			|| ev[i].events & EPOLLERR
			|| ev[i].events & EPOLLHUP)
		{
			std::cout << "EPOLLHUP for id:" << this->to_id(fd) << '\n';
			scheduler.remove_from_queues(this->to_id(fd));
			scheduler.add_to_updates(Update(this->to_id(fd), NULL, false));
			disconnect_client(this->to_id(fd));
			continue;
		}

		//IF POLLIN, add to the read queue and set read ok flag
		if (ev[i].events & EPOLLIN)
		{
			this->get_client(this->to_id(fd)).set_readable(true);

			scheduler.add_to_read(this->to_id(fd));
		}

		//if POLLOUT just set write ok flag
		if (ev[i].events & EPOLLOUT)
			this->get_client(this->to_id(fd)).set_writeable(true);
	}
}

void Server::add_socket_epoll(int fd)
{
	epoll_event ev;

	memset(&ev, 0, sizeof(epoll_event));
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


void Server::accept_new_clients(void)
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
				throw (syscall_error(errno, "accept_new_clients: accept:"));
		}

		ret = fcntl(new_sock, F_SETFL, O_NONBLOCK);
		if (ret < 0)
			throw(syscall_error(errno, "accept_new_clients: fcntl:"));
		
		add_socket_epoll(new_sock);
		this->_connection_counter++;
		this->_connection_index[new_sock] = this->_connection_counter;
		this->_connected_clients[this->_connection_counter] = 
			Client(new_sock, this->_connection_counter, addr, len, true);
	}
}

Server::~Server()
{
	//add little disconnect all maybe
	for(std::map<unsigned int, Client>::iterator it = this->_connected_clients.begin();
		it != this->_connected_clients.end(); it++)
		close(it->first);

	close(this->_epfd);
	close(this->_sockfd);
	//easier than init... innit ?
}

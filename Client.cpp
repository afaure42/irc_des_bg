#include "Client.hpp"

Client::Client(const int &fd, const sockaddr_in & addr,
		const socklen_t & socklen, bool connected)
:_fd(fd), _addr(addr), _socklen(socklen), _connected(connected)
{

}

Client::Client(const Client & ref)
{
	*this = ref;
}

Client & Client::operator=(const Client & ref)
{
	this->_fd		 = ref._fd;
	this->_addr		 = ref._addr;
	this->_socklen	 = ref._socklen;
	this->_connected = ref._connected;
	this->_read_buff = ref._read_buff;
	this->_to_write  = ref._to_write;
	this->_last_read_size = ref._last_read_size;

	return *this;
}

int Client::get_fd() const
{
	return this->_fd;
}

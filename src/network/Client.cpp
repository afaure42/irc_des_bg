#include "Client.hpp"

Client::Client(const int &fd, const unsigned int & id, 
		const sockaddr_in & addr, const socklen_t & socklen,
		bool connected)
:_fd(fd), _id(id), _addr(addr), _socklen(socklen),
_connected(connected)
{

}

Client::Client(const Client & ref)
{
	*this = ref;
}

Client & Client::operator=(const Client & ref)
{
	this->_fd		 = ref._fd;
	this->_id		 = ref._id;
	this->_addr		 = ref._addr;
	this->_socklen	 = ref._socklen;
	this->_connected = ref._connected;
	this->_read_buff = ref._read_buff;
	this->_to_write  = ref._to_write;
	this->_last_read_size = ref._last_read_size;

	return *this;
}

int Client::getFd() const {
	return this->_fd;
}

unsigned int Client::getId() const {
	return this->_id;
}

int Client::getReadSize() const {
	return this->_last_read_size;
}

std::string	& Client::getReadBuff() {
	return this->_read_buff;
}

std::string	& Client::getWriteBuff() {
	return this->_to_write;
}
	
bool	Client::isConnected() const {
	return this->_connected;
}

bool	Client::isWriteable() const {
	return this->_write_ready;
}

bool	Client::isReadable() const {
	return this->_read_ready;
}

void	Client::setWriteable(bool input){
	this->_write_ready = input;
}

void	Client::setConnected(bool input) {
	this->_connected = input;
}

void	Client::setReadable(bool input) {
	this->_read_ready = input;
}

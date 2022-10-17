#include "Info.hpp"

Info::Info(unsigned int id, std::string * buff, bool connected)
:_connection_id(id), _read_buff(buff), _connected(connected)
{

}

Info::Info(const Info & ref)
:_connection_id(ref.getId()), _read_buff(ref.getBuff()),
_connected(ref.isConnected())
{

}

Info::Info(Client & ref)
:_connection_id(ref.getId()), _read_buff(&ref.getReadBuff()),
_connected(ref.isConnected())
{

}

Info & Info::operator=(const Info & rhs)
{
	this->_connected = rhs._connected;
	this->_connection_id = rhs._connection_id;
	this->_read_buff = rhs._read_buff;

	return *this;
}

unsigned int Info::getId() const {
	return this->_connection_id;
}

std::string * Info::getBuff() const{
	return this->_read_buff;
}

bool Info::isConnected() const {
	return this->_connected;
}

void Info::setBuff(const std::string & ref) {
	*(this->_read_buff) = ref;
}

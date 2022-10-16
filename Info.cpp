#include "Info.hpp"

Info::Info(unsigned int id, std::string * buff, bool connected)
:_connection_id(id), _read_buff(buff), _connected(connected)
{

}

Info::Info(const Info & ref)
:_connection_id(ref.get_id()), _read_buff(ref.buff()),
_connected(ref.is_connected())
{

}

Info::Info(Client & ref)
:_connection_id(ref.get_id()), _read_buff(&ref.get_read_buff()),
_connected(ref.is_connected())
{

}

Info & Info::operator=(const Info & rhs)
{
	this->_connected = rhs._connected;
	this->_connection_id = rhs._connection_id;
	this->_read_buff = rhs._read_buff;

	return *this;
}

unsigned int Info::get_id() const {
	return this->_connection_id;
}

std::string * Info::buff() const{
	return this->_read_buff;
}

bool Info::is_connected() const {
	return this->_connected;
}

void Info::set_buff(const std::string & ref) {
	*(this->_read_buff) = ref;
}

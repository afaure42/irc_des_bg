#include "Update.hpp"

Update::Update(unsigned int id, std::string * buff, bool connected)
:_connection_id(id), _read_buff(buff), _connected(connected)
{

}

Update::Update(const Update & ref)
:_connection_id(ref.getId()), _read_buff(ref.getBuff()),
_connected(ref.isConnected())
{

}

Update::Update(Client & ref)
:_connection_id(ref.getId()), _read_buff(&ref.getReadBuff()),
_connected(ref.isConnected())
{

}

Update & Update::operator=(const Update & rhs)
{
	this->_connected = rhs._connected;
	this->_connection_id = rhs._connection_id;
	this->_read_buff = rhs._read_buff;

	return *this;
}

unsigned int Update::getId() const {
	return this->_connection_id;
}

std::string * Update::getBuff() const{
	return this->_read_buff;
}

bool Update::isConnected() const {
	return this->_connected;
}

void Update::setBuff(const std::string & ref) {
	*(this->_read_buff) = ref;
}

#include "Update.hpp"

Update::Update(unsigned int id, std::string * buff, bool connected)
:_connection_id(id), _read_buff(buff), _connected(connected)
{

}

Update::Update(const Update & ref)
:_connection_id(ref.get_id()), _read_buff(ref.buff()),
_connected(ref.is_connected())
{

}

Update::Update(Client & ref)
:_connection_id(ref.get_id()), _read_buff(&ref.get_read_buff()),
_connected(ref.is_connected())
{

}

Update & Update::operator=(const Update & rhs)
{
	this->_connected = rhs._connected;
	this->_connection_id = rhs._connection_id;
	this->_read_buff = rhs._read_buff;

	return *this;
}

unsigned int Update::get_id() const {
	return this->_connection_id;
}

std::string * Update::buff() const{
	return this->_read_buff;
}

bool Update::is_connected() const {
	return this->_connected;
}

void Update::set_buff(const std::string & ref) {
	*(this->_read_buff) = ref;
}

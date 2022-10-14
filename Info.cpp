#include "Info.hpp"

Info::Info(int fd, std::string * buff, bool connected)
:_fd(fd), _read_buff(buff), _connected(connected)
{

}

Info::Info(const Info & ref)
:_fd(ref.get_fd()), _read_buff(ref.buff()),
_connected(ref.is_connected())
{

}

int Info::get_fd() const {
	return this->_fd;
}

std::string * Info::buff() {
	return this->_read_buff;
}

bool Info::is_connected() const {
	return this->_connected;
}

void Info::set_buff(const std::string & ref) {
	this->_read_buff = ref;
}

#include "custom_exceptions.hpp"

/*****EXCEPTIONS*******/
syscall_error::syscall_error(int err, std::string msg)
:_errno(err), _msg(msg)
{
}

const char * syscall_error::what() const throw()
{
	return this->_msg.c_str();
}

const char * syscall_error::err() const throw()
{
	return std::strerror(this->_errno);
}

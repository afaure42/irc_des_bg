#pragma once

#include "common.hpp"
#include "Client.hpp"
#include <map>

class Channel
{
	public:
		Channel(std::string name);
		~Channel();
		std::string const	getName(void) const;
		void	operator<<(void) const;
	private:
		typedef std::map<std::string, Client>	_usrlist;
		std::string	_name;
};

void Channel4::operator<<	(void) const {
	std::cout << this->_name << std::endl;
}

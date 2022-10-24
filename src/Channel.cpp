#include "Channel.hpp"

Channel::Channel() {}
Channel::~Channel() {}

//GETTERS
members_t	&Channel::getMembers() {
	return (this->_members);
}
members_perms_t & Channel::getPermissions() {
	return (this->_permissions);
}
std::string	&	Channel::getName() {
	return (this->_name);
}

//SETTERS
void	Channel::setName(std::string & name) {
	this->_name = name;
}

//METHODS
void		Channel::send(Scheduler & scheduler, std::string & msg)
{
	members_t::iterator it = this->_members.begin();

	while(it != this->_members.end())
	{
		scheduler.queueMessage(it->first, msg);
		it++;
	}
}

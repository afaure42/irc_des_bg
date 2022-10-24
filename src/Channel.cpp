#include "Channel.hpp"

Channel::Channel(const std::string & name)
:_name(name), _topic(), _modes(), _key(), _usrlimit(100),
_members(), _permissions()
{}

Channel::Channel(const Channel & ref)
{
	*this = ref;
}

Channel & Channel::operator=(const Channel & ref)
{
	this->_name = ref._name;
	this->_topic = ref._topic;
	this->_modes = ref._modes;
	this->_key = ref._key;
	this->_usrlimit = ref._usrlimit;
	this->_members = ref._members;
	this->_permissions = ref._permissions;

	return *this;
}

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
std::string &	Channel::getTopic() {
	return (this->_topic);
}

//SETTERS
void	Channel::setName(std::string & name) {
	this->_name = name;
}
void	Channel::setTopic(std::string & topic) {
	this->_topic = topic;
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

unsigned int Channel::join(Scheduler & scheduler, User & user)
{
	//TODO add invite flag

	if (this->_members.find(user.getId()) != this->_members.end())
		return (0); //idk if we should return something or not

	//check for permissions

	//then add user to channel
	this->_members.insert(std::make_pair(user.getId(), &user));
	user.getChannels().push_back(this->_name);

	//then send JOIN msg to everyone
	std::string msg = ":" + user.getNick() + " JOIN " + this->getName() + "\r\n";
	this->send(scheduler, msg);

	return (0);
}

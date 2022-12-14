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
members_t const &Channel::getMembers() const {
	return (this->_members);
}
members_perms_t & Channel::getPermissions() {
	return (this->_permissions);
}
std::string	const &	Channel::getName() const {
	return (this->_name);
}
std::string const &	Channel::getTopic() const {
	return (this->_topic);
}
unsigned int	Channel::getModes() const {
	return (this->_modes);
}

//SETTERS
void	Channel::setName(std::string & name) {
	this->_name = name;
}
void	Channel::setTopic(std::string & topic) {
	this->_topic = topic;
}
void	Channel::setModes(const unsigned int & modes) {
	this->_modes = modes;
}

//METHODS
void		Channel::send(Scheduler & scheduler, std::string & msg, unsigned int from)
{
	members_t::iterator it = this->_members.begin();

	while(it != this->_members.end())
	{
		if (it->first != from)
			scheduler.queueMessage(it->first, msg, true);
		it++;
	}
}

void Channel::joinChannel(Scheduler & scheduler, User & user)
{
	//TODO add invite flag

	if (this->_members.find(user.getId()) != this->_members.end())
		return; //idk if we should return something or not

	//then add user to channel
	this->_members.insert(std::make_pair(user.getId(), &user));
	user.getChannels().insert(this->_name);
	if (this->_permissions.find(user.getId()) == this->_permissions.end())
		this->_permissions.insert(std::make_pair(user.getId(), 0));

	//then send JOIN msg to everyone
	std::string msg = ":" + user.getFullName() + " JOIN " + this->getName() + "\r\n";
	this->send(scheduler, msg, 0);
}

void Channel::removeUser(User & user)
{
	this->_members.erase(user.getId());
	this->_permissions.erase(user.getId());
}

std::ostream & operator<<(std::ostream& os, Channel& channel)
{
	os << "Channel name:" << channel.getName() << std::endl;
	os << "Channel is" << ((channel.getModes() & Channel::ANONYMOUS) ? "" : " not")
		<< " anonymous" << std::endl
	<< "Channel is" << ((channel.getModes() & Channel::INVITE_ONLY) ? "" : " not")
		<< " invite only" << std::endl
	<< "Channel is" << ((channel.getModes() & Channel::MODERATED) ? "" : " not")
		<< " moderated" << std::endl
	<< "Channel does" << ((channel.getModes() & Channel::NO_MSG_FROM_OUTSIDE) ? " not" : "")
		<< " accept messages from outside" << std::endl
	<< "Channel is" << ((channel.getModes() & Channel::QUIET) ? "" : " not")
		<< " quiet" << std::endl
	<< "Channel is" << ((channel.getModes() & Channel::PRIVATE) ? "" : " not")
		<< " private" << std::endl
	<< "Channel is" << ((channel.getModes() & Channel::SECRET) ? "" : " not")
		<< " secret" << std::endl
	<< "Channel has " << ((channel.getModes() & Channel::TOPIC) ? "a" : "no")
		<< " topic\tTopic: " << channel.getTopic() << std::endl;
	os << "Channel user list:\n";
	unsigned int userPerms = 0;
	for (members_t::const_iterator it = channel.getMembers().begin();
			it != channel.getMembers().end(); it++)
	{
		userPerms = channel.getPermissions().at(it->second->getId());
		os << "User [" << it->second->getNick() << "]\n"
			<< "\tIs " << (userPerms & Channel::CREATOR ? "":"not") << " the channel creator\n"
			<< "\tIs " << (userPerms & Channel::OPERATOR ? "":"not") << " a channel operator\n"
			<< "\tHas " << (userPerms & Channel::CREATOR ? "":"no") << " voice privilege\n";
	}
	os << '>' << std::endl;

	return os;
}

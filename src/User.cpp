#include "User.hpp"

User::User(unsigned int client_id) :
	_id(client_id), _away(false), _invisible(false), _registered(false)
{
}

User::~User()
{
}

void	User::check(void) const {
	std::cout << "alles gut !" << std::endl;
}

// SETTERS //
void	User::setAwayStatus(void) {
	this->_away = !this->_away;
}
void	User::setInvisStatus(void) {
	this->_invisible = !this->_invisible;
}
void	User::setRegistered(void) {
	this->_registered = !this->_registered;
}
void	User::setNick(std::string nick) {
	this->_old_nick = this->_nick;
	this->_nick = nick;
}
void	User::setUsername(const std::string & usrname) {
	this->_usrname = usrname;
}

// GETTERS //
bool	User::isAway(void) const {
	return (this->_away);
}
bool	User::isInvisible(void) const {
	return (this->_invisible);
}
bool	User::isRegistered(void) const{
	return (this->_registered);
}
unsigned int	User::getId(void) const {
	return (this->_id);
}
std::string	const	&User::getNick(void) const {
	return (this->_nick);
}
std::string	const	&User::getUsername(void) const {
	return (this->_usrname);
}
std::string const	&User::getOldNick(void) const {
	return this->_old_nick;
}

// OPERATOR OVERLOADS //
std::ostream& operator<<(std::ostream& os, const User& usr)
{
    os << "User id: " << usr.getId() << ", nickname: " << usr.getNick() << std::endl;
    os << "User is" << (usr.isAway() ? "" : " not") << " away" << std::endl;
    os << "User is" << (usr.isInvisible() ? "" : " not") << " invisible" << std::endl;
    return os;
}

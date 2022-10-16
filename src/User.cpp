#include "User.hpp"

User::User(const std::string &nickname) :
	_nick(nickname), _away(false), _invisible(false)
{
	static int id = 0;
	this->_id = id++;
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

// GETTERS //
bool	User::isAway(void) const {
	return (this->_away);
}
bool	User::isInvisible(void) const {
	return (this->_invisible);
}
unsigned int	User::getId(void) const {
	return (this->_id);
}
std::string	User::getNick(void) const {
	return (this->_nick);
}

std::ostream& operator<<(std::ostream& os, const User& usr)
{
    os << "User id: " << usr.getId() << ", nickname: " << usr.getNick() << std::endl;
    os << "User is" << (usr.isAway() ? "" : " not") << " away" << std::endl;
    os << "User is" << (usr.isInvisible() ? "" : " not") << " invisible" << std::endl;
    return os;
}

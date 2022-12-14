#include "User.hpp"

User::User(unsigned int client_id) :
	_id(client_id), _nick(TEMP_NICK), _registered(false), _modes(0)
{
}

User::User(const User & ref)
{
	this->_id = ref._id;
	this->_nick = ref._nick;
	this->_old_nick = ref._old_nick;
	this->_usrname = ref._usrname;
	this->_away_msg = ref._away_msg;
	this->_registered = ref._registered;
	this->_connection_pass = ref._connection_pass;
	this->_modes = ref._modes;
	this->_channels = ref._channels;
}

User::~User()
{
}

// SETTERS //
void	User::setAwayStatus(bool status) {
	if (status)
		this->_modes |= User::AWAY;
	else
		this->_modes &= ~(User::AWAY);
}
void	User::setAwayMessage(const std::string & msg) {
	this->_away_msg = msg;
}
void	User::setOperatorStatus(bool status) {
	if (status)
		this->_modes |= User::OPERATOR;
	else
		this->_modes &= ~(User::OPERATOR);
}
void	User::setInvisStatus(bool status) {
	if (status)
		this->_modes |= User::INVISIBLE;
	else
		this->_modes &= ~(User::INVISIBLE);
}
void	User::setWallopStatus(bool status) {
	if (status)
		this->_modes |= User::WALLOPS;
	else
		this->_modes &= ~(User::WALLOPS);
}
void	User::setRegistered(void) {
	this->_registered = true;
}
void	User::setNick(std::string nick) {
	this->_old_nick = this->_nick;
	this->_nick = nick;
}
void	User::setUsername(const std::string & usrname) {
	this->_usrname = usrname;
}
void	User::setRealname(const std::string & realname) {
	this->_realname = realname;
}
void	User::setConnectPass(const std::string & connect_pass) {
	this->_connection_pass = connect_pass;
}
void	User::setHostName(const std::string & hostname) {
	this->_hostname = hostname;
}
void	User::setModes(const unsigned int & modes) {
	this->_modes = modes;
}

// GETTERS //
bool	User::isAway(void) const {
	return (this->_modes & User::AWAY);
}
bool	User::isInvisible(void) const {
	return (this->_modes & User::INVISIBLE);
}
bool	User::isWallop(void) const{
	return (this->_modes & User::WALLOPS);
}
bool	User::isRegistered(void) const{
	return (this->_registered);
}
bool	User::isOp(void) const {
	return (this->_modes & User::OPERATOR);
}
unsigned int	User::getId(void) const {
	return (this->_id);
}
unsigned int	User::getModes(void) const {
	return (this->_modes);
}
std::string	const	&User::getNick(void) const {
	return (this->_nick);
}
std::string	const	&User::getUsername(void) const {
	return (this->_usrname);
}
std::string const	&User::getRealname(void) const {
	return (this->_realname);
}
std::string const	&User::getOldNick(void) const {
	return this->_old_nick;
}
std::string const &User::getHostName(void) const {
	return this->_hostname;
}
std::set<std::string> & User::getChannels(void) {
	return this->_channels;
}
std::set<std::string> const & User::getChannels(void) const {
	return this->_channels;
}
const std::string &User::getConnectPass(void) const {
	return this->_connection_pass;
}
const std::string User::getFullName(void) const {
	return std::string(this->_nick + "!" + this->_usrname + "@" + this->_hostname);
}
const std::string &User::getAwayMsg(void) const {
	return this->_away_msg;
}

// OPERATOR OVERLOADS //
std::ostream& operator<<(std::ostream& os, const User& usr)
{
    os << "User id: " << usr.getId() << ", nickname: " << usr.getNick() << std::endl;
    os << "User is" << (usr.isRegistered() ? "" : " not") << " registered" << std::endl;
    os << "User is" << (usr.isAway() ? "" : " not") << " away" << std::endl;
    os << "User is" << (usr.isInvisible() ? "" : " not") << " invisible" << std::endl;
	os << "Joined Channels<";
	for (std::set<std::string>::iterator it = usr.getChannels().begin();
		it != usr.getChannels().end(); it++)
		os << '[' << *it << ']';
	os << '>' << std::endl;
    return os;
}

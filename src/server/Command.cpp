#include "Command.hpp"

Command::Command(exec_fn_map & fn_map,
				const t_opers  & operators,
				const std::string *raw_command,
				Scheduler & scheduler,
				Server & server)
:_chars_read(0), _function_map(fn_map), _scheduler(scheduler),
	_server(server), _operators(operators)
{
	this->_setupCommand(*raw_command);
}

Command::~Command() {}


// GETTERS
unsigned int const	&Command::getCharsRead(void) const {
	return (this->_chars_read);
}
std::string	const	&Command::getCmdName(void) const {
	return (this->_cmd_name);
}
std::list<std::string> const	&Command::getParams(void) const {
	return (this->_params);
}
int const	&Command::getNumericReturn(void) const {
	return (this->_numeric_return);
}
t_opers const &Command::getOperators(void) const {
	return (this->_operators);
}
Scheduler &		Command::getScheduler(void) {
	return (this->_scheduler);
}
Server &	Command::getServer(void) {
	return (this->_server);
}

exec_fn_map & Command::getFunctionMap(void) {
	return (this->_function_map);
}

// Creates the cmdType and params for the command
// TODO: This function needs to check for the presence of /r/d or whatever,
// extract the substring(s), and the program must
// delete the buffer up until that point 
void	Command::_setupCommand(const std::string & raw_command) {
	// If separator is in buffer, do stuff
	if (raw_command.find(IRC_MSG_SEPARATOR) != raw_command.npos) {
		std::string command = raw_command.substr(0, raw_command.find(IRC_MSG_SEPARATOR));
		std::cout << "Valid IRC message received: " << command << std::endl;
		this->_chars_read = command.length() + std::strlen(IRC_MSG_SEPARATOR);
		while (!command.empty()) {
			if (command.at(0) == ':') {
				this->_params.push_back(command);
				command.erase();
			}
			else {
				std::size_t space = command.find_first_of(' ');
				if (space == command.npos) {
					space = command.length();
				}
				this->_params.push_back(command.substr(0, space));
				command.erase(0, space + 1);
			}
		}
		if (!this->_params.empty()) {
			this->_cmd_name = this->_params.front();
			this->_params.pop_front();
		}
	}
}

// EXECUTION PIPELINE
// Tries to find the method for the command in the functions map,
// and executes it if found
void	Command::execute(
	unsigned int	client_id,
	t_users			&users,
	t_channels		&channels)
{
	(void)channels;
	if (this->_function_map.find(this->_cmd_name) == this->_function_map.end()) {
		this->_numeric_return = ERR_UNKNOWNCOMMAND;
		return;
	}
	else {
		this->_numeric_return =
			this->_function_map.find(this->_cmd_name)->second(*this, client_id, users, channels);
	}
}

// NUMERIC_REPLIES DISPATCHER
void	Command::sendReplies(
	unsigned int	client_id,
	t_users			&users,
	t_channels		&channels)
{
	(void)channels;
	t_users::iterator 	it = users.find(client_id);
	const std::string	user_nick = (it == users.end()) ? TEMP_NICK : users.at(client_id).getNick();
	std::string			reason;
	std::string			target = "";

	switch (this->_numeric_return)
	{
		case ERR_NOTEXTTOSEND:
			reason = ERR_NOTEXTTOSEND_MSG;
			break;
		case ERR_UNKNOWNCOMMAND:
			target = this->getCmdName();
			reason = ERR_UNKNOWNCOMMAND_MSG;
			break;
		case ERR_NEEDMOREPARAMS:
			target = this->getCmdName();
			reason = ERR_NEEDMOREPARAMS_MSG;
			break;
		case ERR_NONICKNAMEGIVEN:
			reason = ERR_NONICKNAMEGIVEN_MSG;
			break;
		case ERR_ALREADYREGISTERED:
			reason = ERR_ALREADYREGISTERED_MSG;
			break;
		case ERR_NICKNAMEINUSE:
			target = *this->_params.begin();
			reason = ERR_NICKNAMEINUSE_MSG;
			break;
		case ERR_ERRONEUSNICKNAME:
			target = *this->_params.begin();
			reason = ERR_ERRONEUSNICKNAME_MSG;
			break;
		case ERR_NOSUCHNICK:
			target = *this->_params.begin();
			reason = ERR_NOSUCHNICK_MSG;
			break;
		case ERR_NOTREGISTERED:
			reason = ERR_NOTREGISTERED_MSG;
			break;
		case ERR_NOPRIVILEGES:
			reason = ERR_NOPRIVILEGES_MSG;
			break;
		case RPL_YOUREOPER:
			reason = RPL_YOUREOPER_MSG;
			break;
		case ERR_PASSWDMISMATCH:
			reason = ERR_PASSWDMISMATCH_MSG;
			break;
		default:
			reason = "SERVER ERROR: UNKNOWN/UNSUPPORTED NUMERIC REPLY";
			break;
	}
	const std::string numeric_reply = createNumericReply(this->_numeric_return, user_nick,
															target, reason);
	this->getScheduler().queueMessage(client_id, numeric_reply, true);

}

// OPERATOR OVERLOADS //
std::ostream& operator<<(std::ostream& os, const Command& cmd)
{
	os << "=================\n"
		<< "Command [" << cmd.getCmdName() << "]\n";
	std::list<std::string>	params = cmd.getParams();
	std::list<std::string>::iterator it = params.begin(), end = params.end();
	for (size_t i = 0; it != end; it++, i++)
		std::cout << "Param " << i << "=> |" << it->data() << "|\n";
	std::cout << "Numeric return => " << cmd.getNumericReturn() << std::endl \
		<< "Chars read => " << cmd.getCharsRead() << std::endl;
	os << "================\n\n";
    return os;
}

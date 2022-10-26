#include "Command.hpp"

Command::Command(exec_fn_map &fn_map,
				std::string *raw_command,
				Scheduler & scheduler,
				Server & server)
:_chars_read(0), _function_map(fn_map), _scheduler(scheduler),
	_server(server)
{
	this->_setupCommand(*raw_command);
	std::cout << "coucou => " << *raw_command << std::endl;
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
Scheduler &		Command::getScheduler(void) {
	return (this->_scheduler);
}
Server &	Command::getServer(void) {
	return (this->_server);
}

// Creates the cmdType and params for the command
// TODO: This function needs to check for the presence of /r/d or whatever,
// extract the substring(s), and the program must
// delete the buffer up until that point 
void	Command::_setupCommand(std::string raw_command) {
	// If a separator is in buffer, do stuff
	if (raw_command.find(IRC_MSG_SEPARATOR) != raw_command.npos) {
		std::cout << "la\n";
		std::string command = raw_command.substr(0, raw_command.find(IRC_MSG_SEPARATOR));
		this->_chars_read = command.length() + std::strlen(IRC_MSG_SEPARATOR);
		while (!command.empty()) {
			// We dont support prefixes but it should be here first
			if (command.at(0) == ':') {
				this->_params.push_back(command);
				command.erase();
			}
			else {
				std::size_t space = command.find_first_of(' ');
				if (space == command.npos) {
					std::cout << "ici\n";
					space = command.length();
				}
				this->_params.push_back(command.substr(0, space));
				command.erase(0, space + 1);
			}
		}
	}
	if (!this->_params.empty()) {
		this->_cmd_name = this->_params.front();
		this->_params.pop_front();
	}
	// return the size of what you just read for deletion from buffer
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
	std::cout << "Execution starts =>\t";
	// TODO: using an iterator to .find(cmd_name) would make it possible to avoid
	// having to use .find 2 times, but i can't make the syntax work
	if (this->_function_map.find(this->_cmd_name) == this->_function_map.end()) {
		std::cout << "command "<<this->_cmd_name<<" not found\n";
		this->_numeric_return = ERR_UNKNOWNCOMMAND; // -1 if command doesnt exist
		if (users.find(client_id) != users.end())
			this->getScheduler().queueMessage(client_id, errUnknownCommand(*this, users.at(client_id).getNick()), true);
		else
			this->getScheduler().queueMessage(client_id, errUnknownCommand(*this, "*"), true);
	} 
	else {
		std::cout << "command "<<this->_cmd_name<<" execution\n";
		this->_numeric_return = // you can admire the very intuitive syntax lmao
			this->_function_map.find(this->_cmd_name)->second(*this, client_id, users, channels);
	}
}

// OPERATOR OVERLOADS //
std::ostream& operator<<(std::ostream& os, const Command& cmd)
{
	os << "COMMAND OUTPUT START=================\n"
		<< "Command [" << cmd.getCmdName() << "]\n";
	std::list<std::string>	params = cmd.getParams();
	std::list<std::string>::iterator it = params.begin(), end = params.end();
	for (size_t i = 0; it != end; it++, i++)
		std::cout << "Param " << i << "=> |" << it->data() << "|\n";
	std::cout << "Numeric return => " << cmd.getNumericReturn() << std::endl \
		<< "Chars read => " << cmd.getCharsRead() << std::endl;
	os << "COMMAND OUTPUT END=================\n\n";
    return os;
}

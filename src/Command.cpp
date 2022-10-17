#include "Command.hpp"

Command::Command(std::string *raw_command)
{
	this->_createParams(*raw_command);
	std::cout << "coucou => " << *raw_command << std::endl;
}

Command::~Command() {}

// GETTERS
std::string	const &Command::getCmdType(void) const {
	return (this->_cmd_type);
}
std::list<std::string>	Command::getParams(void) const {
	return (this->_params);
}


// Creates the cmdType and params for the command
void	Command::_createParams(std::string raw_command) {
	// Start by deleting the \n at the end, will need to be changed
	// to instead delete le /r/n or whatever
	// if std::string.ends_with ? or .contains()
	raw_command.erase(raw_command.find_first_of('\n'), 1);
	if (!raw_command.empty()) {
		std::istringstream sstream(raw_command);
		std::string token;

		while (std::getline(sstream, token, ' '))
			this->_params.push_back(token);
		
		this->_cmd_type = this->_params.begin()->data();
		this->_params.pop_front();
	}
}

// OPERATOR OVERLOADS //
std::ostream& operator<<(std::ostream& os, const Command& cmd)
{
	os << "COMMAND OUTPUT START=================\n"
		<< "Command [" << cmd.getCmdType() << "]\n";
	std::list<std::string>	params = cmd.getParams();
	std::list<std::string>::iterator it = params.begin();
	for (size_t i = 0; it != params.end(); it++, i++)
		std::cout << "Param " << i << "=> " << it->data() << "\n";
	os << "COMMAND OUTPUT END=================\n";
    return os;
}

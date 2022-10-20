#include "Command.hpp"

Command::Command(std::string *raw_command)
{
	this->_createParams(*raw_command);
	this->_cmd_id = this->_getCommandId();
	this->_set_login_functions();
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

// INITIALIZATION OF MAPS OF FUNCTION POINTERS
// LOGIN MAP
// Here is a good example of how easy it will be to add more function
void	Command::_set_login_functions(void) {
	login_fn_map::iterator it = this->_login_functions.begin();
	this->_login_functions.insert(
			it,
			std::pair<unsigned int, login_fn>(1, &Command::_pass));
}
// GENERAL EXEC MAP, soon TM
// void	Command::_set_all_functions_map(void) {
// }
// PARSING

/**
 * @brief Get a unique ID identifying each command
 * 
 * @return Defaults to 0 if unknown command, else the appropriate command id
 */
unsigned int	Command::_getCommandId() const {
	if (this->_cmd_type.compare("PASS") == 0)
		return (1);
	if (this->_cmd_type.compare("NICK") == 0)
		return (2);
	if (this->_cmd_type.compare("USER") == 0)
		return (3);
	return (0);
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

// EXECUTION PIPELINE
void	Command::execute(
	unsigned int	client_id,
	t_users			&users,
	t_channels		&channels)
{
	(void)channels;
	std::cout << "ici\n";
	// cmd_id will be 0 only if command doesn't exist
	if (this->_cmd_id == 0)
		this->_numeric_return = -1; // -1 if command doesnt exist
	// cmd_id <= 3 ? login functions
	else if (this->_cmd_id <= 3) {
		std::cout << "la\n";
		this->_numeric_return = // you can admire the very intuitive syntax lmao
			(this->*(_login_functions.find(this->_cmd_id))->second)(client_id, users);
	} else {
		// the rest of the functions, call
	}
}

// EXECUTION METHODS START //

// PASS: Sets the password for the user
// Must check if password has been given, not sure if more than one arg atm,
// and must check if the user is already registered on the server
unsigned int	Command::_pass(unsigned int client_id, t_users &users) {
	std::cout << "PASS command spotted\n";
	// Error checking
	if (this->_params.empty())
		return (ERR_NEEDMOREPARAMS);
	if (users.find(client_id) == users.end())
		return (ERR_ALREADYREGISTERED);
	// Error checking done ->
	// create a new User and insert it inside the map
	User	new_user(client_id);
	users.insert(
		users.begin(), 
		std::pair<unsigned int, User>(client_id, new_user));
	return (0);
}

unsigned int	Command::_nick(unsigned int client_id, t_users &users) {
	(void)client_id;(void)users;
	std::cout << "NICK command spotted\n";
	return (0);
}

unsigned int	Command::_user(unsigned int client_id, t_users &users) {
	(void)client_id;(void)users;
	std::cout << "USER command spotted\n";
	return (0);
}


// EXECUTION METHODS END //


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

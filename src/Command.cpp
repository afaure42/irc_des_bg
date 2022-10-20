#include "Command.hpp"

Command::Command(std::string *raw_command)
{
	this->_createParams(*raw_command);
	this->_setFunctionMap();
	std::cout << "coucou => " << *raw_command << std::endl;
}

Command::~Command() {}


// GETTERS
std::string	const &Command::getCmdName(void) const {
	return (this->_cmd_name);
}
std::list<std::string>	Command::getParams(void) const {
	return (this->_params);
}
int Command::getNumericReturn(void) const {
	return (this->_numeric_return);
}

// Creates the cmdType and params for the command
// TODO: This function needs to check for the presence of /r/d or whatever,
// extract the substring(s), and the program must
// delete the buffer up until that point 
void	Command::_createParams(std::string raw_command) {
	// Start by deleting the \n at the end, will need to be changed
	raw_command.erase(raw_command.find_first_of('\n'), 1);
	// while find(\r\d), do that and count the size of what you find
	if (!raw_command.empty()) {
		std::istringstream sstream(raw_command);
		std::string token;

		while (std::getline(sstream, token, ' ')) {
			this->_params.push_back(token);
		}
		this->_cmd_name = this->_params.begin()->data();
		this->_params.pop_front();
	}
	// return the size of what you just read for deletion from buffer
}

// INITIALIZATION OF MAPS OF FUNCTION POINTERS
// LOGIN MAP
// Here is a good example of how easy it will be to add more functions
// in the future: simply add to the map the pair <cmd_id, function>
void	Command::_setFunctionMap(void) {
	exec_fn_map::iterator it = this->_command_functions.begin();
	this->_command_functions.insert(
			it,	fn_map_pair("PASS", &Command::_pass));
	this->_command_functions.insert(
			it,	fn_map_pair("NICK", &Command::_nick));
	this->_command_functions.insert(
			it,	fn_map_pair("USER", &Command::_user));
}
// GENERAL EXEC MAP, soon TM
// void	Command::_set_all_functions_map(void) {
// }
// PARSING

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
	if (this->_command_functions.find(this->_cmd_name) == this->_command_functions.end()) {
		std::cout << "command "<<this->_cmd_name<<" not found\n";
		this->_numeric_return = -1; // -1 if command doesnt exist
	} 
	else {
		std::cout << "command "<<this->_cmd_name<<" execution\n";
		this->_numeric_return = // you can admire the very intuitive syntax lmao
			(this->*(_command_functions.find(this->_cmd_name))->second)(client_id, users, channels);
	}
}

// EXECUTION METHODS START //

// PASS: Sets the password for the user
// Must check if password has been given, not sure if more than one arg atm,
// and must check if the user is already registered on the server
unsigned int	Command::_pass(	unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;
	std::cout << "PASS command execution:\t";
	// Error checking
	if (this->_params.empty()) {
		std::cout << "No parameters\n";
		return (ERR_NEEDMOREPARAMS);
	}
	if (users.find(client_id) != users.end()) {
		std::cout << "Client already registered\n";
		return (ERR_ALREADYREGISTERED);
	}
	// Error checking done ->
	// create a new User and insert it inside the map
	User	new_user(client_id);
	users.insert(
		users.begin(), 
		std::pair<unsigned int, User>(client_id, new_user));
	std::cout << "execution succesful\n";
	return (0);
}

// NICK: Sets the nickname for the user
unsigned int	Command::_nick(	unsigned int client_id,
								t_users &users,
								t_channels &channels) {
	(void)client_id;(void)users;(void)channels;
	std::cout << "NICK command execution\n";
	return (0);
}

// rtfm? LOL
unsigned int	Command::_user(	unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)client_id;(void)users;(void)channels;
	std::cout << "USER command execution\n";
	return (0);
}


// EXECUTION METHODS END //


// OPERATOR OVERLOADS //
std::ostream& operator<<(std::ostream& os, const Command& cmd)
{
	os << "COMMAND OUTPUT START=================\n"
		<< "Command [" << cmd.getCmdName() << "]\n";
	std::list<std::string>	params = cmd.getParams();
	std::list<std::string>::iterator it = params.begin();
	for (size_t i = 0; it != params.end(); it++, i++)
		std::cout << "Param " << i << "=> " << it->data() << "\n";
	std::cout << "Numeric return => " << cmd.getNumericReturn() << std::endl;
	os << "COMMAND OUTPUT END=================\n";
    return os;
}

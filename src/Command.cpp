#include "Command.hpp"

Command::Command(exec_fn_map &fn_map, std::string *raw_command) :
	 _chars_read(0), _function_map(fn_map)
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

// Creates the cmdType and params for the command
// TODO: This function needs to check for the presence of /r/d or whatever,
// extract the substring(s), and the program must
// delete the buffer up until that point 
void	Command::_setupCommand(std::string raw_command) {
	// If a separator is in buffer, do stuff
	if (raw_command.find(IRC_MSG_SEPARATOR) != raw_command.npos) {
		std::string command = raw_command.substr(0, raw_command.find(IRC_MSG_SEPARATOR));
		this->_chars_read = command.length();
		command.erase(command.find_first_of('\n'), 1);
		// while find(\r\d), do that and count the size of what you find
		if (!command.empty()) {
			std::istringstream sstream(command);
			std::string token;

			while (std::getline(sstream, token, ' ')) {
				this->_params.push_back(token);
			}
			this->_cmd_name = this->_params.begin()->data();
			this->_params.pop_front();
		}
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
		this->_numeric_return = -1; // -1 if command doesnt exist
	} 
	else {
		std::cout << "command "<<this->_cmd_name<<" execution\n";
		this->_numeric_return = // you can admire the very intuitive syntax lmao
			this->_function_map.find(this->_cmd_name)->second(*this, client_id, users, channels);
	}
}

// EXECUTION METHODS START //

// PASS: Sets the password for the user, can be done as many times as the user wants
// while he is not registered
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
	t_users::iterator it = users.find(client_id);

	//if user doesnt exist, create it
	if (it == users.end())
		it = users.insert(std::make_pair(client_id, User(client_id))).first;
	//std::map insert returns a pair of iterator, bool, we just need the iterator

	if (it->second.isRegistered()) {
		std::cout << "Client already registered\n";
		return (ERR_ALREADYREGISTERED);
	}
	// Error checking done ->

	// set the password to the param given
	it->second.setConnectPass(this->_params.front());

	std::cout << "execution succesful\n";
	return (0);
}


// NICK: Sets the nickname for the user
unsigned int	Command::_nick(	unsigned int client_id,
								t_users &users,
								t_channels &channels)
{
	(void)channels;
	t_users::iterator user_it = users.find(client_id);
	if (user_it == users.end())
		return (ERR_WRONGORDER);

	if (this->_params.empty())
		return (ERR_NONICKNAMEGIVEN);

	//BASIC SYNTAX CHEKS MAY NEED TO DO MORE LATER
	std::string & nick = this->_params.front();
	if(nick.length() > 9)
		return (ERR_ERRONEUSNICKNAME);
		
	//check for nickname in use
	for(t_users::iterator it = users.begin(); it != users.end(); it++)
	{
		if (it->second.getNick() == nick)
			return (ERR_NICKNAMEINUSE);
	}

	//err checking done

	user_it->second.setNick(nick);
	return (0);
}

// rtfm? LOL
unsigned int	Command::_user(	unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;
	int modes = 0;
	if (this->_params.size() < 4)
		return (ERR_NEEDMOREPARAMS);

	t_users::iterator user_it = users.find(client_id);

	if (user_it == users.end())
		return (ERR_WRONGORDER);
	
	//i dont understand what syntax checks are needed for now
	//https://datatracker.ietf.org/doc/html/rfc2812#section-2.3.1

	//set username
	user_it->second.setUsername(this->_params.front());
	this->_params.pop_front();

	//setting modes using some bitshifting (sorryyy i love this too much)
	modes = std::atoi(this->_params.front().c_str());
	if (modes & USR_MODE_i)
		user_it->second.setInvisStatus(true);
	if (modes & USR_MODE_w)
		user_it->second.setWallopStatus(true);
	this->_params.pop_front();
	this->_params.pop_front(); //3rd parameter unused

	//setting realname
	user_it->second.setRealname(this->_params.front());

	//TODO: SEND RPL 001 to 004

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
	std::list<std::string>::iterator it = params.begin(), end = params.end();
	for (size_t i = 0; it != end; it++, i++)
		std::cout << "Param " << i << "=> " << it->data() << "\n";
	std::cout << "Numeric return => " << cmd.getNumericReturn() << std::endl;
	os << "COMMAND OUTPUT END=================\n";
    return os;
}

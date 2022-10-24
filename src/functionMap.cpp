#include "functionMap.hpp"
#include "Command.hpp"

// INITIALIZATION OF MAPS OF FUNCTION POINTERS
// LOGIN MAP
// To add more functions to he map,	simply add to the map
// the pair <string, function>
// Don't forget to declare it's prototype in functionMap.hpp or it wont work
void	setFunctionMap(exec_fn_map &function_map) {
	exec_fn_map::iterator	it = function_map.begin();

	function_map.insert(
			it,	fn_map_pair("PASS", pass));
	function_map.insert(
			it,	fn_map_pair("NICK", nick));
	function_map.insert(
			it,	fn_map_pair("USER", user));
}

// EXECUTION METHODS START //

// PASS: Sets the password for the user
// Must check if password has been given, not sure if more than one arg atm,
// and must check if the user is already registered on the server
unsigned int	pass(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;
	std::list<std::string> const	params = command.getParams();
	std::cout << "PASS command execution:\t";
	// Error checking
	if (params.empty()) {
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
unsigned int	nick(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels) {
	(void)command;(void)client_id;(void)users;(void)channels;
	std::cout << "NICK command execution\n";
	return (0);
}

// rtfm? LOL
unsigned int	user(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)command;(void)client_id;(void)users;(void)channels;
	std::cout << "USER command execution\n";
	return (0);
}


// EXECUTION METHODS END //
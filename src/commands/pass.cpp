#include "functionMap.hpp"

// PASS: Sets the password for the user
// Must check if password has been given, not sure if more than one arg atm,
// and must check if the user is already registered on the server
unsigned int	pass(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;
	std::list<std::string> const	params = command.getParams();
	t_users::iterator it = users.find(client_id);
	std::cout << "PASS command execution:\n";
	// Error checking

	if (it->second.isRegistered())
		return (ERR_ALREADYREGISTERED);
	if (params.empty())
		return (ERR_NEEDMOREPARAMS);

	//std::map insert returns a pair of iterator, bool, we just need the iterator

	if (it->second.isRegistered()) {
		return (ERR_ALREADYREGISTERED);
	}

	// Error checking done ->
	// set the password to the param given
	it->second.setConnectPass(params.front());
	std::cout << "password set is\"" << it->second.getConnectPass() << "\"\n";

	std::cout << "execution succesful\n";
	return (0);
}

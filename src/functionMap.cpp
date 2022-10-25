#include "functionMap.hpp"


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
	function_map.insert(
			it, fn_map_pair("PRIVMSG", privmsg));
	function_map.insert(
			it, fn_map_pair("JOIN", join));
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
	it->second.setConnectPass(params.front());

	std::cout << "execution succesful\n";
	return (0);
}

// NICK: Sets the nickname for the user
unsigned int	nick(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels) {
	(void)channels;
	std::list<std::string> const	params = command.getParams();

	t_users::iterator user_it = users.find(client_id);
	if (user_it == users.end())
		return (ERR_WRONGORDER);

	if (params.empty())
		return (ERR_NONICKNAMEGIVEN);

	//BASIC SYNTAX CHEKS MAY NEED TO DO MORE LATER
	const std::string & nick = params.front();
	if(!isValidNick(nick))
		return (ERR_ERRONEUSNICKNAME);
		
	//check for nickname in use
	for(t_users::iterator it = users.begin(); it != users.end(); it++)
	{
		if (it->second.getNick() == nick)
			return (ERR_NICKNAMEINUSE);
	}

	//err checking done

	std::cout << "NICK command execution\n";
	user_it->second.setNick(nick);
	return (0);
}

// rtfm? LOL
unsigned int	user(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;
	std::list<std::string>	params = command.getParams();

	if (params.size() < 4)
		return (ERR_NEEDMOREPARAMS);

	t_users::iterator user_it = users.find(client_id);

	if (user_it == users.end())
		return (ERR_WRONGORDER);
	
	if (user_it->second.getConnectPass() != command.getServer().getPass())
	{
		command.getScheduler().queueMessage(client_id, "Invalid Password\n", false);
		return (0);
	}
	
	
	//i dont understand what syntax checks are needed for now
	//https://datatracker.ietf.org/doc/html/rfc2812#section-2.3.1

	//set username
	user_it->second.setUsername(params.front());
	params.pop_front();

	params.pop_front(); //2nd parameter unused
	params.pop_front(); //3rd parameter unused
	//setting hostname
	user_it->second.setHostName(command.getServer().getClientHost(client_id));

	//setting realname
	user_it->second.setRealname(params.front());

	//TODO: SEND RPL 001 to 004
	std::stringstream ss;
	ss << ":irc_des_bg 001 Welcome to irc_des_bg " << user_it->second.getNick()
	<< '!' << user_it->second.getUsername() << '@' << user_it->second.getHostName()
	<<  "\r\n";

	command.getScheduler().queueMessage(client_id, ss.str(), true);

	std::cout << "USER command execution\n";
	return (0);
}

#include "functionMap.hpp"

unsigned int	nick(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels) {
	(void)channels;
	std::list<std::string> const	params = command.getParams();
	User & current_user = users.at(client_id);

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
	current_user.setNick(nick);
	return (0);
}

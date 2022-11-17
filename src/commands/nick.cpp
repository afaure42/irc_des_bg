#include "functionMap.hpp"

unsigned int	nick(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels) {
	(void)channels;
	std::list<std::string> const	params = command.getParams();
	std::string old_nick; //need some rhum ?
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
	old_nick = current_user.getNick();

	current_user.setNick(nick);
	if (old_nick != TEMP_NICK)
	{
		std::string msg = ":" + old_nick + " NICK " + nick + IRC_MSG_SEPARATOR;

		sendAllUser(command.getScheduler(), users, msg, 0);
	}
	return (0);
}

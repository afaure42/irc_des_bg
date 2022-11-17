#include "functionMap.hpp"

unsigned int	nick(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels) {
	(void)channels;
	std::list<std::string> const	params = command.getParams();
	std::string old_nick; //need some rhum ?
	std::string rply;
	User & current_user = users.at(client_id);

	if (params.empty())
	{
		if (current_user.isRegistered())
			return (ERR_NONICKNAMEGIVEN);
		else
		{
			rply = std::string("ERROR :") + ERR_NONICKNAMEGIVEN_MSG + IRC_MSG_SEPARATOR;
			command.getScheduler().queueMessage(client_id, rply, true);
			return (0);
		}
	}
	//BASIC SYNTAX CHEKS MAY NEED TO DO MORE LATER
	const std::string & nick = params.front();
	if(!isValidNick(nick))
	{
		if (current_user.isRegistered())
			return (ERR_ERRONEUSNICKNAME);
		else
		{
			rply = std::string("ERROR :") + ERR_ERRONEUSNICKNAME_MSG + IRC_MSG_SEPARATOR;
			command.getScheduler().queueMessage(client_id, rply, true);
			return (0);
		}
	}
		
	//check for nickname in use
	for(t_users::iterator it = users.begin(); it != users.end(); it++)
	{
		if (it->second.getNick() == nick)
		{
			if (current_user.isRegistered())
				return (ERR_NICKNAMEINUSE);
			else
			{
				rply = std::string("ERROR :") + ERR_NICKNAMEINUSE_MSG
				+ IRC_MSG_SEPARATOR;
				command.getScheduler().queueMessage(client_id, rply, true);
				return (0);
			}
		}
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

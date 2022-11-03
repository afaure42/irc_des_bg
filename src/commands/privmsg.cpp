#include "functionMap.hpp"

unsigned int	privmsg(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	std::list<std::string>	params = command.getParams();

	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);
	
	if (params.back().empty() || params.back()[0] != ':' || params.back().size() == 1)
		return (ERR_NOTEXTTOSEND);

	User & current_user = users.at(client_id);
	std::string from = ":" + current_user.getFullName() + " PRIVMSG ";

	//TODO implement a map of nick -> id 
	//to not have to iterate through all clients
	//but this will add a parameter to the command
	//or just add it ass a reference member in the command

	std::string msg = from + params.front() + " " + params.back() + IRC_MSG_SEPARATOR;

	//first try to find channel
	t_channels::iterator ch_it = findChannel(params.front(), channels);
	if (ch_it != channels.end())
	{
		if(ch_it->getMembers().find(client_id) != ch_it->getMembers().end())
			ch_it->send(command.getScheduler(), msg, client_id);
		else
		{
			std::string rply = createNumericReply(ERR_CANNOTSENDTOCHAN,
							users.at(client_id).getFullName(), "", ERR_CANNOTSENDTOCHAN_MSG);
			command.getScheduler().queueMessage(client_id, rply, true);
		}
	}
	else//if no channel found then try to find user
	{
		t_users::iterator usr_it = findUser(params.front(), users);
		if (usr_it != users.end())
		{
			if (!usr_it->second.isAway())
				command.getScheduler().queueMessage(usr_it->first, msg, true);
			else
			{
				std::string rply = createNumericReply(RPL_AWAY, current_user.getFullName(),
									usr_it->second.getFullName(), usr_it->second.getAwayMsg());
				command.getScheduler().queueMessage(client_id, rply, true);
			}
		}
		else //no channel nor user found then return ERR
			return (ERR_NOSUCHNICK);
	}

	std::cout << "PRIVMSG command execution\n";
	return (0);
}

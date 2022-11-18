#include "functionMap.hpp"

unsigned int	privmsg(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	std::list<std::string>	params = command.getParams();
	User & current_user = users.at(client_id);

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);

	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);
	
	if (params.back().empty() || params.back()[0] != ':' || params.back().size() == 1)
		return (ERR_NOTEXTTOSEND);

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
		members_perms_t::iterator perm_it = ch_it->getPermissions().find(client_id);
		bool to_send = true;
		//checking if user it not on channel and outside msg are forbidden
		if (perm_it == ch_it->getPermissions().end() 
			&& ch_it->getModes() & Channel::NO_MSG_FROM_OUTSIDE)
			to_send = false;
		//checking if channel is moderated and user doesnt have rights to speak
		if (perm_it != ch_it->getPermissions().end()
			&& ch_it->getModes() & Channel::MODERATED
			&& !(perm_it->second & Channel::VOICE)
			&& !(perm_it->second & Channel::OPERATOR))
			to_send = false;

		if (to_send)
			ch_it->send(command.getScheduler(), msg, client_id);
		else
		{
			std::string rply = createNumericReply(ERR_CANNOTSENDTOCHAN,
							current_user.getNick(), ch_it->getName(), ERR_CANNOTSENDTOCHAN_MSG);
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
				std::string rply = createNumericReply(RPL_AWAY, current_user.getNick(),
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

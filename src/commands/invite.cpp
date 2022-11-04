#include "functionMap.hpp"

unsigned int	invite(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	t_stringlist params = command.getParams();
	User & current_user = users.at(client_id);
	std::string rply;
	std::string from = ':' + current_user.getFullName() + " INVITE ";

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);

	t_users::iterator usr_it = findUser(params.front(), users);
	if (usr_it == users.end())
		return (ERR_NOSUCHNICK);
	params.pop_front();

	t_channels::iterator ch_it = findChannel(params.front(), channels);
	if (ch_it == channels.end())
	{
		rply = createNumericReply(ERR_NOSUCHCHANNEL, current_user.getNick(),
					params.front(), ERR_NOSUCHCHANNEL_MSG);
		command.getScheduler().queueMessage(client_id, rply, true);
		return (0);
	}

	if (std::find(current_user.getChannels().begin(),
					current_user.getChannels().end(),
					params.front())
		== current_user.getChannels().end())
	{
		rply = createNumericReply(ERR_NOTONCHANNEL, current_user.getNick(),
					params.front(), ERR_NOTONCHANNEL_MSG);
		command.getScheduler().queueMessage(client_id, rply, true);
		return (0);
	}


	if (ch_it->getMembers().find(usr_it->first) != ch_it->getMembers().end())
	{
		rply = createNumericReply(ERR_USERONCHANNEL, current_user.getNick(),
			usr_it->second.getNick() + " " + ch_it->getName(), ERR_USERONCHANNEL_MSG);
		command.getScheduler().queueMessage(client_id, rply, true);
		return (0);
	}
	
	if (ch_it->getModes() & Channel::INVITE_ONLY &&
			!(ch_it->getPermissions().find(client_id)->second
			& Channel::OPERATOR))
	{
		rply = createNumericReply(ERR_CHANOPRIVSNEEDED, current_user.getNick(),
			ch_it->getName(), ERR_CHANOPRIVSNEEDED_MSG);
		command.getScheduler().queueMessage(client_id, rply, true);
		return (0);
	}
	
	if (usr_it->second.isAway())
	{
		rply = createNumericReply(RPL_AWAY, current_user.getNick(),
				usr_it->second.getFullName(), usr_it->second.getAwayMsg());
		command.getScheduler().queueMessage(client_id, rply, true);
		return (0);
	}

	//add to invite list
	ch_it->getPermissions().insert(std::make_pair(usr_it->first, Channel::INVITED));

	rply = from + usr_it->second.getNick() + " " + ch_it->getName() + IRC_MSG_SEPARATOR;
	command.getScheduler().queueMessage(usr_it->first, rply, true);

	rply = createNumericReply(RPL_INVITING, current_user.getNick(), usr_it->second.getNick() + " " +
						ch_it->getName(), "");
	command.getScheduler().queueMessage(client_id, rply, true);
	return (0);
}

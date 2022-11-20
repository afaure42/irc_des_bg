
#include "functionMap.hpp"

std::string who_reply(User & target)
{
	std::string ret = "* " + target.getUsername() + " " + target.getHostName() + " "
					+ SERVER_NAME + " " + target.getNick();

	ret += " ";
	ret += target.isAway() ? "G" : "H";
	if (target.isOp()) ret += "*";

	return ret;
}

std::string	who_reply(User & target, Channel & channel)
{
	std::string ret = channel.getName() + " " + target.getUsername() + " " + target.getHostName() + " "
					+ SERVER_NAME + " " + target.getNick();
	unsigned int & perms = channel.getPermissions()[target.getId()];
	
	ret += " ";

	//adding flags
	ret += target.isAway() ? "G" : "H";
	if (target.isOp()) ret += "*";
	if (perms & Channel::OPERATOR)
		ret += "@";
	else if (perms & Channel::VOICE)
		ret += "+";
	
	return ret;
}

unsigned int	who(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {

	t_stringlist	params = command.getParams();
	User & current_user = users.at(client_id);

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.empty())
		return (ERR_NEEDMOREPARAMS);
	
	t_channels::iterator ch_it = findChannel(params.front(), channels);
	t_users::iterator usr_it = findUser(params.front(), users);

	if (ch_it != channels.end())
	{
		for (members_t::iterator m_it = ch_it->getMembers().begin();
			m_it != ch_it->getMembers().end(); m_it++)
		{
			std::string rpl = who_reply(*(m_it->second), *ch_it);
			std::string arguments = "0 " + m_it->second->getRealname();

			std::string msg = createNumericReply(RPL_WHOREPLY, current_user.getNick(),
				rpl, arguments);
			
			command.getScheduler().queueMessage(client_id, msg, true);
		}
	}
	else if (usr_it != users.end())
	{
		std::string rpl = who_reply(usr_it->second);
		std::string arguments = "0 " + usr_it->second.getRealname();

		std::string msg = createNumericReply(RPL_WHOREPLY, current_user.getNick(),
				rpl, arguments);
		command.getScheduler().queueMessage(client_id, msg, true);
	}

	std::string msg = createNumericReply(RPL_ENDOFWHO, current_user.getNick(),
			params.front(), RPL_ENDOFWHO_MSG);
	command.getScheduler().queueMessage(client_id, msg, true);

	return (0);
}

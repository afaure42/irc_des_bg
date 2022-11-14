#include "functionMap.hpp"

unsigned int	notice(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	t_stringlist	params = command.getParams();
	User & current_user = users.at(client_id);

	if (!current_user.isRegistered())
		return (0);
	if (params.size() < 2)
		return (0);

	if (params.back().empty()
		|| params.back()[0] != ':'
		|| params.back().size() == 1)
		return (0);
	
	std::string from = ":" + current_user.getFullName() + " NOTICE ";

	std::string msg = from + params.front() + " " + params.back() + IRC_MSG_SEPARATOR;
	t_channels::iterator ch_it = findChannel(params.front(), channels);
	if (ch_it != channels.end())
	{
		if(ch_it->getMembers().find(client_id) != ch_it->getMembers().end())
			ch_it->send(command.getScheduler(), msg, client_id);
	}
	else//if no channel found then try to find user
	{
		t_users::iterator usr_it = findUser(params.front(), users);
		if (usr_it != users.end())
		{
			if (!usr_it->second.isAway())
				command.getScheduler().queueMessage(usr_it->first, msg, true);
		}
	}
	return (0);
}

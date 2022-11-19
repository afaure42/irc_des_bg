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

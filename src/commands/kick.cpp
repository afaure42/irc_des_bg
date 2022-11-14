#include "functionMap.hpp"

unsigned int	kick(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	t_stringlist	params = command.getParams();
	User & current_user = users.at(client_id);
	std::string msg = "Kicked by " + current_user.getNick();
	std::string reply;

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);

	t_stringlist	channel_list = split(params.front());
	params.pop_front();
	t_users::iterator target_it = findUser(params.front(), users);

	if (target_it == users.end())
	{
		reply = createNumericReply(ERR_NOSUCHNICK, current_user.getNick(),
				params.front(), ERR_NOSUCHNICK_MSG);
		command.getScheduler().queueMessage(client_id, reply, true);
		return (0);
	}

	params.pop_front();
	if (!params.empty() && params.front()[0] == ':')
		params.front().erase(0, 1);
	if (params.empty() && !params.front().empty())
		msg = params.front();

	while (channel_list.size() != 0)
	{
		t_channels::iterator ch_it =
				findChannel(channel_list.front(), channels);
		members_t::iterator ch_member;
		members_t::iterator ch_target;
		//check channel exists
		if (ch_it == channels.end())
		{
			reply = createNumericReply(ERR_NOSUCHCHANNEL, current_user.getNick(),
					params.front(), ERR_NOSUCHCHANNEL_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			channel_list.pop_front();
			continue;
		}

		ch_member = ch_it->getMembers().find(client_id);
		//check on channel
		if (ch_member == ch_it->getMembers().end())
		{
			reply = createNumericReply(ERR_NOTONCHANNEL, current_user.getNick(),
					ch_it->getName(), ERR_NOTONCHANNEL_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			channel_list.pop_front();
			continue;
		}

		ch_target = ch_it->getMembers().find(target_it->first);
		//check target on channel
		if (ch_target == ch_it->getMembers().end())
		{
			reply = createNumericReply(ERR_USERNOTINCHANNEL, target_it->second.getNick(),
					ch_it->getName(), ERR_USERNOTINCHANNEL_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			channel_list.pop_front();
			continue;
		}

		unsigned int member_modes = ch_it->getPermissions().find(client_id)->second;
		//check permissions on this channel
		if (!(member_modes & Channel::OPERATOR))
		{
			reply = createNumericReply(ERR_CHANOPRIVSNEEDED, current_user.getNick(),
					ch_it->getName(), ERR_CHANOPRIVSNEEDED_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			channel_list.pop_front();
			continue;
		}

		//actual kicking
		std::string part_str = "PART " + ch_it->getName() + " :"
				+ msg + IRC_MSG_SEPARATOR;
		Command temp_cmd(command.getFunctionMap(), command.getOperators(),
			&part_str, command.getScheduler(), command.getServer());
		temp_cmd.execute(target_it->first, users, channels);
		channel_list.pop_front();
	}
	return (0);
}

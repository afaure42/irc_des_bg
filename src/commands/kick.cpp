#include "functionMap.hpp"

unsigned int	kick(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	t_stringlist	params = command.getParams();
	User & current_user = users.at(client_id);
	std::string msg = "Kicked by " + current_user.getNick();
	std::string from = ":" + current_user.getFullName() + " KICK ";
	std::string reply;
	bool single_channel = false;
	bool single_user = false;

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);

	t_stringlist	channel_list = split(params.front());
	params.pop_front();
	t_stringlist	user_list = split(params.front());

	//because you can do #test user1,user2,user3 OR #test1,#test2,#test# user1
	if (channel_list.size() == 1)
		single_channel = true;
	else if (user_list.size() == 1)
		single_user = true;

	if (!single_channel && !single_user && channel_list.size() != user_list.size())
		return (ERR_NEEDMOREPARAMS);

	params.pop_front();

	if (!params.empty() && params.front()[0] == ':')
		params.front().erase(0, 1);
	if (params.empty() && !params.front().empty())
		msg = params.front();

	while (user_list.size() != 0 && channel_list.size() != 0)
	{
		t_channels::iterator ch_it =
				findChannel(channel_list.front(), channels);
		t_users::iterator target_it =
				findUser(user_list.front(), users);
	
		members_t::iterator ch_member;
		members_t::iterator ch_target;
		//check channel exists
		if (ch_it == channels.end())
		{
			reply = createNumericReply(ERR_NOSUCHCHANNEL, current_user.getNick(),
					channel_list.front(), ERR_NOSUCHCHANNEL_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			channel_list.pop_front();
			if (!single_user)
				user_list.pop_front();
			continue;
		}

		//check target exists
		if (target_it == users.end())
		{
			reply = createNumericReply(ERR_NOSUCHNICK, current_user.getNick(),
					user_list.front(), ERR_NOSUCHNICK_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			if (!single_channel)
				channel_list.pop_front();
			user_list.pop_front();
			continue;
		}


		ch_member = ch_it->getMembers().find(client_id);
		//check client on channel
		if (ch_member == ch_it->getMembers().end())
		{
			reply = createNumericReply(ERR_NOTONCHANNEL, current_user.getNick(),
					ch_it->getName(), ERR_NOTONCHANNEL_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			if (!single_channel)
				channel_list.pop_front();
			if (!single_user)
				user_list.pop_front();
			continue;
		}

		ch_target = ch_it->getMembers().find(target_it->first);
		//check target on channel
		if (ch_target == ch_it->getMembers().end())
		{
			reply = createNumericReply(ERR_USERNOTINCHANNEL, target_it->second.getNick(),
					ch_it->getName(), ERR_USERNOTINCHANNEL_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			if (!single_channel)
				channel_list.pop_front();
			if (!single_user)
				user_list.pop_front();
			continue;
		}

		unsigned int member_modes = ch_it->getPermissions().find(client_id)->second;
		//check permissions on this channel
		if (!(member_modes & Channel::OPERATOR))
		{
			reply = createNumericReply(ERR_CHANOPRIVSNEEDED, current_user.getNick(),
					ch_it->getName(), ERR_CHANOPRIVSNEEDED_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			if (!single_channel)
				channel_list.pop_front();
			if (!single_user)
				user_list.pop_front();
			continue;
		}

		// check for channel creator
		unsigned int target_modes = ch_it->getPermissions().find(target_it->first)->second;
		if (target_modes & Channel::CREATOR)
		{
			reply = createNumericReply(ERR_NOPRIVILEGES, current_user.getNick(),
					ch_it->getName(), ERR_NOPRIVILEGES_MSG);
			command.getScheduler().queueMessage(client_id, reply, true);
			if (!single_channel)
				channel_list.pop_front();
			if (!single_user)
				user_list.pop_front();
			continue;
		}
		
		//actual kicking
		std::string kick_str = from + ch_it->getName() + " " + target_it->second.getNick()
				+ " :" + msg + IRC_MSG_SEPARATOR;

		ch_it->send(command.getScheduler(), kick_str, 0);
		ch_it->removeUser(target_it->second);
		target_it->second.getChannels().erase(ch_it->getName());

		if (!single_channel)
			channel_list.pop_front();
		if (!single_user)
			user_list.pop_front();
	}
	return (0);
}

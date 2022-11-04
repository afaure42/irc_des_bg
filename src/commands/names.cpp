#include "functionMap.hpp"

static void list_channel_names(Channel & channel,
								Scheduler & scheduler,
								User & user)
{
	members_t::iterator members = channel.getMembers().begin();
	std::string prefix = SERVER_PREFIX + " 353 " + user.getNick() + " = " + channel.getName() + " :"; // " RPL CHANNEL " + ":" + CR-LF

	std::string names;
	std::string reply;
	while (members != channel.getMembers().end())
	{
		if (prefix.size() + names.size() + members->second->getNick().size() + 2 > 512)
		{
			names.append(IRC_MSG_SEPARATOR);
			scheduler.queueMessage(user.getId(), prefix + names, true);
			names.clear();
		}
		if (!names.empty())
			names += ' ';
		if (channel.getPermissions().at(members->first) & Channel::OPERATOR)
			names += '@';
		names += members->second->getNick();
		members++;
	}

	if (!names.empty())
		scheduler.queueMessage(user.getId(), prefix + names + IRC_MSG_SEPARATOR, true);
}

unsigned int names(Command & command,
					unsigned int client_id,
					t_users &users,
					t_channels &channels)
{
	std::list<std::string> params = command.getParams();
	User & current_user = users.at(client_id);

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.empty())
	{
		for(t_channels::iterator it = channels.begin(); it != channels.end(); it++)
		{
			if ((it->getModes() & Channel::SECRET)
				&& it->getMembers().find(client_id) != it->getMembers().end())
				continue;
			list_channel_names(*it, command.getScheduler(), current_user);
		}
		std::string endofnames = createNumericReply(RPL_ENDOFNAMES, current_user.getNick(),
						"", RPL_ENDOFNAMES_MSG);
		command.getScheduler().queueMessage(client_id, endofnames, true);
	}
	else
	{
		t_stringlist channel_names = split(params.front());
		while (!channel_names.empty())
		{
			t_channels::iterator it = findChannel(channel_names.front(), channels);
			if (it != channels.end())
			{
				list_channel_names(*it, command.getScheduler(), current_user);
				std::string endofnames =
					createNumericReply(RPL_ENDOFNAMES, current_user.getNick(),
					it->getName(), RPL_ENDOFNAMES_MSG);
				command.getScheduler().queueMessage(client_id, endofnames, true);
			}
			channel_names.pop_front();
		}
	}
	return (0);
}

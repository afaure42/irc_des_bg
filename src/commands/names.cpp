#include "functionMap.hpp"

static void list_channel_names(Channel & channel,
								Scheduler & scheduler,
								User & user)
{
	members_t::iterator members = channel.getMembers().begin();
	std::string prefix = SERVER_PREFIX + " 353 " + user.getFullName() + " = " + channel.getName() + " :"; // " RPL CHANNEL " + ":" + CR-LF

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
		if (channel.getPermissions().at(members->first) & Channel::OPERATOR)
			names += '@';
		names += members->second->getNick();
		names += ' ';
		members++;
	}

	if (!names.empty())
	{
		names.append(IRC_MSG_SEPARATOR);
		scheduler.queueMessage(user.getId(), prefix + names, true);
	}
}

unsigned int names(Command & command,
					unsigned int client_id,
					t_users &users,
					t_channels &channels)
{
	std::list<std::string> params = command.getParams();

	if (params.empty())
	{
		for(t_channels::iterator it = channels.begin(); it != channels.end(); it++)
		{
			if ((it->getModes() & Channel::SECRET)
				&& it->getMembers().find(client_id) != it->getMembers().end())
				continue;
			list_channel_names(*it, command.getScheduler(), users.at(client_id));
		}
	}
	else
	{
		t_channels::iterator it = findChannel(params.front(), channels);

		list_channel_names(*it, command.getScheduler(), users.at(client_id));
	}
	std::string endofnames = createNumericReply(RPL_ENDOFNAMES, users.at(client_id).getNick(),
												"", RPL_ENDOFNAMES_MSG);
	command.getScheduler().queueMessage(client_id, endofnames, true);
	return (0);
}

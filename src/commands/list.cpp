#include "functionMap.hpp"

static void sendChannelRPL(Scheduler & scheduler, t_channels::iterator & it, User & user)
{
	std::stringstream ss;
	ss << it->getName() << " " << it->getMembers().size();

	std::string rply = createNumericReply(RPL_LIST, user.getNick(),
			ss.str(), it->getTopic());

	scheduler.queueMessage(user.getId(), rply, true);
}

unsigned int	list(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	t_stringlist params = command.getParams();
	User & current_user = users.at(client_id);
	std::string rply;

	rply = createNumericReply(RPL_LISTSTART, current_user.getNick(), "",
			RPL_LISTSTART_MSG);
	command.getScheduler().queueMessage(client_id, rply, true);
	if (params.empty())
	{
		for(t_channels::iterator ch_it = channels.begin();
				ch_it != channels.end(); ch_it++)
		{
			if (ch_it->getModes() & Channel::PRIVATE)
				continue;
			sendChannelRPL(command.getScheduler(), ch_it, current_user);
		}
	}
	else
	{
		t_channels::iterator ch_it;
		t_stringlist channel_list = split(params.front());
		while (!channel_list.empty())
		{
			ch_it = findChannel(channel_list.front(), channels);
			if (ch_it != channels.end())
				sendChannelRPL(command.getScheduler(), ch_it, current_user);
			channel_list.pop_front();
		}
	}
	rply = createNumericReply(RPL_LISTEND, current_user.getNick(), "",
							RPL_LISTEND_MSG);
	command.getScheduler().queueMessage(client_id, rply, true);
	return (0);
}

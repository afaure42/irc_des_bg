#include "functionMap.hpp"

void sendTopic(Scheduler & scheduler, User & current_user, Channel & channel)
{
	std::string reply;

	if (!channel.getTopic().empty())
	{
		reply = createNumericReply(RPL_TOPIC, current_user.getNick(),
				channel.getName(), channel.getTopic());
	}
	else
	{
		reply = createNumericReply(RPL_NOTOPIC, current_user.getNick(),
				channel.getName(), RPL_NOTOPIC_MSG);
	}
	scheduler.queueMessage(current_user.getId(), reply, true);
}

unsigned int	topic(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	t_stringlist	params = command.getParams();
	User & current_user = users.at(client_id);
	std::string reply;

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.empty())
		return (ERR_NEEDMOREPARAMS);

	//checking channel existence and user on channel	
	t_channels::iterator ch_it = findChannel(params.front(), channels);
	if (ch_it == channels.end())
	{
		reply = createNumericReply(ERR_NOSUCHCHANNEL, current_user.getNick(),
				params.front(), ERR_NOSUCHCHANNEL_MSG);
		command.getScheduler().queueMessage(client_id, reply, true);
		return (0);
	}

	//PROCESSING OF SIMPLE TOPIC REQUEST
	if (params.size() == 1)
	{
		sendTopic(command.getScheduler(), current_user, *ch_it);
		return (0);
	}

	//PROCESSING OF TOPIC CHANGE REQUEST
	//checking user permissions
	members_t::iterator ch_member = ch_it->getMembers().find(client_id);
	if (ch_member == ch_it->getMembers().end())
	{
		reply = createNumericReply(ERR_NOTONCHANNEL, current_user.getNick(),
				ch_it->getName(), ERR_NOTONCHANNEL_MSG);
		command.getScheduler().queueMessage(client_id, reply, true);
		return (0);
	}
	unsigned int member_modes = ch_it->getPermissions().find(client_id)->second;
	if (ch_it->getModes() & Channel::TOPIC && !(member_modes & Channel::OPERATOR))
	{
		reply = createNumericReply(ERR_CHANOPRIVSNEEDED, current_user.getNick(),
				ch_it->getName(), ERR_CHANOPRIVSNEEDED_MSG);
		command.getScheduler().queueMessage(client_id, reply, true);
		return (0);
	}

	params.pop_front();
	if (!params.front().empty() && params.front()[0] == ':')
		params.front().erase(0, 1);
	ch_it->setTopic(params.front());
	reply = ":" + current_user.getFullName() + " TOPIC " + ch_it->getName() + " :" + ch_it->getTopic() + IRC_MSG_SEPARATOR;
	ch_it->send(command.getScheduler(), reply, 0);
	sendTopic(command.getScheduler(), current_user, *ch_it);
	return (0);
}

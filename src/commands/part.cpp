#include "functionMap.hpp"

unsigned int	part(	Command &command,
					unsigned int client_id,
					t_users &users,
					t_channels &channels )
{
	std::list<std::string> params = command.getParams();
	t_channels::iterator ch_it;

	User & current_user = users.at(client_id);
	std::string prefix = ":" + current_user.getFullName() + " PART";
	std::string suffix;
	std::string msg;

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.empty())
		return (ERR_NEEDMOREPARAMS);

	if (params.back()[0] != ':')
		suffix = IRC_MSG_SEPARATOR;
	else
	{
		suffix = params.back() + IRC_MSG_SEPARATOR;
		params.pop_back();
	}
	
	std::string			channel_name;
	size_t				delimiter;
	std::string			numeric_reply;
	const std::string	user_nick = current_user.getNick();
	
	while (!params.front().empty())
	{
		delimiter = params.front().find(",");

		if (delimiter != std::string::npos) {
			channel_name = params.front().substr(0, delimiter);
			params.front().erase(0, delimiter + 1);
		}
		else {
			channel_name = params.front();
			params.front().erase();
		}

		ch_it = findChannel(channel_name, channels);
		if (ch_it == channels.end()) {
			numeric_reply = createNumericReply(ERR_NOSUCHCHANNEL, user_nick,
							channel_name, ERR_NOSUCHCHANNEL_MSG);
			command.getScheduler().queueMessage(client_id, numeric_reply, true);
		}
		else if (ch_it->getMembers().find(client_id) == ch_it->getMembers().end()) {
			numeric_reply = createNumericReply(ERR_NOTONCHANNEL, user_nick,
							channel_name, ERR_NOTONCHANNEL_MSG);
			command.getScheduler().queueMessage(client_id, numeric_reply, true);
		}
		else {
			msg = prefix + " " + channel_name + " " + suffix;		
			ch_it->send(command.getScheduler(), msg, 0);
			ch_it->removeUser(current_user);
			current_user.getChannels().erase(ch_it->getName());
			if (ch_it->getMembers().empty())
				channels.erase(ch_it);
		}
	}
	return (0);
}

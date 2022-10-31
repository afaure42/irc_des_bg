#include "functionMap.hpp"

static t_stringlist	split(std::string str) {
	t_stringlist	list;
	size_t			delimiter;
	std::string		token;

	while (!str.empty()) {
		delimiter = str.find(",");
		if (delimiter != std::string::npos){
			token = str.substr(0, delimiter);
			str.erase(0, delimiter + 1);
		}
		else {
			token = str;
			str.erase();
		}
		list.push_back(token);
	}

	return (list);
}

unsigned int	join(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	t_stringlist	params = command.getParams();

	if (params.empty())
		return (ERR_NEEDMOREPARAMS);
	
	t_stringlist	channel_list = split(params.front());
	params.pop_front();
	t_stringlist	password_list ;
	if (!params.empty())
		password_list = split(params.back());

	while (channel_list.size() > 0)
	{
		//find if channel exists
		t_channels::iterator it = findChannel(channel_list.front(), channels);
		//if channel exists join it
		if (it != channels.end())
		{
			// TODO: CHECK USER PERMISSIONS
			it->joinChannel(command.getScheduler(), users.at(client_id));
		}
		else //else create it
		{
			// TODO: CHECK USER PERMISSIONS
			channels.push_back(Channel(channel_list.front()));
			channels.back().joinChannel(command.getScheduler(), users.at(client_id));
			it = channels.end() - 1;
		}

		if(!it->getTopic().empty())
			command.getScheduler()
			.queueMessage(client_id, ":irc_des_bg 332 " + it->getTopic(), true);
		channel_list.pop_front();
	}

	return (0);
}

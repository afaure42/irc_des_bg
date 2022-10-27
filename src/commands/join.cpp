#include "functionMap.hpp"

unsigned int	join(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	std::list<std::string> params = command.getParams();
	unsigned int ret = 0;

	if (params.size() < 1)
		return (ERR_NEEDMOREPARAMS);
	
	std::string from = users.at(client_id).getNick() + " JOIN ";

	while (params.size() > 0)
	{

		//find if channel exists
		t_channels::iterator it = findChannel(params.front(), channels);
		//if channel exists join it
		if (it != channels.end())
		{
			it->joinChannel(command.getScheduler(), users.at(client_id));
		}
		else //else create it
		{
			channels.push_back(Channel(params.front()));
			channels.back().joinChannel(command.getScheduler(), users.at(client_id));
			it = channels.end() - 1;
		}

		if (ret != 0)
			return ret;
		if(!it->getTopic().empty())
			command.getScheduler()
			.queueMessage(client_id, ":irc_des_bg 332 " + it->getTopic(), true);
		params.pop_front();
	}

	return 0;
}

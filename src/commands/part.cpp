#include "functionMap.hpp"

unsigned int	part(	Command &command,
					unsigned int client_id,
					t_users &users,
					t_channels &channels )
{
	std::list<std::string> params = command.getParams();
	t_channels::iterator ch_it;

	std::string prefix = ":" + users[client_id].getFullName() + " PART";
	std::string suffix;
	std::string msg;

	if (params.back()[0] != ':')
		suffix = ":User left\r\n";
	else
	{
		suffix = params.back() + "\r\n";
		params.pop_back();
	}
	
	while (params.size() > 0)
	{
		ch_it = findChannel(params.front(), channels);
		if (ch_it == channels.end())
			return (ERR_NOSUCHCHANNEL);		
		if (ch_it->getMembers().find(client_id) == ch_it->getMembers().end())
			return (ERR_NOTONCHANNEL);

		msg = prefix + " " + params.front() + " " + suffix;		
		ch_it->send(command.getScheduler(), msg, 0);
		// ch_it->getPermissions().erase(client_id);
		ch_it->getMembers().erase(client_id);
		params.pop_front();
	}
	return (0);
}

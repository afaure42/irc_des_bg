#include "functionMap.hpp"

unsigned int	ping(	Command &command,
					unsigned int client_id,
					t_users &users,
					t_channels &channels )
{
	(void)channels;(void)users;
	std::list<std::string> const	params = command.getParams();
	std::string msg;

	if (params.empty())
		return (ERR_NEEDMOREPARAMS);

	msg = "PONG " + SERVER_NAME + " " + params.front() + "\r\n";
	command.getScheduler().queueMessage(client_id, msg, true);
	return (0);
}

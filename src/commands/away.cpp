#include "functionMap.hpp"


unsigned int	away( Command &command,
							unsigned int client_id,
							t_users &users,
							t_channels &channels )
{
	(void)channels;
	std::list<std::string> params = command.getParams();
	
	User & current_user = users.at(client_id);
	std::string reply;

	if (params.empty())
	{
		current_user.setAwayStatus(false);
		current_user.setAwayMessage(std::string());

		reply = createNumericReply(RPL_UNAWAY, current_user.getFullName(), "",
								RPL_UNAWAY_MSG);
	}
	else
	{
		current_user.setAwayStatus(true);
		current_user.setAwayMessage(params.front().substr(1));

		reply = createNumericReply(RPL_NOAWAY, current_user.getFullName(), "",
									RPL_NOAWAY_MSG);
	}

	command.getScheduler().queueMessage(client_id, reply, true);
	return (0);
}

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

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.empty()) //removing away status
	{
		current_user.setAwayStatus(false);
		current_user.setAwayMessage(std::string());

		reply = createNumericReply(RPL_UNAWAY, current_user.getNick(), "",
								RPL_UNAWAY_MSG);
	}
	else //setting away status
	{
		current_user.setAwayStatus(true);
		current_user.setAwayMessage(params.front().substr(1));

		reply = createNumericReply(RPL_NOAWAY, current_user.getNick(), "",
									RPL_NOAWAY_MSG);
	}

	command.getScheduler().queueMessage(client_id, reply, true);
	return (0);
}

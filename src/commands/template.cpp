#include "functionMap.hpp"

unsigned int	cmd_name(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)command;(void)client_id;(void)users;(void)channels;
	return (0);
}
	// t_stringlist	params = command.getParams();
	// User & current_user = users.at(client_id);

	// if (!current_user.isRegistered())
	// 	return (ERR_NOTREGISTERED);
	// if (params.empty())
	// 	return (ERR_NEEDMOREPARAMS);

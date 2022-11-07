#include "functionMap.hpp"


unsigned int	die(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;

	User & current_user = users.at(client_id);

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	//TODO ADD SERVER OPERATOR PERMISSION CHECK
	if (!current_user.isOp())
		return (ERR_NOPRIVILEGES);
	command.getServer().setIsOn(false);
	return (0);
}

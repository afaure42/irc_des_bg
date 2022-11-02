#include "functionMap.hpp"


unsigned int	die(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;(void)client_id;(void)users;

	//TODO ADD SERVER OPERATOR PERMISSION CHECK
	command.getServer().setIsOn(false);
	return (0);
}

#include "functionMap.hpp"

static bool	isValidMode(std::string mode) {
	if (mode[0] != '-' && mode[0] != '+')
	return true;
}

unsigned int	mode(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {

	(void)command;(void)client_id;(void)users;(void)channels;
	t_stringlist params = command.getParams();
	t_stringlist::iterator params_it = params.begin();

	if (params.empty())
		return (RPL_UMODEIS);

	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);

	t_users::iterator user_it = users.find(client_id);
	if (user_it->second.getNick() != params_it->data())
		return (ERR_USERSDONTMATCH);
	
	params_it++;
	while (params_it != params.end()) {
		std::string param = params_it->data();
		if (param[0] != '-' && param[0] != '+') {
			; // invalid operand
		}
		
		params_it++;
	}

	
	return (0);
}
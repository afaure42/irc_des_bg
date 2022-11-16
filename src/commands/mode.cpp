#include "functionMap.hpp"

static bool	isValidMode(std::string mode) {
	char sign	= mode[0];
	char c		= mode[1];

	if ( (sign != '-' && sign != '+') || (c != 'a' && c != 'i') )
		return (false);
	return true;
}

unsigned int	mode(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	(void)command;(void)client_id;(void)users;(void)channels;
	t_stringlist 			params = command.getParams();
	t_stringlist::iterator	params_it = params.begin();

	if (params.empty()){
		return (ERR_NEEDMOREPARAMS);
	}
	// mode without args should (?) list all modes of user that sent the cmd

	if (params.size() < 2) {
		return (ERR_NEEDMOREPARAMS);
	}

	const t_users::iterator it 	= users.find(client_id);
	if (it == users.end())
		return (ERR_USERSDONTMATCH);
	const std::string user_nick = it->second.getNick();

	
	params_it++;
	while (params_it != params.end()) {
		std::string param = params_it->data();
		if (!isValidMode(param)) {
			const std::string numeric_reply = createNumericReply(ERR_UMODEUNKNOWNFLAG, user_nick,
																	"", ERR_UMODEUNKNOWNFLAG_MSG);
			command.getScheduler().queueMessage(client_id, numeric_reply, true);
		}
		params_it++;
	}
	return (0);
}

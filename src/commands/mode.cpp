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
								t_channels &channels ) {

	(void)command;(void)client_id;(void)users;(void)channels;
	t_stringlist params = command.getParams();
	t_stringlist::iterator params_it = params.begin();

	if (params.empty())
		return (RPL_UMODEIS);

	// mode without args should (?) list all modes of user that sent the cmd

	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);

	t_users::iterator user_it	= users.find(client_id);
	const std::string user_nick	= params_it->data();
	if (user_it->second.getNick() != user_nick)
		return (ERR_USERSDONTMATCH);
	
	params_it++;
	while (params_it != params.end()) {
		std::string param = params_it->data();
		if (!isValidMode(param)) {
			const std::string numeric_reply = createNumericReply(
												ERR_UMODEUNKNOWNFLAG,
												user_nick,
												"",
												"");
			command.getScheduler().queueMessage(client_id, numeric_reply, true);
		}
		params_it++;
	}
	return (0);
}
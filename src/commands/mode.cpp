#include "functionMap.hpp"

static inline bool isValidUserMode(char c) {
	return (c == 'i' || c == 'o');
}

static bool	isValidChanMode(char c) {
	return (c == 'i' || c == 'o' || c == 's');
}

void handleOperatorMode(Command & command,
			t_users users,
			Channel & chanel,
			User & current_user,
			std::string nick)
{
	t_users::iterator usr_it = findUser(nick, users);

	if (usr_it == users.end())
}

unsigned int	chanMode( Command &command,
							unsigned int client_id,
							t_users &users,
							User & current_user,
							t_channels &channels)
{
	t_stringlist params = command.getParams();
	std::string reply;

	t_channels::iterator ch_it = findChannel(params.front(), channels);
	
	if (ch_it == channels.end())
		return (ERR_NOTONCHANNEL);
	if (ch_it->getMembers().find(client_id) == ch_it->getMembers().end())
		return (ERR_NOTONCHANNEL);
	params.pop_front();

	if(params.empty())
	{
		//send chanelmodeis
	}

	std::string modes = params.front();
	params.pop_front();

	bool adding;
	switch(modes[0])
	{
	case '+':
		adding = true;
		break;
	case '-':
		adding = false;
		break;
	default:
		return (ERR_UNKNOWNMODE);
	}

	for(size_t i = 0; i < modes.size(); i++)
	{
		if (modes[i] == '+') {
			adding = true;
			continue;
		}
		else if (modes[i] == '-'){
			adding = false;
			continue;
		}
		if (!isValidChanMode(modes[i])) 
		{
			//TODO handle error
			return (ERR_UNKNOWNMODE);
		}
		else
		{
			switch(modes[i])
			{
			case 'i':
				ch_it->setInviteMode(adding);
				break;
			case 't':
				ch_it->setTopicMode(adding);
				break;
			case 'p':
				ch_it->setPrivateMode(adding);
				break;
			case 'o':
			{
				if (params.empty())
					return (ERR_NEEDMOREPARAMS);
				
			}
			}
		}
	}
}

static unsigned int	userMode( User & current_user,
							std::string & modes)
{
	bool adding;

	//checking first char is - or +
	switch(modes[0])
	{
	case '+':
		adding = true;
		break;
	case '-':
		adding = false;
		break;
	default:
		return (ERR_UMODEUNKNOWNFLAG);
	}

	for(size_t i = 0; i < modes.size(); i++)
	{
		if (modes[i] == '+'){
			adding = true;
			continue;
		}
		else if (modes[i] == '-') {
			adding = false;
			continue;
		}
	
		if (!isValidUserMode(modes[i]))
			//TOTO HANDLE ERROR
		else
		{
			switch(modes[i])
			{
			case 'i':
				current_user.setInvisStatus(adding);
				break;
			case 'o':
				if (!adding)
					current_user.setOperatorStatus(false);
				break;
			}

		}
	}
	return (0);
}

unsigned int	mode(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	User & current_user = users.at(client_id);
	t_stringlist params = command.getParams();
	t_stringlist::iterator params_it = params.begin();

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);

	// mode without args should (?) list all modes of user that sent the cmd

	if (params.size() < 1) {
		return (ERR_NEEDMOREPARAMS);
	}

	//CHANNEL MODE COMMAND
	if (!params.front().empty() && params.front()[0] == '#')
	{
		return (chanMode(command, client_id, users, current_user, channels));
	}
	else //USER MODE COMMAND
	{
		if (params.front() != current_user.getNick())
			return (ERR_USERSDONTMATCH);
		params.pop_front();
		if (params.empty())
			return (RPL_UMODEIS);		
		
		return (userMode(current_user, params.front()));
	}
	// params_it++;
	// while (params_it != params.end()) {
	// 	std::string param = params_it->data();
	// 	if (!isValidMode(param)) {
	// 		const std::string numeric_reply = createNumericReply(ERR_UMODEUNKNOWNFLAG, user_nick,
	// 																"", ERR_UMODEUNKNOWNFLAG_MSG);
	// 		command.getScheduler().queueMessage(client_id, numeric_reply, true);
	// 	}
	// 	params_it++;
	// }
	return (0);
}

#include "functionMap.hpp"

static bool	isModeFlag(char c)
{
	return !(c == '\0' || c == '+' || c == '-');
}

unsigned int	createChanUserPermMask(Command &command,
											User &current_user,
											std::string &operation,
											std::string &reply_changes)
{
	unsigned int	flag = 0;
	std::string		reply;
	size_t			i = 0;

	while (isModeFlag(operation[i])) {
		if (operation[i] == 'o')
				flag |= Channel::INVITE_ONLY;
		else if (operation[i] == 'v')
				flag |= Channel::VOICE;
		else {
			reply = createNumericReply(ERR_UMODEUNKNOWNFLAG, current_user.getNick(),
										"", ERR_UMODEUNKNOWNFLAG_MSG);
			command.getScheduler().queueMessage(current_user.getId(), reply, true);
		}
		i++;
	}
	reply_changes += operation.substr(0, i);
	operation.erase(0, i);
	return (flag);
}

unsigned int	createUserMask(Command &command,
										User &current_user,
										std::string &operation,
										std::string &reply_changes)
{
	unsigned int	flag = 0;
	std::string		reply;
	size_t			i = 0;

	while (isModeFlag(operation[i])) {
		switch (operation[i])
		{
			case 'i':
				flag |= User::INVISIBLE;
				break;
			case 'w':
				flag |= User::WALLOPS;
				break;
			case 'r':
				flag |= User::RESTRICTED;
				break;
			case 'o':
				flag |= User::OPERATOR;
				break;
			case 'O':
				flag |= User::LOCALOP;
				break;
			case 's':
				flag |= User::SRVNOTICES;
				break;
			default:
				reply = createNumericReply(ERR_UMODEUNKNOWNFLAG, current_user.getNick(),
											"", ERR_UMODEUNKNOWNFLAG_MSG);
				command.getScheduler().queueMessage(current_user.getId(), reply, true);
				break;
		}
		i++;
	}
	reply_changes += operation.substr(0, i);
	operation.erase(0, i);
	return (flag);
}

unsigned int	createChannelMask(Command &command,
										User &current_user,
										std::string &operation,
										std::string &reply_changes)
{
	unsigned int	flag = 0;
	std::string		reply;
	size_t			i = 0;

	while (isModeFlag(operation[i])) {
		switch (operation[i])
		{
			case 'a':
				flag |= Channel::ANONYMOUS;
				break;
			case 'i':
				flag |= Channel::INVITE_ONLY;
				break;
			case 'm':
				flag |= Channel::MODERATED;
				break;
			case 'n':
				flag |= Channel::NO_MSG_FROM_OUTSIDE;
				break;
			case 'q':
				flag |= Channel::QUIET;
				break;
			case 'p':
				flag |= Channel::PRIVATE;
				break;
			case 's':
				flag |= Channel::SECRET;
				break;
			case 't':
				flag |= Channel::TOPIC;
				break;
			default:
				reply = createNumericReply(ERR_UMODEUNKNOWNFLAG, current_user.getNick(),
											"", ERR_UMODEUNKNOWNFLAG_MSG);
				command.getScheduler().queueMessage(current_user.getId(), reply, true);
				break;
		}
		i++;
	}
	std::cout << "Avant " << reply_changes << std::endl;
	reply_changes += operation.substr(0, i);
	std::cout << "Apres " << reply_changes << std::endl;
	operation.erase(0, i);
	return (flag);
}
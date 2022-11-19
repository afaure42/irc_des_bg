#include "functionMap.hpp"

static bool	isModeFlag(char c)
{
	return !(c == '\0' || c == '+' || c == '-');
}

std::string createUserModeString(User & user)
{
	std::string ret = "+";
	unsigned int modes = user.getModes();

	if (modes & User::AWAY)
		ret += 'a';
	if (modes & User::INVISIBLE)
		ret += 'i';
	if (modes & User::WALLOPS)
		ret += 'w';
	if (modes & User::RESTRICTED)
		ret += 'r';
	if (modes & User::OPERATOR)
		ret += 'o';
	if (modes & User::LOCALOP)
		ret += 'O';
	if (modes & User::SRVNOTICES)
		ret += 's';
	
	return ret;
}

std::string createChanModeString(Channel & channel)
{
	std::string ret = "+";
	unsigned int modes = channel.getModes();

	if (modes & Channel::ANONYMOUS)
		ret += 'a';
	if (modes & Channel::INVITE_ONLY)
		ret += 'i';
	if (modes & Channel::MODERATED)
		ret += 'm';
	if (modes & Channel::NO_MSG_FROM_OUTSIDE)
		ret += 'n';
	if (modes & Channel::PRIVATE)
		ret += 'p';
	if (modes & Channel::SECRET)
		ret += 's';
	if (modes & Channel::TOPIC)
		ret += 't';
	return ret;
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
		reply_changes += operation[i];
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
		reply_changes += operation[i];
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
				reply_changes.erase(reply_changes.size() - 1);
				reply = createNumericReply(ERR_UMODEUNKNOWNFLAG, current_user.getNick(),
											"", ERR_UMODEUNKNOWNFLAG_MSG);
				command.getScheduler().queueMessage(current_user.getId(), reply, true);
				break;
		}
		i++;
	}
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
		reply_changes += operation[i];
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
				reply_changes.erase(reply_changes.size() - 1);
				reply = createNumericReply(ERR_UMODEUNKNOWNFLAG, current_user.getNick(),
											"", ERR_UMODEUNKNOWNFLAG_MSG);
				command.getScheduler().queueMessage(current_user.getId(), reply, true);
				break;
		}
		i++;
	}
	operation.erase(0, i);
	return (flag);
}

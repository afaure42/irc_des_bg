#include "functionMap.hpp"

static bool	isModeFlag(char c)
{
	return !(c == '\0' || c == '+' || c == '-');
}

static unsigned int	createChannelMask(Command &command,
										User &current_user,
										std::string &operation)
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
	operation.erase(0, i);
	return (flag);
}

static unsigned int	createChanUserPermMask(Command &command,
											User &current_user,
											std::string &operation)
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
	operation.erase(0, i);
	return (flag);
}

static void	updateChannelModes(Channel &channel, unsigned int mask, unsigned int operation_type)
{
	unsigned int	channel_modes = channel.getModes();

	if (operation_type == 1) {
		channel_modes |= mask;
	}
	else if (operation_type == 2) {
		channel_modes &= ~mask;
	}

	channel.setModes(channel_modes);
}

static void	updateChannelUserPerms(Command &command,
									t_users &users,
									Channel &channel,
									User &current_user,
									std::string &target_user,
									unsigned int mask,
									unsigned int operation_type)
{
	std::string			reply;
	t_users::iterator	target_it = findUser(target_user, users);

	if (target_it == users.end()
		|| channel.getMembers().find(target_it->second.getId()) == channel.getMembers().end())
	{
		reply = createNumericReply(ERR_NOTONCHANNEL, target_user,
									channel.getName(), ERR_NOTONCHANNEL_MSG);
		command.getScheduler().queueMessage(current_user.getId(), reply, true);
		return ;
	}

	unsigned int &usr_perms = channel.getPermissions().at(target_it->second.getId());

	if (operation_type == 1) {
		usr_perms |= mask;
	}
	else if (operation_type == 2) {
		usr_perms &= ~mask;
	}
}

unsigned int	modeChannel(Command &command,
							t_users &users,
							t_stringlist &params,
							std::string &channel_name,
							User &current_user,
							t_channels &channels)
{
	t_channels::iterator	ch_it = findChannel(channel_name, channels);
	std::string				numeric_reply;

	// Channel doesnt exist
	if (ch_it == channels.end()) {
		numeric_reply = createNumericReply(ERR_NOSUCHCHANNEL, current_user.getNick(),
						channel_name, ERR_NOSUCHCHANNEL_MSG);
		command.getScheduler().queueMessage(current_user.getId(), numeric_reply, true);
	}
	// User not on channel
	else if (ch_it->getMembers().find(current_user.getId()) == ch_it->getMembers().end()) {
		numeric_reply = createNumericReply(ERR_NOTONCHANNEL, current_user.getNick(),
						channel_name, ERR_NOTONCHANNEL_MSG);
		command.getScheduler().queueMessage(current_user.getId(), numeric_reply, true);
	}
	else if (params.empty()) {
			; // list all activated modes on channel (??)
	}
	else {
		// Privileges check, so 2022
		members_perms_t::iterator	user_perm_it = ch_it->getPermissions().find(current_user.getId());
		if (user_perm_it == ch_it->getPermissions().end() || !(user_perm_it->second & Channel::OPERATOR)) {
			numeric_reply = createNumericReply(ERR_CHANOPRIVSNEEDED, current_user.getNick(),
			ch_it->getName(), ERR_CHANOPRIVSNEEDED_MSG);
			command.getScheduler().queueMessage(current_user.getId(), numeric_reply, true);
		}
		else {
			unsigned int	operation_type = 0; // 0 = list, 1 = add, 2 = delete
			std::string		chan_operation = params.front();
			params.pop_front();
			while (!chan_operation.empty()) {
				if (chan_operation[0] == '+') {
					operation_type = 1;
					chan_operation.erase(0, 1);
				} else if (chan_operation[0] == '-') {
					operation_type = 2;
					chan_operation.erase(0, 1);
				} else {
					unsigned int mask = 0;
					if (!params.empty()) {
						std::cout << "target mode\n";
						mask = createChanUserPermMask(command, current_user, chan_operation);
						updateChannelUserPerms(command, users, *ch_it, current_user, params.front(), mask, operation_type);
					}
					else {
						mask = createChannelMask(command, current_user, chan_operation);
						updateChannelModes(*ch_it, mask, operation_type);
					}
				}
			}
		}
	}
	return (0);
}

static unsigned int	createUserMask(Command &command,
										User &current_user,
										std::string &operation)
{
	unsigned int	flag = 0;
	std::string		reply;
	size_t			i = 0;

	while (isModeFlag(operation[i])) {
		switch (operation[i])
		{
			case 'a':
				flag |= User::AWAY;
				break;
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
	operation.erase(0, i);
	return (flag);
}

static void	updateUserModes(User &current_user, unsigned int mask, unsigned int operation_type)
{
	unsigned int updated_user_modes = current_user.getModes();

	if (operation_type == 1) {
		updated_user_modes |= mask;
	}
	else if (operation_type == 2) {
		updated_user_modes &= ~mask;
	}

	current_user.setModes(updated_user_modes);
}

unsigned int	modeUser(Command &command,
							User &current_user,
							t_stringlist &params,
							std::string target)
{
	std::string reply;

	if (current_user.getNick() != target) {
		reply = createNumericReply(ERR_USERSDONTMATCH, current_user.getNick(),
									target, ERR_USERSDONTMATCH_MSG);
		command.getScheduler().queueMessage(current_user.getId(), reply, true);
	}
	else if (params.empty()) {
		; // list settings for nick
	}
	else {
		unsigned int	operation_type = 0; // 0 = list, 1 = add, 2 = delete
		std::string		chan_operation = params.front();
		params.pop_front();
		while (!chan_operation.empty()) {
			if (chan_operation[0] == '+') {
				operation_type = 1;
				chan_operation.erase(0, 1);
			} else if (chan_operation[0] == '-') {
				operation_type = 2;
				chan_operation.erase(0, 1);
			} else {
				unsigned int mask = createUserMask(command, current_user, chan_operation);
				updateUserModes(current_user, mask, operation_type);
				std::cout << "USER MASK => " << mask << std::endl;
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
	(void)command;(void)client_id;(void)users;(void)channels;
	t_stringlist 			params = command.getParams();

	if (params.empty())
		return (ERR_NEEDMOREPARAMS);

	User &current_user = users.at(client_id);

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);

	const std::string	user_nick = current_user.getNick();
	std::string			&target = params.front();

	params.pop_front();
	if (target[0] == '#')
		return modeChannel(command, users, params, target, current_user, channels);
	else
		return modeUser(command, current_user, params, target);
	return (0);
}

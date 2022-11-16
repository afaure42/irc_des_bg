#include "functionMap.hpp"

// unsigned int	modeUser(User &current_user, std::string &target,
// 							t_stringlist &params, t_channels &channels)
// {
	
// }

static void	updateChannelFlags(Channel &channel, unsigned int operation_type, char c)
{
	unsigned int	channel_flags = channel.getModes();
	unsigned int	flag = 0;

	switch (c)
	{
		case 'a':
			flag = Channel::ANONYMOUS;
			break;
		case 'i':
			flag = Channel::INVITE_ONLY;
			break;
		case 'm':
			flag = Channel::MODERATED;
			break;
		case 'n':
			flag = Channel::NO_MSG_FROM_OUTSIDE;
			break;
		case 'q':
			flag = Channel::QUIET;
			break;
		case 'p':
			flag = Channel::PRIVATE;
			break;
		case 's':
			flag = Channel::SECRET;
			break;
		case 't':
			flag = Channel::TOPIC;
			break;
	}

	std::cout << "updating channel modes, from " << channel_flags << " to ";
	if (operation_type == 1)
		channel_flags |= flag;
	else if (operation_type == 2)
		channel_flags &= ~flag;
	std::cout << channel_flags << std::endl;
	channel.setModes(channel_flags);
}

unsigned int	modeChannel(Command &command,
							t_stringlist &params,
							unsigned int client_id,
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
		command.getScheduler().queueMessage(client_id, numeric_reply, true);
	}
	// User not on channel
	else if (ch_it->getMembers().find(client_id) == ch_it->getMembers().end()) {
		numeric_reply = createNumericReply(ERR_NOTONCHANNEL, current_user.getNick(),
						channel_name, ERR_NOTONCHANNEL_MSG);
		command.getScheduler().queueMessage(client_id, numeric_reply, true);
	}
	else if (params.empty()) {
			; // list all activated modes on channel (??)
	}
	else {
		// Privileges check, so 2022
		members_perms_t::iterator	user_perm_it = ch_it->getPermissions().find(client_id);
		if (user_perm_it == ch_it->getPermissions().end() || !(user_perm_it->second & Channel::OPERATOR)) {
			numeric_reply = createNumericReply(ERR_CHANOPRIVSNEEDED, current_user.getNick(),
			ch_it->getName(), ERR_CHANOPRIVSNEEDED_MSG);
			command.getScheduler().queueMessage(client_id, numeric_reply, true);
		}
		else {
			unsigned int	operation_type = 0; // 0 = list, 1 = add, 2 = delete
			std::string		chan_operation = params.front();
			while (!chan_operation.empty()) {
				if (chan_operation[0] == '+') {
					operation_type = 1;
				} else if (chan_operation[0] == '-') {
					operation_type = 2;
				} else {
					updateChannelFlags(*ch_it, operation_type, chan_operation[0]);
				}
				chan_operation.erase(0, 1);
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

	User 					&current_user = users.at(client_id);

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);

	const std::string		user_nick = current_user.getNick();
	std::string				&target = params.front();

	params.pop_front();
	if (target[0] == '#')
		return modeChannel(command, params, client_id, target, current_user, channels);
	// else
	// 	return modeUser(current_user, target, params, channels);
	return (0);
}

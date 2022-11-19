#include "functionMap.hpp"
#include "modes.hpp"

static void	sendModeReply(Command &command, User &current_user, std::string target, std::string changes)
{
	std::string reply = ":" + current_user.getFullName() + " MODE " + target + " :" + changes + IRC_MSG_SEPARATOR;
	command.getScheduler().queueMessage(current_user.getId(), reply, true);	
}
static void	sendModeReply(Command &command, User &current_user, Channel &chan, std::string changes, std::string target)
{
	std::string reply = ":" + current_user.getFullName() + " MODE " + chan.getName() + " :" + changes + " " + target + IRC_MSG_SEPARATOR;
	chan.send(command.getScheduler(), reply, 0);
}
static void	sendModeReply(Command &command, Channel &chan, User &current_user, std::string target, std::string changes)
{
	std::string reply = ":" + current_user.getFullName() + " MODE " + target + " :" + changes + IRC_MSG_SEPARATOR;
	chan.send(command.getScheduler(), reply, 0);
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
		numeric_reply = createNumericReply(RPL_CHANNELMODEIS, current_user.getNick(),
				ch_it->getName(), createChanModeString(*ch_it));
		command.getScheduler().queueMessage(current_user.getId(), numeric_reply, true);
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
					unsigned int 	mask = 0;
					std::string		reply_changes = operation_type == 1 ? "+" : operation_type == 2 ? "-" : "";
					if (!params.empty()) {
						mask = createChanUserPermMask(command, current_user, chan_operation, reply_changes);
						if (mask && updateChannelUserPerms(command, users, *ch_it, current_user, params.front(), mask, operation_type))
							sendModeReply(command, current_user, *ch_it, reply_changes, params.front());
					}
					else {
						mask = createChannelMask(command, current_user, chan_operation, reply_changes);
						if (mask != 0) {
							updateChannelModes(*ch_it, mask, operation_type);
							sendModeReply(command, *ch_it, current_user, channel_name, reply_changes);
						}
					}
				}
			}
		}
	}
	return (0);
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
		reply = createNumericReply(RPL_UMODEIS, current_user.getNick(),
				"", createUserModeString(current_user));
		command.getScheduler().queueMessage(current_user.getId(), reply, true);
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
				std::string reply_changes = operation_type == 1 ? "+" : operation_type == 2 ? "-" : "";
				unsigned int mask = createUserMask(command, current_user, chan_operation, reply_changes);
				updateUserModes(current_user, mask, operation_type);
				sendModeReply(command, current_user, target, reply_changes);
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
	std::string			target = params.front();

	params.pop_front();
	if (target[0] == '#')
		return modeChannel(command, users, params, target, current_user, channels);
	else
		return modeUser(command, current_user, params, target);
	return (0);
}

#include "common.hpp"
#include "functionMap.hpp"

void	updateChannelModes(Channel &channel, unsigned int mask, unsigned int operation_type)
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

bool	updateChannelUserPerms(Command &command,
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
		reply = createNumericReply(ERR_USERNOTINCHANNEL, target_user,
									channel.getName(), ERR_USERNOTINCHANNEL_MSG);
		command.getScheduler().queueMessage(current_user.getId(), reply, true);
		return false;
	}

	unsigned int &usr_perms = channel.getPermissions().at(target_it->second.getId());

	if (operation_type == 1) {
		usr_perms |= mask;
	}
	else if (operation_type == 2) {
		usr_perms &= ~mask;
	}
	return true;
}

void	updateUserModes(User &current_user, unsigned int mask, unsigned int operation_type)
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
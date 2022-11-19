#pragma once
#include "common.hpp"
#include "functionMap.hpp"

void	updateChannelModes(Channel &channel, unsigned int mask, unsigned int operation_type);
bool	updateChannelUserPerms(Command &command,
								t_users &users,
								Channel &channel,
								User &current_user,
								std::string &target_user,
								unsigned int mask,
								unsigned int operation_type);
void	updateUserModes(User &current_user, unsigned int mask, unsigned int operation_type);

unsigned int	createChanUserPermMask(Command &command,
										User &current_user,
										std::string &operation,
										std::string &reply_changes);
unsigned int	createUserMask(Command &command,
										User &current_user,
										std::string &operation,
										std::string &reply_changes);
unsigned int	createChannelMask(Command &command,
										User &current_user,
										std::string &operation,
										std::string &reply_changes);

std::string createChanModeString(Channel & channel);
std::string createUserModeString(User & user);

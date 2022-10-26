#pragma once

#include "Command.hpp"
#include "functionMap.hpp"

// users map
typedef std::map<unsigned int, User>	t_users;
// channels map
typedef std::vector<Channel> 			t_channels;

t_channels::iterator	findChannel(std::string & name, t_channels & channels);
t_users::iterator		findUser(const std::string & nick, t_users & users);

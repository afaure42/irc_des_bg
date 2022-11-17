#pragma once

#include "Command.hpp"
#include "functionMap.hpp"
#include "common.hpp"

// string list
typedef std::list<std::string>			t_stringlist;
// users map
typedef std::map<unsigned int, User>	t_users;
// channels map
typedef std::vector<Channel> 			t_channels;

/**
 * @brief Function to send a msg to every user on the server
 * 
 * @param scheduler reference to the scheduler
 * @param msg msg to send
 * @param from user sending the message
 * 	( he will not receive the msg )
 * 	0 if you dont care
 */
void sendAllUser(Scheduler & scheduler, t_users & users,
	const std::string & msg, unsigned int from);
t_channels::iterator	findChannel(const std::string & name, t_channels & channels);
t_users::iterator		findUser(const std::string & nick, t_users & users);
void					freeUser(unsigned int client_id, Server & server,
									Scheduler & scheduler,t_users & users);

t_stringlist			split(std::string str);

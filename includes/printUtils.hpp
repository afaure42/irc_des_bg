#ifndef PRINTUTILS_HPP
#define PRINTUTILS_HPP

#include "common.hpp"
#include "User.hpp"
#include "Channel.hpp"


void print_users(std::map<unsigned int, User>::iterator begin,
				std::map<unsigned int, User>::iterator end);

void print_channels(std::vector<Channel>::iterator begin,
					std::vector<Channel>::iterator end);

#endif

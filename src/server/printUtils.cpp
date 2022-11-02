#include "common.hpp"
#include "User.hpp"
#include "Channel.hpp"

void print_users(std::map<unsigned int, User>::iterator begin,
				std::map<unsigned int, User>::iterator end)
{
	while (begin != end)
	{
		std::cout << begin->second << std::endl;
		begin++;
	}
}

void print_channels(std::vector<Channel>::iterator begin,
					std::vector<Channel>::iterator end)
{
	while (begin != end)
	{
		std::cout << *begin << std::endl;
		begin++;
	}
}

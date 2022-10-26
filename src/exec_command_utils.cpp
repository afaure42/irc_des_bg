#include "exec_command_utils.hpp"

t_channels::iterator findChannel(std::string & name, t_channels & channels)
{
	t_channels::iterator ret = channels.begin();

	while (ret != channels.end())
	{
		if (ret->getName() == name)
			return ret;
		ret++;
	}
	return ret;
}

t_users::iterator findUser(const std::string & nick, t_users & users)
{
	t_users::iterator ret = users.begin();

	while (ret != users.end())
	{
		if (ret->second.getNick() == nick)
			return ret;
		ret++;
	}
	return ret;
}

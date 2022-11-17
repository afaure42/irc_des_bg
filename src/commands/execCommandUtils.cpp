#include "execCommandUtils.hpp"

void sendAllUser(Scheduler & scheduler, t_users & users, const std::string & msg, unsigned int from)
{
	for(t_users::iterator it = users.begin(); it != users.end(); it++)
	{
		if (it->second.isRegistered() && it->first != from)
			scheduler.queueMessage(it->first, msg, true);
	}
}

t_channels::iterator findChannel(const std::string & name, t_channels & channels)
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

void freeUser(unsigned int client_id,
			Server & server,
			Scheduler & scheduler,
			t_users & users)
{
	users.erase(client_id);
	scheduler.removeFromQueues(client_id);
	server.disconnectClient(client_id);
}

t_stringlist	split(std::string str) {
	t_stringlist	list;
	size_t			delimiter;
	std::string		token;

	while (!str.empty()) {
		delimiter = str.find(",");
		if (delimiter != std::string::npos){
			token = str.substr(0, delimiter);
			str.erase(0, delimiter + 1);
		}
		else {
			token = str;
			str.erase();
		}
		list.push_back(token);
	}

	return (list);
}

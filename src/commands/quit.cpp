#include "functionMap.hpp"

unsigned int	quit(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	std::list<std::string> params = command.getParams();

	//create the ERROR MESSAGE
	//(check rfc this is acknowledgement of quit message)
	//And the quit message for other users
	User & user = users.at(client_id);

	std::string quit_msg = ":" + user.getFullName() + " QUIT ";

	if (!params.empty())
		quit_msg += params.front();
	else
		quit_msg += "User Left";
	quit_msg += IRC_MSG_SEPARATOR;

	command.getScheduler().queueMessage(client_id, QUIT_ACK, true);

	//refresh write queue so acknowledge is sent
	command.getScheduler().writeAll();

	//remove the user and send the quit message to every channel
	while(user.getChannels().size() != 0)
	{
		t_channels::iterator ch_it = findChannel(*user.getChannels().begin(), channels);

		ch_it->send(command.getScheduler(), quit_msg, client_id);
		ch_it->removeUser(user);
		user.getChannels().erase(ch_it->getName());
		if (ch_it->getMembers().size() == 0)
			channels.erase(ch_it);
	}

	//then free the user from everywhere
	freeUser(client_id, command.getServer(),
			command.getScheduler(), users);

	return (0);
}

#include "functionMap.hpp"


unsigned int	privmsg(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels )
{
	(void)channels;
	std::list<std::string>	params = command.getParams();

	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);
	
	std::string from = ":" + users[client_id].getNick() + " PRIVMSG ";

	//TODO implement a map of nick -> id 
	//to not have to iterate through all clients
	//but this will add a parameter to the command
	//or just add it ass a reference member in the command

	while (params.size() != 1)
	{
		std::string msg = from + params.front() + " " + params.back() + "\r\n";

		//first try to find channel
		t_channels::iterator ch_it = channels.begin();
		while (ch_it != channels.end())
		{
			if (ch_it->getName() == params.front())
				break;
			ch_it++;
		}
		if (ch_it != channels.end())
			ch_it->send(command.getScheduler(), msg);
		else//if no channel found then try to find user
		{
			
			t_users::iterator usr_it = users.begin();
			while(usr_it != users.end())
			{
				if (usr_it->second.getNick() == params.front())
					break;
				usr_it++;
			}
			if (usr_it != users.end())
				command.getScheduler().queueMessage(usr_it->first, msg);
			else //no channel nor user found then return ERR
				return (ERR_NOSUCHNICK);
		}
		params.pop_front();	
	}

	std::cout << "PRIVMSG command execution\n";
	return (0);
}

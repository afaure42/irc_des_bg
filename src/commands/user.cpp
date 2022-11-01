#include "functionMap.hpp"

unsigned int	user(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;
	std::list<std::string>	params = command.getParams();

	if (params.size() < 4)
		return (ERR_NEEDMOREPARAMS);

	t_users::iterator user_it = users.find(client_id);
	
	if (user_it->second.getNick() == "*")
	{
		command.getScheduler().queueMessage(client_id, "ERROR :USAGE PASS NICK USER\r\n", false);
		//deconnect incoming so use the deconnection routine
		command.getScheduler().writeAll();
		freeUser(client_id, command.getServer(),
					command.getScheduler(), users);
		return (0);
	}
	if (user_it->second.getConnectPass() != command.getServer().getPass())
	{
		std::cerr << "User pass is \""<< user_it->second.getConnectPass()
		<< "\" Server pass is \"" << command.getServer().getPass() << "\"" << std::endl;
		command.getScheduler().queueMessage(client_id, "ERROR :INVALID PASSWORD\r\n", false);
		//deconnect incoming so use the deconnection routine
		command.getScheduler().writeAll();
		freeUser(client_id, command.getServer(),
					command.getScheduler(), users);
		return (0);
	}
	
	
	//i dont understand what syntax checks are needed for now
	//https://datatracker.ietf.org/doc/html/rfc2812#section-2.3.1

	//set username
	user_it->second.setUsername(params.front());
	params.pop_front();

	params.pop_front(); //2nd parameter unused
	params.pop_front(); //3rd parameter unused
	//setting hostname
	user_it->second.setHostName(command.getServer().getClientHost(client_id));

	//setting realname
	user_it->second.setRealname(params.front());

	//TODO: SEND RPL 001 to 004
	std::string rpl = createNumericReply(RPL_WELCOME, user_it->second.getFullName(),
		"", "Welcome to irc_des_bg " + user_it->second.getFullName());

	command.getScheduler().queueMessage(client_id, rpl, true);

	return (0);
}

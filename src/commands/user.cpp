#include "functionMap.hpp"

unsigned int	user(	Command &command,
								unsigned int client_id,
								t_users &users,
								t_channels &channels ) {
	(void)channels;
	std::list<std::string>	params = command.getParams();
	User & current_user = users.at(client_id);

	if (current_user.isRegistered())
		return (ERR_ALREADYREGISTERED);
	if (params.size() < 4)
		return (ERR_NEEDMOREPARAMS);

	
	if (current_user.getNick() == TEMP_NICK)
	{
		command.getScheduler().queueMessage(client_id, "ERROR :NO NICKNAME GIVEN\r\n", false);
		//deconnect incoming so use the deconnection routine
		command.getScheduler().writeAll();
		freeUser(client_id, command.getServer(),
					command.getScheduler(), users);
		return (0);
	}
	if (current_user.getConnectPass() != command.getServer().getPass())
	{
		std::cerr << "User pass is \""<< current_user.getConnectPass()
		<< "\" Server pass is \"" << command.getServer().getPass() << "\"" << std::endl;
		command.getScheduler().queueMessage(client_id, "ERROR :INVALID PASSWORD\r\n", false);
		//deconnect incoming so use the deconnection routine
		command.getScheduler().writeAll();
		freeUser(client_id, command.getServer(),
					command.getScheduler(), users);
		return (0);
	}
	
	if (!isValidUser(params.front()))
	{
		command.getScheduler().queueMessage(client_id, "ERROR :INVALID USERNAME\r\n", false);

		command.getScheduler().writeAll();
		freeUser(client_id, command.getServer(),
					command.getScheduler(), users);
		return (0);
	
	}
	//set username
	current_user.setUsername(params.front());
	params.pop_front();

	params.pop_front(); //2nd parameter unused
	params.pop_front(); //3rd parameter unused
	//setting hostname
	current_user.setHostName(command.getServer().getClientHost(client_id));

	//setting realname
	//removing beginning ":" chars
	if (!params.front().empty() && params.front()[0] == ':')
		params.front().erase(0, 1);

	if (!isValidRealName(params.front()))
	{
		command.getScheduler().queueMessage(client_id, "ERROR :INVALID REALNAME\r\n", false);

		command.getScheduler().writeAll();
		freeUser(client_id, command.getServer(),
					command.getScheduler(), users);
		return (0);	
	}
	current_user.setRealname(params.front());

	current_user.setRegistered();

	//TODO: SEND RPL 001 to 004
	std::string rpl = createNumericReply(RPL_WELCOME, current_user.getNick(),
		"", "Welcome to irc_des_bg " + current_user.getNick());

	command.getScheduler().queueMessage(client_id, rpl, true);

	return (0);
}

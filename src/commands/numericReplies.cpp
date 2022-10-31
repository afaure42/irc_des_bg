#include "numericReplies.hpp"

const std::string	errNeedMoreParams(Command & command, std::string nick)
{
	return (SERVER_PREFIX + " 461 " + nick + " :" + command.getCmdName()
	+ " :Not enough parameters\r\n");
}

const std::string	errUnknownCommand(Command & command, std::string nick){
	return (SERVER_PREFIX + " 421 " + nick + " " + command.getCmdName()
	+ " :Unkown Command\r\n");
}

// UNE FONCTION POUR LES GOUVERNER TOUTES
const std::string	createNumericReply(
	uns,
	std::string nick,
	std::string target, // can be empty
	std::string reason)
{
	return (SERVER_PREFIX + " " + numeric_reply
		+ " " + nick + " " + (target.empty() ? ":" : target + " :")
		+ reason + IRC_MSG_SEPARATOR);
}
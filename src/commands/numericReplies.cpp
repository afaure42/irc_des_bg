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
	unsigned int numeric_reply,
	std::string nick,
	std::string target, // can be empty
	std::string reason)
{
	std::string	nr_str;
	while (numeric_reply > 0) {
		nr_str.insert(0, 1, numeric_reply % 10 + 48);
		numeric_reply /= 10;
	}

	return (SERVER_PREFIX + " " + nr_str + " " + nick + " "
			+ (target.empty() ? ":" : target + " :")
			+ reason + IRC_MSG_SEPARATOR);
}
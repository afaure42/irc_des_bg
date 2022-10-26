#include "numericReplies.hpp"

const std::string errNeedMoreParams(Command & command, std::string nick)
{
	return (SERVER_PREFIX + " 461 " + nick + " :" + command.getCmdName()
	+ " :Not enough parameters\r\n");
}

const std::string errUnknownCommand(Command & command, std::string nick){
	return (SERVER_PREFIX + " 421 " + nick + " " + command.getCmdName()
	+ " :Unkown Command\r\n");
}

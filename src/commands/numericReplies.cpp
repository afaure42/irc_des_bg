#include "numericReplies.hpp"

void errNeedMoreParams()
{

}

const std::string errUnknownCommand(Command & command, std::string nick){
	return (":" + SERVER_NAME + " 421 " + nick + " :" + command.getCmdName()
	+ " : Unkown Command\r\n");
}

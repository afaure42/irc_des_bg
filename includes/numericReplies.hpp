#ifndef NUMERIC_REPLIES_HPP
#define NUMERIC_REPLIES_HPP

#include "common.hpp"
#include "Scheduler.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Command.hpp"

class Command;

const std::string errNeedMoreParams(Command & command, std::string nick);
const std::string errUnknownCommand(Command & command, std::string nick);
const std::string	createNumericReply(
						std::string numeric_reply,
						std::string nick,
						std::string target, // can be empty
						std::string reason);
// TODO (maybe?): add description of the errors
// GENERAL ERRORS

// REGISTRATION						// FORMAT
#define RPL_WELCOME				001	// <client> :Welcome to the Internet Relay Network <nick>!<user>@<host>
#define RPL_YOURHOST			002 // <client> :Your host is <servername>, running version <version>	
#define RPL_CREATED				003 // <client> :This server was created <date>
#define RPL_MYINFO				004 // <client> <server_name> <version> <usermodes> <chanmodes> [chanmodes_with_a_parameter]
#define RPL_UMODEIS				221 // <client> <user_modes> [<user_mode_params>]

// NICK ERRORS
#define ERR_NOSUCHNICK			401 // <client> <nick> :<reason>
#define ERR_NOSUCHSERVER		402 // <client> <server> :<reason>
#define ERR_NOSUCHCHANNEL		403 // <client> <channel> :<reason>
#define ERR_NOORIGIN			409 // <client> :<reason>
#define ERR_UNKNOWNCOMMAND		421 // <client> <command> :<reason>

#define ERR_NONICKNAMEGIVEN		431 // <client> :<reason>
#define ERR_ERRONEUSNICKNAME	432 // <client> <nick> :<reason>
#define ERR_NICKNAMEINUSE		433 // <client> <nick> :<reason>	
#define ERR_NICKCOLLISION		436 // <nick> :<reason>

#define ERR_UNAVAILRESOURCE		437 // <client> <nick/channel/service> :<reason>

#define ERR_NOTONCHANNEL		442 // <client> <channel> :<reason>
#define ERR_NEEDMOREPARAMS		461 // <client> <command> :<reason>
#define ERR_ALREADYREGISTERED	462 // <client> :<reason>
#define ERR_RESTRICTED			464 // <client> :<reason>	

// MODE ERRORS
#define ERR_UMODEUNKNOWNFLAG	"501" // <client> :<reason>
#define ERR_USERSDONTMATCH		502 // <client> :<reason>

#endif

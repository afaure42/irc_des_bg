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
// TODO (maybe?): add description of the errors
// GENERAL ERRORS

//REGISTRATION
#define RPL_WELCOME				001
#define RPL_YOURHOST			002
#define RPL_CREATED				003
#define RPL_MYINFO				004


// NICK ERRORS
#define ERR_NOSUCHNICK			401
#define ERR_NOSUCHSERVER		402
#define ERR_NOSUCHCHANNEL		403
#define ERR_NOORIGIN			409
#define ERR_UNKNOWNCOMMAND		421

#define ERR_NONICKNAMEGIVEN		431
#define ERR_ERRONEUSNICKNAME	432
#define ERR_NICKNAMEINUSE		433
#define ERR_NICKCOLLISION		436

#define ERR_UNAVAILRESOURCE		437

#define ERR_NOTONCHANNEL		442
#define ERR_NEEDMOREPARAMS		461
#define ERR_ALREADYREGISTERED	462
#define ERR_RESTRICTED			464

#endif

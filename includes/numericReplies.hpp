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

//RPL
#define RPL_AWAY				301

#define RPL_UNAWAY				305
#define RPL_UNAWAY_MSG			std::string("You are no longer marked as begin away")

#define RPL_NOAWAY				306
#define RPL_NOAWAY_MSG			std::string("You have been marked as being away")

#define RPL_WHOISUSER			311

#define RPL_WHOISOPERATOR		313
#define RPL_WHOISOPERATOR_MSG	std::string("is an IRC operator")

#define RPL_ENDOFWHOIS			318
#define RPL_ENDOFWHOIS_MSG		std:string("End of /WHOIS list")

#define RPL_WHOISCHANNELS		319

#define RPL_WHOWASUSER			314

#define RPL_ENDOFWHOWAS			369
#define RPL_ENDOFWHOWAS_MSG		std::string("End of WHOWAS")

#define RPL_LISTSTART			321
#define RPL_LISTSTRART_MSG		std::string("Channel :Users\tName")

#define RPL_LIST				322

#define RPL_LISTEND				323
#define RPL_LISTEND_MSG			std::string("End of /LIST")

#define RPL_NOTOPIC				331
#define RPL_NOTOPIC_MSG			std::string("No topic is set")

#define RPL_TOPIC				332

#define RPL_INVITING			341

#define RPL_VERSION				351
#define RPL_VERSION_MSG			std::string("Best version ever")

#define RPL_NAMREPLY			353
#define RPL_ENDOFNAMES			366
#define RPL_ENDOFNAMES_MSG		std::string("End of /NAMES list")

#define RPL_INFO				371
#define RPL_ENDOFINFO			374
#define RPL_ENDOFINFO_MSG		std::string("End of /INFO list")

#define RPL_MODTSTRART			375
#define RPL_MOTD				372
#define RPL_ENDOFMOTD			376
#define RPL_ENDOFMOTD_MSG		std::string("End of /MOTD command")

#define RPL_YOUREOPER			381
#define RPL_YOUREOPER_MSG		std::string("You are now an IRC operator")

#define RPL_USERSSTART			392
#define RPL_USERSSTART_MSG		std::string("UserID\tTerminal\tHost")
#define RPL_USERS				393
#define RPL_ENDOFUSERS			394
#define RPL_ENDOFUSERS_MSG		std::string("End of users")
#define RPL_NOUSERS				395
#define RPL_NOUSERS_MSG			std::string("Nobody logged in")

//ERRORS
#define ERR_NOSUCHNICK			401 // <client> <nick> :<reason>
#define ERR_NOSUCHNICK_MSG		std::string("No such nick/channel")

#define ERR_NOSUCHSERVER		402 // <client> <server> :<reason>
#define ERR_NOSUCHSERVER_MSG	std:string("No such server")

#define ERR_NOSUCHCHANNEL		403 // <client> <channel> :<reason>
#define ERR_NOSUCHCHANNEL		std::string("No such channel")

#define ERR_CANNOTSENDTOCHAN	404// <client> <channel> :<reason>
#define ERR_CANNOTSENDTOCHAN_MSG std::string("Cannot send to channel")

#define ERR_TOOMANYCHANNELS		405// <client> <channel> :<reason>
#define ERR_TOOMANYCHANNELS_MSG std::string("You have joined too many channels")

#define ERR_WASNOSUCHNICK		406// <client> <nick> :<reason>
#define ERR_WASNOSUCHNICK_MSG	std::string("There Was no such nickname")

#define ERR_TOOMANYTARGETS		407// <client> <target> :<reason>
#define ERR_TOOMANYTARGETS_MSG	std::string("Duplicate recipients. No message delivered")

#define ERR_NOORIGIN			409 // <client> :<reason>
#define ERR_NOORIGIN_MSG		std::string("No origin specified")

#define ERR_NORECIPIENT			411
#define ERR_NORECIPIENT_MSG		std::string("No recipient given:")

#define ERR_NOTEXTTOSEND		412
#define ERR_NOTEXTTOSEND_MSG	std::string("No text to send")

#define ERR_UNKNOWNCOMMAND		421 // <client> <command> :<reason>
#define ERR_UNKOWNCOMMAND_MSG	std::string("Unkown command")

#define ERR_NOMOTD				422
#define ERR_NOMOTD				std::string("MOTD File is missing")

#define ERR_NOADMININFO			423
#define	ERR_NOADMININFO			std::string("No administrative info available")

#define ERR_FILEERROR			424
#define ERR_FILEERROR_MSG		std::string("File error")

#define ERR_NONICKNAMEGIVEN		431 // <client> :<reason>
#define ERR_NONICKNAMEGIVEN_MSG	std::string("No nickname given")

#define ERR_ERRONEUSNICKNAME	432 // <client> <nick> :<reason>
#define ERR_ERRONEUSNICKNAME_MSG std::string("Erroneous nickname")

#define ERR_NICKNAMEINUSE		433 // <client> <nick> :<reason>	
#define ERR_NICKNAMEINUSE_MSG	std::string("Nickname is already in use")

#define ERR_NICKCOLLISION		436 // <nick> :<reason>
#define ERR_NICKCOLLISION_MSG	std::string("Nickname collision")

#define ERR_USERNOTINCHANNEL	441 // <lient> <nick> <channel> :<reason>
#define ERR_USERNOTINCHANNEL_MSG std::string("They aren't on that channel")

#define ERR_NOTONCHANNEL		442 // <client> <channel> :<reason>
#define ERR_NOTONCHANNEL_MSG	std::string("You're not on that channel")

#define ERR_USERONCHANNEL		443 // <client> <nick> <channel> :<reason>
#define ERR_USERONCHANNEL_MSG	std::string("is already on that channel")

#define ERR_SUMMONDISABLED		445
#define ERR_SUMMONDISABLED_MSG	std::string("SUMMON has been disabled")

#define ERR_NOTREGISTERED		451
#define ERR_NOTREGISTERED_MSG	"You have not registered"

#define ERR_NEEDMOREPARAMS		461 // <client> <command> :<reason>
#define ERR_NEEDMOREPARAMS_MSG	std::string("Not enough parameters")

#define ERR_ALREADYREGISTERED	462 // <client> :<reason>
#define ERR_ALREADYREGISTERED_MSG std::string("You may not reregister")

#define ERR_PASSWDMISMATCH		464 // <client> :<reason>
#define ERR_PASSWDMISMATCH_MSG	std::string("Password incorrect")

#define ERR_KEYSET				467 // <client> <channel> :<reason>
#define ERR_KEYSET_MSG			std::string("Channel key already set")

#define ERR_CHANELISFULL		471
#define ERR_CHANELISFULL_MSG	std::string("Cannot join chanel (+l")

#define ERR_UNKOWNMODE			472 // <client> <channel> <char> :<reason>
#define ERR_UNKOWNMODE_MSG		std::string("is unnkown mode char for this channel")

#define ERR_INVITEONLYCHAN		473// <client> <channel> :<reason>
#define ERR_INVITEONLYCHAN_MSG	std::string("Cannot join channel (+i)")

#define ERR_BANNEDFROMCHAN		474// <client> <channel> :<reason>
#define ERR_BANNEDFROMCHAN_MSG	std::string("Cannot join channel (+b)")

#define ERR_BADCHANNELKEY		475// <client> <channel> :<reason>
#define ERR_BADCHANNELKEY_MSG	std::string("Cannot join channel (+k)")

#define ERR_BADCHANMASK			476// <client> <channel> :<reason>
#define ERR_BADCHANMASK_MSG		std::string("Bad Channel Mask")

#define ERR_NOCHANMODES			477// <client> <channel> :<reason>
#define ERR_NOCHANMODES_MSG		std::string("Channel doesn't support modes")

#define ERR_BANLISTFULL			478// <client> <channel> :<reason>
#define ERR_BANLISTFULL			std::string("Channel list is full")

#define ERR_NOPRIVILEGES		481// <client> :<reason>
#define ERR_NOPRIVILEGES_MSG	std::string("Permission Denied- You're not an IRC operator")

#define ERR_CHANOPRIVSNEEDED	482// <client> <channel> :<reason>
#define ERR_CHANOPRIVSNEEDED_MSG std::string("You're not channel operator")

#define ERR_CANTKILLSERVER		483
#define ERR_CANTKILLSERVER		std::string("You can't kill a server!")

#define ERR_RESTRICTED			484
#define	ERR_RESTRICTED			std::string("Your connection is restricted")

#define ERR_UNNIQOPRIVSNEEDED	485
#define ERR_UNNIQOPRIVSNEEDED_MSG std::string("You're not the original channel operator")

#define	ERR_UMODEUNKOWNFLAG		501
#define ERR_UMODEUNKOWNFLAG_MSG	"Unknown MODE flag"

#define ERR_USERSDONTMATCH		502
#define ERR_USERSDONTMATCH_MSG	std::string("Cannot change mode for other users")

// MODE ERRORS

#endif

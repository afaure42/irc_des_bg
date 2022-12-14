#include "syntaxChecks.hpp"

const std::string SpecialCharSet ("[]{}\\`_^|");

const std::string ForbiddenUserCharSet ("\0\r\n @");

const std::string ForbiddenChanCharSet ("\0\x07\r\n ,:");

static bool isSpecialChar(const char & c) {
	return (SpecialCharSet.find(c) != SpecialCharSet.npos);
}

bool isValidRealName(const std::string & name)
{
	if (name.length() > 30)
		return false;
	
	for(size_t i = 0; i < name.length(); i++)
	{
		if (!std::isalnum(name[i]) && name[i] != ' ')
			return false;
	}
	return true;
}

// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
bool isValidNick(const std::string & nick)
{
	if (nick.length() > 9 || nick.empty())
		return false;

	for (size_t i = 0; i < nick.length(); i++)
	{
		if (i == 0 && !std::isalpha(nick[i]) && !isSpecialChar(nick[i]))
			return false;
		else if (!std::isalnum(nick[i]) && !isSpecialChar(nick[i]) && nick[i] != '-')
			return false;
	}
	return true;
}

bool isValidUser(const std::string & user)
{
	if (user.empty() || user.length() > 15)
		return false;
	
	for(size_t i = 0; i < user.length(); i++)
	{
		if (ForbiddenUserCharSet.find(user[i]) != ForbiddenUserCharSet.npos)
			return false;
	}
	return true;
}
// user =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
//   ; any octet except NUL, CR, LF, " " and "@"?

static bool isValidChanstring(const std::string & chanstring)
{
	if (chanstring.empty())
		return false;
	
	for(size_t i = 0; i < chanstring.length(); i++)
	{
		if (!isalnum(chanstring[i]))
			return false;
	}
	return true;
}

static bool isValidChannelid(const std::string & channelid)
{
	if (channelid.length() != 5)
		return false;
	
	for (size_t i = 0; i < channelid.length(); i++)
	{
		if (!std::isupper(channelid[i]) && !std::isdigit(channelid[i]))
			return false;
	}
	return true;
}

bool isValidChannel(const std::string & channel)
{
	if (channel.empty() || channel.length() > 50)
		return false;
	
	if (channel[0] != '#')
		return false;
	
	//need to chekc for channel id
	if (channel[0] == '!' && !isValidChannelid(channel.substr(1, 5)))
		return (false);
	
	return isValidChanstring(channel.substr(1));
}

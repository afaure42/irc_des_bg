#pragma once

#include "common.hpp"
#include "User.hpp"
#include <map>


class Channel
{
	// The various modes available for channels are as follows:

	// 	O - give "channel creator" status;
	// 	o - give/take channel operator privilege;
	// 	v - give/take the voice privilege;

	// 	a - toggle the anonymous channel flag;
	// 	i - toggle the invite-only channel flag;
	// 	m - toggle the moderated channel;
	// 	n - toggle the no messages to channel from clients on the
	// 		outside;
	// 	q - toggle the quiet channel flag;
	// 	p - toggle the private channel flag;
	// 	s - toggle the secret channel flag;
	// 	r - toggle the server reop channel flag;
	// 	t - toggle the topic settable by channel operator only flag;

	// 	k - set/remove the channel key (password);
	// 	l - set/remove the user limit to channel;

	// 	b - set/remove ban mask to keep users out;
	// 	e - set/remove an exception mask to override a ban mask;
	// 	I - set/remove an invitation mask to automatically override
	// 		the invite-only flag;
    public:
        Channel();
        ~Channel();
		enum userPermissions {
			CREATOR = 1, // O
			OPERATOR = 1 << 1, // o
			VOICE = 1 << 2 // v
		};
		enum channelModes {
			ANONYMOUS = 1, // a
			INVITE_ONLY = 1 << 1, // i
			MODERATED = 1 << 2, // m
			NO_MSG_FROM_OUTSIDE = 1 << 3, // n
			QUIET = 1 << 4, // q
			PRIVATE = 1 << 5, // p
			SECRET = 1 << 6, // s
		};
    private:
        std::string		_name;
		unsigned int	_modes;
		std::string		_key;
		unsigned int	_usrlimit;
        typedef std::map<std::string, User> _members; // nick -> User
		typedef std::map<std::string, unsigned int> _members_permissions; // nick -> permissions

};

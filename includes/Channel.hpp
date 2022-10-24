#pragma once

#include "common.hpp"
#include "User.hpp"
#include "Scheduler.hpp"


typedef std::map<unsigned int, User *> members_t; // id -> User
typedef std::map<unsigned int, unsigned int> members_perms_t; // id -> permissions
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

		//getters
		members_t		&getMembers();
		members_perms_t &getPermissions();
		std::string		&getName();


		//setters
		void		setName(std::string & name);


		//METHODS

		/**
		 * @brief Will queue the msg to
		 * every single member of current channel
		 * 
		 * @param scheduler ref to scheduler
		 * @param msg msg to send
		 */
		void		send(Scheduler & scheduler, std::string & msg);
    private:
        std::string		_name;
		unsigned int	_modes;
		std::string		_key;
		unsigned int	_usrlimit;
		members_t		_members;
		members_perms_t _permissions;
};

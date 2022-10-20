#pragma once
#include "common.hpp"

class User
{
	public:
		User(unsigned int client_id);
		~User();
		void				check(void) const;
		// setters
		void				setAwayStatus(void);
		void				setInvisStatus(void);
		void				setRegistered(void);
		void				setConnectPass(std::string pass);
		void				setNick(std::string nick);
		void				setUsername(const std::string & username);

		// getters
		bool				isRegistered(void) const;
		bool				isAway(void) const;
		bool				isInvisible(void) const;
		unsigned int		getId(void) const;
		std::string	const	&getNick(void) const;
		std::string const	&getOldNick(void) const;
		std::string const	&getUsername(void) const;
		std::string const	&getConnectPass(void) const;
	// The available modes are as follows:

	// 	a - user is flagged as away;
	// 	i - marks a users as invisible;
	// 	w - user receives wallops;
	// 	r - restricted user connection;
	// 	o - operator flag; -> CHANNEL
	// 	O - local operator flag; -> CHANNEL
	// 	s - marks a user for receipt of server notices.
	private:
		unsigned int		_id; // client_id, also it's index in the map
		std::string			_nick; // can change
		std::string			_old_nick;
		std::string			_usrname; // this one cannot
		bool				_away;
		std::string			_away_msg;
		bool				_invisible;
		bool				_registered;
		std::string			_connection_pass;
};

std::ostream& operator<<(std::ostream& os, const User& usr);

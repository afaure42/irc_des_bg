#pragma once
#include "common.hpp"

class User
{
	public:
		User(const std::string	&nickname);
		~User();
		void			check(void) const;
		// setters
		void			setAwayStatus(void);
		void			setInvisStatus(void);
		// getters
		bool			isAway(void) const;
		bool			isInvisible(void) const;
		unsigned int	getId(void) const;
		std::string		getNick(void) const;
	// The available modes are as follows:

	// 	a - user is flagged as away;
	// 	i - marks a users as invisible;
	// 	w - user receives wallops;
	// 	r - restricted user connection;
	// 	o - operator flag; -> CHANNEL
	// 	O - local operator flag; -> CHANNEL
	// 	s - marks a user for receipt of server notices.
	private:
		unsigned int	_id;
		std::string		_nick;
		bool			_away;
		std::string		_away_msg;
		bool			_invisible;
};

std::ostream& operator<<(std::ostream& os, const User& usr);
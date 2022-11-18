#pragma once
#include "common.hpp"

class User
{
	public:
		// User(){};
		User(const User & ref);
		User(unsigned int client_id);
		~User();
		void				check(void) const;
		// setters
		void				setAwayStatus(bool status);
		void				setAwayMessage(const std::string & msg);
		void				setOperator(void);
		void				setInvisStatus(bool status);
		void				setWallopStatus(bool status);
		void				setRegistered(void);
		void				setNick(std::string nick);
		void				setUsername(const std::string & username);
		void				setRealname(const std::string & realname);
		void				setHostName(const std::string & hostname);
		void				setConnectPass(const std::string & connect_pass);
		void				setModes(const unsigned int & modes);

		// getters
		bool				isRegistered(void) const;
		bool				isAway(void) const;
		bool				isInvisible(void) const;
		bool				isWallop(void) const;
		bool				isOp(void) const;
		unsigned int		getId(void) const;
		unsigned int		getModes(void) const;

		const std::string	&getNick(void) const;
		const std::string	&getOldNick(void) const;
		const std::string	&getUsername(void) const;
		const std::string	&getRealname(void) const;
		const std::string	&getHostName(void) const;
		const std::string	getFullName(void) const;
		const std::string	&getConnectPass(void) const;
		std::set<std::string> & getChannels(void);
		std::set<std::string> const &getChannels(void) const;
		const std::string	&getAwayMsg(void) const;
		
		// The available modes are as follows:

		// 	a - user is flagged as away;
		// 	i - marks a users as invisible;
		// 	w - user receives wallops;
		// 	r - restricted user connection;
		// 	o - operator flag; -> CHANNEL
		// 	O - local operator flag; -> CHANNEL
		// 	s - marks a user for receipt of server notices.
		enum	userModes {
			AWAY = 1,
			INVISIBLE = 1 << 1,
			WALLOPS = 1 << 2,
			RESTRICTED = 1 << 3,
			OPERATOR = 	1 << 4,
			LOCALOP = 	1 << 5,
			SRVNOTICES = 	1 << 6
		};

	private:
		unsigned int		_id; // client_id, also it's index in the map
		std::string			_nick; // can change
		std::string			_old_nick;
		std::string			_usrname; // this one cannot
		std::string			_realname;
		std::string			_hostname;
		std::string			_away_msg;
		bool				_registered;
		std::string			_connection_pass;
		unsigned int		_modes;
		std::set<std::string> _channels;
};

std::ostream& operator<<(std::ostream& os, const User& usr);

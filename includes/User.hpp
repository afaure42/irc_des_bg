#pragma once
#include "common.hpp"

#define USR_MODE_a			1 << 1
#define USR_MODE_a_OFFSET	1
#define USR_MODE_i			1 << 2
#define USR_MODE_i_OFFSET	2
#define USR_MODE_w			1 << 3
#define USR_MODE_w_OFFSET	3
#define USR_MODE_r			1 << 4
#define USR_MODE_r_OFFSET	4
#define USR_MODE_o 			1 << 5
#define USR_MODE_o_OFFSET 	5
#define USR_MODE_O 			1 << 6
#define USR_MODE_O_OFFSET 	6
#define USR_MODE_s 			1 << 7
#define USR_MODE_s_OFFSET	7

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
		void				setOperatorStatus(bool status);
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
		std::vector<std::string> & getChannels(void);
		std::vector<std::string> const &getChannels(void) const;
		const std::string	&getAwayMsg(void) const;
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
		std::string			_realname;
		std::string			_hostname;
		std::string			_away_msg;
		bool				_registered;
		std::string			_connection_pass;
		unsigned int		_modes;
		std::vector<std::string> _channels;
};

std::ostream& operator<<(std::ostream& os, const User& usr);

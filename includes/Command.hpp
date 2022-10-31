#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "common.hpp"
#include "Scheduler.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include "numericReplies.hpp"
#include "functionMap.hpp"

// users map
typedef std::map<unsigned int, User>	t_users;
// channels map
typedef std::vector<Channel> 			t_channels;
// string list
typedef std::list<std::string>			t_stringlist;

/**
 * @brief Command class, basic setup is done during initialization,
 * after which you can call the execute() method to... uh.. execute
 * 
 * @param _cmd_type the command type/name
 * @param _params list of all parameters excluding command name
 * @param _numeric_return Numeric reply code :
 * 	All numeric replies return values in numeric_replies.hpp
 * 	Numeric return will be set to :
 * 	-1 if command doesnt exist
 * 	0 if command executed succesfully
 * 	>0 if command encountered an error
 */

class Command;

// users map
typedef std::map<unsigned int, User>	t_users;
// channels map
typedef std::vector<Channel> 			t_channels;

typedef unsigned int (*exec_fn) (
		Command &,
		unsigned int,
		t_users &,
		t_channels & );

typedef std::map<std::string, exec_fn>		exec_fn_map;
// login map pair typedef
typedef std::pair<std::string, exec_fn>		fn_map_pair;



class Command
{
	public:
		Command(
			exec_fn_map &fn_map,
			const std::string *raw_command,
			Scheduler & scheduler,
			Server & server);
		~Command();
		// Public method to be called after setup of the command
		void					execute(unsigned int client_id,
									t_users &users,
									t_channels &channels);
		// Public method to be called after execution
		void					sendReplies(unsigned int client_id,
									t_users &users,
									t_channels &channels);
		// Getters
		unsigned int const		&getCharsRead(void) const;
		std::string const		&getCmdName(void) const;
		t_stringlist const		&getParams(void) const;
		int const				&getNumericReturn(void) const;
		Scheduler				&getScheduler(void);
		Server					&getServer(void);
	private:
		// Internal variables
		unsigned int			_chars_read;
		exec_fn_map				_function_map;
		std::string				_cmd_name;
		t_stringlist			_params;
		int						_numeric_return;
		Scheduler	&			_scheduler;
		Server		&			_server;
		// Internal methods
		void					_setupCommand(const std::string & raw_command);
};

std::ostream& operator<<(std::ostream& os, const Command& cmd);

#endif

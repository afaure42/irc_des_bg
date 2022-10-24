#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "common.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Scheduler.hpp"

#include "numeric_replies.hpp"
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
class Command
{
	public:
		Command(exec_fn_map &fn_map, std::string *raw_command, Scheduler & scheduler);
		~Command();
		// Public method to be called after setup of the command
		void					execute(unsigned int client_id,
									t_users &users,
									t_channels &channels);
		// Getters
		unsigned int const		&getCharsRead(void) const;
		std::string const		&getCmdName(void) const;
		t_stringlist const		&getParams(void) const;
		int const				&getNumericReturn(void) const;
		Scheduler				&getScheduler(void);
	private:
		// Internal variables
		unsigned int			_chars_read;
		exec_fn_map				_function_map;
		std::string				_cmd_name;
		t_stringlist			_params;
		int						_numeric_return;
		Scheduler	&			_scheduler;
		// Internal methods
		void					_setupCommand(std::string raw_command);
};

std::ostream& operator<<(std::ostream& os, const Command& cmd);

#endif

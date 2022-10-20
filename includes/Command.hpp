#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <functional>

#include "common.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include "numeric_replies.hpp"
#include "command_ids.hpp"

// users map
typedef std::map<unsigned int, User>	t_users;
// channels map
typedef std::vector<Channel> 			t_channels;

/**
 * @brief Command class, basic setup is done during initialization,
 * after which you can call the execute() method to... uh.. execute
 * 
 * @param _cmd_type the command type/name
 * @param _params list of all parameters excluding command name
 * @param _cmd_id unique identifier used for the execution pipeline
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
		Command(std::string *raw_command);
		~Command();
		// Public method to be called after setup of the command
		void					execute(unsigned int client_id,
									t_users &users,
									t_channels &channels);
		// Are only really useful for << overload and debug purposes
		std::string const		&getCmdType(void) const;
		std::list<std::string>	getParams(void) const;
		int						getNumericReturn(void) const;
	private:
		std::string				_cmd_type;
		std::list<std::string>	_params;
		unsigned int			_cmd_id;
		/* All numeric replies return values in numeric_replies.hpp
		** Numeric return will be set to :
		**	-1 if command doesnt exist
		**	0 if command executed succesfully
		**	>0 if command encountered an error */
		int						_numeric_return;
		// Setup methods
		void					_set_login_functions(void);
		void					_createParams(
									std::string raw_command);
		// Method used for parsing //
		unsigned int			_getCommandId() const;

		/* COMMAND MAPS TYPEDEFS */
		/* Implemented for clarity */
		// login functions typedef
		typedef unsigned int (Command::*login_fn)(unsigned int, t_users &);
		// login functions map typedef
		typedef std::map<unsigned int, login_fn>	login_fn_map;
		// the login functions map
		login_fn_map								_login_functions;
		// all functions typedef, add channels
		typedef unsigned int (Command::*exec_fn)(
				unsigned int,
				t_users &,
				t_channels &);
		// all functions map typedef
		typedef std::map<unsigned int, exec_fn> exec_fn_map;
		// the all functions map
		exec_fn_map				_all_functions;
		// login map pair typedef
		typedef std::pair<unsigned int, login_fn>	fn_map_pair;

		// Execution methods
		unsigned int			_pass(unsigned int client_id,
									t_users &users);
		unsigned int			_nick(unsigned int client_id,
									t_users &users);
		unsigned int			_user(unsigned int client_id,
									t_users &users);
};

std::ostream& operator<<(std::ostream& os, const Command& cmd);

#endif
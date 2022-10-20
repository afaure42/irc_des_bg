#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <functional>

#include "common.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include "numeric_replies.hpp"

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
		std::string const		&getCmdName(void) const;
		std::list<std::string>	getParams(void) const;
		int						getNumericReturn(void) const;
	private:
		std::string				_cmd_name;
		std::list<std::string>	_params;
		/* All numeric replies return values in numeric_replies.hpp
		** Numeric return will be set to :
		**	-1 if command doesnt exist
		**	0 if command executed succesfully
		**	>0 if command encountered an error */
		int						_numeric_return;
		// Setup methods
		void					_setFunctionMap(void);
		void					_createParams(std::string raw_command);

		/* COMMAND MAP TYPEDEFS */
		/* Implemented for clarity */
		// command methods typedef, add channels
		typedef unsigned int (Command::*exec_fn)(
				unsigned int,
				t_users &,
				t_channels &);
		// all methods map typedef
		typedef std::map<std::string, exec_fn>		exec_fn_map;
		// the methods map
		exec_fn_map									_command_functions;
		// login map pair typedef
		typedef std::pair<std::string, exec_fn>	fn_map_pair;

		// Execution methods
		unsigned int			_pass(unsigned int client_id,
									t_users &users, t_channels &channels);
		unsigned int			_nick(unsigned int client_id,
									t_users &users, t_channels &channels);
		unsigned int			_user(unsigned int client_id,
									t_users &users, t_channels &channels);
};

std::ostream& operator<<(std::ostream& os, const Command& cmd);

#endif
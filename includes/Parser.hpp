#ifndef PARSER_HPP
#define PARSER_HPP

#include "common.hpp"
#include "Command.hpp"

class Parser
{
	public:
		Parser();
		~Parser();
		void			parseCommand(Command const &cmd);
	private:
		bool			_cmd_is_valid;
		int				_numeric_return;
		unsigned int	_identify_cmd_type(std::string cmd_type) const;
		void			_parse_pass_cmd(Command const &cmd);
		void			_parse_nick_cmd(Command const &cmd);
		void			_parse_user_cmd(Command const &cmd);
};

#endif
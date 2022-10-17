#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "common.hpp"
#include <list>

class Command
{
	public:
		Command(std::string *raw_command);
		~Command();
		std::string const		&getCmdType(void) const;
		std::list<std::string>	getParams(void) const;

	private:
		std::string				_cmd_type;
		std::list<std::string>	_params;
		void					_createParams(
									std::string raw_command);
};

std::ostream& operator<<(std::ostream& os, const Command& cmd);

#endif
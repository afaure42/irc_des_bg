#include "Parser.hpp"

Parser::Parser() :
	_cmd_is_valid(false), _numeric_return(0)
{

}

Parser::~Parser() {}

// INDIVIDUAL COMMANDS PARSERS
void	Parser::_parse_pass_cmd(Command const &cmd){
	(void)cmd;
}
void	Parser::_parse_nick_cmd(Command const &cmd){
	(void)cmd;
}
void	Parser::_parse_user_cmd(Command const &cmd){
	(void)cmd;
}

unsigned int	Parser::_identify_cmd_type(std::string cmd_type) const {
	if (cmd_type.compare("PASS"))
		return (1);
	if (cmd_type.compare("NICK"))
		return (2);
	if (cmd_type.compare("USER"))
		return (3);
	return (0);
}

void	Parser::parseCommand(Command const &cmd) {
	unsigned int	identify_id = this->_identify_cmd_type(cmd.getCmdType());

	switch (identify_id)
	{
		case 1:
			;
			break;
		case 2:
			;
			break;
		case 3:
			;
			break;
		default:
			this->_cmd_is_valid = false;
			this->_numeric_return = -1;
			break;
	}
}
#include "functionMap.hpp"


unsigned int	oper(	Command &command,
						unsigned int client_id,
						t_users &users,
						t_channels &channels )
{
	(void)channels;
	t_stringlist	params = command.getParams();
	User & current_user = users.at(client_id);

	if (!current_user.isRegistered())
		return (ERR_NOTREGISTERED);
	if (params.size() < 2)
		return (ERR_NEEDMOREPARAMS);
	
	t_opers::const_iterator op_it = command.getOperators().find(params.front());
	if (op_it == command.getOperators().end())
		return (0);
	if (op_it->second != *(++params.begin()))
		return (ERR_PASSWDMISMATCH);
	
	//now password is ok
	current_user.setOperator();
	return (RPL_YOUREOPER);
}

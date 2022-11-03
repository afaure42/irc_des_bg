#ifndef FUNCTION_MAP_HPP
#define FUNCTION_MAP_HPP

#include "common.hpp"
#include "Command.hpp"
#include "syntaxChecks.hpp"
#include "execCommandUtils.hpp"

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

void	setFunctionMap(exec_fn_map &fn_map);

// Prototypes
unsigned int	pass(Command &command, unsigned int client_id,
						t_users &users, t_channels &channels);
unsigned int	nick(Command &command, unsigned int client_id,
						t_users &users, t_channels &channels);
unsigned int	user(Command &command, unsigned int client_id,
						t_users &users, t_channels &channels);
unsigned int	privmsg(Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	join(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	ping(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	part(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	quit(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	mode(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	away(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	names(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	die(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );
unsigned int	invite(	Command &command, unsigned int client_id,
						t_users &users, t_channels &channels );


#endif

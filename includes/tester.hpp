#pragma once

#include "common.hpp"
#include "Scheduler.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include "functionMap.hpp"
#include <arpa/inet.h>
#include <fstream>

#define BUFFER_LEN			1024
#define CONFIG_FILE_PATH	"./tester_files/config.test"
#define STRESSTEST_CLIENTS	100
#define STRESSTEST_MSG_N	10

static void		send_command(int socket_fd, std::string test_str);
static void 	replace_tokens(std::string &str, struct tester_client_info_s &tci);

typedef std::list<struct tester_client_s> client_list_t;

struct tester_client_info_s
{
	int						port;
	std::string				password, nick, username, fullname;
};

struct tester_client_s
{
	struct tester_client_info_s	tci;
	int							socket_fd;
	struct sockaddr_in			server_addr;
};

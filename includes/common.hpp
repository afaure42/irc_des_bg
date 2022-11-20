#ifndef COMMON_HPP
# define COMMON_HPP

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <exception>
#include <string>
#include <sstream> // string stream
#include <cstring> //strerror
#include <unistd.h> //close
#include <errno.h>
#include <fcntl.h> //fcntl
#include <cstdlib>
#include <iostream>
#include <csignal>
// STD containers
#include <map>
#include <set>
#include <vector>
#include <list>
#include <algorithm>

#include "customExceptions.hpp"

/***DEFINES*****/
#define SERVER_NAME std::string("irc_des_bg")
#define SERVER_PREFIX std::string(":" + SERVER_NAME)
#define	QUIT_ACK	std::string("ERROR ") + IRC_MSG_SEPARATOR
#define TEMP_NICK "*"

#define EVENTS_SIZE 100
#define MAX_CONNECTIONS_LISTN 20
#define USR_EVENT_FLAGS EPOLLIN | EPOLLOUT | EPOLLRDHUP
#define READ_SIZE 128
#define WRITE_SIZE 512

// will be changed to the real thing after
#define IRC_MSG_SEPARATOR "\r\n"



#endif

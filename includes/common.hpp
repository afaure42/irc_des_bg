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
#include <map>
#include <set>
#include <vector>
#include <list>
#include "custom_exceptions.hpp"
#include <iostream>

/***DEFINES*****/

#define EVENTS_SIZE 100
#define MAX_CONNECTIONS_LISTN 20
#define USR_EVENT_FLAGS EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP
#define READ_SIZE 128
#define WRITE_SIZE 128

// will be changed to the real thing after
#define IRC_MSG_SEPARATOR "\n"



#endif

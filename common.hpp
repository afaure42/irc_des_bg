#ifndef COMMON_HPP
# define COMMON_HPP

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <exception>
#include <string>
#include <cstring> //strerror
#include <unistd.h> //close
#include <errno.h>
#include <fcntl.h> //fcntl
#include <cstdlib>
#include <map>
#include <set>
#include <vector>
#include "custom_exceptions.hpp"
#include <iostream>

/***DEFINES*****/

#define EVENTS_SIZE 100
#define MAX_CONNECTIONS_LISTN 20
#define USR_EVENT_FLAGS EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP
#define READ_SIZE 128
#define WRITE_SIZE 128



#endif

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
#include <cstdlib>
#include "custom_exceptions.hpp"


/***DEFINES*****/

#define EVENTS_SIZE 100
#define MAX_CONNECTIONS_LISTN 20
#define USR_EVENT_FLAGS EPOLLIN | EPOLLOUT | EPOLLET
#define READ_SIZE 128
#define WRITE_SIZE 128



#endif

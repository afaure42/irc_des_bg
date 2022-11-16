#include <iostream>
#include <string>

enum userPermissions {
	CREATOR = 1, // O
	OPERATOR = 1 << 1, // o
	VOICE = 1 << 2, // v
	INVITED = 1 << 3
};
enum channelModes {
	ANONYMOUS = 1, // a
	INVITE_ONLY = 1 << 1, // i
	MODERATED = 1 << 2, // m
	NO_MSG_FROM_OUTSIDE = 1 << 3, // n
	QUIET = 1 << 4, // q
	PRIVATE = 1 << 5, // p
	SECRET = 1 << 6, // s
	TOPIC = 1 << 7 //t
};

int main()
{
	unsigned int test = 0;
	test |= ANONYMOUS;
	std::cout << test << std::endl;

}

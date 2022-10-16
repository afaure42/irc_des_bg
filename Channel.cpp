#include "Channel.hpp"

Channel::Channel(std::string name) {this->_name = name;}

// Channel::Channel(const Channel &ref)
// {
// 	// *this = ref;
// }

Channel::~Channel() {};

std::string const	Channel::getName(void) const {
	return this->_name;
}
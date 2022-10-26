#pragma once
#include "common.hpp"

/**
 * @brief Function to use to test if a channel has a valid syntax
 * 
 * @param channel 
 * @return true if valid, false if not
 */
bool isValidChannel(const std::string & channel);
bool isValidUser(const std::string & user);
bool isValidNick(const std::string & nick);

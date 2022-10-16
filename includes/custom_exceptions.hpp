#ifndef CUSTOM_EXCEPTIONS_HPP
#define CUSTOM_EXCEPTIONS_HPP

#include "common.hpp"

/**
 * @brief	An exception thrown when a syscall returns an error
 * 			it contains informations about errno at the time the function
 * 			was thrown
*/
class syscall_error : public std::exception
{
public:
	/**
	 * @brief Construct a new init error object
	 * 
	 * @param err errno
	 * @param msg custom message (usually the syscall that returned an error)
	 */
	syscall_error(int err, std::string msg = "An error Ocurred");

	/**
	 * @return string defined by the thrower
	 * 			(usually the syscall that returned an error)
	 */
	const char * what () const throw ();

	/**
	 * @return a string from errno at the time
	 * 			the exception was thrown
	 */
	const char * err() const throw ();
private:
	int	_errno;
	std::string _msg;
};

#endif

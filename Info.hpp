#ifndef INFO_HPP
#define INFO_HPP

#include "common.hpp"
#include "Client.hpp"

/**
 * @brief Object containing information regarding a recent status update
 * A status update can be either a deconnection (connected == false)
 * or an update in the read buffer
 * You can access its read buffer with the buff method
 * (Dont forget to erase what you have processed)
 */
class Info
{
public:

	Info(int fd, std::string * buff, bool connected);
	Info(const Client & ref);
	Info(const Info & ref);

	/*getters*/
	int				get_fd() const;
	std::string *	buff() const;
	bool			is_connected() const;

	/*setters*/
	void	set_buff(const std::string & ref);

private:
	int		_fd;
	std::string *	_read_buff;
	bool			_connected;

	Info();
};

#endif

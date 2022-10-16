#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "common.hpp"

class Client
{
public:
	Client(){};
	Client(const Client & ref);
	Client(const int &fd, const unsigned int & id,
		const sockaddr_in & addr, const socklen_t & socklen,
		bool connected);

	Client & operator= (const Client & ref);

	int get_fd() const;
	unsigned int get_id() const;
	int get_read_size() const;
	std::string	& get_read_buff();
	std::string	& get_write_buff();
	
	bool	is_connected() const;
	bool	is_writeable() const;
	bool	is_readable() const;

	void	set_writeable(bool input);
	void	set_connected(bool input);
	void	set_readable(bool input);

private:
	int			_fd;
	unsigned int _id;
	sockaddr_in _addr;
	socklen_t	_socklen;
	bool		_connected;
	bool		_write_ready;
	bool		_read_ready;

	int _last_read_size;
	std::string	_read_buff;
	std::string	_to_write;
};

#endif

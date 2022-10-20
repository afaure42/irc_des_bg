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

	int getFd() const;
	unsigned int getId() const;
	int getReadSize() const;
	std::string	& getReadBuff();
	std::string	& getWriteBuff();
	
	bool	isConnected() const;
	bool	isWriteable() const;
	bool	isReadable() const;

	void	setWriteable(bool input);
	void	setConnected(bool input);
	void	setReadable(bool input);

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

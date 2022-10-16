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

	Info(unsigned int connection_id, std::string * buff, bool connected);
	Info(Client & ref);
	Info(const Info & ref);
	Info(){};

	Info & operator=(const Info & rhs);

	/*getters*/
	/**
	 * @brief get the Connection id, the connection id is unique value
	 * identifying a current connection with a client
	 * 
	 * @return connection id
	 */
	unsigned int	get_id() const;

	/**
	 * @brief get the read_buffer of this connection
	 * Be sure to erase what you have processed
	 * 
	 * @return pointer to a string
	 */
	std::string *	buff() const;

	/**
	 * @brief get connection status
	 */
	bool			is_connected() const;

	/*setters*/
	/**
	 * @brief You can set the whole buffer at once if you want to
	 * 
	 * @param ref reference to the string you want the buffer replaced with
	 * be careful to not lose unprocessed data !
	 */
	void	set_buff(const std::string & ref);

private:

	unsigned int	_connection_id;
	std::string *	_read_buff;
	bool			_connected;

};

#endif

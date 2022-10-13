#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "common.hpp"
#include <map>
#include "Server.hpp"
#include "Client.hpp"

/**
 * @brief Here is the Scheduler, shiny and beautifull
 * 	This Bad boy packs quite a lot with him, wanna tell the server
 *  to start working and sending messages ? You can !
 * 	Wanna tell processing to move their asses ? You can !
 *  Wanna get better at lol URF ? WIP !
 */
class Scheduler
{
public:

	Scheduler(Server & server);

	/**
	 * @brief Function to add to the read queue
	 *  The Scheduler will then try to ready from this client
	 *  when the read loop happens
	 * @param client client to add
	 */
	void add_to_read(const Client & client);

	/**
	 * @brief Function to add to the write queue
	 * 	The Scheduler will then try to send the msg in the buffer
	 * 
	 * @param client Client to send the msg to
	 * @param msg Data that you want to send if the buffer is not empty,
	 * 	subsequent call will concatenate
	 */
	void add_to_write(const Client & client, std::string msg);

	/**
	 * @brief Function used by the server to tell processing
	 *	when new data has been received
	 * @param client 
	 */
	void add_to_process(const Client & client);
	
	/**
	 * @brief Function used by processing when there is nothing
	 * to be done on this client
	 * @param client Client to be removed from processing queue
	 */

	void remove_from_process(const Client & client);

	/**
	 * @brief This will make the scheduler read from every
	 * client in the reading queue and add those clients to the processing
	 * queue
	 */
	void read_all(void);

	/**
	 * @brief This will make the scheduler write to every client
	 * in the writing qeue, clients will be automatically removed when
	 * the message is sent
	 */
	void write_all(void);
private:
	Server & _server;

	std::map<int, Client> _read;
	std::map<int, Client> _write;
	std::map<int, Client> _process;

	Scheduler(const Scheduler & ref); // NO COPY
	Scheduler(); // NOR DEFAULT
};

#endif

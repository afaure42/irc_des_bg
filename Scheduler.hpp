#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "common.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "read_write.hpp"
#include "Info.hpp"


typedef std::map<int, Info>::iterator Info_iter;

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
	 * @brief This will return a ref to a vector containing
	 * All info class that received an update and that havent been
	 * Processed yet
	 * @return A reference to a vector of <Info>
	 */
	std::map<int, Info> & get_updates(void);

	/**
	 * @brief Function used to send a message to a FD
	 */
	void queue_message(int fd, std::string msg);

	/**
	 * @brief Function to add to the read queue
	 *  The Scheduler will then try to read from this client
	 *  when the read loop happens
	 * @param client client to add
	 */
	void add_to_read(Client * client);

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
	Server & _server; // i need it to deconnect clients

	std::map<int, Client *> _read;
	std::map<int, Client *> _write;
	std::map<int, Info> _updates;

	Scheduler(const Scheduler & ref); // NO COPY
	Scheduler(); // NOR DEFAULT
};

#endif

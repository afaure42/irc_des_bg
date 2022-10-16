#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "common.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "read_write.hpp"
#include "Update.hpp"

class Update;
class Server;
class Client;

typedef std::map<int, Update>::iterator Info_iter;

/**
 * @brief Here is the Scheduler, shiny and beautifull
 * 	This Bad boy packs quite a lot with him, wanna tell the server
 *  to start working and sending messages ? You can !
 * 	Wanna tell processing to move their asses ? You can !
 *  Wanna get better at lol URF ? WIP !
 * 
 * \n The Scheduler uses connection ids, those are unique to each current and futur Connections
 * A new connection will not have the same id as an old closed one ( or only a INT_MAX old connection ;) )
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
	std::map<unsigned int, Update> & get_updates(void);

	/**
	 * @brief Function used to send a message to a connection
	 * 
	 * @return true if msg was succesfully added to buffer
	 * @return false if client is not connected
	 */
	bool queue_message(unsigned int connection_id, std::string msg);

	/**
	 * @brief Function to add to the read queue
	 *  The Scheduler will then try to read from this client
	 *  when the read loop happens
	 * @param connection_id id of the client you want to add
	 */
	void add_to_read(unsigned int connection_id);

	/**
	 * @brief function used by the server to tell the scheduler
	 * something new happened to this connection
	 * 
	 * @param connection_id 
	 */
	void add_to_updates(Update info);

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

	/**
	 * @brief will remove a client from read write and process queue
	 * 
	 * @param connection_id id of the client you 
	 * want to remove from the Scheduler
	 */
	void remove_from_queues(unsigned int connection_id);
private:
	Server & _server; // i need it to deconnect clients

	//sets are used because there is a no duplicate guarantee 
	//when inserting the same thing many times
	//though i think i might not need it for the read queue
	std::set<unsigned int> _read;
	std::set<unsigned int> _write;

	std::map<unsigned int, Update> _updates;



	Scheduler(const Scheduler & ref); // NO COPY
	Scheduler(); // NOR DEFAULT
};

#endif

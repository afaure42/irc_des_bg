#include "common.hpp"
#include "Scheduler.hpp"
#include "Server.hpp"

typedef std::map<unsigned int, Info>::iterator update_iter;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "USAGE : ./program <port>\n";
		return EXIT_FAILURE;
	}
	int port = std::atoi(argv[1]);

	//every actions is in a try catch block
	//the server's dtor can free all its resources
	try
	{
		Server server(port);
		Scheduler scheduler(server); 

		std::cout << "Init is Done\n";
		//init is done in the ctors so if any errors were to happen
		//an exception would have been thrown by now

		//The Server event loop consist of 4 steps
		//wait and accept
		//read
		//process
		//write

		//the process step can know what has happened during the 2 precedent phases
		//by calling scheduler.getUpdates()
		//it will then gain access to a list of Info calss containing information
		//about what happened on a specific connection
		//those may be a deconnection or new chars received

		//every connection is defined by a unique connection id
		bool server_on = true;
		while (server_on)
		{
			std::cout << "waitAndAccept\n";
			//WAIT AND ACCEPT STEP
			server.waitAndAccept(scheduler);


			std::cout << "Reading\n";
			//READING STEP
			scheduler.readAll();

			/*			PROCESSING STEP ( here just echoing what was received )*/

			std::cout << "Processing\n";
			//creating a reference to a map
			//and calling the scheduler to get all updates
			std::map <unsigned int, Info> & updates = scheduler.getUpdates();
			update_iter it = updates.begin();

			//looping on all updates
			while (it != updates.end())
			{
				if (!it->second.isConnected())
				{
					std::cout << "Client_id:" << it->second.getId()
					<< " has disconnected\n";
				}
				else if (!it->second.getBuff()->empty())
				{
					if (it->second.getBuff()->find("shutdown\n", 0)
						!= it->second.getBuff()->npos)
						server_on = false;

					scheduler.queueMessage(it->second.getId(), *it->second.getBuff());

					//do not forget to remove what you have processed from the read buffer
					//i recommend reading a little bit about erase method for std::string
					it->second.getBuff()->clear();
				}
				it++;
			}
			//never forget to remove what you have processed from the update list
			// the line <<updates.erase(*(it++));>> may find itself usefull for someone
			//wanting to erase current item and go to the next

			//because here we processed everything systematically we just clear
			updates.clear();

			std::cout << "Writing\n";
			//WRITING STEP
			scheduler.writeAll();

		}

	}
	catch(const syscall_error & e)
	{
		std::cerr << e.what() << e.err() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

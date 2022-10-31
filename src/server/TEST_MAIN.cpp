#include "common.hpp"
#include "Scheduler.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include "functionMap.hpp"

typedef std::map<unsigned int, Update>::iterator update_iter;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "USAGE : ./program <port> <password>\n";
		return EXIT_FAILURE;
	}
	int port = std::atoi(argv[1]);
	std::string pass = std::string(argv[2]);

	//every actions is in a try catch block
	//the server's dtor can free all its resources
	try
	{
		Server							server(port, pass);
		Scheduler						scheduler(server); 
		std::map<unsigned int, User>	users;
		std::vector<Channel>			channels;
		exec_fn_map						function_map;
		
		setFunctionMap(function_map);
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
			//WAIT AND ACCEPT STEP
			server.waitAndAccept(scheduler);
			//READING STEP
			scheduler.readAll();
			/*			PROCESSING STEP ( here just echoing what was received )*/

			//creating a reference to a map
			//and calling the scheduler to get all updates
			std::map <unsigned int, Update> & updates = scheduler.getUpdates();
			update_iter it = updates.begin();

			//looping on all updates
			while (it != updates.end())
			{
				if (users.find(it->first) == users.end())
					users.insert(std::make_pair(it->first, User(it->first)));
				if (!it->second.isConnected())
				{
					std::string quit_cmd = "QUIT";
					Command command(function_map, &quit_cmd, scheduler, server);

					std::cout << "Client_id:" << it->second.getId()
					<< " has disconnected unexpectedly\n";

					command.execute((it)->second.getId(), users, channels);
				}
				else if (!it->second.getBuff()->empty())
				{
					// parse and create command
					Command command(function_map, it->second.getBuff(), scheduler, server);
					// Execuuuuute
					command.execute(it->second.getId(), users, channels);
					std::cout << "\n\nCOMMAND AFTER EXECUTION:\n" << command;
					if (command.getNumericReturn() != 0)
						command.sendReplies(it->second.getId(), users, channels);
					// scheduler.queueMessage(it->second.getId(), *it->second.getBuff(), true);

					//do not forget to remove what you have processed from the read buffer
					//i recommend reading a little bit about erase method for std::string
					if (it->second.isConnected())
					{
						it->second.getBuff()->erase(0, command.getCharsRead());
						if (it->second.getBuff()->find(IRC_MSG_SEPARATOR) != it->second.getBuff()->npos)
							continue; //to keep processing
					}
				}
				updates.erase(it++);
			}
			//never forget to remove what you have processed from the update list
			// the line <<updates.erase(*(it++));>> may find itself usefull for someone
			//wanting to erase current item and go to the next

			//because here we processed everything systematically we just clear
			// updates.clear();

			// std::cout << "Writing\n";
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

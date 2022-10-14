#include "Scheduler.hpp"

Scheduler::Scheduler(Server & server)
:_server(server)
{
}

void Scheduler::add_to_read(Client * client)
{
	this->_read[client->get_fd()] = client;
}

std::map<int, Info> & Scheduler::get_updates()
{
	return this->_updates;
}

void Scheduler::read_all(void)
{
	std::map<int, Client *>::iterator it = this->_read.begin();
	//read from client
	while (it != this->_read.end())
	{
		//if something changed (deconnexion or data received)
		if (read_from_client(*it->second))
			this->_updates[it->first] = Info(*(it->second));
		// add to the updates

		if (!it->second->is_connected())
		{
			this->_server.disconnect_client(*it->second);
			//be carefull client pointer is invalid now
			this->_read.erase((it++)->first);
		}

		//if we cant read anymore, we will remove it from the read_list
		else if (!it->second->is_readable())
			this->_read.erase((it++)->first);
		else
			it++;
	}
}

void Scheduler::write_all(void)
{
	std::map<int, Client *>::iterator it = this->_write.begin();

	while (it != this->_write.end())
	{
		//if client is writeable
		if (it->second->is_writeable())
			write_to_client(*it->second);

		//if buffer empty remove from write queue
		if (it->second->get_write_buff().empty())
			this->_write.erase((it++)->first);
		else
			it++;
	}
}

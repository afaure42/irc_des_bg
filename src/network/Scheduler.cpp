#include "Scheduler.hpp"

Scheduler::Scheduler(Server & server)
:_server(server)
{
}

void Scheduler::add_to_read(unsigned int connection_id)
{
	this->_read.insert(connection_id);
}

void Scheduler::add_to_updates(Update info)
{
	this->_updates[info.get_id()] = info;
}

std::map<unsigned int, Update> & Scheduler::get_updates()
{
	return this->_updates;
}

bool Scheduler::queue_message(unsigned int connection_id, std::string msg)
{
	std::cout << "A message is being queued for connection:" << connection_id << '\n';
	if (this->_server.is_client_connected(connection_id))
	{
		this->_server.get_client(connection_id).
		get_write_buff().append(msg);
		this->_write.insert(connection_id);
		return true;
	}
	return false;
}

void Scheduler::remove_from_queues(unsigned int connection_id)
{
	this->_read.erase(connection_id);
	this->_write.erase(connection_id);
}

void Scheduler::read_all(void)
{
	std::set<unsigned int>::iterator it = this->_read.begin();
	//read from client
	while (it != this->_read.end())
	{
		//if something changed (deconnexion or data received)
		if (read_from_client(this->_server.get_client(*it)))
			this->_updates[*it] = Update(this->_server.get_client(*it));
		// add to the updates

		if (!this->_server.get_client(*it).is_connected())
		{
			this->_write.erase(*it);
			//remove from write queue
			//and properly disconnect (may throw)
			this->_server.disconnect_client(*it);
			//be carefull client pointer is invalid now

			this->_read.erase(*(it++));
		}

		//if we cant read anymore, we will remove it from the read_list
		else if (!this->_server.get_client(*it).is_readable())
			this->_read.erase(*(it++));
		else
			it++;
	}
}

void Scheduler::write_all(void)
{
	std::set<unsigned int>::iterator it = this->_write.begin();

	while (it != this->_write.end())
	{
		//if client is writeable
		if (this->_server.get_client(*it).is_writeable())
			write_to_client(this->_server.get_client(*it));

		//if buffer empty remove from write queue
		if (this->_server.get_client(*it).get_write_buff().empty())
			this->_write.erase(*(it++));
		else
			it++;
	}
}

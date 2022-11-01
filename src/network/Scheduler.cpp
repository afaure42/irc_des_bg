#include "Scheduler.hpp"

Scheduler::Scheduler(Server & server)
:_server(server)
{
}

void Scheduler::addToRead(unsigned int connection_id)
{
	this->_read.insert(connection_id);
}

void Scheduler::addToUpdates(Update info)
{
	this->_updates[info.getId()] = info;
}

std::map<unsigned int, Update> & Scheduler::getUpdates()
{
	return this->_updates;
}

bool Scheduler::queueMessage(unsigned int connection_id, std::string msg, bool connected)
{
	if (this->_server.isClientConnected(connection_id))
	{
		std::cout << "A message is being queued for connection:" << connection_id << '\n';
		this->_server.getClient(connection_id).getWriteBuff().append(msg);
		this->_write.insert(connection_id);
		this->_server.getClient(connection_id).setConnected(connected);
		return true;
	}
	return false;
}

void Scheduler::removeFromQueues(unsigned int connection_id)
{
	this->_read.erase(connection_id);
	this->_write.erase(connection_id);
	this->_updates.at(connection_id).setConnected(false);
}

void Scheduler::readAll(void)
{
	std::set<unsigned int>::iterator it = this->_read.begin();
	//read from client
	while (it != this->_read.end())
	{
		//if something changed (deconnexion or data received)
		if (read_from_client(this->_server.getClient(*it)))
			this->_updates[*it] = Update(this->_server.getClient(*it));
		// add to the updates
		
		//if we cant read anymore, we will remove it from the read_list
		else if (!this->_server.getClient(*it).isReadable())
			this->_read.erase(*(it++));
		else
			it++;
	}
}

void Scheduler::writeAll(void)
{
	std::set<unsigned int>::iterator it = this->_write.begin();

	while (it != this->_write.end())
	{
		//if client is writeable
		if (this->_server.getClient(*it).isWriteable())
			write_to_client(this->_server.getClient(*it));

		//if buffer empty remove from write queue
		if (this->_server.getClient(*it).getWriteBuff().empty())
				this->_write.erase(*(it++));
		else
			it++;
	}
}

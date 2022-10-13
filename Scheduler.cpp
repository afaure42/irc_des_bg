#include "Scheduler.hpp"

Scheduler::Scheduler(Server & server)
:_server(server)
{
}

void Scheduler::add_to_read(const Client & client)
{
	this->_read[client.get_fd()] = client;
}

void Scheduler::add_to_write(const Client & client, std::string msg)
{
	this->_write[client.get_fd()] = client;
}

void Scheduler::add_to_process(const Client & client)
{
	this->_process[client.get_fd()] = client;
}

void Scheduler::remove_from_process(const Client & client)
{
	this->_process.erase(client.get_fd());
}

void read_all(void)
{
	
}

void write_all(void)
{

}

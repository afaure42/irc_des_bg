#include "readWrite.hpp"

void write_to_client(Client & client)
{
	int r;

	std::cout << "Reply sent to client " << client.getId() << " :" << client.getWriteBuff();
	int length = client.getWriteBuff().length();
	r = write(client.getFd(), client.getWriteBuff().c_str(),
		WRITE_SIZE < length ? WRITE_SIZE : length);

	if (r > 0)
		client.getWriteBuff().erase(0, r);

	client.setWriteable(false);
}

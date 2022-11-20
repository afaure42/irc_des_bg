#include "readWrite.hpp"

bool read_from_client(Client & client)
{
	char buff[READ_SIZE + 1];
	int r;
	bool in_received = false;

	r = read(client.getFd(), buff, READ_SIZE);
	if (r > 0)
	{
		buff[r] = '\0';
		client.getReadBuff().append(buff);
		in_received = true;
	}

	client.setReadable(false);
	return in_received;
}

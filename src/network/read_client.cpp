#include "read_write.hpp"

bool read_from_client(Client & client)
{
	char buff[READ_SIZE + 1];
	int r;
	bool in_received = false;

	while (client.isReadable())
	{
		r = read(client.getFd(), buff, READ_SIZE);
		if (r < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				errno = 0;
				client.setReadable(false);
			}
			else
				throw(syscall_error(errno,
					"read_from_client: read:"));
		}
		else if (r == 0)
		{
			client.setReadable(false);
			client.setConnected(false);
		}
		else
		{
			buff[r] = '\0';
			client.getReadBuff().append(buff);
		}
		in_received = true;
	}
	std::cout << "Message received from connection:"
	<< client.getId() << ":<" << client.getReadBuff() << ">\n";
	return in_received;
}

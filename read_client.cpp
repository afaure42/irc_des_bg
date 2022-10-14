#include "read_write.hpp"

bool read_from_client(Client & client)
{
	char buff[READ_SIZE + 1];
	int r;
	bool in_received = false;

	while (client.is_readable())
	{
		r = read(client.get_fd(), buff, READ_SIZE);
		if (r < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				errno = 0;
				client.set_readable(false);
			}
			else
				throw(syscall_error(errno,
					"read_from_client: read:"));
		}
		if (r == 0)
		{
			client.set_readable(false);
			client.set_connected(false);
		}
		buff[r] == '\0';
		client.get_read_buff().append(buff);
		in_received = true;
	}
	return in_received;
}

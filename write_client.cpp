#include "read_write.hpp"

void write_to_client(Client & client)
{
	int r;

	while (client.is_writeable())
	{
		r = write(client.get_fd(), client.get_read_buff().c_str(), WRITE_SIZE);
		if (r < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				errno = 0;
				client.set_writeable(false);
			}
			else
				throw (syscall_error(errno,
						"write_to_client: write:"));
		}
		if (r == 0)
			client.set_writeable(false);
		client.get_read_buff().erase(0, r);
	}
}

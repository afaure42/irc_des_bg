#include "read_write.hpp"

void write_to_client(Client & client)
{
	int r;

	std::cout << "Writing to client:" << client.get_id() << '\n'
	<< "Message sent is :" << client.get_write_buff();
	while (client.is_writeable() && !client.get_write_buff().empty())
	{
		int length = client.get_write_buff().length();
		r = write(client.get_fd(), client.get_write_buff().c_str(),
		 WRITE_SIZE < length ? WRITE_SIZE : length);
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
		else if (r == 0)
			client.set_writeable(false);
		else
		{
			client.get_write_buff().erase(0, r);
		}
	}
}

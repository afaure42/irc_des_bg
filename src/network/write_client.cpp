#include "read_write.hpp"

void write_to_client(Client & client)
{
	int r;

	std::cout << "Writing to client:" << client.getId() << '\n'
	<< "Message sent is :" << client.getWriteBuff();
	while (client.isWriteable() && !client.getWriteBuff().empty())
	{
		int length = client.getWriteBuff().length();
		r = write(client.getFd(), client.getWriteBuff().c_str(),
		 WRITE_SIZE < length ? WRITE_SIZE : length);
		if (r < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				errno = 0;
				client.setWriteable(false);
			}
			else
				throw (syscall_error(errno,
						"write_to_client: write:"));
		}
		else if (r == 0)
			client.setWriteable(false);
		else
		{
			client.getWriteBuff().erase(0, r);
		}
	}
}

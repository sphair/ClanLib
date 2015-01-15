
#include "precomp.h"
#include "ident_server_connection.h"
#include "ident_server.h"

IdentServerConnection::IdentServerConnection(IdentServer *server, clan::TCPConnection connection)
: server(server), connection(connection)
{
	thread = std::thread(&IdentServerConnection::worker_main, this);
}

IdentServerConnection::~IdentServerConnection()
{
	std::unique_lock<std::mutex> lock(mutex);
	stop_flag = true;
	lock.unlock();
	change_event.notify();
	thread.join();
}

void IdentServerConnection::worker_main()
{
	try
	{
		std::string input_message;
		while (true)
		{
			std::unique_lock<std::mutex> lock(mutex);

			if (stop_flag)
				break;

			char buffer[1024];
			int bytes_read = connection.read(buffer, 1024);
			if (bytes_read != -1)
			{
				lock.unlock();

				int start = 0;
				for (int i = 0; i < bytes_read; i++)
				{
					if (buffer[i] == '\n')
					{
						input_message.append(buffer, i - start);
						if (!input_message.empty() && input_message[input_message.length() - 1] == '\r')
							input_message.erase(input_message.begin() + (input_message.length() - 1));

						if (!received_message(input_message))
							return;

						input_message.clear();
						start = i + 1;
					}
				}
				input_message.append(buffer + start, bytes_read - start);

				if (input_message.length() > 1024)
					throw clan::Exception("Invalid request");
			}
			else
			{
				clan::NetworkEvent *events[] = { &connection };
				if (!change_event.wait(lock, 1, events, 60 * 1000))
					return;
			}
		}
	}
	catch (clan::Exception &)
	{
	}
}

bool IdentServerConnection::received_message(std::string message)
{
	std::string::size_type p = message.find(',');
	if (p == std::string::npos)
		throw clan::Exception("Invalid request");
	std::string port_on_server = clan::StringHelp::trim(clan::StringHelp::utf8_to_text(message.substr(0, p)));
	std::string port_on_client = clan::StringHelp::trim(clan::StringHelp::utf8_to_text(message.substr(p+1)));
	
	std::string response = clan::string_format("%1,%2:UNIX:carambola\r\n", port_on_server, port_on_client);

	std::unique_lock<std::mutex> lock(mutex);

	int pos = 0;
	while (pos != response.length())
	{
		int written = connection.write(response.data() + pos, response.length() - pos);
		if (written == -1)
		{
			clan::NetworkEvent *events[] = { &connection };
			if (!change_event.wait(lock, 1, events, 60 * 1000))
				return false;
		}
		else
		{
			pos += written;
		}
	}

	return true;
}

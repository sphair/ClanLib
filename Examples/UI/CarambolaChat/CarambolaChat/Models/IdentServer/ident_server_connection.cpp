
#include "precomp.h"
#include "ident_server_connection.h"
#include "ident_server.h"

IdentServerConnection::IdentServerConnection(IdentServer *server, clan::TCPConnection connection)
: server(server), connection(connection)
{
	thread.start(this, &IdentServerConnection::worker_main);
}

void IdentServerConnection::join()
{
	thread.join();
}

void IdentServerConnection::worker_main()
{
	try
	{
		std::string input_message;
		while (true)
		{
			int wakeup_reason = clan::Event::wait(server->get_stop_event(), connection.get_read_event(), 60*1000);
			if (wakeup_reason <= 0)
			{
				break;
			}
			else if (wakeup_reason == 1)
			{
				char buffer[1024];
				connection.get_read_event().wait();
				int bytes_read = connection.read(buffer, 1024, false);
				if (bytes_read == 0)
					break;
				int start = 0;
				for (int i = 0; i < bytes_read; i++)
				{
					if (buffer[i] == '\n')
					{
						input_message.append(buffer, i-start);
						if (!input_message.empty() && input_message[input_message.length()-1] == '\r')
							input_message.erase(input_message.begin()+(input_message.length()-1));
						received_message(input_message);
						input_message.clear();
						start = i+1;
					}
				}
				input_message.append(buffer+start, bytes_read-start);

				if (input_message.length() > 1024)
					throw clan::Exception("Invalid request");
			}
		}
		connection.disconnect_graceful();
	}
	catch (clan::Exception &)
	{
		connection.disconnect_abortive();
	}
}

void IdentServerConnection::received_message(std::string message)
{
	std::string::size_type p = message.find(',');
	if (p == std::string::npos)
		throw clan::Exception("Invalid request");
	std::string port_on_server = clan::StringHelp::trim(clan::StringHelp::utf8_to_text(message.substr(0, p)));
	std::string port_on_client = clan::StringHelp::trim(clan::StringHelp::utf8_to_text(message.substr(p+1)));
	
	std::string response = clan::string_format("%1,%2:UNIX:carambola\r\n", port_on_server, port_on_client);
	connection.write(response.data(), response.length(), true);
}


#include "precomp.h"
#include "ident_server.h"
#include "ident_server_connection.h"

IdentServer::IdentServer()
{
}

IdentServer::~IdentServer()
{
	stop();
}

void IdentServer::start()
{
	thread.start(this, &IdentServer::worker_main);
}

void IdentServer::stop()
{
	stop_event.set();
	thread.join();
	stop_event.reset();
}

void IdentServer::worker_main()
{
	std::vector<IdentServerConnection *> connections;

	try
	{
		clan::TCPListen listen(clan::SocketName("113"));
		while (true)
		{
			int wakeup_reason = clan::Event::wait(stop_event, listen.get_accept_event());
			if (wakeup_reason != 1)
				break;
			connections.push_back(new IdentServerConnection(this, listen.accept()));
		}
	}
	catch (clan::Exception &)
	{
		stop_event.set();
	}

	for (size_t i = 0; i < connections.size(); i++)
	{
		connections[i]->join();
		delete connections[i];
	}
}

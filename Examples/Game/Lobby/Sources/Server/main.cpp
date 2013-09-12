
#include "precomp.h"
#include "server.h"

class ServerService : public clan::Service
{
public:
	ServerService() : clan::Service("ClanLib Server")
	{
	}

	void service_start(std::vector<std::string> &args)
	{
		clan::log_event("system", "Starting service..");
		thread.start(this, &ServerService::game_thread_main);
		clan::log_event("system", "Service started.");
	}

	void service_reload()
	{
	}

	void service_stop()
	{
		clan::log_event("system", "Stopping service..");
		stop_event.set();
		thread.join();
		clan::log_event("system", "Service stopped.");
	}

private:
	void game_thread_main()
	{
		Server server;
		server.exec(stop_event);
	}

	clan::ConsoleLogger logger;
	clan::Thread thread;
	clan::Event stop_event;
};

int main(int argc, char **argv)
{
	try
	{
		clan::SetupCore setup_core;
		clan::SetupNetwork setup_network;

		ServerService service;
		return service.main(argc, argv);
	}
	catch (clan::Exception &e)
	{
		clan::Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
	}

//	clan::Console::wait_for_key();
}

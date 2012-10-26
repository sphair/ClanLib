
#include "precomp.h"
#include "server.h"

class DiceWarService : public Service
{
public:
	DiceWarService() : Service("Dice War Server")
	{
	}

	void service_start(std::vector<std::string> &args)
	{
		cl_log_event("system", "Starting service..");
		thread.start(this, &DiceWarService::game_thread_main);
		cl_log_event("system", "Service started.");
	}

	void service_reload()
	{
	}

	void service_stop()
	{
		cl_log_event("system", "Stopping service..");
		stop_event.set();
		thread.join();
		cl_log_event("system", "Service stopped.");
	}

private:
	void game_thread_main()
	{
		Server server;
		server.exec(stop_event);
	}

	ConsoleLogger logger;
	Thread thread;
	Event stop_event;
};

int main(int argc, char **argv)
{
	try
	{
		SetupCore setup_core;
		SetupNetwork setup_network;

		DiceWarService service;
		return service.main(argc, argv);
	}
	catch (Exception &e)
	{
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
	}
}

#include <ClanLib/core.h>
#include <ClanLib/network.h>
using namespace clan;

#include "client.h"

#ifdef WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int, char**)
#endif
{
	try
	{
		SetupCore setup_core;
		SetupNetwork setup_network;

		ConsoleWindow console("Console", 160, 1000);
		ConsoleLogger logger;

		Client client;
		client.exec();

		return 0;
	}
	catch (Exception e)
	{
#ifdef WIN32
		MessageBox(0, e.get_message_and_stack_trace().c_str(), TEXT("Unhandled Exception"), MB_OK);	
#else
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
#endif

		return 1;
	}
}

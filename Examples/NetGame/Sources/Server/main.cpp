#include "server.h"

#ifdef WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int, char**)
#endif
{
	try
	{
		CL_SetupCore setup_core;
		CL_SetupNetwork setup_network;

		CL_ConsoleWindow console("Console", 160, 1000);
		CL_ConsoleLogger logger;

		Server server;
		server.exec();

		return 0;
	}
	catch (CL_Exception e)
	{
#ifdef WIN32
		MessageBox(0, e.get_message_and_stack_trace().c_str(), TEXT("Unhandled Exception"), MB_OK);	
#else
		CL_Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
#endif
		return 1;
	}
}

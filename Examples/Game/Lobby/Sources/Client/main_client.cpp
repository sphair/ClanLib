
#include "precomp.h"
#include "client.h"

#ifdef WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int, char**)
#endif
{
	try
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_SetupNetwork setup_network;

		CL_ConsoleWindow console("Console", 160, 1000);
		CL_ConsoleLogger logger;

		Client client;
		client.exec();

		return 0;
	}
	catch (CL_Exception e)
	{
#ifdef WIN32
		MessageBoxA(0, e.get_message_and_stack_trace().c_str(), "Unhandled Exception", MB_OK);	
#else
		CL_Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
#endif
		return 1;
	}
}

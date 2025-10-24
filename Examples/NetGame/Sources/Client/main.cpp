#include <ClanLib/core.h>
#include <ClanLib/network.h>

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
		std::vector<CL_String> stack_trace = e.get_stack_trace();
		CL_String text = e.message + "\r\n";
		for (unsigned int i = 0; i < stack_trace.size(); i++)
			text += cl_format("\r\n#%1 %2", i, stack_trace[i]);

		MessageBox(0, text.c_str(), TEXT("Unhandled Exception"), MB_OK);
#else
		CL_Console::write_line("Unhandled exception: %1", e.message);
#endif

		return 1;
	}
}

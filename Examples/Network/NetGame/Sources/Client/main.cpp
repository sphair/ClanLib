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
		ConsoleWindow console("Console", 160, 1000);
		ConsoleLogger logger;

		Client client;
		client.exec();

		return 0;
	}
	catch (Exception e)
	{
#ifdef WIN32
#ifdef UNICODE
		std::string trace = e.get_message_and_stack_trace();
		std::wstring wtrace(trace.begin(), trace.end());
		MessageBox(0, wtrace.c_str(), TEXT("Unhandled Exception"), MB_OK);
#else
		MessageBox(0, e.get_message_and_stack_trace().c_str(), TEXT("Unhandled Exception"), MB_OK);
#endif
#else
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
#endif

		return 1;
	}
}

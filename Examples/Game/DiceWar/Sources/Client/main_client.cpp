
#include "precomp.h"
#include "client.h"

//#ifdef _MSC_VER
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#endif

#ifdef WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int, char**)
#endif
{
	try
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;
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
		MessageBoxA(0, e.get_message_and_stack_trace().c_str(), "Unhandled Exception", MB_OK);	
#else
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
#endif
		return 1;
	}
}

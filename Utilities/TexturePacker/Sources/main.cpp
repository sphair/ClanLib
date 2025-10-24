#include "precomp.h"
#include "application.h"

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		//CL_SetupGDI setup_gdi;

		CL_ConsoleWindow console("Console", 140, 2000);
		CL_ConsoleLogger logger;

		try
		{
			Application app;
			app.run();
		}
		catch (CL_Exception e)
		{
#ifdef WIN32
			MessageBox(0, e.message.c_str(), TEXT("Unhandled Exception"), MB_OK|MB_ICONERROR);
#else
			CL_Console::write_line("Unhandled exception: %1", e.message);
#endif
  			console.display_close_message();
			return 0;
		}

		return 1;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

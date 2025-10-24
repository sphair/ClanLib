
#include "precomp.h"
#include "application.h"

#if _MSC_VER >= 1300
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		//CL_SetupGDI setup_gdi;

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
			return 0;
		}

		return 1;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

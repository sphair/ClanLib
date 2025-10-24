
#include "precomp.h"
#include "hid.h"
#include "hid_window.h"

int main(int, char**)
{
	try
	{
		CL_SetupCore setup_core;
		HIDWindow hid_window;

		Window::exec();
	}
	catch (CL_Exception &e)
	{
		CL_Console::write_line(e.message);
	}

}

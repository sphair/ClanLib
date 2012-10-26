
#include "precomp.h"
#include "hid.h"
#include "hid_window.h"

int main(int, char**)
{
	try
	{
		SetupCore setup_core;
		HIDWindow hid_window;

		Window::exec();
	}
	catch (Exception &e)
	{
		Console::write_line(e.message);
	}

}

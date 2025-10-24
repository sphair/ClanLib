/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Kenneth Gangstoe
*/

#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/swrender.h>

class TestApp
{
public:
	virtual int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");

		try
		{
			CL_DisplayWindowDescription desc;
			desc.set_size(CL_Size(800,600), true);
			desc.set_title("Window Sizing Test");
			desc.set_allow_resize(true);
			CL_DisplayWindow window(desc);
			
			CL_Slot slot_close = window.sig_window_close().connect(this, &TestApp::on_window_close);
			CL_Slot slot_resize = window.sig_resize().connect(this, &TestApp::on_resize);
			CL_Slot slot_maximized = window.sig_window_maximized().connect(this, &TestApp::on_window_maximized);
			CL_Slot slot_minimized = window.sig_window_minimized().connect(this, &TestApp::on_window_minimized);

			CL_GraphicContext &gc = window.get_gc();

			while (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
			{
				gc.clear(CL_Colorf::gray70);

				window.flip();
				CL_KeepAlive::process();
				CL_System::sleep(50);
			}
		}
		catch(CL_Exception error)
		{
			CL_Console::write_line("Exception caught:");
			CL_Console::write_line(error.message);

			console.display_close_message();
			return -1;
		}

		return 0;
	}

	void on_window_close()
	{
		CL_Console::write_line("sig_window_close()");
	}

	void on_resize(int width, int height)
	{
		CL_Console::write_line("sig_resize(%1, %2)", width, height);
	}

	void on_window_maximized()
	{
		CL_Console::write_line("sig_window_maximized()");
	}

	void on_window_minimized()
	{
		CL_Console::write_line("sig_window_minimized()");
	}
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupSWRender setup_swrender;

		TestApp app;
		return app.main(args);
	}
};

CL_ClanApplication app(&Program::main);

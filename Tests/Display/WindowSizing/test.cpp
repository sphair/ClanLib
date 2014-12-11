/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include <ClanLib/d3d.h>
using namespace clan;

class TestApp
{
public:
	virtual int main(const std::vector<std::string> &args)
	{
		ConsoleWindow console("Console");

		try
		{
			DisplayWindowDescription desc;
			desc.set_size(Size(800,600), true);
			desc.set_title("Window Sizing Test");
			desc.set_allow_resize(true);
			DisplayWindow window(desc);
			
			Slot slot_close = window.sig_window_close().connect(this, &TestApp::on_window_close);
			Slot slot_resize = window.sig_resize().connect(this, &TestApp::on_resize);
			Slot slot_maximized = window.sig_window_maximized().connect(this, &TestApp::on_window_maximized);
			Slot slot_minimized = window.sig_window_minimized().connect(this, &TestApp::on_window_minimized);

			GraphicContext &gc = window.get_gc();

			while (!window.get_ic().get_keyboard().get_keycode(keycode_escape))
			{
				gc.clear(Colorf::gray70);

				window.flip();
				KeepAlive::process();
				System::sleep(50);
			}
		}
		catch(Exception error)
		{
			Console::write_line("Exception caught:");
			Console::write_line(error.message);

			console.display_close_message();
			return -1;
		}

		return 0;
	}

	void on_window_close()
	{
		Console::write_line("sig_window_close()");
	}

	void on_resize(int width, int height)
	{
		Console::write_line("sig_resize(%1, %2)", width, height);
	}

	void on_window_maximized()
	{
		Console::write_line("sig_window_maximized()");
	}

	void on_window_minimized()
	{
		Console::write_line("sig_window_minimized()");
	}
};

class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupD3D setup_d3d;

		TestApp app;
		return app.main(args);
	}
};

Application app(&Program::main);

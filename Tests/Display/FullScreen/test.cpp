/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Harry Storbacka
*/

#include "test.h"

// This is the Program class that is called by Application
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;

		// Start the Application
		TestApp app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

int TestApp::main(const std::vector<std::string> &args)
{
	// Create a console window for text-output if not available
	ConsoleWindow console("Console");

	try
	{
		DisplayWindowDescription desc;
		desc.set_size(Size(800,600), true);
		desc.set_title("Fullscreen test");
		DisplayWindow window(desc);
		
		while (!window.get_ic().get_keyboard().get_keycode(keycode_escape))
		{
			if (window.get_ic().get_keyboard().get_keycode(keycode_f11))
			{
				desc.set_fullscreen(!desc.is_fullscreen());
				window = DisplayWindow(desc);
			}

			window.get_gc().clear(Colorf::gray30);

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





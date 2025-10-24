/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		// Start the Application
		TestApp app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

int TestApp::main(const std::vector<CL_String> &args)
{
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console");

	try
	{
		CL_DisplayWindowDescription desc;
		desc.set_size(CL_Size(800,600), true);
		desc.set_title("Image clipboard test");
		CL_DisplayWindow window(desc);
		
		CL_PixelBuffer to_clipboard = CL_ImageProviderFactory::load("copy.png");
		window.set_clipboard_image(to_clipboard);

/*		while (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
		{
			window.get_gc().clear(CL_Colorf::gray70);
			window.flip();
			CL_KeepAlive::process();
			CL_System::sleep(50);
		}*/

/*		// Save to file
		CL_PNGProvider::save(to_clipboard, "copy2.png");

		// Save to CL_IODevice_Memory memory buffer
		CL_DataBuffer data(1024*8); // 8 kb
		CL_IODevice_Memory mem(data);
		CL_PNGProvider::save(to_clipboard, mem);
		CL_File file("copy3.png", CL_File::create_always);
		CL_DataBuffer mem_data = mem.get_data();
		file.write(mem_data.get_data(), mem_data.get_size());
*/	

	
 		CL_PixelBuffer from_clipboard;
		if (window.is_clipboard_image_available())
		{
			from_clipboard = window.get_clipboard_image();
			CL_PNGProvider::save(from_clipboard, "from_clipboard.png");
		} 

		if (!from_clipboard.is_null())
		{
			CL_Texture tex(window.get_gc(), from_clipboard.get_size());
			tex.set_image(from_clipboard);
			tex.set_min_filter(cl_filter_nearest);
			tex.set_mag_filter(cl_filter_nearest);

			while (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
			{
				window.get_gc().clear(CL_Colorf::gray30);

				if (!from_clipboard.is_null())
				{
					CL_Draw::texture(window.get_gc(), CL_Rect(0,0,tex.get_width(), tex.get_height()));
				}

				window.flip();
				CL_KeepAlive::process();
				CL_System::sleep(50);
			}
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





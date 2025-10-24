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
	CL_ConsoleWindow console("Console", 150,300);

	try
	{
		CL_ResourceManager resources("../../../Resources/GUIThemeAero/resources.xml");

		CL_GUIWindowManagerSystem wm;
		gui.set_window_manager(wm);

		CL_GUIThemeDefault theme;
		theme.set_resources(resources);
		gui.set_theme(theme); 
		gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");


		window = new CL_Window(&gui, win_desc);
		button = new CL_PushButton(window);
		button->set_geometry(CL_RectPS(11,11,75,23));
		button->set_text("I do nothing!");

		CL_AcceleratorTable accel_table;
		CL_AcceleratorKey accel_fullscreen(CL_KEY_F11);
		accel_fullscreen.func_pressed().set(this, &TestApp::on_accelerator_callback);
		accel_table.add_accelerator(accel_fullscreen);

		gui.exec(accel_table);
	}
	catch(CL_Exception error)
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(error.message);

		std::vector<CL_String> stack_trace = error.get_stack_trace();
		std::vector<CL_String>::size_type i;
		for (i = 0; i < stack_trace.size(); i++)
		{
			CL_Console::write_line(stack_trace[i]);
		}

		console.display_close_message();
		return -1;
	}

	return 0;
}

void TestApp::on_accelerator_callback( CL_GUIMessage msg, CL_AcceleratorKey key )
{
	if (key.get_id() == CL_KEY_F11)
	{
		CL_DisplayWindowDescription desc;
		desc.set_allow_resize(true);
		desc.set_title("WM System Fullscreen Toggling Test Application");
		desc.set_position(CL_Rect(200, 200, 940, 640), false);

		CL_Window *new_window = new CL_Window(&gui, desc);

		button->set_parent_component(new_window);

		//_ASSERTE( _CrtCheckMemory( ) );

		delete window;

		//_ASSERTE( _CrtCheckMemory( ) );

		window = new_window;
	}
}




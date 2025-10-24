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
**    Magnus Norddahl
*/

#include "precomp.h"
#include "main_window.h"

class Application
{
public:
	int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_ResourceManager local_resources("resources.xml");

			CL_String resource_filename("../../Resources/GUIThemeAero/resources.xml");
			CL_String theme_filename("theme.css");
			if (!CL_FileHelp::file_exists(resource_filename))
			{
				resource_filename = "../../Resources/GUIThemeBasic/resources.xml";
				theme_filename = "theme_basic.css";
			}

			CL_ResourceManager resources(resource_filename);
			local_resources.add_resources(resources);

			CL_GUIThemeDefault theme;
			theme.set_resources(local_resources);

			CL_GUIWindowManagerSystem wm;

			CL_GUIManager gui;
			gui.set_window_manager(wm);
			gui.set_theme(theme);
			gui.set_css_document(theme_filename);

			MainWindow mainwindow(&gui, &resources);
			gui.exec();
		}
		catch (CL_Exception &exception)
		{
			CL_ConsoleWindow console("Console", 160, 1000);
			CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
			console.display_close_message();
		}

		return 0;
	}
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
//		CL_SetupSWRender setup_swrender;
		CL_SetupGL setup_gl;

		Application app;
		return app.main(args);
	}
};

CL_ClanApplication app(&Program::main);


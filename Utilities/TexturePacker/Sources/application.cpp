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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "main_window.h"

class TextureApplication
{
public:
	int main(const std::vector<std::string> &args)
	{
		clan::XMLResourceDocument local_resources_doc("resources.xml");

		std::string resource_filename("../../Resources/GUIThemeAero/resources.xml");
		std::string theme_filename("../../Resources/GUIThemeAero/theme.css");
		std::string local_theme_filename("theme.css");
		if (!FileHelp::file_exists(resource_filename))
		{
			resource_filename = "../../Resources/GUIThemeBasic/resources.xml";
			std::string theme_filename("../../Resources/GUIThemeBasic/theme.css");
			local_theme_filename = "theme_basic.css";
		}
		clan::XMLResourceDocument resources_doc(resource_filename);
		local_resources_doc.add_resources(resources_doc);

		ResourceManager resources(clan::XMLResourceManager::create(local_resources_doc));

		GUIWindowManagerSystem wm;

		GUIManager gui;
		gui.set_window_manager(wm);
		gui.add_resources(resources_doc);
		gui.add_resources(resource_filename);
		gui.add_theme(theme_filename);
		gui.add_theme(local_theme_filename);

		MainWindow mainwindow(&gui, &resources);
		gui.exec();
		return 0;
	}
};

class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
//		SetupD3D setup_d3d;
		SetupGL setup_gl;

		TextureApplication app;
		return app.main(args);
	}
};

Application app(&Program::main);


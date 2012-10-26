/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "precomp.h"
#include "application.h"
#include "MainWindow/gui_editor_window.h"

Application::Application()
{
	std::string base_path = System::get_exe_path();
	if(FileHelp::file_exists(base_path + "Resources\\resources.xml") == false)
		base_path += "..\\";
	Directory::set_current(base_path);

	resources = ResourceManager("Resources\\resources.xml");

	std::string global_resource_filename("../../Resources/GUIThemeAero/resources.xml");
	std::string global_theme_filename("../../Resources/GUIThemeAero/theme.css");
	std::string local_theme_filename("Resources\\theme.css");
	if (!FileHelp::file_exists(global_resource_filename))
	{
		global_resource_filename = "../../Resources/GUIThemeBasic/resources.xml";
		global_theme_filename = "../../Resources/GUIThemeBasic/theme.css";
		local_theme_filename = "Resources\\theme_basic.css";
	}

	ResourceManager global_resources(global_resource_filename);
	resources.add_resources(global_resources);
	theme.set_resources(resources);
	gui.set_window_manager(window_manager);
	gui.set_theme(theme);

	CSSDocument css;
	css.load(global_theme_filename);
	css.load(local_theme_filename);

	gui.set_css_document(css);
}

Application::~Application()
{
}

void Application::run(const std::string &filename)
{
	GuiEditorWindow main_window(get_gui());

	if(!filename.empty())
		main_window.load(filename);

	gui.exec();
}

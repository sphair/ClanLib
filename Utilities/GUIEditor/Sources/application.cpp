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

XApplication::XApplication()
{
	std::string base_path = System::get_exe_path();
	if(FileHelp::file_exists(base_path + "Resources\\resources.xml") == false)
		base_path += "..\\";
	Directory::set_current(base_path);

	std::string global_resource_filename("../../Resources/GUIThemeAero");
	std::string local_theme_filename("Resources\\theme.css");
	if (!FileHelp::file_exists("../../Resources/GUIThemeAero/resources.xml"))
	{
		global_resource_filename = "../../Resources/GUIThemeBasic";
		local_theme_filename = "Resources\\theme_basic.css";
	}

	gui = clan::GUIManager(window_manager, global_resource_filename);
	gui.add_resources(clan::XMLResourceDocument("Resources\\resources.xml"));
	gui.add_theme(local_theme_filename);
}

XApplication::~XApplication()
{
}

void XApplication::run(const std::string &filename)
{
	GuiEditorWindow main_window(get_gui());

	if(!filename.empty())
		main_window.load(filename);

	gui.exec();
}

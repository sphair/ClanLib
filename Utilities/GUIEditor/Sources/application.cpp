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

#include "precomp.h"
#include "application.h"
#include "main_window.h"
#include "component_types.h"

Application::Application()
{
	ComponentTypes::initialize();

	CL_String base_path = CL_System::get_exe_path();
	if(CL_FileHelp::file_exists(base_path + "resources.xml") == false)
		base_path += "..\\";
	CL_Directory::set_current(base_path);

	resources = CL_ResourceManager("resources.xml");

	CL_String resource_filename("../../Resources/GUIThemeAero/resources.xml");
	CL_String theme_filename("theme.css");
	if (!CL_FileHelp::file_exists(resource_filename))
	{
		resource_filename = "../../Resources/GUIThemeBasic/resources.xml";
		theme_filename = "theme_basic.css";
	}

	CL_ResourceManager resources2(resource_filename);
	resources.add_resources(resources2);
	theme.set_resources(resources);
	gui.set_window_manager(window_manager);
	gui.set_theme(theme);
	gui.set_css_document(theme_filename);
}

Application::~Application()
{
	ComponentTypes::deinitialize();
}

void Application::run(const CL_String &filename)
{
	MainWindow main_window(this);

	if(!filename.empty())
		main_window.load(filename);

	gui.exec();
}

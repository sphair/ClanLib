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
**    Mark Page
*/

#include "precomp.h"
#include "GUI.h"

GUI::GUI()
{
}

int GUI::start(const std::vector<std::string> &args)
{
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
	{
		current_theme = Theme::theme_aero;
	}
	else
	{
		current_theme = Theme::theme_basic;
	}

	resources_internal = clan::ResourceManager("../CommonCode/Resources/resources.xml");

	gui_system.reset(new GUI_System(this) );

	gui_system->gui_manager.exec();

	return 0;
}

GUI::~GUI()
{
}

const char *GUI::get_theme_location()
{
	if (current_theme == Theme::theme_aero)
		return "../../../Resources/GUIThemeAero";

	if (current_theme == Theme::theme_aero_packed)
		return "../../../Resources/GUIThemeAeroPacked";

	if (current_theme == Theme::theme_basic)
		return "../../../Resources/GUIThemeBasic";

	return "../../../Resources/GUIThemeAero";

}


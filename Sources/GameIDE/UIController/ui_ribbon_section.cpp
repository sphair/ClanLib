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
**    Magnus Norddahl
*/

#include "GameIDE/precomp.h"
#include "API/GameIDE/UIController/ui_ribbon_section.h"
#include "API/GameIDE/UIController/ui_controller.h"

namespace clan
{

class UIRibbonSection_Impl
{
public:
	UIRibbonSection_Impl() : ui_controller(nullptr), is_visible(false) { }
	UIController *ui_controller;
	std::string page_name;
	std::string section_name;
	bool is_visible;
};

UIRibbonSection::UIRibbonSection(UIController *ui_controller, const std::string &page_name, const std::string &section_name)
	: GUIComponent(ui_controller->get_ribbon_section(page_name, section_name)), impl(new UIRibbonSection_Impl())
{
	impl->ui_controller = ui_controller;
	impl->page_name = page_name;
	impl->section_name = section_name;
}

void UIRibbonSection::show_section(bool enable)
{
	if (impl->is_visible != enable)
	{
		impl->is_visible = enable;
		if (enable)
		{
			impl->ui_controller->show_ribbon_section(impl->page_name, impl->section_name);
		}
		else
		{
			impl->ui_controller->hide_ribbon_section(impl->page_name, impl->section_name);
		}
	}
}

}

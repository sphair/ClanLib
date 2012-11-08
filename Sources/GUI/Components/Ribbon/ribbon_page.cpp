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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/Components/ribbon_page.h"
#include "API/GUI/Components/ribbon_section.h"

#ifdef INCLUDE_COMPONENTS

namespace clan
{

//////////////////////////////////////////////////////////////////////////
// Construction

RibbonPage::RibbonPage(GUIComponent *parent, const std::string &text)
: GUIComponent(parent), text(text), show_tab(true)
{
	set_tag_name("ribbon-page");
	func_resized().set(this, &RibbonPage::on_resized);
}

//////////////////////////////////////////////////////////////////////////
// Attributes

bool RibbonPage::get_show_tab() const 
{
	return show_tab;
}

//////////////////////////////////////////////////////////////////////////
// Operations

void RibbonPage::add_section(RibbonSection *section)
{
	sections.push_back(section);
	on_resized();
}

void RibbonPage::set_show_tab( bool value )
{
	show_tab = value;
}


void RibbonPage::set_tab_custom_css_state(const std::string &css_class_name)
{
	this->tab_css_custom_state = css_class_name;
}

//////////////////////////////////////////////////////////////////////////
// Implementation

void RibbonPage::on_resized()
{
	Rect page_box = get_size();
	int section_x = 5;
	for (size_t i = 0; i < sections.size(); i++)
	{
		Rect section_box(section_x, 5, section_x + sections[i]->size, page_box.bottom);
		sections[i]->set_geometry(section_box);
		section_x = section_box.right + 5;
	}
}

}

#endif

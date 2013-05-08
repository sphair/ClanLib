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
**    Magnus Norddahl
**    Harry Storbacka
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/Components/ribbon.h"
#include "API/GUI/Components/ribbon_page.h"
#include "API/GUI/Components/ribbon_section.h"
#include "API/GUI/Components/ribbon_menu.h"
#include "API/GUI/Components/push_button.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/canvas.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"
#include "ribbon_impl.h"
#include "ribbon_page_impl.h"
#include "ribbon_menu_impl.h"
#include "../../gui_css_strings.h"

namespace clan
{

Ribbon_Impl::Ribbon_Impl() : menu_button(0), current_page_index(0)
{
}

Ribbon_Impl::~Ribbon_Impl()
{
}

void Ribbon_Impl::add_page(RibbonPage *page)
{
	bool is_page_visible = (current_page_index == pages.size());

	PushButton *tab_button = new PushButton(tab_row);
	tab_button->set_class("tab", true);
	tab_button->set_text(page->impl->text);
	tab_button->func_clicked().set(this, &Ribbon_Impl::on_tab_clicked, page);

	tab_buttons.push_back(tab_button);
	pages.push_back(page);

	tab_button->set_pseudo_class("active", is_page_visible);
	page->set_visible(is_page_visible, false);
}

void Ribbon_Impl::on_menu_button_clicked()
{
	menu->impl->start(component->component_to_screen_coords(Point(0, -2)));
}

void Ribbon_Impl::on_tab_clicked(RibbonPage *page)
{
	for (std::vector<RibbonPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
	{
		if (pages[page_index] == page)
		{
			if (current_page_index != page_index)
			{
				tab_buttons[current_page_index]->set_pseudo_class("active", false);
				pages[current_page_index]->set_visible(false);

				current_page_index = page_index;

				tab_buttons[current_page_index]->set_pseudo_class("active", true);
				pages[current_page_index]->set_visible(true);
			}
			break;
		}
	}
}

}

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
#include "../../gui_css_strings.h"

#include "ribbon_impl.h"
#include "ribbon_page_impl.h"
#include "ribbon_menu_impl.h"

namespace clan
{

Ribbon_Impl::Ribbon_Impl() :menu_button(0), current_page_index(0)
{

}

Ribbon_Impl::~Ribbon_Impl()
{
	for (size_t i = 0; i < pages.size(); i++)
		delete pages[i];
}

void Ribbon_Impl::add_page(RibbonPage *page)
{
	bool is_page_visible = (current_page_index == pages.size());
	pages.push_back(page);
	page->set_visible(is_page_visible, false);
	on_resized();
}

void Ribbon_Impl::on_resized()
{
	int tab_height = part_tab.get_css_height();
	Rect page_box = component->get_size();
	page_box.top = tab_height;
	for (size_t i = 0; i < pages.size(); i++)
		pages[i]->set_geometry(page_box);
	component->request_repaint();
}

void Ribbon_Impl::on_render(Canvas &canvas, const Rect &clip_rect)
{
	part_tab_background.render_box(canvas, Size(component->get_size().width, part_tab_background.get_css_height()));
	paint_tabs(canvas, clip_rect);
}

void Ribbon_Impl::paint_tabs(Canvas &canvas, const Rect &clip_rect)
{
	int tab_x = menu_button->get_width() + 1;
	for (std::vector<RibbonPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
	{
		if (pages[page_index]->impl->show_tab)
		{
			Size size_tab_text = part_tab.get_render_text_size(canvas, pages[page_index]->impl->text);
			int tab_width = max(size_tab_text.width, 40)+12;
			Rect current_tab(Point(tab_x, 0), Size(tab_width+12, part_tab.get_css_height()));

			std::string &custom_state = pages[page_index]->impl->tab_css_custom_state;
			if (!custom_state.empty())
				part_tab.set_pseudo_class(custom_state, true);
			part_tab.set_pseudo_class(CssStr::selected, page_index == current_page_index);
			part_tab.render_box(canvas, current_tab);
			part_tab.render_text(canvas, pages[page_index]->impl->text, current_tab, current_tab.bottom - 7);

			if (!custom_state.empty())
				part_tab.set_pseudo_class(custom_state, false);

			pages[page_index]->impl->position = current_tab;
			tab_x = current_tab.right+2;
		}
	}
}

bool Ribbon_Impl::on_input_pressed(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		for (std::vector<RibbonPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
		{
			if (pages[page_index]->impl->show_tab)
			{
				if (pages[page_index]->impl->position.contains(e.mouse_pos))
				{
					if (current_page_index != page_index)
						pages[current_page_index]->set_visible(false);
					current_page_index = page_index;
					pages[current_page_index]->set_visible(true);
					component->request_repaint();
					break;
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Ribbon_Impl::on_input_released(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Ribbon_Impl::on_input_pointer_moved(const InputEvent &e)
{
	return true;
}

void Ribbon_Impl::on_menu_button_clicked()
{
	menu->impl->start(component->component_to_screen_coords(Point(0, -2)));
}

}


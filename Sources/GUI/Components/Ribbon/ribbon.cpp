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
#include "API/GUI/Components/ribbon.h"
#include "API/GUI/Components/ribbon_page.h"
#include "API/GUI/Components/ribbon_section.h"
#include "API/GUI/Components/ribbon_menu.h"
#include "API/GUI/Components/push_button.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "../../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

#ifdef WIN32
#define GLASS_EFFECT
#endif

#ifdef GLASS_EFFECT
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

namespace clan
{

Ribbon::Ribbon(GUIComponent *container)
: GUIComponent(container, "ribbon"), menu_button(0), current_page_index(0)
{
	func_render().set(this, &Ribbon::on_render);
	func_resized().set(this, &Ribbon::on_resized);
	func_input_pressed().set(this, &Ribbon::on_input_pressed);
	func_input_released().set(this, &Ribbon::on_input_released);
	func_input_pointer_moved().set(this, &Ribbon::on_input_pointer_moved);

	menu_button = new PushButton(this);
	menu_button->set_class("menu");
	menu_button->set_geometry(Rect(0, 0, menu_button->get_preferred_width(), 1 + menu_button->get_preferred_height()));
	menu_button->func_clicked().set(this, &Ribbon::on_menu_button_clicked);

	menu = new RibbonMenu(this);

	part_tab = GUIThemePart(this, "tab");
	part_tab_background = GUIThemePart(this, "tab-background");
//	font_tab = Font(get_canvas(), "Segoe UI", -11);
	font_tab = Font(get_canvas(), "Tahoma", -11);

#ifdef GLASS_EFFECT
	// To do: only blur the right area instead of everything.
	HWND hwnd = get_display_window().get_hwnd();
	DWM_BLURBEHIND blur_behind = { 0 };
	blur_behind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION | DWM_BB_TRANSITIONONMAXIMIZED;
	blur_behind.fEnable = TRUE;
	blur_behind.fTransitionOnMaximized = TRUE;
	blur_behind.hRgnBlur = 0;
	DwmEnableBlurBehindWindow(hwnd, &blur_behind);

	MARGINS margins = { 0 };
	margins.cyTopHeight = part_tab.get_preferred_height();
	DwmExtendFrameIntoClientArea(hwnd, &margins);
#endif
}

Ribbon::~Ribbon()
{
	for (size_t i = 0; i < pages.size(); i++)
		delete pages[i];
}

Size Ribbon::get_preferred_size() const
{
	return Size(10, 10);//FIXME: part_background.get_preferred_size();
}

void Ribbon::add_page(RibbonPage *page)
{
	bool is_page_visible = (current_page_index == pages.size());
	pages.push_back(page);
	page->set_visible(is_page_visible, false);
	on_resized();
}

void Ribbon::on_resized()
{
	int tab_height = part_tab.get_preferred_height();
	Rect page_box = get_size();
	page_box.top = tab_height;
	for (size_t i = 0; i < pages.size(); i++)
		pages[i]->set_geometry(page_box);
	request_repaint();
}

void Ribbon::on_render(Canvas &canvas, const Rect &clip_rect)
{
	Rect glass_area = get_size();
	glass_area.clip(clip_rect);
	set_cliprect(canvas, glass_area);
#ifdef GLASS_EFFECT
	canvas.clear(Colorf::transparent);
#else
	canvas.clear(Colorf("#e1e6f6"));
#endif
	reset_cliprect(canvas);
	Size client_size = get_size();
	part_tab_background.render_box(canvas, client_size, clip_rect);
	paint_tabs(canvas, clip_rect);
}

void Ribbon::paint_tabs(Canvas &canvas, const Rect &clip_rect)
{
	int tab_x = menu_button->get_preferred_width() + 2;
	for (std::vector<RibbonPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
	{
		if (pages[page_index]->show_tab)
		{
			Size size_tab_text = font_tab.get_text_size(canvas, pages[page_index]->text);
			int tab_width = max(size_tab_text.width, 40)+12;
			Rect current_tab(Point(tab_x, 0), Size(tab_width+12, part_tab.get_preferred_height()));

			std::string &custom_state = pages[page_index]->tab_css_custom_state;
			if (!custom_state.empty())
				part_tab.set_pseudo_class(custom_state, true);
			part_tab.set_pseudo_class(CssStr::selected, page_index == current_page_index);
			part_tab.render_box(canvas, current_tab, clip_rect);
			font_tab.draw_text(canvas, current_tab.left+current_tab.get_width()/2-size_tab_text.width/2, current_tab.bottom-7, pages[page_index]->text, Colorf::black);

			if (!custom_state.empty())
				part_tab.set_pseudo_class(custom_state, false);

			pages[page_index]->position = current_tab;
			tab_x = current_tab.right+2;
		}
	}
}

bool Ribbon::on_input_pressed(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		for (std::vector<RibbonPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
		{
			if (pages[page_index]->show_tab)
			{
				if (pages[page_index]->position.contains(e.mouse_pos))
				{
					if (current_page_index != page_index)
						pages[current_page_index]->set_visible(false);
					current_page_index = page_index;
					pages[current_page_index]->set_visible(true);
					request_repaint();
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

bool Ribbon::on_input_released(const InputEvent &e)
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

bool Ribbon::on_input_pointer_moved(const InputEvent &e)
{
	return true;
}

void Ribbon::on_menu_button_clicked()
{
	menu->start(component_to_screen_coords(Point(0, -2)));
}

}

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
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/menubar.h"
#include "API/GUI/Components/popupmenu.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Window/input_event.h"
#include "menu_modal_loop.h"
#include "menubar_impl.h"
#include "popupmenu_impl.h"
#include "../../gui_css_strings.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// MenuBar Construction:

MenuBar::MenuBar( GUIComponent *parent)
: GUIComponent(parent, CssStr::MenuBar::type_name), impl(new MenuBar_Impl)
{
	impl->menubar = this;
	func_process_message().set(impl.get(), &MenuBar_Impl::on_process_message);
	func_render().set(impl.get(), &MenuBar_Impl::on_render);
	
	impl->create_parts();
}

MenuBar::~MenuBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// MenuBar Attributes:

MenuBar *MenuBar::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	MenuBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<MenuBar*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find MenuBar named item: %1", id));

	return object;
}

Size MenuBar::get_css_size() const
{
	return Size(100, 40);//FIXME: impl->component->get_css_size();
}

PopupMenu MenuBar::get_menu(int index) const
{
	return impl->menus[index].menu;
}

/////////////////////////////////////////////////////////////////////////////
// MenuBar Operations:

void MenuBar::clear()
{
	impl->menus.clear();
}

int MenuBar::add_menu(const std::string &name, PopupMenu popup_menu)
{
	popup_menu.set_class_name("menubar_joined");

	TopMenu top_menu;
	top_menu.name = name;
	top_menu.menu = popup_menu;
	impl->menus.push_back(top_menu);
	return (int) (impl->menus.size()-1);
}

void MenuBar::remove_menu(int index)
{
	impl->menus.erase(impl->menus.begin() + index);
}

/////////////////////////////////////////////////////////////////////////////
// MenuBar Implementation:

void MenuBar_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		
		const InputEvent &e = input_msg->input_event;

		if (e.type == InputEvent::pressed && e.id == mouse_left)
		{
			msg->consumed = true;

			std::vector<TopMenu>::size_type index;
			for (index = 0; index < menus.size(); index++)
			{
				Rect item_rect = get_menu_item_rect(index);

				if (item_rect.contains(e.mouse_pos))
				{
					selected_index = index;
					hot_index = -1;
					menubar->request_repaint();

					menu_loop.reset(new MenuModalLoop(menubar->get_gui_manager()));
					menu_loop.get()->start(menubar, menubar);
					return;
				}
			}
		}
		else if (e.type == InputEvent::pointer_moved )
		{
			msg->consumed = true;

			std::vector<TopMenu>::size_type index;
			for (index = 0; index < menus.size(); index++)
			{
				Rect item_rect = get_menu_item_rect(index);

				if (item_rect.contains(e.mouse_pos))
				{
					if (selected_index != index && index != hot_index)
					{
						hot_index = index;
						menubar->request_repaint();
					}
					break;
				}
			}
		}
	}
	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_leave)
		{
			msg->consumed = true;
			hot_index = -1;
			menubar->request_repaint();
		}
	}
}

void MenuBar_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect(Point(0,0), menubar->get_geometry().get_size());

	std::vector<TopMenu>::size_type index;
	for (index = 0; index < menus.size(); index++)
	{
		TopMenu &topmenu = menus[index];

		part_item.set_pseudo_class(CssStr::selected, index == selected_index);
		part_item.set_pseudo_class(CssStr::hot, (index != selected_index) && (index == hot_index));

		Rect item_rect = get_menu_item_rect(index);
		Rect item_content = part_item.get_content_box(item_rect);
		part_item.render_box(canvas, item_rect);
		part_item.render_text(canvas, topmenu.name, item_content);
	}
}

void MenuBar_Impl::create_parts()
{
	part_item = GUIThemePart(menubar, "item");

	item_border_left = part_item.get_property_int("border-left", "0");
	item_padding_left = part_item.get_property_int("padding-left", "0");
	item_padding_right = part_item.get_property_int("border-right", "0");
	item_border_right = part_item.get_property_int("padding-right", "0");
}

void MenuBar_Impl::select_item_at(const Point &mouse_pos)
{
	std::vector<TopMenu>::size_type index;
	for (index = 0; index < menus.size(); index++)
	{
		Rect r = get_menu_item_rect(index);
		r.top = 0;
		r.bottom = menubar->get_geometry().bottom;
		if (r.contains(mouse_pos))
		{
			selected_index = index;
			menubar->request_repaint();
			return;
		}
	}

	// Selection isn't changed if the mouse is on the menubar but not on an item.
}

void MenuBar_Impl::select_next()
{
	selected_index++;
	if (selected_index >= menus.size())
		selected_index = 0;
	menubar->request_repaint();
}

void MenuBar_Impl::select_previous()
{
	selected_index--;
	if (selected_index < 0)
		selected_index = menus.size()-1;
	menubar->request_repaint();
}

void MenuBar_Impl::clear_selection()
{
	selected_index = -1;
	menubar->request_repaint();
}

Point MenuBar_Impl::get_submenu_screen_pos()
{
	Rect rect = get_menu_item_rect(selected_index);
	return menubar->component_to_screen_coords(rect.get_bottom_left());
}

Rect MenuBar_Impl::get_menu_item_rect(int our_index)
{
	Rect menubar_rect = menubar->get_size();
	Rect menubar_content_rect = menubar->get_content_box();

	int x = menubar_content_rect.left;
	Canvas &canvas = menubar->get_canvas();
	std::vector<TopMenu>::size_type index;
	for (index = 0; index < menus.size(); index++)
	{
		TopMenu &topmenu = menus[index];

		Rect item_rect = menubar_content_rect;
		item_rect.left = x;
		int text_width = part_item.get_render_text_size(canvas, topmenu.name).width;
		item_rect.right = x + item_border_left + item_padding_left + text_width + item_padding_right + item_border_right;

		if (index == our_index)
			return item_rect;

		x = item_rect.right;
	}
	return Rect(0,0,0,0);
}

PopupMenu MenuBar_Impl::get_selected_menu()
{
	if (selected_index >= 0  && selected_index < menus.size())
		return menus[selected_index].menu;

	return PopupMenu();
}

int MenuBar_Impl::get_selected_item_index()
{
	return selected_index;
}

}

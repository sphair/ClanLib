/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/menubar.h"
#include "API/GUI/Components/popupmenu.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/input_event.h"
#include "menu_modal_loop.h"
#include "menubar_impl.h"
#include "popupmenu_impl.h"
#include "../../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_MenuBar Construction:

CL_MenuBar::CL_MenuBar( CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_MenuBar_Impl)
{
	set_type_name(CssStr::MenuBar::type_name);
	impl->menubar = this;
	func_process_message().set(impl.get(), &CL_MenuBar_Impl::on_process_message);
	func_render().set(impl.get(), &CL_MenuBar_Impl::on_render);
	func_resized().set(impl.get(), &CL_MenuBar_Impl::on_resized);
	func_style_changed().set(impl.get(), &CL_MenuBar_Impl::on_style_changed);

	impl->create_parts();
}

CL_MenuBar::~CL_MenuBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_MenuBar Attributes:

CL_MenuBar *CL_MenuBar::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_MenuBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_MenuBar*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_MenuBar named item: %1", id));

	return object;
}

CL_Size CL_MenuBar::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

CL_PopupMenu CL_MenuBar::get_menu(int index) const
{
	return impl->menus[index].menu;
}

/////////////////////////////////////////////////////////////////////////////
// CL_MenuBar Operations:

void CL_MenuBar::clear()
{
	impl->menus.clear();
}

int CL_MenuBar::add_menu(const CL_StringRef &name, CL_PopupMenu popup_menu)
{
	popup_menu.set_class_name("menubar_joined");

	CL_TopMenu top_menu;
	top_menu.name = name;
	top_menu.menu = popup_menu;
	impl->menus.push_back(top_menu);
	return (int) (impl->menus.size()-1);
}

void CL_MenuBar::remove_menu(int index)
{
	impl->menus.erase(impl->menus.begin() + index);
}

/////////////////////////////////////////////////////////////////////////////
// CL_MenuBar Implementation:

void CL_MenuBar_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
		{
			msg.set_consumed();
			CL_GraphicContext &gc = menubar->get_gc();

			std::vector<CL_TopMenu>::size_type index;
			for (index = 0; index < menus.size(); index++)
			{
				CL_TopMenu &topmenu = menus[index];

				CL_Rect item_rect = get_menu_item_rect(index);

				if (item_rect.contains(e.mouse_pos))
				{
					selected_index = index;
					hot_index = -1;
					menubar->request_repaint();

					CL_MenuModalLoop *menu_ptr = new CL_MenuModalLoop(menubar->get_gui_manager());
					menu_ptr->start(menubar, menubar);
					return;
				}
			}
		}
		else if (e.type == CL_InputEvent::pointer_moved )
		{
			msg.set_consumed();
			CL_GraphicContext &gc = menubar->get_gc();

			std::vector<CL_TopMenu>::size_type index;
			for (index = 0; index < menus.size(); index++)
			{
				CL_TopMenu &topmenu = menus[index];

				CL_Rect item_rect = get_menu_item_rect(index);

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
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pmsg(msg);
		if (pmsg.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
		{
			msg.set_consumed();
			hot_index = -1;
			menubar->request_repaint();
		}
	}
}

void CL_MenuBar_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect(CL_Point(0,0), menubar->get_geometry().get_size());
	part_component.render_box(gc, rect, update_rect);

	std::vector<CL_TopMenu>::size_type index;
	for (index = 0; index < menus.size(); index++)
	{
		CL_TopMenu &topmenu = menus[index];

		part_item.set_state(CssStr::selected, index == selected_index);
		part_item.set_state(CssStr::hot, (index != selected_index) && (index == hot_index));

		CL_Rect item_rect = get_menu_item_rect(index);
		CL_Rect item_content = part_item.get_content_box(item_rect);
		part_item.render_box(gc, item_rect, update_rect);
		part_item.render_text(gc, topmenu.name, item_content, update_rect);
	}
}

void CL_MenuBar_Impl::on_resized()
{
}

void CL_MenuBar_Impl::on_style_changed()
{
	create_parts();
}	

void CL_MenuBar_Impl::create_parts()
{
	part_component = CL_GUIThemePart(menubar);
	part_item = CL_GUIThemePart(menubar, "item");

	CL_GUIThemePartProperty prop_bl("border-left", "0");
	CL_GUIThemePartProperty prop_pl("padding-left", "0");
	CL_GUIThemePartProperty prop_br("border-right", "0");
	CL_GUIThemePartProperty prop_pr("padding-right", "0");

	item_border_left = part_item.get_property_int(prop_bl);
	item_padding_left = part_item.get_property_int(prop_pl);
	item_padding_right = part_item.get_property_int(prop_br);
	item_border_right = part_item.get_property_int(prop_pr);
}

void CL_MenuBar_Impl::select_item_at(const CL_Point &mouse_pos)
{
	std::vector<CL_TopMenu>::size_type index;
	for (index = 0; index < menus.size(); index++)
	{
		CL_Rect r = get_menu_item_rect(index);
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

void CL_MenuBar_Impl::select_next()
{
	selected_index++;
	if (selected_index >= menus.size())
		selected_index = 0;
	menubar->request_repaint();
}

void CL_MenuBar_Impl::select_previous()
{
	selected_index--;
	if (selected_index < 0)
		selected_index = menus.size()-1;
	menubar->request_repaint();
}

void CL_MenuBar_Impl::clear_selection()
{
	selected_index = -1;
	menubar->request_repaint();
}

CL_Point CL_MenuBar_Impl::get_submenu_screen_pos()
{
	CL_Rect rect = get_menu_item_rect(selected_index);
	return menubar->component_to_screen_coords(rect.get_bottom_left());
}

CL_Rect CL_MenuBar_Impl::get_menu_item_rect(int our_index)
{
	CL_Rect menubar_rect = menubar->get_size();
	CL_Rect menubar_content_rect = part_component.get_content_box(menubar_rect);

	int x = menubar_content_rect.left;
	CL_GraphicContext &gc = menubar->get_gc();
	std::vector<CL_TopMenu>::size_type index;
	for (index = 0; index < menus.size(); index++)
	{
		CL_TopMenu &topmenu = menus[index];

		CL_Rect item_rect = menubar_content_rect;
		item_rect.left = x;
		int text_width = part_item.get_text_size(gc, topmenu.name).width;
		item_rect.right = x + item_border_left + item_padding_left + text_width + item_padding_right + item_border_right;

		if (index == our_index)
			return item_rect;

		x = item_rect.right;
	}
	return CL_Rect(0,0,0,0);
}

CL_PopupMenu CL_MenuBar_Impl::get_selected_menu()
{
	if (selected_index >= 0  && selected_index < menus.size())
		return menus[selected_index].menu;

	return CL_PopupMenu();
}

int CL_MenuBar_Impl::get_selected_item_index()
{
	return selected_index;
}
/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

CL_Size CL_MenuBar::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

CL_PopupMenu CL_MenuBar::get_menu(int index) const
{
	return impl->menus[index].menu;
}

int CL_MenuBar::get_index(const CL_Point &p)
{
	std::vector<CL_TopMenu>::size_type index;
	for (index = 0; index < impl->menus.size(); index++)
	{
		CL_Rect r = get_menu_rect(index);
		r.top = 0;
		r.bottom = get_geometry().bottom;
		if (r.contains(p))
		{
			return index;
		}
	}
	return -1;
}

CL_Rect CL_MenuBar::get_menu_rect(int our_index)
{
	CL_Rect rect(CL_Point(0,0), get_geometry().get_size());
	CL_Rect component_content = impl->part_component.get_content_box(rect);
	
	int x = 4;
	CL_GraphicContext gc = get_gc();
	std::vector<CL_TopMenu>::size_type index;
	for (index = 0; index < impl->menus.size(); index++)
	{
		CL_TopMenu &topmenu = impl->menus[index];

		CL_Rect item_content = impl->part_item.get_content_box(component_content);
		item_content.left += x;
		item_content.right = item_content.left + 8 + impl->font.get_text_size(gc, topmenu.name).width + 8;

		CL_Rect item_render = impl->part_item.get_render_box(item_content);
		if (index == our_index)
			return item_render;

		x = item_render.right;
	}
	return CL_Rect(0,0,0,0);
}

/////////////////////////////////////////////////////////////////////////////
// CL_MenuBar Operations:

void CL_MenuBar::clear()
{
	impl->menus.clear();
}

int CL_MenuBar::add_menu(const CL_StringRef &name, CL_PopupMenu popup_menu)
{
	popup_menu.set_class_name(cl_text("menubar_joined"));

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
			CL_GraphicContext gc = menubar->get_gc();
			CL_Rect rect(CL_Point(0,0), menubar->get_geometry().get_size());
			CL_Rect component_content = part_component.get_content_box(rect);
			int x = 4;

			std::vector<CL_TopMenu>::size_type index;
			for (index = 0; index < menus.size(); index++)
			{
				CL_TopMenu &topmenu = menus[index];

				CL_Rect item_content = part_item.get_content_box(component_content);
				item_content.left += x;
				item_content.right = item_content.left + 8 + font.get_text_size(gc, topmenu.name).width + 8;

				CL_Rect item_render = part_item.get_render_box(item_content);
				if (item_render.contains(e.mouse_pos))
				{
					selected_index = index;
					hot_index = -1;
					menubar->invalidate_rect();

					//TODO: If this required, should be exec handling be handled by the main window manager
					if (menubar->get_gui_manager().get_window_manager()->get_window_manager_type() == CL_GUIWindowManager::cl_wm_type_system)
					{
						CL_MenuModalLoop menu_modal_loop(menubar, menubar, index);
						menu_modal_loop.exec();
					}
					else
					{
						CL_MenuModalLoop *menu_ptr = new CL_MenuModalLoop(menubar, menubar, index);
						menu_ptr->start();
						return;
					}
					break;
				}

				x = item_render.right;
			}
		}
		else if (e.type == CL_InputEvent::pointer_moved )
		{
			msg.set_consumed();
			CL_GraphicContext gc = menubar->get_gc();
			CL_Rect rect(CL_Point(0,0), menubar->get_geometry().get_size());
			CL_Rect component_content = part_component.get_content_box(rect);
			int x = 4;

			std::vector<CL_TopMenu>::size_type index;
			for (index = 0; index < menus.size(); index++)
			{
				CL_TopMenu &topmenu = menus[index];

				CL_Rect item_content = part_item.get_content_box(component_content);
				item_content.left += x;
				item_content.right = item_content.left + 8 + font.get_text_size(gc, topmenu.name).width + 8;

				CL_Rect item_render = part_item.get_render_box(item_content);
				if (item_render.contains(e.mouse_pos))
				{
					if (selected_index != index && index != hot_index)
					{
						hot_index = index;
						menubar->invalidate_rect();
					}
					break;
				}

				x = item_render.right;
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
			menubar->invalidate_rect();
		}
	}
}

void CL_MenuBar_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect(CL_Point(0,0), menubar->get_geometry().get_size());
	part_component.render_box(gc, rect, update_rect);

	CL_Rect component_content = part_component.get_content_box(rect);

	int x = 4;
	std::vector<CL_TopMenu>::size_type index;
	for (index = 0; index < menus.size(); index++)
	{
		CL_TopMenu &topmenu = menus[index];

		topmenu.menu.impl->joiner_width = 8 + font.get_text_size(gc, topmenu.name).width + 8;

		part_item.set_state(CssStr::selected, index == selected_index);
		part_item.set_state(CssStr::hot, (index != selected_index) && (index == hot_index));

		CL_Rect item_content = part_item.get_content_box(component_content);
		item_content.left += x;
		item_content.right = item_content.left + 8 + font.get_text_size(gc, topmenu.name).width + 8;
		CL_Rect item_render = part_item.get_render_box(item_content);
		part_item.render_box(gc, item_render, update_rect);
		part_item.render_text(gc, topmenu.name, item_content, update_rect);
		// font.draw_text(gc, item_content.left+8, item_content.top+18, topmenu.name, text_color);

		x = item_render.right;
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
	part_item = CL_GUIThemePart(menubar, cl_text("item"));

	font = part_component.get_font();
	// text_color = part_component.get_property(CssStr::text_color, cl_text("black"));
}

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
#include "popupmenu_window.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_message_close.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/Components/popupmenu.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/draw.h"
#include "API/Display/Window/keys.h"
#include "popupmenu_impl.h"
#include "../../gui_css_strings.h"

#include "API/Core/Text/string_format.h"

CL_GUITopLevelDescription CL_PopupMenuWindow::create_toplevel_description()
{
	CL_GUITopLevelDescription window_desc;
	window_desc.set_allow_resize(false);
	window_desc.set_topmost(true);
	window_desc.show_caption(false);
	window_desc.set_tool_window(true);
	window_desc.set_visible(false);
	window_desc.set_drop_shadow(true);
	return window_desc;
}


CL_PopupMenuWindow::CL_PopupMenuWindow(const CL_PopupMenu &menu, const CL_Point &position, CL_GUIComponent *owner, CL_GUIManager *manager)
: CL_GUIComponent(CL_Rect(0,0,32,32), manager, create_toplevel_description(), true),
  menu(menu), selected(-1), keyboard_selected(false), mouse_pressed(false),
  child_popup(0), owner_popup(0)
{
	set_type_name(CssStr::PopupMenuWindow::type_name);
	set_class_name(menu.get_class_name());

	prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, cl_text("black"));
	prop_text_color_disabled = CL_GUIThemePartProperty(CssStr::text_color_disabled, cl_text("gray"));
	create_parts();

	CL_Point screen_pos = owner->component_to_screen_coords(position);
//	CL_Point local_pos = screen_to_component_coords(screen_pos);
	CL_Rect rect_pos(screen_pos, calc_desired_size());
	set_window_geometry(rect_pos);
	set_visible(true, false);
}

CL_PopupMenuWindow::~CL_PopupMenuWindow()
{
	if (child_popup != 0)
	{
		delete child_popup;
	}
}

void CL_PopupMenuWindow::create_parts()
{
	part_component = CL_GUIThemePart(this);
	part_item_row = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_row);
	part_item_icon = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_icon);
	part_item_label = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_label);
	part_separator = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_separator);
	part_submenu_arrow = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_submenu_arrow);
	part_item_accel_label = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_accel_label);
	part_menubar_joiner = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_menubar_joiner);

	func_process_message().set(this, &CL_PopupMenuWindow::on_process_message);
	func_render().set(this, &CL_PopupMenuWindow::on_render);

	font = part_component.get_font();
	text_color = part_component.get_property(prop_text_color);
	text_color_disabled = part_component.get_property(prop_text_color_disabled);
}

void CL_PopupMenuWindow::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Close::get_type_name()))
	{
		CL_GUIMessage_Close msg_close = msg;
		exit_with_code(-1);
	}
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input msg_input = msg;
		CL_InputEvent input_event = msg_input.get_event();

		if (input_event.type == CL_InputEvent::pressed)
		{

/*			if (input_event.device.get_type() == CL_InputDevice::keyboard)
			{
				CL_PopupMenuWindow *root_win = get_root_popup();
				CL_PopupMenu root_menu = root_win->menu;
				if (!root_menu.func_keyboard_event().is_null())
					root_menu.func_keyboard_event().invoke(input_event);
			}
*/
			if (input_event.id == CL_KEY_DOWN)
			{
				selected++;
				if (selected < menu.item_count()-1)
				{
					CL_PopupMenuItem pmi = menu.get_item_at(selected);
					if (pmi.is_separator())
						selected++;
				}

				if (selected >= menu.item_count())
					selected = 0;
				keyboard_selected = true;
				invalidate_rect();
			}
			else if (input_event.id == CL_KEY_UP)
			{
				selected--;
				if (selected > 0)
				{
					CL_PopupMenuItem pmi = menu.get_item_at(selected);
					if (pmi.is_separator())
						selected--;
				}
				if (selected < 0)
					selected = menu.item_count()-1;
				keyboard_selected = true;
				invalidate_rect();
			}
			if (input_event.id == CL_KEY_LEFT)
			{
				if (get_owner_popup())
				{
					msg.set_consumed();
					get_owner_popup()->child_popup = 0;
					delete this;
					return;
				}
			}
			if (input_event.id == CL_KEY_RIGHT)
			{
				if (selected == -1)
					return;

				CL_PopupMenuItem pmi = menu.get_item_at(selected);
				if (pmi.has_submenu())
				{
					CL_PopupMenu child_menu = pmi.get_submenu();
					CL_Rect item_rect = get_item_rect(selected);
					CL_GUIManager manager = get_gui_manager();
					child_popup = new CL_PopupMenuWindow(child_menu, item_rect.get_top_right(), this, &manager);
					child_popup->set_owner_popup(this);
					msg.set_consumed();
				}
			}
			else if (input_event.id == CL_KEY_ENTER)
			{
				CL_PopupMenuWindow *root = get_root_popup();
				CL_PopupMenu root_menu = root->menu;
				if (!root_menu.func_item_selected().is_null())
					root_menu.func_item_selected().invoke(menu.get_item_at(selected));
				
				if (selected != -1)
				{
					CL_PopupMenuItem pmi = menu.get_item_at(selected);
					if (pmi.has_submenu())
						return;
					root->set_clicked_item(pmi);
				}

				exit_with_code(selected);
			}
			else if (
				input_event.id == CL_MOUSE_LEFT ||
				input_event.id == CL_MOUSE_RIGHT)
			{
				mouse_pressed = true;
			}
		}
		else if (input_event.type == CL_InputEvent::released)
		{
			if (
				input_event.id == CL_MOUSE_LEFT ||
				input_event.id == CL_MOUSE_RIGHT)
			{
				if (selected != -1 || mouse_pressed)
				{
					CL_PopupMenuWindow *root_popup_win = get_root_popup();
					CL_PopupMenu root_menu = root_popup_win->menu;
					if (!root_menu.func_item_selected().is_null())
						root_menu.func_item_selected().invoke(menu.get_item_at(selected));

					CL_PopupMenuItem pmi = menu.get_item_at(selected);
					if (pmi.is_null() || pmi.is_disabled() || pmi.is_separator() || pmi.has_submenu())
						return;

					root_popup_win->set_clicked_item(pmi);
					exit_with_code(pmi.get_id());
				}
			}
		}
		else if (input_event.type == CL_InputEvent::pointer_moved)
		{
			int line = 0;

			CL_Rect content_area = part_component.get_content_box(get_geometry().get_size());
			if (!content_area.contains(input_event.mouse_pos))
			{
				line = -1;
			}
			else
			{
				int row_height = part_item_row.get_preferred_height();
				int separator_height = part_separator.get_preferred_height();
				int y = part_component.get_content_box(get_geometry().get_size()).top;
				while (line < menu.item_count() && input_event.mouse_pos.y > y)
				{
					if (menu.get_item_at(line).is_separator())
						y += separator_height;
					else
						y += row_height; 

					if (y > input_event.mouse_pos.y)
						break;
					else
						line++;
				}
			}

			if (line >= 0 && line < menu.item_count())
			{
				if (selected != line)
				{
					selected = line;
					keyboard_selected = false;
					invalidate_rect();

					delete child_popup;
					child_popup = 0;

					CL_PopupMenuItem i = menu.get_item_at(selected);
					if (!i.is_null() && !i.get_submenu().is_null())
					{
						CL_PopupMenu child_menu = i.get_submenu();
						if (!child_menu.is_null())
						{
							CL_Rect item_rect = get_item_rect(selected);
							CL_GUIManager manager = get_gui_manager();
							CL_Point subpos = item_rect.get_top_right();
							subpos.x--; // Visually looks better when they share border.
							subpos.y -= content_area.top; // Align first item with our item.
							child_popup = new CL_PopupMenuWindow(child_menu, subpos, this, &manager);
							child_popup->set_owner_popup(this);
						}
					}
					else
					{
						child_popup = 0;
					}
				}
			}
			else
			{
				if (child_popup == 0 && selected != -1)
				{
					selected = -1;
					keyboard_selected = false;
					invalidate_rect();
				}
			}
		}
	}
	if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer msg_pointer = msg;
		if (!keyboard_selected)
		{
			selected = false;
			invalidate_rect();
		}
	}
}

CL_Size CL_PopupMenuWindow::get_preferred_size() const
{
	return part_component.get_preferred_size();
}


void CL_PopupMenuWindow::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = get_geometry().get_size();
	part_component.render_box(gc, rect, update_rect);

	if (menu.impl->joiner_width > 0)
	{
	    CL_Rect joiner_rect(0, 0, menu.impl->joiner_width, part_menubar_joiner.get_preferred_height());
		part_menubar_joiner.render_box(gc, joiner_rect, update_rect);
	}

	CL_Rect client_box = part_component.get_content_box(rect);

	int offset = 0;

	int count = menu.item_count();
	for (int index = 0; index < count; index++)
	{
		CL_PopupMenuItem item = menu.get_item_at(index);
		bool is_selected = (index == selected);
		int row_height = 0;

		if (item.is_separator())
		{
			row_height = part_separator.get_preferred_height();
			CL_Rect separator_render_rect(client_box.left, offset, client_box.right, offset+row_height);
			CL_Rect separator_content_rect = part_separator.get_content_box(separator_render_rect);
			separator_content_rect.right -= 4; // This thing is already a hack (render to content to render content as render, wtf? :))
			separator_content_rect.top += 3; // More hacks..
			separator_content_rect.bottom += 3; // Something is really wrong about this stuff. But it fixes the visual layout.
			part_separator.render_box(gc, separator_content_rect, update_rect);
		}
		else
		{
			part_item_row.set_state(CssStr::selected, is_selected);
			part_item_icon.set_state(CssStr::selected, is_selected);
			part_item_label.set_state(CssStr::selected, is_selected);

			row_height = part_item_row.get_preferred_height();

			// row rect
			CL_Rect row_rect(CL_Point(client_box.left, client_box.top + offset), CL_Size(client_box.right, row_height));
			part_item_row.render_box(gc, row_rect, update_rect);
			CL_Rect row_box = part_item_row.get_content_box(row_rect);

			// icon
			int icon_width = part_item_icon.get_preferred_width();
			CL_Rect icon_rect(CL_Point(row_box.left, row_box.top), CL_Size(icon_width, row_box.get_height()));
			part_item_icon.render_box(gc, icon_rect, update_rect);
			CL_Rect icon_box = part_item_icon.get_content_box(icon_rect);

			// item text
			CL_Size text_size = font.get_text_size(gc, item.get_text());
			CL_Size text_full_size = part_item_label.get_render_box(text_size).get_size();

			CL_Rect label_render_rect(icon_rect.right, row_box.top, icon_rect.right + text_full_size.width, row_box.bottom);
			CL_Rect label_content_rect = part_item_label.get_content_box(label_render_rect);

			part_item_label.render_box(gc, label_render_rect, update_rect);

			font.draw_text(gc, label_content_rect.left,
				label_content_rect.top + label_content_rect.get_height()/2 + text_size.height/2 - 2,
				item.get_text(),
				item.is_disabled() ? text_color_disabled : text_color);

			int center_y = row_box.get_center().y;
			int arrow_width = part_submenu_arrow.get_preferred_width();
			int arrow_height = part_submenu_arrow.get_preferred_height();

			if (item.has_submenu())
			{
				CL_Rect arrow_rect(
					row_box.right - arrow_width,
					center_y - arrow_height/2,
					row_box.right,
					center_y + arrow_height/2);

				CL_Rect arrow_content = part_submenu_arrow.get_content_box(arrow_rect);

				part_submenu_arrow.render_box(gc, arrow_content, update_rect);
			}
			else if (!item.get_accelerator_text().empty())
			{
				// accelerator text
				CL_Size accel_text_size = font.get_text_size(gc, item.get_accelerator_text());
				CL_Size accel_text_full_size = part_item_accel_label.get_render_box(accel_text_size).get_size();

				CL_Rect accel_render_rect(
					row_box.right-arrow_width-accel_text_full_size.width, 
					label_content_rect.top, 
					row_box.right-arrow_width, 
					label_content_rect.bottom);

				CL_Rect accel_content_rect = part_item_accel_label.get_content_box(accel_render_rect);

				font.draw_text( gc, accel_content_rect.left,
					label_content_rect.top + label_content_rect.get_height()/2 + text_size.height/2 - 2,
					item.get_accelerator_text(),
					item.is_disabled() ? text_color_disabled : text_color);
			}
		}

		offset += row_height;
	}
}

CL_Size CL_PopupMenuWindow::calc_desired_size()
{
	CL_GraphicContext gc = get_gc();

	CL_Size size(0,0);
	int count = menu.item_count();
	for (int index = 0; index < count; index++)
	{
		CL_PopupMenuItem item = menu.get_item_at(index);
		bool is_selected = (index == selected);

		part_item_row.set_state(CssStr::selected, is_selected);
		part_item_icon.set_state(CssStr::selected, is_selected);
		part_item_label.set_state(CssStr::selected, is_selected);

		int row_height = 0;
		if (item.is_separator())
			row_height = part_separator.get_preferred_height();
		else
			row_height = part_item_row.get_preferred_height();

		int icon_width = part_item_icon.get_preferred_width();

		CL_Size text_size = font.get_text_size(gc, item.get_text());
		CL_Size text_full_size = part_item_label.get_render_box(text_size).get_size();

		CL_Size accel_text_size = font.get_text_size(gc, item.get_accelerator_text());
		CL_Size accel_text_full_size = part_item_accel_label.get_render_box(accel_text_size).get_size();

		int arrow_width = part_submenu_arrow.get_preferred_width();

		int w = icon_width + text_full_size.width + accel_text_full_size.width + arrow_width;

		if (size.width < w)
			size.width = w;

		size.height += row_height;
	}

	size = part_component.get_render_box(size).get_size();

	if (size.width < menu.get_minimum_width())
	{
		size.width = menu.get_minimum_width();
	}

	return size;
}

CL_Rect CL_PopupMenuWindow::get_item_rect(int index)
{
	int count = menu.item_count();
	int row_height = part_item_row.get_preferred_height();
	int separator_height = part_separator.get_preferred_height();
	int icon_width = part_item_icon.get_preferred_width();

	CL_PopupMenuItem item = menu.get_item_at(index);

	CL_GraphicContext gc = get_gc();

	CL_Rect content_area = part_component.get_content_box(get_geometry().get_size());
	int w = content_area.get_width();
	int y = content_area.top;

	for (int i = 0; i < count; i++)
	{
		if (i == index)
			break;

		if (menu.get_item_at(i).is_separator())
			y += separator_height;
		else
			y += row_height;
	}

	CL_Rect rect(0, y, w, y+row_height);

	return rect;
}

void CL_PopupMenuWindow::set_owner_popup(CL_PopupMenuWindow *popup)
{
	owner_popup = popup;
}

CL_PopupMenuWindow *CL_PopupMenuWindow::get_root_popup()
{
	CL_PopupMenuWindow *root = this;
	while (root->get_owner_popup())
		root = root->get_owner_popup();

	return root;
}

void CL_PopupMenuWindow::set_clicked_item(CL_PopupMenuItem &item)
{
	clicked_item = item;
}

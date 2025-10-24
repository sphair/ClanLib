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
#include "API/Display/Render/texture.h"
#include "API/Display/Window/keys.h"
#include "popupmenu_impl.h"
#include "../../gui_css_strings.h"

CL_GUITopLevelDescription CL_PopupMenuWindow::create_toplevel_description()
{
	CL_GUITopLevelDescription window_desc;
	window_desc.set_allow_resize(false);
	window_desc.set_topmost(true);
	window_desc.show_caption(false);
	window_desc.set_tool_window(true);
	window_desc.set_visible(false);
	window_desc.set_drop_shadow(true);
	window_desc.set_using_gui_window_cache(true);
	window_desc.set_position(CL_Rect(0,0,32,32), false);
	return window_desc;
}

//////////////////////////////////////////////////////////////////////////
// Construction

CL_PopupMenuWindow::CL_PopupMenuWindow(const CL_PopupMenu &menu, const CL_Point &screen_position, CL_GUIComponent *owner)
: CL_GUIComponent(owner, create_toplevel_description()),
  menu(menu), selected(-1)
{
	set_type_name(CssStr::PopupMenuWindow::type_name);
	set_class_name(menu.get_class_name());

	prop_icon_column_width = CL_GUIThemePartProperty("icon-column-width", "30");

	create_parts();

	CL_Rect rect_pos(screen_position, calc_desired_size());
	set_window_geometry(rect_pos);
	set_visible(true, false);

	func_close().set(this, &CL_PopupMenuWindow::on_close);
}

CL_PopupMenuWindow::~CL_PopupMenuWindow()
{
}

//////////////////////////////////////////////////////////////////////////
// Attributes

CL_PopupMenuItem CL_PopupMenuWindow::get_selected_item()
{
	return menu.get_item_at(selected);
}

int CL_PopupMenuWindow::get_selected_item_index()
{
	return selected;
}

CL_Size CL_PopupMenuWindow::get_preferred_size() const
{
	return part_component.get_preferred_size();
}

CL_Point CL_PopupMenuWindow::get_submenu_screen_position()
{
	CL_Rect sel_item_rect = get_item_rect(selected);
	return component_to_screen_coords(sel_item_rect.get_top_right());
}

//////////////////////////////////////////////////////////////////////////
// Operations

void CL_PopupMenuWindow::select_next()
{
	selected++;
	if (selected < menu.get_item_count()-1)
	{
		CL_PopupMenuItem pmi = menu.get_item_at(selected);
		if (pmi.is_separator() || pmi.is_disabled())
			selected++;
	}

	if (selected >= menu.get_item_count())
		selected = 0;

	request_repaint();
}

void CL_PopupMenuWindow::select_previous()
{
	selected--;
	if (selected > 0)
	{
		CL_PopupMenuItem pmi = menu.get_item_at(selected);
		if (pmi.is_separator() || pmi.is_disabled())
			selected--;
	}

	if (selected < 0)
		selected = menu.get_item_count()-1;

	request_repaint();
}

void CL_PopupMenuWindow::do_selected_item_special_action()
{
	CL_PopupMenuItem pmi = menu.get_item_at(selected);
	if (pmi.is_null() || pmi.is_disabled() || pmi.is_separator() || pmi.has_submenu())
		return;

	if (pmi.is_checkable())
	{
		pmi.set_checked(!pmi.is_checked());
	}
}

void CL_PopupMenuWindow::select_item_at(CL_Point mouse_pos)
{
	for (int i = 0; i < menu.get_item_count(); i++)
	{
		if (menu.get_item_at(i).is_separator())
			i++;

		CL_Rect item_rect = get_item_rect(i);
		if (item_rect.contains(mouse_pos))
		{
			selected = i;
			request_repaint();
			return;
		}
	}
}

void CL_PopupMenuWindow::set_item_pressed(bool pressed)
{
	item_pressed = true;
}

//////////////////////////////////////////////////////////////////////////
// Implementation

bool CL_PopupMenuWindow::on_close()
{
	if(!menu.func_close().is_null())
		menu.func_close().invoke();
	return true;
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

	int count = menu.get_item_count();
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

			part_item_row.set_state(CssStr::disabled, item.is_disabled());
			part_item_icon.set_state(CssStr::disabled, item.is_disabled());
			part_item_check.set_state(CssStr::disabled, item.is_disabled());
			part_item_label.set_state(CssStr::disabled, item.is_disabled());
			part_item_accel_label.set_state(CssStr::disabled, item.is_disabled());

			row_height = part_item_row.get_preferred_height();

			// row rect
			CL_Rect row_rect(CL_Point(client_box.left, client_box.top + offset), CL_Size(client_box.right, row_height));
			part_item_row.render_box(gc, row_rect, update_rect);
			CL_Rect row_box = part_item_row.get_content_box(row_rect);

			// icon or check
			if (item.is_checkable())
			{
				if (item.is_checked())
				{
					CL_Rect rect(CL_Point(row_box.left, (row_box.top + row_box.bottom)/2 - check_size.height/2), check_size);
					part_item_check.render_box(gc, rect, update_rect);
				}
			}
			else
			{
				CL_PixelBuffer pbuf_icon = item.get_icon();
				if (!pbuf_icon.is_null())
				{
					CL_Size icon_size = pbuf_icon.get_size();
					CL_Rect rect(CL_Point(row_box.left, (row_box.top + row_box.bottom)/2 - icon_size.height/2), icon_size);
					CL_Texture tex(gc, pbuf_icon.get_size());
					tex.set_min_filter(cl_filter_nearest);
					tex.set_mag_filter(cl_filter_nearest);
					tex.set_image(pbuf_icon);
					CL_Colorf color = CL_Colorf::white;
					if (item.is_disabled())
						color.a = 0.25f;
					gc.set_texture(0, tex);
					CL_Draw::texture(gc, rect, color);
				}
			}

			// item text
			CL_Size text_size = part_item_label.get_text_size(gc, item.get_text());
			CL_Size text_full_size = part_item_label.get_render_box(text_size).get_size();

			CL_Rect label_render_rect(row_box.left + icon_column_width, row_box.top, row_box.left + icon_column_width + text_full_size.width, row_box.bottom);
			CL_Rect label_content_rect = part_item_label.get_content_box(label_render_rect);

			part_item_label.render_box(gc, label_render_rect, update_rect);
			part_item_label.render_text(gc, item.get_text(), label_content_rect, update_rect);

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
				CL_Size accel_text_size = part_item_accel_label.get_text_size(gc, item.get_accelerator_text());
				CL_Size accel_text_full_size = part_item_accel_label.get_render_box(accel_text_size).get_size();

				CL_Rect accel_render_rect(
					row_box.right-arrow_width-accel_text_full_size.width, 
					label_content_rect.top, 
					row_box.right-arrow_width, 
					label_content_rect.bottom);

				CL_Rect accel_content_rect = part_item_accel_label.get_content_box(accel_render_rect);

				part_item_accel_label.render_text( gc, item.get_accelerator_text(), accel_content_rect, update_rect);
			}
		}

		offset += row_height;
	}
}

CL_Size CL_PopupMenuWindow::calc_desired_size()
{
	CL_GraphicContext &gc = get_gc();

	CL_Size size(0,0);
	int count = menu.get_item_count();
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

		CL_Size text_size = part_item_label.get_text_size(gc, item.get_text());
		CL_Size text_full_size = part_item_label.get_render_box(text_size).get_size();

		CL_Size accel_text_size = part_item_accel_label.get_text_size(gc, item.get_accelerator_text());
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
	int count = menu.get_item_count();
	int row_height = part_item_row.get_preferred_height();
	int separator_height = part_separator.get_preferred_height();

	CL_PopupMenuItem item = menu.get_item_at(index);

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

void CL_PopupMenuWindow::create_parts()
{
	part_component = CL_GUIThemePart(this);
	part_item_row = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_row);
	part_item_icon = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_icon);
	part_item_label = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_label);
	part_item_check = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_check);
	part_separator = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_separator);
	part_submenu_arrow = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_submenu_arrow);
	part_item_accel_label = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_item_accel_label);
	part_menubar_joiner = CL_GUIThemePart(this, CssStr::PopupMenuWindow::part_menubar_joiner);

	icon_column_width = part_component.get_property_int(prop_icon_column_width);
	icon_size = part_item_icon.get_preferred_size();
	check_size = part_item_check.get_preferred_size();

	func_render().set(this, &CL_PopupMenuWindow::on_render);
}

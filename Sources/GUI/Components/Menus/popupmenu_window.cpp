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
#include "API/Display/2D/image.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Window/keys.h"
#include "popupmenu_impl.h"
#include "../../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

GUITopLevelDescription PopupMenuWindow::create_toplevel_description()
{
	GUITopLevelDescription window_desc;
	window_desc.set_allow_resize(false);
	window_desc.set_topmost(true);
	window_desc.show_caption(false);
	window_desc.set_tool_window(true);
	window_desc.set_visible(false);
	window_desc.set_drop_shadow(true);
	window_desc.set_using_gui_window_cache(true);
	window_desc.set_position(Rect(0,0,32,32), false);
	return window_desc;
}

//////////////////////////////////////////////////////////////////////////
// Construction

PopupMenuWindow::PopupMenuWindow(const PopupMenu &menu, const Point &screen_position, GUIComponent *owner)
: GUIComponent(owner, create_toplevel_description(), CssStr::PopupMenuWindow::type_name),
  menu(menu), selected(-1)
{
	set_class(menu.get_class());

	create_parts();

	Rect rect_pos(screen_position, calc_desired_size());
	set_window_geometry(rect_pos);
	set_visible(true, false);

	func_close().set(this, &PopupMenuWindow::on_close);
}

PopupMenuWindow::~PopupMenuWindow()
{
}

//////////////////////////////////////////////////////////////////////////
// Attributes

PopupMenuItem PopupMenuWindow::get_selected_item()
{
	return menu.get_item_at(selected);
}

int PopupMenuWindow::get_selected_item_index()
{
	return selected;
}

Size PopupMenuWindow::get_css_size() const
{
	return Size(40, 40); //FIXME: component->get_css_size();
}

Point PopupMenuWindow::get_submenu_screen_position()
{
	Rect sel_item_rect = get_item_rect(selected);
	return component_to_screen_coords(sel_item_rect.get_top_right());
}

//////////////////////////////////////////////////////////////////////////
// Operations

void PopupMenuWindow::select_next()
{
	selected++;
	if (selected < menu.get_item_count()-1)
	{
		PopupMenuItem pmi = menu.get_item_at(selected);
		if (pmi.is_separator() || pmi.is_disabled())
			selected++;
	}

	if (selected >= menu.get_item_count())
		selected = 0;

	request_repaint();
}

void PopupMenuWindow::select_previous()
{
	selected--;
	if (selected > 0)
	{
		PopupMenuItem pmi = menu.get_item_at(selected);
		if (pmi.is_separator() || pmi.is_disabled())
			selected--;
	}

	if (selected < 0)
		selected = menu.get_item_count()-1;

	request_repaint();
}

void PopupMenuWindow::do_selected_item_special_action()
{
	PopupMenuItem pmi = menu.get_item_at(selected);
	if (pmi.is_null() || pmi.is_disabled() || pmi.is_separator() || pmi.has_submenu())
		return;

	if (pmi.is_checkable())
	{
		pmi.set_checked(!pmi.is_checked());
	}
}

void PopupMenuWindow::select_item_at(Point mouse_pos)
{
	for (int i = 0; i < menu.get_item_count(); i++)
	{
		if (menu.get_item_at(i).is_separator())
			i++;

		Rect item_rect = get_item_rect(i);
		if (item_rect.contains(mouse_pos))
		{
			selected = i;
			request_repaint();
			return;
		}
	}
}

void PopupMenuWindow::set_item_pressed(bool pressed)
{
	item_pressed = true;
}

//////////////////////////////////////////////////////////////////////////
// Implementation

bool PopupMenuWindow::on_close()
{
	if(!menu.func_close().is_null())
		menu.func_close().invoke();
	return true;
}

void PopupMenuWindow::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = get_geometry().get_size();

	if (menu.impl->joiner_width > 0)
	{
	    Rect joiner_rect(0, 0, menu.impl->joiner_width, part_menubar_joiner.get_css_height());
		part_menubar_joiner.render_box(canvas, joiner_rect);
	}

	Rect client_box = get_content_box();

	int offset = 0;

	int count = menu.get_item_count();
	for (int index = 0; index < count; index++)
	{
		PopupMenuItem item = menu.get_item_at(index);
		bool is_selected = (index == selected);
		int row_height = 0;

		if (item.is_separator())
		{
			row_height = part_separator.get_css_height();
			Rect separator_render_rect(client_box.left, offset, client_box.right, offset+row_height);
			Rect separator_content_rect = part_separator.get_content_box(separator_render_rect);
			separator_content_rect.right -= 4; // This thing is already a hack (render to content to render content as render, wtf? :))
			separator_content_rect.top += 3; // More hacks..
			separator_content_rect.bottom += 3; // Something is really wrong about this stuff. But it fixes the visual layout.
			part_separator.render_box(canvas, separator_content_rect);
		}
		else
		{
			part_item_row.set_pseudo_class(CssStr::selected, is_selected);
			part_item_icon.set_pseudo_class(CssStr::selected, is_selected);
			part_item_label.set_pseudo_class(CssStr::selected, is_selected);

			part_item_row.set_pseudo_class(CssStr::disabled, item.is_disabled());
			part_item_icon.set_pseudo_class(CssStr::disabled, item.is_disabled());
			part_item_check.set_pseudo_class(CssStr::disabled, item.is_disabled());
			part_item_label.set_pseudo_class(CssStr::disabled, item.is_disabled());
			part_item_accel_label.set_pseudo_class(CssStr::disabled, item.is_disabled());

			row_height = part_item_row.get_css_height();

			// row rect
			Rect row_rect(Point(client_box.left, client_box.top + offset), Size(client_box.right, row_height));
			part_item_row.render_box(canvas, row_rect);
			Rect row_box = part_item_row.get_content_box(row_rect);

			// icon or check
			if (item.is_checkable())
			{
				if (item.is_checked())
				{
					Rect rect(Point(row_box.left, (row_box.top + row_box.bottom)/2 - check_size.height/2), check_size);
					part_item_check.render_box(canvas, rect);
				}
			}
			else
			{
				PixelBuffer pbuf_icon = item.get_icon();
				if (!pbuf_icon.is_null())
				{
					Size icon_size = pbuf_icon.get_size();
					Rect rect(Point(row_box.left, (row_box.top + row_box.bottom)/2 - icon_size.height/2), icon_size);

					Image image(canvas, pbuf_icon, pbuf_icon.get_size());	//TODO: This is slow, and probably should be cached
					image.set_linear_filter(false);
						Colorf color = Colorf::white;
					if (item.is_disabled())
					image.set_alpha(0.25f);
					image.draw(canvas, rect);
				}
			}

			// item text
			Size text_size = part_item_label.get_render_text_box(canvas, item.get_text()).get_size();
			Size text_full_size = part_item_label.get_border_box(text_size).get_size();

			Rect label_render_rect(row_box.left + icon_column_width, row_box.top, row_box.left + icon_column_width + text_full_size.width, row_box.bottom);
			Rect label_content_rect = part_item_label.get_content_box(label_render_rect);

			part_item_label.render_box(canvas, label_render_rect);
			part_item_label.render_text(canvas, item.get_text(), label_content_rect);

			int center_y = row_box.get_center().y;
			int arrow_width = part_submenu_arrow.get_css_width();
			int arrow_height = part_submenu_arrow.get_css_height();

			if (item.has_submenu())
			{
				Rect arrow_rect(
					row_box.right - arrow_width,
					center_y - arrow_height/2,
					row_box.right,
					center_y + arrow_height/2);

				Rect arrow_content = part_submenu_arrow.get_content_box(arrow_rect);

				part_submenu_arrow.render_box(canvas, arrow_content);
			}
			else if (!item.get_accelerator_text().empty())
			{
				// accelerator text
				Size accel_text_size = part_item_accel_label.get_render_text_box(canvas, item.get_accelerator_text()).get_size();
				Size accel_text_full_size = part_item_accel_label.get_border_box(accel_text_size).get_size();

				Rect accel_render_rect(
					row_box.right-arrow_width-accel_text_full_size.width, 
					label_content_rect.top, 
					row_box.right-arrow_width, 
					label_content_rect.bottom);

				Rect accel_content_rect = part_item_accel_label.get_content_box(accel_render_rect);

				part_item_accel_label.render_text( canvas, item.get_accelerator_text(), accel_content_rect);
			}
		}

		offset += row_height;
	}
}

Size PopupMenuWindow::calc_desired_size()
{
	Canvas &canvas = get_canvas();

	Size size(0,0);
	int count = menu.get_item_count();
	for (int index = 0; index < count; index++)
	{
		PopupMenuItem item = menu.get_item_at(index);
		bool is_selected = (index == selected);

		part_item_row.set_pseudo_class(CssStr::selected, is_selected);
		part_item_icon.set_pseudo_class(CssStr::selected, is_selected);
		part_item_label.set_pseudo_class(CssStr::selected, is_selected);

		int row_height = 0;
		if (item.is_separator())
			row_height = part_separator.get_css_height();
		else
			row_height = part_item_row.get_css_height();

		int icon_width = part_item_icon.get_css_width();

		Size text_size = part_item_label.get_render_text_box(canvas, item.get_text()).get_size();
		Size text_full_size = part_item_label.get_border_box(text_size).get_size();

		Size accel_text_size = part_item_accel_label.get_render_text_box(canvas, item.get_accelerator_text()).get_size();
		Size accel_text_full_size = part_item_accel_label.get_border_box(accel_text_size).get_size();

		int arrow_width = part_submenu_arrow.get_css_width();

		int w = icon_width + text_full_size.width + accel_text_full_size.width + arrow_width;

		if (size.width < w)
			size.width = w;

		size.height += row_height;
	}

	size = get_size();

	if (size.width < menu.get_minimum_width())
	{
		size.width = menu.get_minimum_width();
	}

	return size;
}

Rect PopupMenuWindow::get_item_rect(int index)
{
	int count = menu.get_item_count();
	int row_height = part_item_row.get_css_height();
	int separator_height = part_separator.get_css_height();

	PopupMenuItem item = menu.get_item_at(index);

	Rect content_area = get_content_box();
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

	Rect rect(0, y, w, y+row_height);

	return rect;
}

void PopupMenuWindow::create_parts()
{
	part_item_row = GUIThemePart(this, CssStr::PopupMenuWindow::part_item_row);
	part_item_icon = GUIThemePart(this, CssStr::PopupMenuWindow::part_item_icon);
	part_item_label = GUIThemePart(this, CssStr::PopupMenuWindow::part_item_label);
	part_item_check = GUIThemePart(this, CssStr::PopupMenuWindow::part_item_check);
	part_separator = GUIThemePart(this, CssStr::PopupMenuWindow::part_separator);
	part_submenu_arrow = GUIThemePart(this, CssStr::PopupMenuWindow::part_submenu_arrow);
	part_item_accel_label = GUIThemePart(this, CssStr::PopupMenuWindow::part_item_accel_label);
	part_menubar_joiner = GUIThemePart(this, CssStr::PopupMenuWindow::part_menubar_joiner);

	icon_column_width = get_property_int("icon-column-width", "30");
	icon_size = part_item_icon.get_css_size();
	check_size = part_item_check.get_css_size();

	func_render().set(this, &PopupMenuWindow::on_render);
}

}

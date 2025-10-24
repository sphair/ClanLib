/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview.h"
#include "API/GUI/Components/listview_icon_list.h"
#include "API/GUI/Components/listview_header.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/GUI/Components/listview_selected_item.h"
#include "API/GUI/Components/scrollbar.h"
#include "API/GUI/Components/lineedit.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Window/keys.h"
#include "../../gui_css_strings.h"
#include "listview_item_impl.h"
#include "listview_layout_details.h"
#include "listview_shown_item.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayoutDetails Construction:

CL_ListViewLayoutDetails::CL_ListViewLayoutDetails(CL_ListView *listview)
: CL_ListViewLayout(listview), icon_offset_y(0), opener_gap(0), icon_gap(0),
  indent_width(0), max_rows_visible(0), row_draw_y_pos(0), columns_valid(false)
{
	prop_opener_gap = CL_GUIThemePartProperty(CssStr::opener_gap, "6");
	prop_opener_offset_x = CL_GUIThemePartProperty(CssStr::opener_offset_x, "0");
	prop_opener_offset_y = CL_GUIThemePartProperty(CssStr::opener_offset_y, "0");
	prop_icon_text_gap = CL_GUIThemePartProperty(CssStr::icon_text_gap, "6");
	prop_icon_width = CL_GUIThemePartProperty(CssStr::icon_width, "16");
	prop_icon_height = CL_GUIThemePartProperty(CssStr::icon_height, "16");
	prop_icon_offset_y = CL_GUIThemePartProperty(CssStr::icon_offset_y, "0");
	prop_indent_width = CL_GUIThemePartProperty(CssStr::indent_width, "20");
	prop_lineedit_offset_left = CL_GUIThemePartProperty(CssStr::lineedit_offset_left, "0");
	prop_lineedit_offset_top = CL_GUIThemePartProperty(CssStr::lineedit_offset_top, "0");
	prop_lineedit_offset_right = CL_GUIThemePartProperty(CssStr::lineedit_offset_right, "0");
	prop_lineedit_offset_bottom = CL_GUIThemePartProperty(CssStr::lineedit_offset_bottom, "0");
}

CL_ListViewLayoutDetails::~CL_ListViewLayoutDetails()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayoutDetails Attributes:

ListViewShownItem CL_ListViewLayoutDetails::get_item_at(const CL_Point &point)
{
	std::vector<ListViewShownItem>::iterator it;
	for (it = shown_items.begin(); it!=shown_items.end(); ++it)
	{
		if ((*it).rect_cell[0].contains(point))
			return (*it);
	}

	return ListViewShownItem();
}

CL_Size CL_ListViewLayoutDetails::get_total_size()
{
	if (height_row <= 0)
		return CL_Size(0,0);

	int num_rows = root_item.get_child_count(true,true);
	if (num_rows <= 0)
		return CL_Size(0,0);

	return CL_Size(rect_view.get_width(), int(height_row * num_rows) + (rect_view.get_height() % height_row));
}

CL_ListViewItem CL_ListViewLayoutDetails::get_neighbour(CL_ListViewItem item, Neighbour neighbour)
{
	CL_ListViewItem retval = item;

	if (neighbour == neighbour_left)
	{
		retval = item.get_parent();
		if (retval.get_parent().is_null()) // don't go all the way to the document_item.
			retval = item;
	}
	else if (neighbour == neighbour_right)
	{
		retval = item.get_first_child();
	}
	else if (neighbour == neighbour_up)
	{
		retval = item.get_prev_sibling();
		if (retval.is_null())
			return get_neighbour(item, neighbour_left); // return parent
		else
		{
			while (retval.has_children() && retval.is_open())
				retval = retval.get_last_child();
		}
	}
	else if (neighbour == neighbour_down)
	{
		if (item.is_open() && item.has_children())
		{
			retval = item.get_first_child();
		}
		else
		{
			retval = item.get_next_sibling(); // move down

			int test_parent_count = 0;
			int item_parent_count = item.get_parent_count();

			while (retval.is_null()) // move to next sibling of parent with next_sibling, if any.
			{
				retval = item;
				for (int i=0; i<test_parent_count; i++)
					retval = retval.get_parent();

				retval = retval.get_next_sibling();
				test_parent_count++;
				if (test_parent_count == item_parent_count)
					break;
			}
		}
	}

	if (retval.is_null())
		return item;

	return retval;
}

std::vector<ListViewShownItem> &CL_ListViewLayoutDetails::get_shown_items()
{
	if (valid)
		return shown_items;

	shown_items.clear();
	rows.clear();

	if (size_cell.height <= 0)
		return shown_items;

	row_draw_y_pos = rect_view.top - (int(scroll_y/size_cell.height)*size_cell.height);
	row_counter = 0;

	CL_Font font = part_cell.get_font();
	max_rows_visible = rect_view.get_height() / height_row;

	CL_ListViewItem child = root_item.get_first_child();
	while (child.is_item())
	{
		if (shown_items.size() == max_rows_visible)
			break;

		update_shown_items_rows(font, child);
		child = child.get_next_sibling();
	}

	valid = true;
	return shown_items;
}

std::vector<ListViewColumn> &CL_ListViewLayoutDetails::get_columns()
{
	if (columns_valid)
		return columns;

	columns.clear();

	int xpos = rect_view.left;

	CL_ListViewColumnHeader it = header->get_first_column();
	while (!it.is_null())
	{
		CL_Rect rect_column(xpos, rect_view.top, xpos + it.get_width(), rect_view.bottom);

		if (it.get_next_sibling().is_null())
			rect_column.right = rect_view.right;

		ListViewColumn col;
		col.header = it;
		col.rect = rect_column;
		columns.push_back(col);

		xpos += it.get_width();
		it = it.get_next_sibling();
	}

	columns_valid = true;
	return columns;	
}

std::vector<ListViewRow> &CL_ListViewLayoutDetails::get_rows()
{
	std::vector<ListViewShownItem> &foo = get_shown_items(); // updates rows.
	return rows;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayoutDetails Operations:

void CL_ListViewLayoutDetails::create_parts()
{
	part_row = CL_GUIThemePart(listview, CssStr::ListView::part_row);
	part_opener = CL_GUIThemePart(listview, CssStr::ListView::part_opener);
	part_cell = CL_GUIThemePart(listview, CssStr::ListView::part_cell);
	part_cell.set_state("details", true);
	part_opener.set_state("open", true);

	opener_gap = part_cell.get_property_int(prop_opener_gap);
	size_opener = part_opener.get_preferred_size();
	opener_offset.x = part_opener.get_property_int(prop_opener_offset_x);
	opener_offset.y = part_opener.get_property_int(prop_opener_offset_y);

	icon_gap = part_cell.get_property_int(prop_icon_text_gap);
	size_icon.width = part_cell.get_property_int(prop_icon_width);
	size_icon.height = part_cell.get_property_int(prop_icon_height);
	icon_offset_y = part_cell.get_property_int(prop_icon_offset_y);

	CL_Font font = part_cell.get_font();
	CL_FontMetrics metrics = font.get_font_metrics(gc);
	//height_text = gc.get_text_size("l").height; // todo: use font metrics
	height_text = (int)metrics.get_height();
	descent = (int)metrics.get_descent();

	indent_width = part_row.get_property_int(prop_indent_width);

	part_cell.set_state(CssStr::normal, true);
	size_cell = part_cell.get_preferred_size();
	height_row = size_cell.height;

	lineedit_textrect_offset.left = part_cell.get_property_int(prop_lineedit_offset_left);
	lineedit_textrect_offset.top = part_cell.get_property_int(prop_lineedit_offset_top);
	lineedit_textrect_offset.bottom = part_cell.get_property_int(prop_lineedit_offset_bottom);
	lineedit_textrect_offset.right = part_cell.get_property_int(prop_lineedit_offset_right);
}

CL_Rect CL_ListViewLayoutDetails::get_lineedit_rect(CL_ListViewItem &item, const CL_Size &text_size) const
{
	if (shown_items.empty())
		return CL_Rect(0,0,0,0);

	ListViewShownItem si;

	// 1. Find shown items, si.
	std::vector<ListViewShownItem>::const_iterator it;
	for (it=shown_items.begin(); it!=shown_items.end(); ++it)
	{
		if ((*it).item == item)
		{
			si = (*it);
			break;
		}
	}

	if (si.valid == false)
		return CL_Rect(0,0,0,0);

	// 2. Calculate text rect for 'str'.
	CL_Rect text_rect = si.rect_text[0];
	text_rect.right = text_rect.left + text_size.width + 3; // add a few pixels extra to allow cursor to be at end of string without scrolling.

	// 3. Expand text rect with line-edit-offset values defined in css.
	text_rect.top -= lineedit_textrect_offset.top;
	text_rect.left -= lineedit_textrect_offset.left;
	text_rect.bottom += lineedit_textrect_offset.bottom;
	text_rect.right += lineedit_textrect_offset.right;

	return text_rect;
}


/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayoutDetails Events:


/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayoutDetails Implementation:

void CL_ListViewLayoutDetails::update_shown_items_rows(CL_Font &font, CL_ListViewItem item)
{
	if (row_draw_y_pos + height_row <= rect_view.top)
	{
		row_draw_y_pos += height_row;
		row_counter++;

		// Row not drawn, children might be. Recurse into children
		if (item.has_children() && item.is_open())
		{
			CL_ListViewItem it = item.get_first_child();
			while (it.is_item())
			{
				if (shown_items.size() == max_rows_visible)
					break;

				update_shown_items_rows(font, it);
				it = it.get_next_sibling();
			}
		}

		return;
	}

	ListViewShownItem si;
	si.valid = true;
	si.item = item;

	CL_Rect rect_row = CL_Rect(rect_view.left, row_draw_y_pos, rect_view.right, row_draw_y_pos+height_row);
	bool first_column = true;
	int x = rect_row.left;

	CL_ListViewColumnHeader col = header->get_first_column();
	while (!col.is_null())
	{
		const CL_StringRef &col_id = col.get_column_id();
		const CL_StringRef &text = item.get_column(col_id).get_text();

		CL_Rect rect_cell;
		rect_cell.left = x;
		rect_cell.top = rect_row.top;
		rect_cell.right = x + col.get_width();
		rect_cell.bottom = rect_row.bottom;
		if (col.get_next_sibling().is_null())
			rect_cell.right = rect_view.right;
		si.rect_cell.push_back(rect_cell);

		CL_Rect rect_cell_content = part_cell.get_content_box(rect_cell);
		int offset_x = 0; // css

		CL_Size text_size = font.get_text_size(gc, text);
		si.descent = descent;

		if (first_column)
		{
			si.rect_opener = get_opener_rect(rect_cell_content, item, offset_x);
			si.rect_icon = get_icon_rect(rect_cell_content, item, si.rect_opener.right + opener_gap);

			CL_Rect rect_txt(CL_Point(si.rect_icon.right + icon_gap, rect_cell_content.top), text_size);

			si.rect_text.push_back(rect_txt);
			first_column = false;
		}
		else
		{
			//			si.rect_text.push_back(CL_Rect(CL_Point(rect_cell_content.left, rect_cell_content.bottom - text_size.height), text_size));
			si.rect_text.push_back(CL_Rect(CL_Point(rect_cell_content.left, rect_cell_content.top), text_size));
		}

		x += col.get_width();
		col = col.get_next_sibling();
	}

	// todo: checker rect

	row_draw_y_pos += height_row;
	
	shown_items.push_back(si);

	ListViewRow row;
	row.index = row_counter;
	row.rect = rect_row;
	rows.push_back(row);

	row_counter++;

	if (item.has_children() && item.is_open())
	{
		CL_ListViewItem it = item.get_first_child();
		while (it.is_item())
		{
			if (shown_items.size() == max_rows_visible)
				break;

			update_shown_items_rows(font, it);
			it = it.get_next_sibling();
		}
	}
}

CL_Rect CL_ListViewLayoutDetails::get_opener_rect(const CL_Rect &cell_content_rect, CL_ListViewItem item, int offset_x)
{
	int indent_level = item.get_parent_count() - 1;
	int opener_x = cell_content_rect.left + offset_x + indent_level*indent_width + opener_offset.x;
	int opener_y = cell_content_rect.get_center().y - size_opener.height/2 + opener_offset.y;
	CL_Rect rect_open( CL_Point(opener_x, opener_y), size_opener);
	return rect_open;
}

CL_Rect CL_ListViewLayoutDetails::get_icon_rect(const CL_Rect &cell_content_rect, CL_ListViewItem item, int offset_x)
{
	int icon_x = offset_x;
	int icon_y = cell_content_rect.get_center().y - size_icon.height/2 + icon_offset_y;
	CL_Rect rect_icon(CL_Point(icon_x, icon_y), size_icon);
	return rect_icon;
}

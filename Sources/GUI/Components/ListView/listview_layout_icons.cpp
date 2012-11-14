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
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview.h"
#include "API/GUI/Components/listview_header.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "../../gui_css_strings.h"
#include "listview_layout.h"
#include "listview_layout_icons.h"

#ifdef INCLUDE_COMPONENTS

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListViewLayoutIcons Construction:

ListViewLayoutIcons::ListViewLayoutIcons(ListView *listview)
: ListViewLayout(listview), opener_gap(0), icon_gap(0), indent_width(0),
  max_rows_visible(0), max_grid_cols_visible(0), row_draw_y_pos(0),
  columns_valid(false)
{
	prop_icon_width = GUIThemePartProperty(CssStr::icon_width, "16");
	prop_icon_height = GUIThemePartProperty(CssStr::icon_height, "16");
}

ListViewLayoutIcons::~ListViewLayoutIcons()
{
}

/////////////////////////////////////////////////////////////////////////////
// ListViewLayoutIcons Attributes:

ListViewShownItem ListViewLayoutIcons::get_item_at(const Point &point)
{
	std::vector<ListViewShownItem>::iterator it;
	for (it = shown_items.begin(); it!=shown_items.end(); ++it)
	{
		const Rect &r = (*it).rect_cell[0];
		if (r.contains(point))
			return (*it);
	}

	return ListViewShownItem();
}

Size ListViewLayoutIcons::get_total_size()
{
	int items = root_item.get_child_count();
	int rows = items/max_grid_cols_visible + ((items%max_grid_cols_visible) > 0);

	int total_height = rows * size_cell.height;

	return Size(rect_view.get_width(), total_height);
}

ListViewItem ListViewLayoutIcons::get_neighbour(ListViewItem item, Neighbour neighbour)
{
	ListViewItem retval = item;

	if (neighbour == neighbour_left)
	{
		retval = item.get_prev_sibling();
	}
	else if (neighbour == neighbour_right)
	{
		retval = item.get_next_sibling();
	}
	else if (neighbour == neighbour_up)
	{
		int i = 0;
		while (i < (max_grid_cols_visible-1) && !retval.is_null())
		{
			retval = retval.get_prev_sibling();
			i++;
		}
	}
	else if (neighbour == neighbour_down)
	{
		int i = 0;
		retval = item;
		while (i < (max_grid_cols_visible-1) && !retval.is_null())
		{
			retval = retval.get_next_sibling();
			i++;
		}
	}

	if (retval.is_null())
		return item;

	return retval;
}

std::vector<ListViewShownItem> &ListViewLayoutIcons::get_shown_items()
{
	if (valid)
		return shown_items;

	shown_items.clear();
	rows.clear();
	row_draw_y_pos = rect_view.top - scroll_y;
	row_counter = 0;

	Font font = part_cell.get_font();
	update_shown_items_grid(font, root_item);

	valid = true;
	return shown_items;
}

std::vector<ListViewColumn> &ListViewLayoutIcons::get_columns()
{
	if (columns_valid)
		return columns;

	columns.clear();

	ListViewColumn col;
	col.header = header->get_first_column();
	col.rect = rect_view;
	columns.push_back(col);

	columns_valid = true;
	return columns;	
}

std::vector<ListViewRow> &ListViewLayoutIcons::get_rows()
{
	std::vector<ListViewShownItem> &foo = get_shown_items(); // updates rows.
	return rows;
}

void ListViewLayoutIcons::create_parts()
{
	part_row = GUIThemePart(listview, "row");
	part_opener = GUIThemePart(listview, "row_opener");
	part_cell = GUIThemePart(listview, "cell");

	part_cell.set_state("icons", true);
	part_cell.set_pseudo_class(CssStr::normal, true);
	part_row.set_pseudo_class(CssStr::normal, true);

	size_icon.width = part_cell.get_property_int(prop_icon_width);
	size_icon.height = part_cell.get_property_int(prop_icon_height);

	Font font = part_cell.get_font();
	height_text = font.get_text_size(canvas, "l").height;

	size_cell = part_cell.get_preferred_size();
	height_row = size_cell.height;
	max_rows_visible = rect_view.get_height() / size_cell.width;
	max_grid_cols_visible = rect_view.get_width() / size_cell.height;

	lineedit_textrect_offset.left = part_cell.get_property_int(CssStr::lineedit_offset_left);
	lineedit_textrect_offset.top = part_cell.get_property_int(CssStr::lineedit_offset_top);
	lineedit_textrect_offset.bottom = part_cell.get_property_int(CssStr::lineedit_offset_bottom);
	lineedit_textrect_offset.right = part_cell.get_property_int(CssStr::lineedit_offset_right);
}

Rect ListViewLayoutIcons::get_lineedit_rect(ListViewItem &item, const Size &text_size) const
{
	if (shown_items.empty())
		return Rect(0,0,0,0);
	
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
		return Rect(0,0,0,0);

	// 2. Calculate text rect for 'str'.
	Rect text_rect = si.rect_text[0];
	text_rect.right = text_rect.left + text_size.width + 3; // add a few pixels extra to allow cursor to be at end of string without scrolling.

	// 3. Expand text rect with line-edit-offset values defined in css.
	text_rect.top -= lineedit_textrect_offset.top;
	text_rect.left -= lineedit_textrect_offset.left;
	text_rect.bottom += lineedit_textrect_offset.bottom;
	text_rect.right += lineedit_textrect_offset.right;

	return text_rect;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewLayoutIcons Implementation:

void ListViewLayoutIcons::update_shown_items_grid(Font &font, ListViewItem doc_item)
{
	Canvas &canvas = listview->get_canvas(); 
	shown_items.clear();
	row_counter = 0;

	ListViewItem item = doc_item.get_first_child();

	int x = rect_view.left;
	int y = rect_view.top - scroll_y;

	int skip_items = (scroll_y/size_cell.height) * max_grid_cols_visible;
	int skip_rows = skip_items/max_grid_cols_visible;
	y += skip_rows * size_cell.height;
	int skip_item_counter = 0;

	while (item.is_item())
	{
		if (skip_item_counter < skip_items)
		{
			item = item.get_next_sibling();
			skip_item_counter++;
		}

		ListViewShownItem si;
		si.valid = true;
		si.item = item;
		Rect rect_cell(x, y, x+size_cell.width, y+size_cell.height);
		si.rect_cell.push_back(rect_cell);
		rect_cell.shrink(rect_cell_shrink.left, rect_cell_shrink.top, rect_cell_shrink.right, rect_cell_shrink.bottom); // part_cell.get_content_box(si.rect_cell);
		Rect rect_cell_content = rect_cell;
		si.rect_icon = get_icon_rect(rect_cell_content, item, rect_cell_content.left);

		const std::string &text = si.item.get_column(header->get_first_column().get_column_id()).get_text();
		Size text_size = font.get_text_size(canvas, text);
		Rect text_rect;
		text_rect.left = rect_cell_content.get_center().x - text_size.width/2;
		text_rect.top = rect_cell_content.bottom - text_size.height;
		text_rect.set_size(text_size);
		si.descent = font.get_font_metrics().get_descent();
		si.rect_text.push_back(text_rect);

		shown_items.push_back(si);

		x += size_cell.width;

		if (x + size_cell.width > rect_view.right)
		{
			row_counter++;
			if (row_counter > (max_rows_visible+1))
				break;
			y += size_cell.height;
			x = rect_view.left;
		}

		item = item.get_next_sibling();
	}

	valid = true;
}

Rect ListViewLayoutIcons::get_icon_rect(const Rect &cell_content_rect, ListViewItem item, int offset_x)
{
	int icon_x = cell_content_rect.get_center().x - size_icon.width/2;
	int icon_y = cell_content_rect.get_center().y - size_icon.height/2;
	Rect rect_icon(Point(icon_x, icon_y), size_icon);
	return rect_icon;
}

}

#endif

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
#include "API/GUI/Components/listview_item.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/GUI/Components/listview_column_header.h"
#include "API/GUI/Components/listview_header.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/Window/keys.h"
#include "../../gui_css_strings.h"
#include "listview_renderer.h"
#include "listview_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewRenderer Construction:

CL_ListViewRenderer::CL_ListViewRenderer(CL_ListView *listview)
: listview(listview), display_mode(listview_mode_details),
  scroll_x(0), scroll_y(0), height_row(0)
{
	prop_icon_color = CL_GUIThemePartProperty(CssStr::icon_color, "white");
	prop_icon_color_selected = CL_GUIThemePartProperty(CssStr::icon_color_selected, "white");
	prop_icon_color_overlay = CL_GUIThemePartProperty(CssStr::icon_color_overlay, "white");
	prop_icon_color_selected_overlay = CL_GUIThemePartProperty(CssStr::icon_color_selected_overlay, "white");

	prop_selection_margin_left = CL_GUIThemePartProperty(CssStr::ListView::part_selection_margin_left, "3");
	prop_selection_margin_right = CL_GUIThemePartProperty(CssStr::ListView::part_selection_margin_right, "3");
	prop_selection_margin_top = CL_GUIThemePartProperty(CssStr::ListView::part_selection_margin_top, "3");
	prop_selection_margin_bottom = CL_GUIThemePartProperty(CssStr::ListView::part_selection_margin_bottom, "3");
}

CL_ListViewRenderer::~CL_ListViewRenderer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewRenderer Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_ListViewRenderer Operations:

void CL_ListViewRenderer::render(
	CL_GraphicContext &gc,
	std::vector<ListViewColumn> &columns,
	std::vector<ListViewRow> &rows,
	std::vector<ListViewShownItem> &items,
	const CL_Rect &update_rect)
{
	// 1. Draw columns
	std::vector<ListViewColumn>::iterator col;
	for (col = columns.begin(); col != columns.end(); ++col)
	{
		part_column.render_box(gc, (*col).rect, update_rect);
	}

	// 2. Draw rows.
	std::vector<ListViewRow>::iterator row;
	for (row = rows.begin(); row != rows.end(); ++row)
	{
		if ((*row).index % 2 == 0)
			part_row.render_box(gc, (*row).rect, update_rect);
		else
			part_row_alternate.render_box(gc, (*row).rect, update_rect);
	}

	// 3. Draw items
	unsigned int it_size = items.size();
	std::vector<ListViewShownItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		ListViewShownItem &si = (*it);

		int x = rect_view.left;
		bool first_column = true;
		CL_ListViewColumnHeader col = header->get_first_column();
		int i=0;

		part_cell.set_state(CssStr::hot, si.mouse_over);
		part_cell.set_state(CssStr::normal, !si.mouse_over);
		part_selection.set_state(CssStr::hot, si.mouse_over);
		part_selection.set_state(CssStr::normal, !si.mouse_over);

		while (!col.is_null())
		{
			const CL_StringRef &col_id = col.get_column_id();
			const CL_StringRef &text = si.item.get_column(col_id).get_text();
	
			if (i < si.rect_cell.size())
				part_cell.render_box(gc, si.rect_cell[i], update_rect);

			if (first_column)
			{
				if (si.item.has_children())
				{
					if (si.item.is_open())
						part_opener_open.render_box(gc, si.rect_opener, update_rect);
					else
						part_opener_closed.render_box(gc, si.rect_opener, update_rect);
				}

				if (si.item.get_icon())
				{
					int index = si.item.get_icon();
					bool selected = si.item.is_selected();
					CL_Colorf color = selected ? icon_list.get_selected_color() : icon_list.get_color();
					if (color == CL_Colorf::transparent)
                        color = si.item.is_selected() ? color_icon : color_icon_selected;
					CL_ListViewIcon icon = icon_list.get_icon(index);
					icon.draw(gc, si.rect_icon, display_mode, color);

					std::vector<int> overlay_icons = si.item.get_overlay_icons();
					std::vector<int>::iterator it;
					for (it = overlay_icons.begin(); it != overlay_icons.end(); ++it)
					{
						color = selected ? icon_list.get_selected_color_overlay() : icon_list.get_color_overlay();
						if (color == CL_Colorf::transparent)
							color = si.item.is_selected() ? color_icon_overlay : color_icon_selected_overlay;
						CL_ListViewIcon icon = icon_list.get_icon(*it);
						icon.draw(gc, si.rect_icon, display_mode, color);
					}
				}
				else if (!si.item.func_render_icon().is_null())
				{
					si.item.func_render_icon().invoke(si.rect_icon);
				}

				if (si.item.is_selected())
				{
					part_selection.set_state(CssStr::unfocused, !listview->has_focus());

					CL_Rect selection_rect = si.rect_text[i];

					selection_rect.top -= selection_margin.top;
					selection_rect.left -= selection_margin.left;
					selection_rect.right += selection_margin.right;
					selection_rect.bottom += selection_margin.bottom;

					part_selection.render_box(gc, selection_rect, update_rect);

					if (display_mode == listview_mode_thumbnails)
					{
						part_icon_selection.render_box(gc, si.rect_icon_selection, update_rect);
					}
				}
			}

			if ((si.item == listview->impl->edited_item && !first_column) || si.item != listview->impl->edited_item)
			{
				if (i < si.rect_text.size())
				{
					if (si.item.is_selected() && i == 0)
						part_selection.render_text(gc, text, si.rect_text[i], update_rect);
					else
						part_cell.render_text(gc, text, si.rect_text[i], update_rect);

					if (i == 0)
					{
						part_cell.set_state(CssStr::hot, false); // only the first column gets a hot state. Might want to change this at some point?
						part_cell.set_state(CssStr::normal, true);
					}
				}
			}

			x += col.get_width();
			col = col.get_next_sibling();
			i++;

			first_column = false;
		}
	}
}

void CL_ListViewRenderer::set_view_rect(const CL_Rect &rect)
{
	rect_view = rect;
}

void CL_ListViewRenderer::set_scroll_offset(const CL_Point &point)
{
	scroll_x = point.x;
	scroll_y = point.y;
}

void CL_ListViewRenderer::create_parts()
{
	CL_GUIThemePart part_component(listview); // just for font.

	part_row = CL_GUIThemePart(listview, "row");
	part_row_alternate = CL_GUIThemePart(listview, "row");
	part_opener_open = CL_GUIThemePart(listview, "row_opener");
	part_opener_closed = CL_GUIThemePart(listview, "row_opener");
	part_selection = CL_GUIThemePart(listview, "selection");
	part_columns_bg = CL_GUIThemePart(listview, "columns_bg");
	part_column = CL_GUIThemePart(listview, "column");
	part_cell = CL_GUIThemePart(listview, "cell");

	part_icon_selection = CL_GUIThemePart(listview, "icon_selection");

	selection_margin.left = part_selection.get_property_int(prop_selection_margin_left);
	selection_margin.right = part_selection.get_property_int(prop_selection_margin_right);
	selection_margin.top = part_selection.get_property_int(prop_selection_margin_top);
	selection_margin.bottom = part_selection.get_property_int(prop_selection_margin_bottom);

	part_opener_closed.set_state(CssStr::closed, true);
	part_opener_open.set_state(CssStr::open, true);
	part_row_alternate.set_state(CssStr::alternate, true);

	part_cell.set_state(CssStr::normal, true);
	part_row.set_state(CssStr::normal, true);
	part_row_alternate.set_state(CssStr::normal, true);
	part_column.set_state(CssStr::normal, true);
	part_icon_selection.set_state(CssStr::normal, true);

	color_icon = part_cell.get_property(prop_icon_color);
	color_icon_selected = part_cell.get_property(prop_icon_color_selected);
	color_icon_overlay = part_cell.get_property(prop_icon_color_overlay);
	color_icon_selected_overlay = part_cell.get_property(prop_icon_color_selected_overlay);
}

void CL_ListViewRenderer::set_listview_header(CL_ListViewHeader *header)
{
	this->header = header;
}

void CL_ListViewRenderer::set_icon_list(CL_ListViewIconList ilist)
{
	icon_list = ilist;
}

void CL_ListViewRenderer::set_display_mode(CL_ListViewDisplayMode mode)
{
	display_mode = mode;
	part_cell.set_state(CssStr::details, mode == listview_mode_details);
	part_cell.set_state(CssStr::icons, mode == listview_mode_icons);

	height_row = part_cell.get_preferred_height();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewRenderer Implementation:


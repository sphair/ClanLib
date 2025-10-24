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

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "listview_shown_item.h"
#include "listview_layout.h"
#include "API/GUI/Components/listview.h"
#include "API/GUI/gui_theme_part_property.h"

class CL_ListViewHeader;

class CL_ListViewRenderer
{
/// \name Enumerations
/// \{

public:


/// \}
/// \name Construction
/// \{

public:
	CL_ListViewRenderer(CL_ListView *listview);

	virtual ~CL_ListViewRenderer();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	/// \brief Render the column backgrounds and visible items.
	void render(
		CL_GraphicContext &gc,
		std::vector<ListViewColumn> &columns,
		std::vector<ListViewRow> &rows,
		std::vector<ListViewShownItem> &items,
		const CL_Rect &update_rect);

	/// \brief Set the size of the view.
	void set_view_rect(const CL_Rect &rect);

	/// \brief Set the scrollbar offset, in pixels.
	void set_scroll_offset(const CL_Point &point);

	/// \brief Set icon list.
	void set_icon_list(CL_ListViewIconList icon_list);

	/// \brief Load the theme parts needed for rendering.
	void create_parts();

	/// \brief Set the display mode. (needed for selecting the right icons to draw)
	void set_display_mode(CL_ListViewDisplayMode mode);

	/// \brief Set the listview header needed for column sizes.
	void set_listview_header(CL_ListViewHeader *header);

/// \}
/// \name Implementation
/// \{

private:
	CL_ListView *listview;
	CL_GUIThemePart part_cell;
	CL_GUIThemePart part_row;
	CL_GUIThemePart part_row_alternate;
	CL_GUIThemePart part_column;
	CL_GUIThemePart part_columns_bg;
	CL_GUIThemePart part_selection;
	CL_GUIThemePart part_opener_open;
	CL_GUIThemePart part_opener_closed;
	CL_GUIThemePart part_icon_selection;
	CL_GUIThemePartProperty prop_icon_color;
	CL_GUIThemePartProperty prop_icon_color_selected;
	CL_GUIThemePartProperty prop_icon_color_overlay;
	CL_GUIThemePartProperty prop_icon_color_selected_overlay;
	CL_GUIThemePartProperty prop_selection_margin_left;
	CL_GUIThemePartProperty prop_selection_margin_right;
	CL_GUIThemePartProperty prop_selection_margin_top;
	CL_GUIThemePartProperty prop_selection_margin_bottom;
	CL_ListViewDisplayMode display_mode;
	CL_ListViewHeader *header;
	CL_Rect rect_view;
	CL_Rect selection_margin;
	CL_Colorf color_icon;
	CL_Colorf color_icon_selected;
	CL_Colorf color_icon_overlay;
	CL_Colorf color_icon_selected_overlay;

	CL_ListViewIconList icon_list;
	int scroll_x;
	int scroll_y;
	int height_row;

/// \}
};


/// \}

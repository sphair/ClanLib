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

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once

#include "listview_shown_item.h"
#include "listview_layout.h"
#include "API/GUI/Components/listview.h"

namespace clan
{

class ListViewHeader;

class ListViewRenderer
{
/// \name Enumerations
/// \{

public:


/// \}
/// \name Construction
/// \{

public:
	ListViewRenderer(ListView *listview);

	virtual ~ListViewRenderer();


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
		Canvas &canvas,
		std::vector<ListViewColumn> &columns,
		std::vector<ListViewRow> &rows,
		std::vector<ListViewShownItem> &items,
		const Rect &update_rect);

	/// \brief Set the size of the view.
	void set_view_rect(const Rect &rect);

	/// \brief Set the scrollbar offset, in pixels.
	void set_scroll_offset(const Point &point);

	/// \brief Set icon list.
	void set_icon_list(ListViewIconList icon_list);

	/// \brief Load the theme parts needed for rendering.
	void create_parts();

	/// \brief Set the display mode. (needed for selecting the right icons to draw)
	void set_display_mode(ListViewDisplayMode mode);

	/// \brief Set the listview header needed for column sizes.
	void set_listview_header(ListViewHeader *header);

/// \}
/// \name Implementation
/// \{

private:
	ListView *listview;
	GUIThemePart part_cell;
	GUIThemePart part_row;
	GUIThemePart part_row_alternate;
	GUIThemePart part_column;
	GUIThemePart part_columns_bg;
	GUIThemePart part_selection;
	GUIThemePart part_opener_open;
	GUIThemePart part_opener_closed;
	GUIThemePart part_icon_selection;
	ListViewDisplayMode display_mode;
	ListViewHeader *header;
	Rect rect_view;
	Rect selection_margin;
	Colorf color_icon;
	Colorf color_icon_selected;
	Colorf color_icon_overlay;
	Colorf color_icon_selected_overlay;

	ListViewIconList icon_list;
	int scroll_x;
	int scroll_y;
	int height_row;

/// \}
};

}

/// \}

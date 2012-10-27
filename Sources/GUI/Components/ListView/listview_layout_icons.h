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

#include "API/GUI/gui_component.h"

namespace clan
{

class TabHeader_Impl;

class ListViewLayoutIcons : public ListViewLayout
{
/// \name Construction
/// \{

public:
	ListViewLayoutIcons(ListView *listview);
	~ListViewLayoutIcons();

/// \}
/// \name Attributes
/// \{

public:
	virtual ListViewShownItem get_item_at(const Point &point);
	virtual Size get_total_size();
	virtual ListViewItem get_neighbour(ListViewItem item, Neighbour neighbour);
	virtual std::vector<ListViewShownItem> &get_shown_items();
	virtual std::vector<ListViewColumn> &get_columns();
	virtual std::vector<ListViewRow> &get_rows();
	virtual Rect get_lineedit_rect(ListViewItem &si, const Size &size) const;

/// \}
/// \name Operations
/// \{

public:
	virtual void create_parts();


/// \}
/// \name Callbacks
/// \{

public:


/// \}
/// \name Implementation
/// \{

private:
	void update_shown_items_grid(Font &font, ListViewItem item);
	Rect get_icon_rect(const Rect &cell_content_rect, ListViewItem item, int offset_x);

	std::vector<ListViewShownItem> shown_items;
	Size size_opener;
	Rect rect_cell_shrink;
	int opener_gap;
	int icon_gap;
	int indent_width;
	int max_rows_visible;
	int max_grid_cols_visible;
	int row_draw_y_pos;
	bool columns_valid;
	//GUIThemePartProperty prop_icon_width;
	//GUIThemePartProperty prop_icon_height;
/// \}
};

}

/// \}

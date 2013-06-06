/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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



#pragma once

#include "listview_shown_item.h"
#include "API/GUI/Components/listview_column_header.h"
#include "API/Core/Math/point.h"
#include "listview_shown_item.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class ListViewHeader;
class ListView;

struct ListViewColumn
{
	ListViewColumnHeader header;
	Rect rect;
	int index;
};

struct ListViewRow
{
	Rect rect;
	int index;
};

class ListViewLayout
{
/// \name Enumerations
/// \{

public:
	enum Neighbour
	{
		neighbour_left,
		neighbour_right,
		neighbour_up,
		neighbour_down
	};


/// \}
/// \name Construction
/// \{

public:
	ListViewLayout(ListView *listview);

	virtual ~ListViewLayout();


/// \}
/// \name Attributes
/// \{

public:
	virtual ListViewShownItem get_item_at(const Point &point) = 0;

	virtual Size get_total_size() = 0;

	virtual ListViewItem get_neighbour(ListViewItem item, Neighbour neighbour) = 0;

	virtual std::vector<ListViewShownItem> &get_shown_items() = 0;

	virtual std::vector<ListViewColumn> &get_columns() = 0;

	virtual std::vector<ListViewRow> &get_rows() = 0;

	/// \brief Recalculate the text and selection rects of the specified item. If the item isn't shown, nothing is done.
	virtual Rect get_lineedit_rect(ListViewItem &item, const Size &size) const = 0;

	int get_row_height() { return height_row; }

	bool is_visible(ListViewItem &item);


/// \}
/// \name Operations
/// \{

public:
	/// \brief Set the theme parts needed for layout.
	virtual void create_parts() = 0;

	void set_root_item(ListViewItem &item);

	/// \brief Set the font.
	void set_font(const Font &font);

	/// \brief Set the size of the view.
	void set_view_rect(const Rect &rect);

	/// \brief Set the scrollbar offset, in pixels.
	void set_scroll_offset(const Point &point);

	/// \brief Set the listview header needed for column sizes.
	void set_listview_header(ListViewHeader *hearder);

	/// \brief Force an update of the shown items vector at the next get_shown_items() call.
	void invalidate();

	virtual void set_show_detail_icon(bool enable) { }
	virtual void set_show_detail_opener(bool enable) { }

/// \}
/// \name Implementation
/// \{

public:
	ListView *listview;
	Canvas canvas;
	GUIThemePart part_cell;
	GUIThemePart part_row;
	GUIThemePart part_opener;
	ListViewHeader *header;
	ListViewItem root_item;
	Rect rect_view;
	Size size_icon;
	int scroll_x;
	int scroll_y;
	int height_row;
	int height_text;
	int descent;
	int row_counter;
	Size size_cell;
	std::vector<ListViewColumn> columns;
	std::vector<ListViewRow> rows;
	bool valid;
	bool columns_valid;
	Rect lineedit_textrect_offset;
/// \}
};

}

/// \}

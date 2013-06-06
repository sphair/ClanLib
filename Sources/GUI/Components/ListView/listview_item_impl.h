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
**    Magnus Norddahl
*/



#pragma once

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class ListViewColumnData;
class ListViewItem;
class Sprite;

class ListViewItem_Impl
{
public:
	ListViewItem_Impl()
	: open(false), editable(false), selected(false), icon(0), id(0)
	{
	}

	std::weak_ptr<ListViewItem_Impl> parent;
	std::shared_ptr<ListViewItem_Impl> next_sibling;
	std::weak_ptr<ListViewItem_Impl> prev_sibling;
	std::shared_ptr<ListViewItem_Impl> first_child;
	std::weak_ptr<ListViewItem_Impl> last_child;
	bool open;
	bool editable;
	bool selected;
	std::vector<ListViewColumnData> column_data;
	int icon;
	int id;
	std::vector<int> overlay_icons;
	ListViewItem_Impl *get_root_parent();
	Callback_v0 func_item_added;
	Callback_v1<ListViewItem> func_item_modified;
	Callback_v1<ListViewItem> func_item_deleted;
	Callback_v1<Rect> func_render_icon;
	std::shared_ptr<ListViewItemUserData> userdata;
};

}

/// \}

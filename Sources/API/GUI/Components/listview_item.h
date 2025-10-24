/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "../api_gui.h"

class CL_ListViewColumnData;
class CL_ListViewItem_Impl;

/// \brief ListView item.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ListViewItem
{
/// \name Construction
/// \{

public:
	CL_ListViewItem();

	~CL_ListViewItem();


/// \}
/// \name Attributes
/// \{

public:
	bool is_null() const;

	bool is_item() const;

	CL_ListViewColumnData get_column(const CL_StringRef &column_id);

	CL_ListViewItem get_parent();

	CL_ListViewItem get_next_sibling();

	CL_ListViewItem get_prev_sibling();

	CL_ListViewItem get_first_child();

	CL_ListViewItem get_last_child();

	CL_ListViewItem get_document_item();

	int get_parent_count();

	int get_child_count(bool recursive=false, bool recurse_only_into_open_items=false);

	bool has_children() const;

	bool is_open() const;

	bool is_selected() const;

	bool is_editable() const;

	/// \brief Get the item icon id. Defaults to 0 if unset.
	int get_icon() const;

	/// \brief Get the item id. Defaults to 0 if unset.
	int get_id() const;

	CL_UnknownSharedPtr get_userdata() const;

	bool operator==(CL_ListViewItem &other) const;

	bool operator!=(CL_ListViewItem &other) const;

	std::vector<int> get_overlay_icons();


/// \}
/// \name Operations
/// \{

public:
	CL_ListViewItem append_child(CL_ListViewItem &item);

	/// \brief Removes this item from the item tree.
	CL_ListViewItem remove();

	/// \brief Remove all children of this item.
	void remove_children();

	void set_column_text(const CL_StringRef &column_id, const CL_StringRef &text);

	void set_open(bool open);

	void set_icon(int icon_index);

	/// \brief Set the item id. Defaults to 0 if unset.
	void set_id(int id);

	void add_overlay_icon(int icon_index);

	void remove_overlay_icon(int icon_index);

	void set_editable(bool editable);

	void set_userdata(CL_UnknownSharedPtr ptr);


/// \}
/// \name Events
/// \{

public:
	CL_Callback_v1<CL_Rect> &func_render_icon();


/// \}
/// \name Implementation
/// \{

private:
	CL_ListViewItem(const CL_SharedPtr<CL_ListViewItem_Impl> &impl);

	CL_SharedPtr<CL_ListViewItem_Impl> impl;

	friend class CL_ListView;
	friend class CL_ListView_Impl;
	friend class CL_ListViewSelection;
	friend class CL_ListViewLayout;
/// \}
};


/// \}

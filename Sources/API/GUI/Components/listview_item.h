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
**    Magnus Norddahl
**    Harry Storbacka
*/


#pragma once

#include "../api_gui.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class ListViewColumnData;
class ListViewItem_Impl;

class ListViewItemUserData
{
public:
	virtual ~ListViewItemUserData() { }
};

/// \brief ListView item.
class CL_API_GUI ListViewItem
{
/// \name Construction
/// \{

public:
	/// \brief Construct a null instance
	ListViewItem();

	~ListViewItem();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Is Item
	///
	/// \return true = item
	bool is_item() const;

	/// \brief Get column
	///
	/// \param column_id = String Ref
	///
	/// \return List View Column Data
	ListViewColumnData get_column(const std::string &column_id);

	/// \brief Get Parent
	///
	/// \return parent
	ListViewItem get_parent();

	/// \brief Get Next sibling
	///
	/// \return next_sibling
	ListViewItem get_next_sibling();

	/// \brief Get Prev sibling
	///
	/// \return prev_sibling
	ListViewItem get_prev_sibling();

	/// \brief Get First child
	///
	/// \return first_child
	ListViewItem get_first_child();

	/// \brief Get Last child
	///
	/// \return last_child
	ListViewItem get_last_child();

	/// \brief Get Document item
	///
	/// \return document_item
	ListViewItem get_document_item();

	/// \brief Get Parent count
	///
	/// \return parent_count
	int get_parent_count();

	/// \brief Get child count
	///
	/// \param recursive = bool
	/// \param recurse_only_into_open_items = bool
	///
	/// \return int
	int get_child_count(bool recursive=false, bool recurse_only_into_open_items=false);

	/// \brief Has children
	///
	/// \return bool
	bool has_children() const;

	/// \brief Is Open
	///
	/// \return true = open
	bool is_open() const;

	/// \brief Is Selected
	///
	/// \return true = selected
	bool is_selected() const;

	/// \brief Is Editable
	///
	/// \return true = editable
	bool is_editable() const;

	/// \brief Get the item icon id. Defaults to 0 if unset.
	int get_icon() const;

	/// \brief Get the item id. Defaults to 0 if unset.
	int get_id() const;

	/// \brief Get Userdata
	///
	/// \return userdata
	std::shared_ptr<ListViewItemUserData> get_userdata() const;

	bool operator==(ListViewItem &other) const;

	bool operator!=(ListViewItem &other) const;

	std::vector<int> get_overlay_icons();

/// \}
/// \name Operations
/// \{

public:

	/// \brief Append child
	///
	/// \param item = List View Item
	///
	/// \return List View Item
	ListViewItem append_child(ListViewItem &item);

	/// \brief Removes this item from the item tree.
	ListViewItem remove();

	/// \brief Remove all children of this item.
	void remove_children();

	/// \brief Set column text
	///
	/// \param column_id = String Ref
	/// \param text = String Ref
	void set_column_text(const std::string &column_id, const std::string &text);

	/// \brief Set open
	///
	/// \param open = bool
	void set_open(bool open);

	/// \brief Set icon
	///
	/// \param icon_index = value
	void set_icon(int icon_index);

	/// \brief Set the item id. Defaults to 0 if unset.
	void set_id(int id);

	/// \brief Add overlay icon
	///
	/// \param icon_index = value
	void add_overlay_icon(int icon_index);

	/// \brief Remove overlay icon
	///
	/// \param icon_index = value
	void remove_overlay_icon(int icon_index);

	/// \brief Set editable
	///
	/// \param editable = bool
	void set_editable(bool editable);

	/// \brief Set userdata
	///
	/// \param ptr = Unknown Shared Ptr
	void set_userdata(std::shared_ptr<ListViewItemUserData> ptr);

/// \}
/// \name Events
/// \{

public:
	Callback<void(Rect)> &func_render_icon();

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Constructs a ListViewItem
	///
	/// \param ListViewItem_Impl = Shared Ptr
	ListViewItem(const std::shared_ptr<ListViewItem_Impl> &impl);

	std::shared_ptr<ListViewItem_Impl> impl;

	friend class ListView;
	friend class ListView_Impl;
	friend class ListViewSelection;
	friend class ListViewLayout;
/// \}
};

}

/// \}

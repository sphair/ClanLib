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

namespace clan
{
// (Note, enum moved here because of problems with gcc not allowing "enum ListViewDisplayMode;")
enum ListViewDisplayMode
{
	listview_mode_thumbnails,
	listview_mode_tiles,
	listview_mode_icons,
	listview_mode_list,
	listview_mode_details
};
}

#include "../api_gui.h"
#include "../gui_component.h"
#include "listview_item.h"
#include "listview_selected_item.h"
#include "listview_column_header.h"
#include "listview_icon_list.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class ListViewHeader;
class ListView_Impl;
class ListViewIconList;
class InputEvent;

/// \brief List view component.
class CL_API_GUI ListView : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a ListView
	///
	/// \param parent = GUIComponent
	ListView(GUIComponent *parent);

	virtual ~ListView();

/// \}
/// \name Attributes
/// \{

public:
	using GUIComponent::get_named_item;

	/// \brief Find the child ListView with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static ListView *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Get current listview scrollbar position
	///
	/// \return int
	int get_scroll_position();

	/// \brief Get maximal listview scrollbar position
	///
	/// \return int
	int get_scroll_max_position();
	
	/// \brief Get Header
	///
	/// \return header
	ListViewHeader *get_header() const;

	/// \brief Get Document item
	///
	/// \return document_item
	ListViewItem get_document_item() const;

	/// \brief Get Select whole row
	///
	/// \return select_whole_row
	bool get_select_whole_row() const;

	/// \brief Get Icon list
	///
	/// \return icon_list
	ListViewIconList get_icon_list() const;

	/// \brief Returns a container class holding the selected items in a linked list structure.
	ListViewSelection get_selected_items() const;

	/// \brief Returns the currently selected item in single selection mode. 
	///
	/// If multiple items are selected, the item selected first (chronological order) is returned.
	/// If no element is selected, an empty ListViewItem is returned.
	ListViewItem get_selected_item();

	/// \brief Returns the preferred size of the component, as defined in its CSS file.
	Size get_css_size() const;

	/// \brief Get Display mode
	///
	/// \return display_mode
	ListViewDisplayMode get_display_mode() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Create item
	///
	/// \return List View Item
	ListViewItem create_item();

	/// \brief Set listview scrollbar position
	///
	/// \param pos = int
	void set_scroll_position(int pos);

	/// \brief On process message
	///
	/// \param msg = GUIMessage
	void on_process_message(std::shared_ptr<GUIMessage> &msg);

	/// \brief When true, the whole row is drawn selected, as opposed to only the first column.
	void set_select_whole_row(bool value);

	/// \brief When true, multiple items can be selected.
	void set_multi_select(bool value);

	/// \brief Set display mode
	///
	/// \param mode = List View Display Mode
	void set_display_mode(ListViewDisplayMode mode);

	/// \brief Set if icons are shown in the detail display mode
	void show_detail_icon(bool enable = true);

	/// \brief Set if node opener is shown in the detail display mode
	void show_detail_opener(bool enable = true);

	/// \brief Remove all items.
	void clear();

	/// \brief Unselect all selected items.
	void clear_selection();

	/// \brief Set selected
	///
	/// \param item = List View Item
	/// \param selected = bool
	void set_selected(ListViewItem &item, bool selected=true);

	/// \brief Returns a ListViewItem with the string 'str' in the column 'col_id' or a NULL item if none found.
	ListViewItem find(const std::string &col_id, const std::string &str, bool recursive=true);

	/// \brief Returns the first ListViewItem with the specified ID, or a NULL item if none found.
	ListViewItem find(int id, bool recursive=true);

	/// \brief Returns a ListViewItem with the userdata 'ptr', or a NULL item if none found.
	ListViewItem find(std::shared_ptr<ListViewItemUserData> userdata, bool recursive=true);

/// \}
/// \name Events
/// \{

public:
	/// \brief Selected items are returned as children of the returned item.
	std::function<void(ListViewSelection)> &func_selection_changed();

	/// \brief Invoked on double clicking a listview item.
	std::function<void(const ListViewItem &)> &func_item_doubleclick();

	/// \brief Invoked when enter is pressed in the ListViews built-in LineEdit control.
	/// The caller may change the passed string. Returning false from the callback cancels the edit.
	std::function<bool(ListViewItem, std::string &)> &func_item_edited();

	std::function<void(InputEvent&)> &func_key_pressed();

	std::function<void(InputEvent&)> &func_key_released();

	/// \brief Callback called when the right mouse button is released (a click).
	/// Use this callback for listview context menus. Click position sent as parameter.
	std::function<void(Point)> &func_mouse_right_up();

	/// \brief Callback called when an item node is opened
	std::function<void(const ListViewItem &)> &func_item_opened();

	/// \brief Callback called when an item node is closed
	std::function<void(const ListViewItem &)> &func_item_closed();

	/// \brief Callback called when the user begins a drag'n'drop action
	std::function<void()> &func_begin_drag();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<ListView_Impl> impl;

	friend class ListViewRenderer;
/// \}
};

}

/// \}

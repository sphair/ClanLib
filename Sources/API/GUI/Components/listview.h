/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

// (Note, enum moved here because of problems with gcc not allowing "enum CL_ListViewDisplayMode;")
enum CL_ListViewDisplayMode
{
	listview_mode_thumbnails,
	listview_mode_tiles,
	listview_mode_icons,
	listview_mode_list,
	listview_mode_details
};

#include "../api_gui.h"
#include "../gui_component.h"
#include "listview_item.h"
#include "listview_selected_item.h"
#include "listview_column_header.h"
#include "listview_icon_list.h"

class CL_ListViewHeader;
class CL_ListView_Impl;
class CL_ListViewIconList;
class CL_InputEvent;

/// \brief List view component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ListView : public CL_GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a ListView
	///
	/// \param parent = GUIComponent
	CL_ListView(CL_GUIComponent *parent);

	virtual ~CL_ListView();

/// \}
/// \name Attributes
/// \{

public:
	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_ListView with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_ListView *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Header
	///
	/// \return header
	CL_ListViewHeader *get_header() const;

	/// \brief Get Document item
	///
	/// \return document_item
	CL_ListViewItem get_document_item() const;

	/// \brief Get Select whole row
	///
	/// \return select_whole_row
	bool get_select_whole_row() const;

	/// \brief Get Icon list
	///
	/// \return icon_list
	CL_ListViewIconList get_icon_list() const;

	/// \brief Returns a container class holding the selected items in a linked list structure.
	CL_ListViewSelection get_selected_items() const;

	/// \brief Returns the currently selected item in single selection mode. If multiple items are selected, the item selected first (chronological order) is returned.
	CL_ListViewItem get_selected_item();

	/// \brief Returns the preferred size of the component, as defined in its CSS file.
	CL_Size get_preferred_size() const;

	/// \brief Get Display mode
	///
	/// \return display_mode
	CL_ListViewDisplayMode get_display_mode() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Create item
	///
	/// \return List View Item
	CL_ListViewItem create_item();

	/// \brief On process message
	///
	/// \param msg = GUIMessage
	void on_process_message(CL_GUIMessage &msg);

	/// \brief When true, the whole row is drawn selected, as opposed to only the first column.
	void set_select_whole_row(bool value);

	/// \brief When true, multiple items can be selected.
	void set_multi_select(bool value);

	/// \brief Set display mode
	///
	/// \param mode = List View Display Mode
	void set_display_mode(CL_ListViewDisplayMode mode);

	/// \brief Remove all items.
	void clear();

	/// \brief Unselect all selected items.
	void clear_selection();

	/// \brief Set selected
	///
	/// \param item = List View Item
	/// \param selected = bool
	void set_selected(CL_ListViewItem &item, bool selected=true);

	/// \brief Returns a CL_ListViewItem with the string 'str' in the column 'col_id' or a NULL item if none found.
	CL_ListViewItem find(const CL_StringRef &col_id, const CL_StringRef &str, bool recursive=true);

	/// \brief Returns the first CL_ListViewItem with the specified ID, or a NULL item if none found.
	CL_ListViewItem find(int id, bool recursive=true);

	/// \brief Returns a CL_ListViewItem with the userdata 'ptr', or a NULL item if none found.
	CL_ListViewItem find(CL_UnknownSharedPtr userdata, bool recursive=true);

/// \}
/// \name Events
/// \{

public:
	/// \brief Selected items are returned as children of the returned item.
	CL_Callback_v1<CL_ListViewSelection> &func_selection_changed();

	/// \brief Invoked when enter is pressed in the ListViews built-in LineEdit control.
	/// The caller may change the passed string. Returning false from the callback cancels the edit.
	CL_Callback_2<bool, CL_ListViewItem, CL_String &> &func_item_edited();

	CL_Callback_v1<CL_InputEvent> &func_key_pressed();

	CL_Callback_v1<CL_InputEvent> &func_key_released();

	/// \brief Callback called when the right mouse button is released (a click).
	/// Use this callback for listview context menus. Click position sent as parameter.
	CL_Callback_v1<CL_Point> &func_mouse_right_up();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ListView_Impl> impl;

	friend class CL_ListViewRenderer;
/// \}
};

/// \}

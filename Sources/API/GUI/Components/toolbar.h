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

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"

class CL_Sprite;
class CL_ToolBarItem;
class CL_ToolBar_Impl;

/// \brief Tool bar component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ToolBar : public CL_GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a ToolBar
	///
	/// \param parent = GUIComponent
	CL_ToolBar(CL_GUIComponent *parent);

	virtual ~CL_ToolBar();

/// \}
/// \name Attributes
/// \{

public:

	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_ToolBar with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_ToolBar *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	CL_Size get_preferred_size() const;

	/// \brief Get item by id
	///
	/// \param id = value
	///
	/// \return Tool Bar Item
	CL_ToolBarItem get_item_by_id(int id);

	/// \brief Get item
	///
	/// \param index = value
	///
	/// \return Tool Bar Item
	CL_ToolBarItem get_item(int index);

	/// \brief Get item index
	///
	/// \param id = value
	///
	/// \return int
	int get_item_index(int id);

	/// \brief Get Item count
	///
	/// \return item_count
	int get_item_count() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Add item
	///
	/// \param icon = Sprite
	/// \param frame = value
	/// \param text = String Ref
	/// \param id = value
	///
	/// \return Tool Bar Item
	CL_ToolBarItem add_item(const CL_Sprite &icon, int frame, const CL_StringRef &text, int id);

	/// \brief Insert item
	///
	/// \param icon = Sprite
	/// \param frame = value
	/// \param text = String Ref
	/// \param id = value
	/// \param index = value
	///
	/// \return Tool Bar Item
	CL_ToolBarItem insert_item(const CL_Sprite &icon, int frame, const CL_StringRef &text, int id = -1, int index = -1);

	/// \brief Insert separator
	///
	/// \param index = value
	///
	/// \return Tool Bar Item
	CL_ToolBarItem insert_separator(int index = -1);

	/// \brief Delete item
	///
	/// \param index = value
	void delete_item(int index);

	/// \brief Set single selection
	///
	/// \param value = bool
	void set_single_selection(bool value);

	/// \brief Clear selection
	void clear_selection();

/// \}
/// \name Events
/// \{

public:
	CL_Callback_v1<CL_ToolBarItem> &func_item_clicked();

	CL_Callback_v1<CL_ToolBarItem> &func_item_selected();

	CL_Callback_v1<CL_ToolBarItem> &func_item_unselected();

	/// \brief Signal invoked when the toolbar is right-clicked.
	/// The parameters are click position and clicked item index.
	CL_Callback_v2<CL_Point, int> &func_mouse_right_up();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ToolBar_Impl> impl;
/// \}
};

/// \}

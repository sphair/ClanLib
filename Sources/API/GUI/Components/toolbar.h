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
#include "../gui_component.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class Sprite;
class ToolBarItem;
class ToolBar_Impl;

/// \brief Tool bar component.
class CL_API_GUI ToolBar : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a ToolBar
	///
	/// \param parent = GUIComponent
	ToolBar(GUIComponent *parent);

	virtual ~ToolBar();

/// \}
/// \name Attributes
/// \{

public:

	using GUIComponent::get_named_item;

	/// \brief Find the child ToolBar with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static ToolBar *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	Size get_css_size() const;

	/// \brief Get item by id
	///
	/// \param id = value
	///
	/// \return Tool Bar Item
	ToolBarItem get_item_by_id(int id);

	/// \brief Get item
	///
	/// \param index = value
	///
	/// \return Tool Bar Item
	ToolBarItem get_item(int index);

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
	ToolBarItem add_item(const Sprite &icon, int frame, const std::string &text, int id);

	/// \brief Insert item
	///
	/// \param icon = Sprite
	/// \param frame = value
	/// \param text = String Ref
	/// \param id = value
	/// \param index = value
	///
	/// \return Tool Bar Item
	ToolBarItem insert_item(const Sprite &icon, int frame, const std::string &text, int id = -1, int index = -1);

	/// \brief Insert separator
	///
	/// \param index = value
	///
	/// \return Tool Bar Item
	ToolBarItem insert_separator(int index = -1);

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
	std::function<void(ToolBarItem)> &func_item_clicked();

	std::function<void(ToolBarItem)> &func_item_selected();

	std::function<void(ToolBarItem)> &func_item_unselected();

	/// \brief Signal invoked when the toolbar is right-clicked.
	/// The parameters are click position and clicked item index.
	std::function<void(Point, int)> &func_mouse_right_up();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<ToolBar_Impl> impl;
/// \}
};

}

/// \}

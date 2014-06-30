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
**    Kenneth Gangstoe
*/


#pragma once

#include "../api_gui.h"
#include "../gui_component.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class Sprite;
class ComboBox_Impl;
class PopupMenu;

/// \brief Combo box component.
class CL_API_GUI ComboBox : public GUIComponent
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a ComboBox
	///
	/// \param parent = GUIComponent
	ComboBox(GUIComponent *parent);

	virtual ~ComboBox();

/// \}
/// \name Attributes
/// \{
public:
	using GUIComponent::get_named_item;

	/// \brief Find the child ComboBox with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static ComboBox *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Is Editable
	///
	/// \return true = editable
	bool is_editable() const;

	/// \brief Get Dropdown height
	///
	/// \return dropdown_height
	int get_dropdown_height() const;

	/// \brief Get Selected item
	///
	/// \return selected_item
	int get_selected_item() const;

	/// \brief Get Text
	///
	/// \return text
	std::string get_text() const;

	/// \brief Get item
	///
	/// \param index = value
	///
	/// \return String Ref
	std::string get_item(int index) const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Set editable
	///
	/// \param enable = bool
	void set_editable(bool enable = true);

	/// \brief Set dropdown height
	///
	/// \param height = value
	void set_dropdown_height(int height);

	/// \brief Set dropdown minimum width
	///
	/// \param min_width = value
	void set_dropdown_minimum_width(int min_width);

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const std::string &text);

	/// \brief Set selected item
	///
	/// \param index = value
	void set_selected_item(int index);

	/// \brief Set popup menu
	///
	/// \param menu = Popup Menu
	void set_popup_menu(PopupMenu &menu);

/// \}
/// \name Events
/// \{
public:
	/// \brief Func dropdown opened
	///
	/// \return std::function<void()>
	std::function<void()> &func_dropdown_opened();

	/// \brief Func dropdown closed
	///
	/// \return std::function<void()>
	std::function<void()> &func_dropdown_closed();

	/// \brief Func enter pressed
	///
	/// \return std::function<void()>
	std::function<void()> &func_enter_pressed();

	/// \brief Func before edit changed
	///
	/// \return std::function<void()>
	std::function<void()> &func_before_edit_changed();

	/// \brief Func after edit changed
	///
	/// \return std::function<void()>
	std::function<void()> &func_after_edit_changed();

	/// \brief Func after item selected
	///
	/// \return std::function<void(int)>
	std::function<void(int)> &func_item_selected();

	/// \brief Func after selection changed
	///
	/// \return std::function<void(int)>
	std::function<void(int)> &func_selection_changed();

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<ComboBox_Impl> impl;
/// \}
};

}

/// \}

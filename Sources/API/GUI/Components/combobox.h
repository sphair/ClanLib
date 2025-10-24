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
**    Kenneth Gangstoe
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Core/Signals/callback_v1.h"
#include "../../Core/Signals/callback_1.h"

class CL_Sprite;
class CL_ComboBox_Impl;
class CL_PopupMenu;

/// \brief Combo box component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ComboBox : public CL_GUIComponent
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a ComboBox
	///
	/// \param parent = GUIComponent
	CL_ComboBox(CL_GUIComponent *parent);

	virtual ~CL_ComboBox();

/// \}
/// \name Attributes
/// \{
public:
	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_ComboBox with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_ComboBox *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

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
	CL_StringRef get_text() const;

	/// \brief Get item
	///
	/// \param index = value
	///
	/// \return String Ref
	CL_StringRef get_item(int index) const;

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
	void set_text(const CL_StringRef &text);

	/// \brief Set selected item
	///
	/// \param index = value
	void set_selected_item(int index);

	/// \brief Set popup menu
	///
	/// \param menu = Popup Menu
	void set_popup_menu(CL_PopupMenu &menu);

/// \}
/// \name Events
/// \{
public:
	/// \brief Func dropdown opened
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_dropdown_opened();

	/// \brief Func dropdown closed
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_dropdown_closed();

	/// \brief Func before edit changed
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_before_edit_changed();

	/// \brief Func after edit changed
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_after_edit_changed();

	/// \brief Func after item selected
	///
	/// \return Callback_v1<int>
	CL_Callback_v1<int> &func_item_selected();

	/// \brief Func after selection changed
	///
	/// \return Callback_v1<int>
	CL_Callback_v1<int> &func_selection_changed();

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_ComboBox_Impl> impl;
/// \}
};

/// \}

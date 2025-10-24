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

class CL_RadioButton_Impl;

/// \brief Radio button component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_RadioButton : public CL_GUIComponent
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a RadioButton
	///
	/// \param parent = GUIComponent
	CL_RadioButton(CL_GUIComponent *parent);

	virtual ~CL_RadioButton();

/// \}
/// \name Attributes
/// \{
public:

	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_RadioButton with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_RadioButton *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Text
	///
	/// \return text
	CL_String get_text() const;

	/// \brief Get Id
	///
	/// \return id
	int get_id() const;

	/// \brief Is Selected
	///
	/// \return true = selected
	bool is_selected() const;

	/// \brief Get Group name
	///
	/// \return group_name
	CL_String get_group_name() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const CL_StringRef &text);

	/// \brief Set id
	///
	/// \param id = value
	void set_id(int id);

	/// \brief Set selected
	///
	/// \param selected = bool
	void set_selected(bool selected);

	/// \brief Set group name
	///
	/// \param str = String Ref
	void set_group_name(const CL_String &str);

/// \}
/// \name Events
/// \{
public:

	/// \brief Func selected
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_selected();

	/// \brief Func unselected
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_unselected();

	/// \brief Callback invoked when any of the radio buttons in the group is selected.
	/** The newly selected radio button is passed as parameter to the callback.*/
	CL_Callback_v1<CL_RadioButton*> &func_group_selection_changed();
/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_RadioButton_Impl> impl;

	friend class CL_RadioButton_Impl;
/// \}
};

/// \}

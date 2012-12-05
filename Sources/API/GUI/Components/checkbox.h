/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

namespace clan
{

class CheckBox_Impl;

/// \brief Check box component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CheckBox : public GUIComponent
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a CheckBox
	///
	/// \param parent = GUIComponent
	CheckBox(GUIComponent *parent);

	virtual ~CheckBox();

/// \}
/// \name Attributes
/// \{
public:
	using GUIComponent::get_named_item;

	/// \brief Find the child CheckBox with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CheckBox *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Is Indeterminated
	///
	/// \return true = indeterminated
	bool is_indeterminated() const;

	/// \brief Is Checked
	///
	/// \return true = checked
	bool is_checked() const;

	/// \brief Is 3state
	///
	/// \return true = 3state
	bool is_3state() const;

	const std::string &get_text() const;

	/// \brief Returns the preferred content width
	///
	/// Override this function if the component has non-css content.
	float get_preferred_content_width();

	/// \brief Returns the preferred content height for the specified content width
	///
	/// Override this function if the component has non-css content.
	float get_preferred_content_height(float width);

/// \}
/// \name Operations
/// \{
public:

	/// \brief Set indeterminated
	void set_indeterminated();

	/// \brief Set checked
	///
	/// \param enable = bool
	void set_checked(bool enable);

	/// \brief Set 3state
	///
	/// \param enable = bool
	void set_3state(bool enable);

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const std::string &text);

/// \}
/// \name Events
/// \{
public:

	/// \brief Func checked
	///
	/// \return Callback_v0
	Callback_v0 &func_checked();

	/// \brief Func unchecked
	///
	/// \return Callback_v0
	Callback_v0 &func_unchecked();

	/// \brief Func indeterminated
	///
	/// \return Callback_v0
	Callback_v0 &func_indeterminated();

	/// \brief Func state changed
	///
	/// \return Callback_v0
	Callback_v0 &func_state_changed();

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<CheckBox_Impl> impl;
/// \}
};

}

/// \}

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

class RadioButton_Impl;

/// \brief Radio button component.
class CL_API_GUI RadioButton : public GUIComponent
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a RadioButton
	///
	/// \param parent = GUIComponent
	RadioButton(GUIComponent *parent);

	virtual ~RadioButton();

/// \}
/// \name Attributes
/// \{
public:

	using GUIComponent::get_named_item;

	/// \brief Find the child RadioButton with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static RadioButton *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Get Text
	///
	/// \return text
	std::string get_text() const;

	/// \brief Get Id
	///
	/// \return id
	int get_radio_id() const;

	/// \brief Is Selected
	///
	/// \return true = selected
	bool is_selected() const;

	/// \brief Get Group name
	///
	/// \return group_name
	std::string get_group_name() const;

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

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const std::string &text);

	/// \brief Set id
	///
	/// \param id = value
	void set_radio_id(int id);

	/// \brief Set selected
	///
	/// \param selected = bool
	void set_selected(bool selected);

	/// \brief Set group name
	///
	/// \param str = String Ref
	void set_group_name(const std::string &str);

/// \}
/// \name Events
/// \{
public:

	/// \brief Func selected
	///
	/// \return std::function<void()>
	std::function<void()> &func_selected();

	/// \brief Func unselected
	///
	/// \return std::function<void()>
	std::function<void()> &func_unselected();

	/// \brief Callback invoked when any of the radio buttons in the group is selected.
	/** The newly selected radio button is passed as parameter to the callback.*/
	std::function<void(RadioButton*)> &func_group_selection_changed();
/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<RadioButton_Impl> impl;

	friend class RadioButton_Impl;
/// \}
};

}

/// \}

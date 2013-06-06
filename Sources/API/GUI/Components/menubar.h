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

class PopupMenu;
class MenuBar_Impl;

/// \brief Menu bar component.
class CL_API_GUI MenuBar : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a MenuBar
	///
	/// \param parent = GUIComponent
	MenuBar(GUIComponent *parent);

	virtual ~MenuBar();

/// \}
/// \name Attributes
/// \{

public:
	using GUIComponent::get_named_item;

	/// \brief Find the child MenuBar with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static MenuBar *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	virtual Size get_css_size() const;

	/// \brief Get menu
	///
	/// \param index = value
	///
	/// \return Popup Menu
	PopupMenu get_menu(int index) const;


/// \}
/// \name Operations
/// \{

public:

	/// \brief Clear
	void clear();

	/// \brief Add menu
	///
	/// \param name = String Ref
	/// \param popup_menu = Popup Menu
	///
	/// \return int
	int add_menu(const std::string &name, PopupMenu popup_menu);

	/// \brief Remove menu
	///
	/// \param index = value
	void remove_menu(int index);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<MenuBar_Impl> impl;

	friend class MenuModalLoop;
/// \}
};

}

/// \}

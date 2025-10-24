/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_PopupMenu;
class CL_MenuBar_Impl;

/// \brief Menu bar component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_MenuBar : public CL_GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a MenuBar
	///
	/// \param parent = GUIComponent
	CL_MenuBar(CL_GUIComponent *parent);

	virtual ~CL_MenuBar();

/// \}
/// \name Attributes
/// \{

public:
	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_MenuBar with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_MenuBar *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	virtual CL_Size get_preferred_size() const;

	/// \brief Get menu
	///
	/// \param index = value
	///
	/// \return Popup Menu
	CL_PopupMenu get_menu(int index) const;


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
	int add_menu(const CL_StringRef &name, CL_PopupMenu popup_menu);

	/// \brief Remove menu
	///
	/// \param index = value
	void remove_menu(int index);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_MenuBar_Impl> impl;

	friend class CL_MenuModalLoop;
/// \}
};

/// \}

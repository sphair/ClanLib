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

class CL_Sprite;
class CL_ToolBarItem_Impl;
class CL_ToolBar;

/// \brief Toolbar menu item.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_ToolBarItem
{
/// \name Construction
/// \{

public:
	/// \brief Construct a null instance
	CL_ToolBarItem();

	/// \brief Constructs a ToolBarItem
	///
	/// \param id = value
	CL_ToolBarItem(int id);

	/// \brief Constructs a ToolBarItem
	///
	/// \param CL_ToolBarItem_Impl = Shared Ptr
	CL_ToolBarItem(const CL_SharedPtr<CL_ToolBarItem_Impl> &impl);

	virtual ~CL_ToolBarItem();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Text
	///
	/// \return text
	CL_StringRef get_text() const;

	/// \brief Get Id
	///
	/// \return id
	int get_id() const;

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Is Disabled
	///
	/// \return true = disabled
	bool is_disabled() const;

	/// \brief Is Toggling
	///
	/// \return true = toggling
	bool is_toggling() const;

	/// \brief Is Pressed
	///
	/// \return true = pressed
	bool is_pressed() const;

/// \}
/// \name Events
/// \{

public:

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set id
	///
	/// \param id = value
	void set_id(int id);

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const CL_StringRef &text);

	/// \brief Set icon
	///
	/// \param image = Sprite
	/// \param frame = value
	void set_icon(const CL_Sprite &image, int frame = 0);

	/// \brief Controls if buttons stay down when pressed.
	void set_toggling(bool toggling);

	/// \brief Set pressed
	///
	/// \param value = bool
	void set_pressed(bool value);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ToolBarItem_Impl> impl;

	friend class CL_ToolBar;
	friend class CL_ToolBar_Impl;
/// \}
};

/// \}

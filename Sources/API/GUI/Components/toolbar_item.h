/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
	CL_ToolBarItem();

	CL_ToolBarItem(int id);

	CL_ToolBarItem(const CL_SharedPtr<CL_ToolBarItem_Impl> &impl);

	virtual ~CL_ToolBarItem();


/// \}
/// \name Attributes
/// \{

public:
	CL_StringRef get_text() const;

	int get_id() const;

	bool is_null() const;

	bool is_disabled() const;

	bool is_toggling() const;

	bool is_pressed() const;


/// \}
/// \name Events
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	void set_id(int id);

	void set_text(const CL_StringRef &text);

	void set_icon(const CL_Sprite &image, int frame = 0);

	/// \brief Controls if buttons stay down when pressed.
	void set_toggling(bool toggling);

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

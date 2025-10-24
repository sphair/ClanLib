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
#include "../../Display/Window/input_event.h"
#include "popupmenu_item.h"

class CL_PixelBuffer;
class CL_PopupMenu;

class CL_PopupMenu_Impl;

/// \brief Pop-up menu.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_PopupMenu
{
/// \name Construction
/// \{

public:
	CL_PopupMenu();

	virtual ~CL_PopupMenu();

	static CL_PopupMenu create_null_object();


/// \}
/// \name Attributes
/// \{

public:
	bool is_null() const;

	CL_String get_class_name() const;

	CL_PopupMenuItem get_item(int id);

	CL_PopupMenuItem get_item_at(int index);

	int item_count() const;

	int get_minimum_width() const;

	virtual CL_Size get_preferred_size() const;


/// \}
/// \name Operations
/// \{

public:
	/// Run the popup menu for modal dialog
	void exec(CL_GUIComponent *parent, const CL_Point &pos);

	/// Start the popup menu for dialog (not modal)
	void start(CL_GUIComponent *parent, const CL_Point &pos);

	CL_PopupMenuItem insert_item(const CL_StringRef &text, int id = -1, int index = -1);

	CL_PopupMenuItem insert_item_accel(const CL_StringRef &text, const CL_StringRef &accel_text=cl_text(""), int id = -1, int index = -1);

	CL_PopupMenuItem insert_separator(int index = -1);

	void set_minimum_width(int width_pixels);

	void set_maximum_height(int height_pixels);

	void set_class_name(const CL_StringRef &class_name);

	/// \brief Returns the index of the item that matches the start or the string furthest.
	int find_item(const CL_StringRef &text, bool case_sensitive = false);


/// \}
/// \name Events
/// \{

	CL_Callback_v1<CL_PopupMenuItem> &func_item_selected();

	CL_Callback_v1<CL_InputEvent> &func_keyboard_event();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_PopupMenu_Impl> impl;

	friend class CL_MenuBar;
	friend class CL_MenuBar_Impl;
	friend class CL_PopupMenuWindow;
/// \}
};


/// \}

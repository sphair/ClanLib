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

	/// \brief Create null object
	///
	/// \return Popup Menu
	static CL_PopupMenu create_null_object();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get Class name
	///
	/// \return class_name
	CL_String get_class_name() const;

	/// \brief Get item
	///
	/// \param id = value
	///
	/// \return Popup Menu Item
	CL_PopupMenuItem get_item(int id);

	/// \brief Get item at
	///
	/// \param index = value
	///
	/// \return Popup Menu Item
	CL_PopupMenuItem get_item_at(int index);

	/// \brief Item count
	///
	/// \return int
	int get_item_count() const;

	/// \brief Get Minimum width
	///
	/// \return minimum_width
	int get_minimum_width() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	virtual CL_Size get_preferred_size() const;

/// \}
/// \name Events
/// \{
public:
	/// \brief bool func_close()
	CL_Callback_v0 &func_close();

/// \}
/// \name Operations
/// \{
public:
	/// Start the popup menu for dialog (not modal)
	void start(CL_GUIComponent *parent, const CL_Point &pos);

	/// \brief Insert item
	///
	/// \param text = String Ref
	/// \param id = value
	/// \param index = value
	///
	/// \return Popup Menu Item
	CL_PopupMenuItem insert_item(const CL_StringRef &text, int id = -1, int index = -1);

	/// \brief Insert item accel
	///
	/// \param text = String Ref
	/// \param accel_text = String Ref
	///
	/// \return Popup Menu Item
	CL_PopupMenuItem insert_item_accel(const CL_StringRef &text, const CL_StringRef &accel_text="", int id = -1, int index = -1);

	/// \brief Insert separator
	///
	/// \param index = value
	///
	/// \return Popup Menu Item
	CL_PopupMenuItem insert_separator(int index = -1);

	/// \brief Set minimum width
	///
	/// \param width_pixels = value
	void set_minimum_width(int width_pixels);

	/// \brief Set maximum height
	///
	/// \param height_pixels = value
	void set_maximum_height(int height_pixels);

	/// \brief Set class name
	///
	/// \param class_name = String Ref
	void set_class_name(const CL_StringRef &class_name);

	/// \brief Returns the index of the item that matches the start or the string furthest.
	int find_item(const CL_StringRef &text, bool case_sensitive = false);
	
	/// \brief Delete all menu items
	///
	/// \param class_name = String Ref
	void clear();

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

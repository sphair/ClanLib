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

class PixelBuffer;
class PopupMenuItem_Impl;
class PopupMenu;

/// \brief Popup menu item.
class PopupMenuItem
{
/// \name Construction
/// \{

public:
	/// \brief Construct a null instance
	PopupMenuItem();

	/// \brief Constructs a PopupMenuItem
	///
	/// \param id = value
	PopupMenuItem(int id);

	/// \brief Constructs a PopupMenuItem
	///
	/// \param PopupMenuItem_Impl = Shared Ptr
	PopupMenuItem(const std::shared_ptr<PopupMenuItem_Impl> &impl);

	virtual ~PopupMenuItem();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Text
	///
	/// \return text
	std::string get_text() const;

	/// \brief Get Accelerator text
	///
	/// \return accelerator_text
	std::string get_accelerator_text() const;

	/// \brief Get Id
	///
	/// \return id
	int get_id() const;

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Has submenu
	///
	/// \return bool
	bool has_submenu() const;

	/// \brief Is Disabled
	///
	/// \return true = disabled
	bool is_disabled() const;

	/// \brief Is Separator
	///
	/// \return true = separator
	bool is_separator() const;

	/// \brief Get Submenu
	///
	/// \return submenu
	PopupMenu get_submenu();

	/// \brief Is Checkable
	///
	/// \return true = checkable
	bool is_checkable() const;

	/// \brief Is Checked
	///
	/// \return true = checked
	bool is_checked() const;

	/// \brief Get Icon
	///
	/// \return icon
	PixelBuffer get_icon() const;

/// \}
/// \name Events
/// \{

public:

	/// \brief Func clicked
	///
	/// \return Callback<void()>
	Callback<void()> &func_clicked();

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
	void set_text(const std::string &text);

	/// \brief Set icon
	///
	/// \param image = Pixel Buffer
	void set_icon(const PixelBuffer &image);

	/// \brief Set submenu
	///
	/// \param submenu = Popup Menu
	void set_submenu(PopupMenu &submenu);

	/// \brief Set enabled
	///
	/// \param enabled = bool
	void set_enabled(bool enabled);

	/// \brief Set separator
	///
	/// \param separator = bool
	void set_separator(bool separator);

	/// \brief Set accelerator text
	///
	/// \param str = String Ref
	void set_accelerator_text(const std::string &str);

	/// \brief Set checkable
	///
	/// \param checkable = bool
	void set_checkable(bool checkable);

	/// \brief Set checked
	///
	/// \param checked = bool
	void set_checked(bool checked);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<PopupMenuItem_Impl> impl;
/// \}
};

}

/// \}

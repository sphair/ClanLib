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
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../../Display/2D/sprite.h"
#include "listview_icon.h"

class CL_ListViewIconList_Impl;
class CL_ListViewIcon_Impl;

/// \brief ListView icon list.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ListViewIconList
{
/// \name Construction
/// \{

public:
	CL_ListViewIconList();
	~CL_ListViewIconList();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get icon
	///
	/// \param index = value
	///
	/// \return List View Icon
	CL_ListViewIcon get_icon(int index);

	/// \brief Get Color
	///
	/// \return color
	CL_Colorf get_color();

	/// \brief Get Selected color
	///
	/// \return selected_color
	CL_Colorf get_selected_color();

	/// \brief Get Color overlay
	///
	/// \return color_overlay
	CL_Colorf get_color_overlay();

	/// \brief Get Selected color overlay
	///
	/// \return selected_color_overlay
	CL_Colorf get_selected_color_overlay();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Clear the icon list.
	void clear();

	/// \brief Erase the icon at the specified index.
	void erase(int index);

	/// \brief Set the icon at the specified index.
	void set_icon(int index, const CL_ListViewIcon &icon);

	/// \brief Set color
	///
	/// \param color = Colorf
	void set_color(const CL_Colorf &color);

	/// \brief Set selected color
	///
	/// \param color = Colorf
	void set_selected_color(const CL_Colorf &color);

	/// \brief Set color overlay
	///
	/// \param color = Colorf
	void set_color_overlay(const CL_Colorf &color);

	/// \brief Set selected color overlay
	///
	/// \param color = Colorf
	void set_selected_color_overlay(const CL_Colorf &color);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ListViewIconList_Impl> impl;
/// \}
};

/// \}

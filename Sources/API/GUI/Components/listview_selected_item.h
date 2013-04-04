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
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "listview_item.h"

namespace clan
{

class ListViewSelectedItem_Impl;
class ListViewSelection_Impl;

/// \brief ListView selected item.
class CL_API_GUI ListViewSelectedItem
{
/// \name Construction
/// \{

public:
	/// \brief Contruct a null instance
	ListViewSelectedItem();

	/// \brief Constructs a ListViewSelectedItem
	///
	/// \param item = List View Item
	ListViewSelectedItem(const ListViewItem &item);

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Is Item
	///
	/// \return true = item
	bool is_item() const;

	/// \brief Get Item
	///
	/// \return item
	ListViewItem get_item();

	/// \brief Prev
	///
	/// \return List View Selected Item
	ListViewSelectedItem prev();

	/// \brief Get Next sibling
	///
	/// \return next_sibling
	ListViewSelectedItem get_next_sibling();

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Constructs a ListViewSelectedItem
	///
	/// \param ListViewSelectedItem_Impl = Shared Ptr
	ListViewSelectedItem(const std::shared_ptr<ListViewSelectedItem_Impl> &impl);

	std::shared_ptr<ListViewSelectedItem_Impl> impl;

	friend class ListViewSelection;
/// \}
};

/// \brief ListView selection.
class CL_API_GUI ListViewSelection
{
/// \name Construction
/// \{

public:
	ListViewSelection();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get First
	///
	/// \return first
	ListViewSelectedItem get_first();

	/// \brief Get Last
	///
	/// \return last
	ListViewSelectedItem get_last();

/// \}
/// \name Operations
/// \{

public:

	/// \brief Append
	///
	/// \param item = List View Item
	void append(const ListViewItem &item);

	/// \brief Remove
	///
	/// \param item = List View Item
	void remove(const ListViewItem &item);

	/// \brief Clear
	void clear();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<ListViewSelection_Impl> impl;
/// \}
};

}

/// \}

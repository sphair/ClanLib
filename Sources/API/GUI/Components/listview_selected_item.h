/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

class CL_ListViewSelectedItem_Impl;
class CL_ListViewSelection_Impl;

/// \brief ListView selected item.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ListViewSelectedItem
{
/// \name Construction
/// \{

public:
	/// \brief Contruct a null instance
	CL_ListViewSelectedItem();

	/// \brief Constructs a ListViewSelectedItem
	///
	/// \param item = List View Item
	CL_ListViewSelectedItem(const CL_ListViewItem &item);

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Is Item
	///
	/// \return true = item
	bool is_item() const;

	/// \brief Get Item
	///
	/// \return item
	CL_ListViewItem get_item();

	/// \brief Prev
	///
	/// \return List View Selected Item
	CL_ListViewSelectedItem prev();

	/// \brief Get Next sibling
	///
	/// \return next_sibling
	CL_ListViewSelectedItem get_next_sibling();

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
	/// \param CL_ListViewSelectedItem_Impl = Shared Ptr
	CL_ListViewSelectedItem(const CL_SharedPtr<CL_ListViewSelectedItem_Impl> &impl);

	CL_SharedPtr<CL_ListViewSelectedItem_Impl> impl;

	friend class CL_ListViewSelection;
/// \}
};

/// \brief ListView selection.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ListViewSelection
{
/// \name Construction
/// \{

public:
	CL_ListViewSelection();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get First
	///
	/// \return first
	CL_ListViewSelectedItem get_first();

	/// \brief Get Last
	///
	/// \return last
	CL_ListViewSelectedItem get_last();

/// \}
/// \name Operations
/// \{

public:

	/// \brief Append
	///
	/// \param item = List View Item
	void append(const CL_ListViewItem &item);

	/// \brief Remove
	///
	/// \param item = List View Item
	void remove(const CL_ListViewItem &item);

	/// \brief Clear
	void clear();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ListViewSelection_Impl> impl;
/// \}
};

/// \}

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
	CL_ListViewSelectedItem();

	CL_ListViewSelectedItem(const CL_ListViewItem &item);


/// \}
/// \name Attributes
/// \{

public:
	bool is_null() const;

	bool is_item() const;

	CL_ListViewItem get_item();

	CL_ListViewSelectedItem prev();

	CL_ListViewSelectedItem get_next_sibling();


/// \}
/// \name Operations
/// \{

public:


/// \}
/// \name Implementation
/// \{

private:
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
	CL_ListViewSelectedItem get_first();

	CL_ListViewSelectedItem get_last();


/// \}
/// \name Operations
/// \{

public:
	void append(const CL_ListViewItem &item);

	void remove(const CL_ListViewItem &item);

	void clear();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ListViewSelection_Impl> impl;
/// \}
};


/// \}

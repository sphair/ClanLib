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

class CL_ListViewColumnHeader_Impl;
class CL_GUIThemePart;

/// \brief ListView column header class.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ListViewColumnHeader
{
/// \name Construction
/// \{

public:
	CL_ListViewColumnHeader();

	~CL_ListViewColumnHeader();


/// \}
/// \name Attributes
/// \{

public:
	enum TextAlignment
	{
		align_left,
		align_right,
		align_center
	};

	bool is_null() const;

	CL_StringRef get_column_id() const;

	CL_StringRef get_caption() const;

	int get_width();

	CL_ListViewColumnHeader::TextAlignment get_alignment();

	CL_ListViewColumnHeader get_next_sibling();

	CL_ListViewColumnHeader get_prev_sibling();


/// \}
/// \name Operations
/// \{

public:
	void set_alignment(CL_ListViewColumnHeader::TextAlignment align);

	void set_width(int width);

	void set_caption(const CL_StringRef &str);

	void set_column_id(const CL_StringRef &str);


/// \}
/// \name Events
/// \{

public:
	CL_Callback_v1<CL_ListViewColumnHeader> &func_size_changed();


/// \}
/// \name Implementation
/// \{

private:
	CL_ListViewColumnHeader(const CL_SharedPtr<CL_ListViewColumnHeader_Impl> &impl);

	void set_rect(const CL_Rect &rect);

	void set_gui_theme_part(const CL_GUIThemePart &part);

	CL_SharedPtr<CL_ListViewColumnHeader_Impl> impl;

	friend class CL_ListView;
	friend class CL_ListView_Impl;
	friend class CL_ListViewHeader;
	friend class CL_ListViewHeader_Impl;
/// \}
};


/// \}

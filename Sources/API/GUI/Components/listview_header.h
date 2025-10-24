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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Core/Signals/callback_v1.h"
#include "listview.h"

class CL_ListViewColumnHeader;
class CL_ListViewHeader_Impl;

/// \brief ListViewHeader component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ListViewHeader : public CL_GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a ListViewHeader
	///
	/// \param parent = GUIComponent
	CL_ListViewHeader(CL_GUIComponent *parent);

	~CL_ListViewHeader();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get First column
	///
	/// \return first_column
	CL_ListViewColumnHeader get_first_column();

	/// \brief Get column
	///
	/// \param column_id = String Ref
	///
	/// \return List View Column Header
	CL_ListViewColumnHeader get_column(const CL_StringRef &column_id);

	/// \brief Get Visible
	///
	/// \return visible
	bool get_visible() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	virtual CL_Size get_preferred_size() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Create column
	///
	/// \param column_id = String Ref
	/// \param caption = String Ref
	///
	/// \return List View Column Header
	CL_ListViewColumnHeader create_column(const CL_StringRef &column_id, const CL_StringRef &caption);

	/// \brief Append
	///
	/// \param column = List View Column Header
	///
	/// \return List View Column Header
	CL_ListViewColumnHeader append(CL_ListViewColumnHeader column);

	/// \brief Remove
	///
	/// \param column_id = String Ref
	///
	/// \return List View Column Header
	CL_ListViewColumnHeader remove(const CL_StringRef &column_id);

	/// \brief Insert after
	///
	/// \param column_id = String Ref
	/// \param new_column = List View Column Header
	///
	/// \return List View Column Header
	CL_ListViewColumnHeader insert_after(const CL_StringRef &column_id, CL_ListViewColumnHeader new_column);

	/// \brief Insert before
	///
	/// \param column_id = String Ref
	/// \param new_column = List View Column Header
	///
	/// \return List View Column Header
	CL_ListViewColumnHeader insert_before(const CL_StringRef &column_id, CL_ListViewColumnHeader new_column);

	/// \brief Set visible
	///
	/// \param visible = bool
	void set_visible(bool visible);

	/// \brief Set display mode
	///
	/// \param mode = List View Display Mode
	void set_display_mode(CL_ListViewDisplayMode mode);

/// \}
/// \name Events
/// \{

public:
	CL_Callback_v1<CL_ListViewColumnHeader> &func_column_added();

	CL_Callback_v1<CL_ListViewColumnHeader> &func_column_removed();

	CL_Callback_v1<CL_ListViewColumnHeader> &func_column_size_changed();

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Constructs a ListViewHeader
	///
	/// \param geometry = Rect
	/// \param parent = GUIComponent
	CL_ListViewHeader(const CL_Rect &geometry, CL_GUIComponent *parent);

	/// \brief Update geometry
	///
	/// \param parent_content_rect = Rect
	void update_geometry(const CL_Rect &parent_content_rect);

	CL_SharedPtr<CL_ListViewHeader_Impl> impl;

	friend class CL_ListView_Impl;
/// \}
};

/// \}

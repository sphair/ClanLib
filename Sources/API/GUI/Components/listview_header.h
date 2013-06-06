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
#include "../../Core/Signals/callback_v0.h"
#include "../../Core/Signals/callback_v1.h"
#include "listview.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class ListViewColumnHeader;
class ListViewHeader_Impl;

/// \brief ListViewHeader component.
class CL_API_GUI ListViewHeader : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a ListViewHeader
	///
	/// \param parent = GUIComponent
	ListViewHeader(GUIComponent *parent);

	~ListViewHeader();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get First column
	///
	/// \return first_column
	ListViewColumnHeader get_first_column();

	/// \brief Get column
	///
	/// \param column_id = String Ref
	///
	/// \return List View Column Header
	ListViewColumnHeader get_column(const std::string &column_id);

	/// \brief Get Visible
	///
	/// \return visible
	bool get_visible() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	virtual Size get_css_size() const;

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
	ListViewColumnHeader create_column(const std::string &column_id, const std::string &caption);

	/// \brief Append
	///
	/// \param column = List View Column Header
	///
	/// \return List View Column Header
	ListViewColumnHeader append(ListViewColumnHeader column);

	/// \brief Remove
	///
	/// \param column_id = String Ref
	///
	/// \return List View Column Header
	ListViewColumnHeader remove(const std::string &column_id);

	/// \brief Insert after
	///
	/// \param column_id = String Ref
	/// \param new_column = List View Column Header
	///
	/// \return List View Column Header
	ListViewColumnHeader insert_after(const std::string &column_id, ListViewColumnHeader new_column);

	/// \brief Insert before
	///
	/// \param column_id = String Ref
	/// \param new_column = List View Column Header
	///
	/// \return List View Column Header
	ListViewColumnHeader insert_before(const std::string &column_id, ListViewColumnHeader new_column);

	/// \brief Set display mode
	///
	/// \param mode = List View Display Mode
	void set_display_mode(ListViewDisplayMode mode);

/// \}
/// \name Events
/// \{

public:
	Callback_v1<ListViewColumnHeader> &func_column_added();

	Callback_v1<ListViewColumnHeader> &func_column_removed();

	Callback_v1<ListViewColumnHeader> &func_column_size_changed();

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Constructs a ListViewHeader
	///
	/// \param geometry = Rect
	/// \param parent = GUIComponent
	ListViewHeader(const Rect &geometry, GUIComponent *parent);

	/// \brief Update geometry
	///
	/// \param parent_content_rect = Rect
	void update_geometry(const Rect &parent_content_rect);

	std::shared_ptr<ListViewHeader_Impl> impl;

	friend class ListView_Impl;
/// \}
};

}

/// \}

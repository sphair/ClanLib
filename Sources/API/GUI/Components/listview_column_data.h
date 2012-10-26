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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"

namespace clan
{

class ListViewColumnData_Impl;

/// \brief ListView column data.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI ListViewColumnData
{
/// \name Construction
/// \{

public:
	ListViewColumnData();

	~ListViewColumnData();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Text
	///
	/// \return text
	std::string get_text() const;

	/// \brief Get Column id
	///
	/// \return column_id
	std::string get_column_id() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const std::string &text);

	/// \brief Set column id
	///
	/// \param column_id = String Ref
	void set_column_id(const std::string &column_id);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<ListViewColumnData_Impl> impl;
/// \}
};

}

/// \}

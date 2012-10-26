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
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include <memory>
#include "gui_message.h"

namespace clan
{

/// \brief GUI focus change notification message.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI GUIMessage_FocusChange : public GUIMessage
{
/// \name Construction
/// \{

public:
	/// \brief Constructs an input GUI message.
	GUIMessage_FocusChange();

	/// \brief Constructs a GUIMessage FocusChange
	///
	/// \param message = GUIMessage
	GUIMessage_FocusChange(const GUIMessage &message);

	~GUIMessage_FocusChange();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Type name
	///
	/// \return type_name
	static std::string get_type_name();

	enum FocusType
	{
		losing_focus,
		gained_focus
	};

	/// \brief Retrieves the focus type of this message.
	FocusType get_focus_type() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the focus type for this Message.
	void set_focus_type(FocusType focus_type);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}

/// \}

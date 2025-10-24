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

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"
#include "gui_message.h"

class CL_Size;
class CL_Rect;

/// \brief GUI resize message.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIMessage_Resize : public CL_GUIMessage
{
/// \name Construction
/// \{

public:
	/// \brief Constructs an input GUI message.
	CL_GUIMessage_Resize();

	/// \brief Constructs a GUIMessage Resize
	///
	/// \param message = GUIMessage
	CL_GUIMessage_Resize(const CL_GUIMessage &message);

	~CL_GUIMessage_Resize();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Type name
	///
	/// \return type_name
	static CL_StringRef get_type_name();

	/// \brief Retrieves the geometry contained in this Message.
	CL_Rect get_geometry() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the geometry contained in this Message.
	void set_geometry(const CL_Rect &);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}

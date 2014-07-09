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
*/


#pragma once

#include <memory>
#include "gui_message.h"

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

/// \brief GUI mouse pointer change notification message.
class GUIMessage_Pointer : public GUIMessage
{
/// \name Construction
/// \{
public:
	enum PointerType
	{
		pointer_enter,
		pointer_leave
	};

	/// \brief Constructs an input GUI message.
	GUIMessage_Pointer() : pointer_type(pointer_enter) { }
	GUIMessage_Pointer(PointerType type) : pointer_type(type) { }
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Retrieves the pointer type of this message.
	PointerType pointer_type;
/// \}
};

}

/// \}

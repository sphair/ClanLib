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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_message_close.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_CloseData Class:

class CL_GUIMessage_CloseData
{
public:
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Close Construction:

CL_GUIMessage_Close::CL_GUIMessage_Close()
{
	set_type("window close");
	set_data("window close", CL_SharedPtr<CL_GUIMessage_CloseData>(
		new CL_GUIMessage_CloseData));
}

CL_GUIMessage_Close::CL_GUIMessage_Close(const CL_GUIMessage &message)
: CL_GUIMessage(message)
{
}

CL_GUIMessage_Close::~CL_GUIMessage_Close()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Close Attributes:

CL_StringRef CL_GUIMessage_Close::get_type_name()
{
	return "window close";
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_Close Operations:

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_Close Implementation:

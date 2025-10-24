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

#include "GUI/precomp.h"
#include "API/GUI/gui_message_input.h"
#include "API/Display/Window/input_event.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_InputData Class:

class CL_GUIMessage_InputData
{
public:
	CL_InputEvent input_event;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Input Construction:

CL_GUIMessage_Input::CL_GUIMessage_Input()
{
	set_type("input");
	set_data("input", CL_SharedPtr<CL_GUIMessage_InputData>(
		new CL_GUIMessage_InputData));
}

CL_GUIMessage_Input::CL_GUIMessage_Input(const CL_GUIMessage &message)
: CL_GUIMessage(message)
{
}

CL_GUIMessage_Input::~CL_GUIMessage_Input()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Input Attributes:

CL_StringRef CL_GUIMessage_Input::get_type_name()
{
	return "input";
}

CL_InputEvent CL_GUIMessage_Input::get_event() const
{
	CL_SharedPtr<CL_GUIMessage_InputData> d = static_cast<CL_SharedPtr<CL_GUIMessage_InputData> >(get_data("input"));
	if (d.is_null())
		return CL_InputEvent();
	return d->input_event;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_Input Operations:

void CL_GUIMessage_Input::set_event(const CL_InputEvent &event)
{
	CL_SharedPtr<CL_GUIMessage_InputData> d = static_cast<CL_SharedPtr<CL_GUIMessage_InputData> >(get_data("input"));
	if (d.is_null())
	{
		d = CL_SharedPtr<CL_GUIMessage_InputData>(
			new CL_GUIMessage_InputData);
		set_data("input", d);
	}
	d->input_event = event;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_Input Implementation:

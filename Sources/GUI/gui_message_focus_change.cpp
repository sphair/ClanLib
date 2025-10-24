/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/GUI/gui_message_focus_change.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_FocusChangeData Class:

class CL_GUIMessage_FocusChangeData
{
public:
	CL_GUIMessage_FocusChange::FocusType focus_type;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_FocusChange Construction:

CL_GUIMessage_FocusChange::CL_GUIMessage_FocusChange()
{
	set_type("focus change");
	set_data("focus change", CL_SharedPtr<CL_GUIMessage_FocusChangeData>(
		new CL_GUIMessage_FocusChangeData));
}

CL_GUIMessage_FocusChange::CL_GUIMessage_FocusChange(const CL_GUIMessage &message)
: CL_GUIMessage(message)
{
}

CL_GUIMessage_FocusChange::~CL_GUIMessage_FocusChange()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_FocusChange Attributes:

CL_StringRef CL_GUIMessage_FocusChange::get_type_name()
{
	return "focus change";
}

CL_GUIMessage_FocusChange::FocusType CL_GUIMessage_FocusChange::get_focus_type() const
{
	CL_SharedPtr<CL_GUIMessage_FocusChangeData> d = static_cast<CL_SharedPtr<CL_GUIMessage_FocusChangeData> >(get_data("focus change"));
	if (d.is_null())
		return losing_focus;
	return d->focus_type;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_FocusChange Operations:

void CL_GUIMessage_FocusChange::set_focus_type(FocusType focus_type)
{
	CL_SharedPtr<CL_GUIMessage_FocusChangeData> d = static_cast<CL_SharedPtr<CL_GUIMessage_FocusChangeData> >(get_data("focus change"));
	if (d.is_null())
	{
		d = CL_SharedPtr<CL_GUIMessage_FocusChangeData>(
			new CL_GUIMessage_FocusChangeData);
		set_data("focus change", d);
	}
	d->focus_type = focus_type;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_FocusChange Implementation:

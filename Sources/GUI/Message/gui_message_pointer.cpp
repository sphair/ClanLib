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
#include "API/GUI/gui_message_pointer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_PointerData Class:

class CL_GUIMessage_PointerData : public CL_GUIMessageData
{
public:
	CL_GUIMessage_Pointer::PointerType pointer_type;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Pointer Construction:

CL_GUIMessage_Pointer::CL_GUIMessage_Pointer()
{
	set_type("pointer");
	set_data("pointer", CL_SharedPtr<CL_GUIMessage_PointerData>(new CL_GUIMessage_PointerData));
}

CL_GUIMessage_Pointer::CL_GUIMessage_Pointer(const CL_GUIMessage &message)
: CL_GUIMessage(message)
{
}

CL_GUIMessage_Pointer::~CL_GUIMessage_Pointer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Pointer Attributes:

CL_StringRef CL_GUIMessage_Pointer::get_type_name()
{
	return "pointer";
}

CL_GUIMessage_Pointer::PointerType CL_GUIMessage_Pointer::get_pointer_type() const
{
	CL_SharedPtr<CL_GUIMessage_PointerData> d = cl_dynamic_pointer_cast<CL_GUIMessage_PointerData>(get_data("pointer"));
	if (!d)
		return pointer_enter;
	return d->pointer_type;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_Pointer Operations:

void CL_GUIMessage_Pointer::set_pointer_type(PointerType pointer_type)
{
	CL_SharedPtr<CL_GUIMessage_PointerData> d = cl_dynamic_pointer_cast<CL_GUIMessage_PointerData>(get_data("pointer"));
	if (!d)
	{
		d = CL_SharedPtr<CL_GUIMessage_PointerData>(
			new CL_GUIMessage_PointerData);
		set_data("pointer", d);
	}
	d->pointer_type = pointer_type;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_Pointer Implementation:

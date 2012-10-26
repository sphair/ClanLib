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

#include "GUI/precomp.h"
#include "API/GUI/gui_message_pointer.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_PointerData Class:

class GUIMessage_PointerData : public GUIMessageData
{
public:
	GUIMessage_Pointer::PointerType pointer_type;
};

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_Pointer Construction:

GUIMessage_Pointer::GUIMessage_Pointer()
{
	set_type("pointer");
	set_data("pointer", std::shared_ptr<GUIMessage_PointerData>(new GUIMessage_PointerData));
}

GUIMessage_Pointer::GUIMessage_Pointer(const GUIMessage &message)
: GUIMessage(message)
{
}

GUIMessage_Pointer::~GUIMessage_Pointer()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_Pointer Attributes:

std::string GUIMessage_Pointer::get_type_name()
{
	return "pointer";
}

GUIMessage_Pointer::PointerType GUIMessage_Pointer::get_pointer_type() const
{
	std::shared_ptr<GUIMessage_PointerData> d = std::dynamic_pointer_cast<GUIMessage_PointerData>(get_data("pointer"));
	if (!d)
		return pointer_enter;
	return d->pointer_type;
}

/////////////////////////////////////////////////////////////////////////////	
// GUIMessage_Pointer Operations:

void GUIMessage_Pointer::set_pointer_type(PointerType pointer_type)
{
	std::shared_ptr<GUIMessage_PointerData> d = std::dynamic_pointer_cast<GUIMessage_PointerData>(get_data("pointer"));
	if (!d)
	{
		d = std::shared_ptr<GUIMessage_PointerData>(
			new GUIMessage_PointerData);
		set_data("pointer", d);
	}
	d->pointer_type = pointer_type;
}

/////////////////////////////////////////////////////////////////////////////	
// GUIMessage_Pointer Implementation:

}

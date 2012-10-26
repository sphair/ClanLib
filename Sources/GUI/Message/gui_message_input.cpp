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

#include "GUI/precomp.h"
#include "API/GUI/gui_message_input.h"
#include "API/Display/Window/input_event.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_InputData Class:

class GUIMessage_InputData : public GUIMessageData
{
public:
	InputEvent input_event;
};

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_Input Construction:

GUIMessage_Input::GUIMessage_Input()
{
	set_type("input");
	set_data("input", std::shared_ptr<GUIMessage_InputData>(new GUIMessage_InputData));
}

GUIMessage_Input::GUIMessage_Input(const GUIMessage &message)
: GUIMessage(message)
{
}

GUIMessage_Input::~GUIMessage_Input()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_Input Attributes:

std::string GUIMessage_Input::get_type_name()
{
	return "input";
}

InputEvent GUIMessage_Input::get_event() const
{
	std::shared_ptr<GUIMessage_InputData> d = std::dynamic_pointer_cast<GUIMessage_InputData>(get_data("input"));
	if (!d)
		return InputEvent();
	return d->input_event;
}

/////////////////////////////////////////////////////////////////////////////	
// GUIMessage_Input Operations:

void GUIMessage_Input::set_event(const InputEvent &event)
{
	std::shared_ptr<GUIMessage_InputData> d = std::dynamic_pointer_cast<GUIMessage_InputData>(get_data("input"));
	if (!d)
	{
		d = std::shared_ptr<GUIMessage_InputData>(new GUIMessage_InputData);
		set_data("input", d);
	}
	d->input_event = event;
}

/////////////////////////////////////////////////////////////////////////////	
// GUIMessage_Input Implementation:

}

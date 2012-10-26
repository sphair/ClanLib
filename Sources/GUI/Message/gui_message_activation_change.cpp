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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_message_activation_change.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_ActivationChangeData Class:

class GUIMessage_ActivationChangeData : public GUIMessageData
{
public:
	GUIMessage_ActivationChange::ActivationType activation_type;
};

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_ActivationChange Construction:

GUIMessage_ActivationChange::GUIMessage_ActivationChange()
{
	set_type("activation change");
	set_data("activation change", std::shared_ptr<GUIMessage_ActivationChangeData>(
		new GUIMessage_ActivationChangeData));
}

GUIMessage_ActivationChange::GUIMessage_ActivationChange(const GUIMessage &message)
: GUIMessage(message)
{
}

GUIMessage_ActivationChange::~GUIMessage_ActivationChange()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_ActivationChange Attributes:

std::string GUIMessage_ActivationChange::get_type_name()
{
	return "activation change";
}

GUIMessage_ActivationChange::ActivationType GUIMessage_ActivationChange::get_activation_type() const
{
	std::shared_ptr<GUIMessage_ActivationChangeData> d = std::dynamic_pointer_cast<GUIMessage_ActivationChangeData>(get_data("activation change"));
	if (!d)
		return activation_lost;
	return d->activation_type;
}

/////////////////////////////////////////////////////////////////////////////	
// GUIMessage_ActivationChange Operations:

void GUIMessage_ActivationChange::set_activation_type(ActivationType activation_type)
{
	std::shared_ptr<GUIMessage_ActivationChangeData> d = std::dynamic_pointer_cast<GUIMessage_ActivationChangeData>(get_data("activation change"));
	if (!d)
	{
		d = std::shared_ptr<GUIMessage_ActivationChangeData>(
			new GUIMessage_ActivationChangeData);
		set_data("activation change", d);
	}
	d->activation_type = activation_type;
}

/////////////////////////////////////////////////////////////////////////////	
// GUIMessage_ActivationChange Implementation:

}

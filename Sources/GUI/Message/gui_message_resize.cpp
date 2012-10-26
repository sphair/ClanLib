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
#include "API/GUI/gui_message_resize.h"
#include "API/Core/Math/rect.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_ResizeData Class:

class GUIMessage_ResizeData : public GUIMessageData
{
public:
	Rect rect;
};

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_Resize Construction:

GUIMessage_Resize::GUIMessage_Resize()
{
	set_type("resize");
	set_data("resize", std::shared_ptr<GUIMessage_ResizeData>(new GUIMessage_ResizeData));
}

GUIMessage_Resize::GUIMessage_Resize(const GUIMessage &message)
: GUIMessage(message)
{
}

GUIMessage_Resize::~GUIMessage_Resize()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_Resize Attributes:

std::string GUIMessage_Resize::get_type_name()
{
	return "resize";
}

Rect GUIMessage_Resize::get_geometry() const
{
	std::shared_ptr<GUIMessage_ResizeData> d = std::dynamic_pointer_cast<GUIMessage_ResizeData>(get_data("resize"));
	if (!d)
		return Rect();
	return d->rect;
}

/////////////////////////////////////////////////////////////////////////////	
// GUIMessage_Resize Operations:

void GUIMessage_Resize::set_geometry(const Rect &rect)
{
	std::shared_ptr<GUIMessage_ResizeData> d = std::dynamic_pointer_cast<GUIMessage_ResizeData>(get_data("resize"));
	if (!d)
	{
		d = std::shared_ptr<GUIMessage_ResizeData>(new GUIMessage_ResizeData);
		set_data("resize", d);
	}
	d->rect = rect;
}

/////////////////////////////////////////////////////////////////////////////	
// GUIMessage_Resize Implementation:

}

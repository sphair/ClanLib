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

#include "GUI/precomp.h"
#include "API/GUI/gui_message_resize.h"
#include "API/Core/Math/rect.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_ResizeData Class:

class CL_GUIMessage_ResizeData
{
public:
	CL_Rect rect;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Resize Construction:

CL_GUIMessage_Resize::CL_GUIMessage_Resize()
{
	set_type("resize");
	set_data("resize", CL_SharedPtr<CL_GUIMessage_ResizeData>(
		new CL_GUIMessage_ResizeData));
}

CL_GUIMessage_Resize::CL_GUIMessage_Resize(const CL_GUIMessage &message)
: CL_GUIMessage(message)
{
}

CL_GUIMessage_Resize::~CL_GUIMessage_Resize()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Resize Attributes:

CL_StringRef CL_GUIMessage_Resize::get_type_name()
{
	return "resize";
}

CL_Rect CL_GUIMessage_Resize::get_geometry() const
{
	CL_SharedPtr<CL_GUIMessage_ResizeData> d = static_cast<CL_SharedPtr<CL_GUIMessage_ResizeData> >(get_data("resize"));
	if (d.is_null())
		return CL_Rect();
	return d->rect;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_Resize Operations:

void CL_GUIMessage_Resize::set_geometry(const CL_Rect &rect)
{
	CL_SharedPtr<CL_GUIMessage_ResizeData> d = static_cast<CL_SharedPtr<CL_GUIMessage_ResizeData> >(get_data("resize"));
	if (d.is_null())
	{
		d = CL_SharedPtr<CL_GUIMessage_ResizeData>(
			new CL_GUIMessage_ResizeData);
		set_data("resize", d);
	}
	d->rect = rect;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_GUIMessage_Resize Implementation:

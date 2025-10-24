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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_message.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage_Impl Class:

class CL_GUIMessage_Impl
{
//! Construction:
public:
	CL_GUIMessage_Impl()
	: consumed(false), target(0)
	{
	}

//! Attributes:
public:
	bool consumed;

	CL_String type;

	CL_GUIComponent *target;

	std::map<CL_String, CL_SharedPtr<CL_GUIMessageData> > data_objects;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage Construction:

CL_GUIMessage::CL_GUIMessage()
: impl(new CL_GUIMessage_Impl)
{
}

CL_GUIMessage::~CL_GUIMessage()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage Attributes:

CL_StringRef CL_GUIMessage::get_type() const
{
	return impl->type;
}

bool CL_GUIMessage::is_type(const CL_StringRef &type) const
{
	return impl->type == type;
}

CL_SharedPtr<CL_GUIMessageData> CL_GUIMessage::get_data(const CL_StringRef &data_name) const
{
	std::map<CL_String, CL_SharedPtr<CL_GUIMessageData> >::const_iterator it = impl->data_objects.find(data_name);
	if (it != impl->data_objects.end())
		return it->second;
	else
		return CL_SharedPtr<CL_GUIMessageData>();
}

CL_GUIComponent *CL_GUIMessage::get_target() const
{
	return impl->target;
}

bool CL_GUIMessage::is_consumed() const
{
	return impl->consumed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage Operations:

void CL_GUIMessage::set_type(const CL_StringRef &type)
{
	impl->type = type;
}

void CL_GUIMessage::set_data(const CL_StringRef &data_name, const CL_SharedPtr<CL_GUIMessageData> &ptr)
{
	impl->data_objects[data_name] = ptr;
}

void CL_GUIMessage::set_target(CL_GUIComponent *target)
{
	impl->target = target;
}

void CL_GUIMessage::set_null()
{
	impl = CL_SharedPtr<CL_GUIMessage_Impl>();
}

void CL_GUIMessage::set_consumed()
{
	impl->consumed = true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIMessage Implementation:

/*
class CL_GUIMessage_Input
{
public:
	CL_InputEvent input;
};

class CL_GUIMessage_SetFocus
{
public:
};

class CL_GUIMessage_KillFocus
{
public:
};

class CL_GUIMessage_Paint
{
public:
	CL_Rect rect;
};

class CL_GUIMessage_Quit
{
public:
	int exit_code;
};

class CL_GUIMessage_DisplayChange
{
public:
	int width;

	int height;

	int depth;
};

class CL_GUIMessage_ActivateApp
{
public:
	bool activated;
};

class CL_GUIMessage_CancelMode
{
public:
};

class CL_GUIMessage_Close
{
public:
};

class CL_GUIMessage_Enable
{
public:
	bool enabled;
};

class CL_GUIMessage_EnterSizeMove
{
public:
};

class CL_GUIMessage_ExitSizeMove
{
public:
};

class CL_GUIMessage_Move
{
public:
};

class CL_GUIMessage_Moving
{
public:
	CL_Rect geometry;
};

class CL_GUIMessage_ShowWindow
{
public:
	bool visible;
	bool being_uncovered;
	bool being_covered;
};

class CL_GUIMessage_Size
{
public:
	int width;

	int height;
};

class CL_GUIMessage_Sizing
{
public:
	enum Edge
	{
		edge_bottom,
		edge_bottom_left,
		edge_bottom_right,
		edge_left,
		edge_right,
		edge_top,
		edge_top_left,
		edge_top_right
	};

	Edge edge;

	Rect dragRect;
};
*/

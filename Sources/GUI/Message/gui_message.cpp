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
#include "API/GUI/gui_message.h"
#include <map>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIMessage_Impl Class:

class GUIMessage_Impl
{
//! Construction:
public:
	GUIMessage_Impl()
	: consumed(false), target(0)
	{
	}

//! Attributes:
public:
	bool consumed;

	std::string type;

	GUIComponent *target;

	std::map<std::string, std::shared_ptr<GUIMessageData> > data_objects;
};

/////////////////////////////////////////////////////////////////////////////
// GUIMessage Construction:

GUIMessage::GUIMessage()
: impl(new GUIMessage_Impl)
{
}

GUIMessage::~GUIMessage()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIMessage Attributes:

std::string GUIMessage::get_type() const
{
	return impl->type;
}

bool GUIMessage::is_type(const std::string &type) const
{
	return impl->type == type;
}

std::shared_ptr<GUIMessageData> GUIMessage::get_data(const std::string &data_name) const
{
	std::map<std::string, std::shared_ptr<GUIMessageData> >::const_iterator it = impl->data_objects.find(data_name);
	if (it != impl->data_objects.end())
		return it->second;
	else
		return std::shared_ptr<GUIMessageData>();
}

GUIComponent *GUIMessage::get_target() const
{
	return impl->target;
}

bool GUIMessage::is_consumed() const
{
	return impl->consumed;
}

/////////////////////////////////////////////////////////////////////////////
// GUIMessage Operations:

void GUIMessage::set_type(const std::string &type)
{
	impl->type = type;
}

void GUIMessage::set_data(const std::string &data_name, const std::shared_ptr<GUIMessageData> &ptr)
{
	impl->data_objects[data_name] = ptr;
}

void GUIMessage::set_target(GUIComponent *target)
{
	impl->target = target;
}

void GUIMessage::set_null()
{
	impl = std::shared_ptr<GUIMessage_Impl>();
}

void GUIMessage::set_consumed()
{
	impl->consumed = true;
}

/////////////////////////////////////////////////////////////////////////////
// GUIMessage Implementation:

/*
class GUIMessage_Input
{
public:
	InputEvent input;
};

class GUIMessage_SetFocus
{
public:
};

class GUIMessage_KillFocus
{
public:
};

class GUIMessage_Paint
{
public:
	Rect rect;
};

class GUIMessage_Quit
{
public:
	int exit_code;
};

class GUIMessage_DisplayChange
{
public:
	int width;

	int height;

	int depth;
};

class GUIMessage_ActivateApp
{
public:
	bool activated;
};

class GUIMessage_CancelMode
{
public:
};

class GUIMessage_Close
{
public:
};

class GUIMessage_Enable
{
public:
	bool enabled;
};

class GUIMessage_EnterSizeMove
{
public:
};

class GUIMessage_ExitSizeMove
{
public:
};

class GUIMessage_Move
{
public:
};

class GUIMessage_Moving
{
public:
	Rect geometry;
};

class GUIMessage_ShowWindow
{
public:
	bool visible;
	bool being_uncovered;
	bool being_covered;
};

class GUIMessage_Size
{
public:
	int width;

	int height;
};

class GUIMessage_Sizing
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

}

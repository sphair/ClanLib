/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "Network/precomp.h"
#include "API/Network/NetGame/event.h"
#include "API/Core/Text/string_format.h"

CL_NetGameEvent::CL_NetGameEvent(const CL_String &name)
: name(name)
{
}

CL_NetGameEvent::CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1)
: name(name)
{
	add_argument(arg1);
}

CL_NetGameEvent::CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2)
: name(name)
{
	add_argument(arg1);
	add_argument(arg2);
}

CL_NetGameEvent::CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3)
: name(name)
{
	add_argument(arg1);
	add_argument(arg2);
	add_argument(arg3);
}

CL_NetGameEvent::CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3, const CL_NetGameEventValue &arg4)
: name(name)
{
	add_argument(arg1);
	add_argument(arg2);
	add_argument(arg3);
	add_argument(arg4);
}

CL_NetGameEvent::CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3, const CL_NetGameEventValue &arg4, const CL_NetGameEventValue &arg5)
: name(name)
{
	add_argument(arg1);
	add_argument(arg2);
	add_argument(arg3);
	add_argument(arg4);
	add_argument(arg5);
}

unsigned int CL_NetGameEvent::get_argument_count() const
{
	return arguments.size();
}

CL_NetGameEventValue CL_NetGameEvent::get_argument(unsigned int index) const
{
	if (index >= arguments.size())
		throw CL_Exception(cl_format("Arguments out of bounds for game event %1", name));
	return arguments[index];
}

void CL_NetGameEvent::add_argument(const CL_NetGameEventValue &value)
{
	arguments.push_back(value);
}

CL_String CL_NetGameEvent::to_string() const
{
	CL_String event_info = cl_format("%1(", name);

	for(unsigned int i = 0; i < arguments.size(); ++i)
	{
		if(arguments[i].is_string())
			event_info += "\"" + arguments[i].get_value() + "\"";
		else
			event_info += arguments[i].get_value();

		if(i < arguments.size() - 1)
			event_info += ",";
	}

	event_info += ")";

	return event_info;
}

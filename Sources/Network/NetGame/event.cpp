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
**    Magnus Norddahl
*/

#include "Network/precomp.h"
#include "API/Network/NetGame/event.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"

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
		if(i > 0)
			event_info += ",";

		event_info += to_string(arguments[i]);
	}

	event_info += ")";

	return event_info;
}

CL_String CL_NetGameEvent::to_string(const CL_NetGameEventValue &v) const
{
	switch (v.get_type())
	{
	case CL_NetGameEventValue::null:
		return "null";
	case CL_NetGameEventValue::integer:
		return CL_StringHelp::int_to_text(v.to_integer());
	case CL_NetGameEventValue::uinteger:
		return CL_StringHelp::uint_to_text(v.to_uinteger());
	case CL_NetGameEventValue::string:
		return "\"" + v.to_string() + "\"";
	case CL_NetGameEventValue::boolean:
		return v.to_boolean() ? "true" : "false";
	case CL_NetGameEventValue::number:
		return CL_StringHelp::float_to_text(v.to_number());
	case CL_NetGameEventValue::complex:
		{
			CL_String str;
			str += "[";
			for (unsigned int j = 0; j < v.get_member_count(); j++)
			{
				if(j > 0)
					str += ",";
				str += to_string(v.get_member(j));
			}
			str += "]";
			return str;
		}
	default:
		return "??" + CL_StringHelp::int_to_text(v.get_type());
	}
}

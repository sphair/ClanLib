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

#include "Network/precomp.h"
#include "API/Network/NetGame/event.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"

namespace clan
{

NetGameEvent::NetGameEvent(const std::string &name)
: name(name)
{
}

NetGameEvent::NetGameEvent(const std::string &name, const NetGameEventValue &arg1)
: name(name)
{
	add_argument(arg1);
}

NetGameEvent::NetGameEvent(const std::string &name, const NetGameEventValue &arg1, const NetGameEventValue &arg2)
: name(name)
{
	add_argument(arg1);
	add_argument(arg2);
}

NetGameEvent::NetGameEvent(const std::string &name, const NetGameEventValue &arg1, const NetGameEventValue &arg2, const NetGameEventValue &arg3)
: name(name)
{
	add_argument(arg1);
	add_argument(arg2);
	add_argument(arg3);
}

NetGameEvent::NetGameEvent(const std::string &name, const NetGameEventValue &arg1, const NetGameEventValue &arg2, const NetGameEventValue &arg3, const NetGameEventValue &arg4)
: name(name)
{
	add_argument(arg1);
	add_argument(arg2);
	add_argument(arg3);
	add_argument(arg4);
}

NetGameEvent::NetGameEvent(const std::string &name, const NetGameEventValue &arg1, const NetGameEventValue &arg2, const NetGameEventValue &arg3, const NetGameEventValue &arg4, const NetGameEventValue &arg5)
: name(name)
{
	add_argument(arg1);
	add_argument(arg2);
	add_argument(arg3);
	add_argument(arg4);
	add_argument(arg5);
}

unsigned int NetGameEvent::get_argument_count() const
{
	return arguments.size();
}

NetGameEventValue NetGameEvent::get_argument(unsigned int index) const
{
	if (index >= arguments.size())
		throw Exception(string_format("Arguments out of bounds for game event %1", name));
	return arguments[index];
}

void NetGameEvent::add_argument(const NetGameEventValue &value)
{
	arguments.push_back(value);
}

std::string NetGameEvent::to_string() const
{
	std::string event_info = string_format("%1(", name);

	for(unsigned int i = 0; i < arguments.size(); ++i)
	{
		if(i > 0)
			event_info += ",";

		event_info += to_string(arguments[i]);
	}

	event_info += ")";

	return event_info;
}

std::string NetGameEvent::to_string(const NetGameEventValue &v) const
{
	switch (v.get_type())
	{
	case NetGameEventValue::null:
		return "null";
	case NetGameEventValue::integer:
		return StringHelp::int_to_text(v.to_integer());
	case NetGameEventValue::uinteger:
		return StringHelp::uint_to_text(v.to_uinteger());
	case NetGameEventValue::string:
		return "\"" + v.to_string() + "\"";
	case NetGameEventValue::boolean:
		return v.to_boolean() ? "true" : "false";
	case NetGameEventValue::number:
		return StringHelp::float_to_text(v.to_number());
	case NetGameEventValue::complex:
		{
			std::string str;
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
		return "??" + StringHelp::int_to_text(v.get_type());
	}
}

}

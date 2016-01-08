/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

namespace clan
{
	NetGameEvent::NetGameEvent(const std::string &name, std::vector<NetGameEventValue> arg)
		: name(name)
		, arguments(arg)
	{
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

		for (unsigned int i = 0; i < arguments.size(); ++i)
		{
			if (i > 0)
				event_info += ",";

			event_info += NetGameEventValue::to_string(arguments[i]);
		}

		event_info += ")";

		return event_info;
	}
}

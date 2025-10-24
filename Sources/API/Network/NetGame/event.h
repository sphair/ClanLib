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

/// \addtogroup clanNetwork_NetGame clanNetwork NetGame
/// \{

#pragma once

#include "event_value.h"

/// \brief CL_NetGameEvent
///
/// \xmlonly !group=Network/NetGame! !header=network.h! \endxmlonly
class CL_NetGameEvent
{
public:
	CL_NetGameEvent(const CL_String &name);
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1);
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2);
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3);
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3, const CL_NetGameEventValue &arg4);
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3, const CL_NetGameEventValue &arg4, const CL_NetGameEventValue &arg5);

	CL_String get_name() const { return name; };

	unsigned int get_argument_count() const;
	CL_NetGameEventValue get_argument(unsigned int index) const;

	void add_argument(const CL_NetGameEventValue &value);

	CL_String to_string() const;

private:
	CL_String name;
	std::vector<CL_NetGameEventValue> arguments;
};

/// \}


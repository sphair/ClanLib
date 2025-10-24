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
*/

/// \addtogroup clanNetwork_NetGame clanNetwork NetGame
/// \{

#pragma once

#include "../api_network.h"
#include "event_value.h"

/// \brief CL_NetGameEvent
///
/// \xmlonly !group=Network/NetGame! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetGameEvent
{
public:

	/// \brief Constructs a NetGameEvent
	///
	/// \param name = String
	CL_NetGameEvent(const CL_String &name);

	/// \brief Constructs a NetGameEvent
	///
	/// \param name = String
	/// \param arg1 = Net Game Event Value
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1);

	/// \brief Constructs a NetGameEvent
	///
	/// \param name = String
	/// \param arg1 = Net Game Event Value
	/// \param arg2 = Net Game Event Value
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2);

	/// \brief Constructs a NetGameEvent
	///
	/// \param name = String
	/// \param arg1 = Net Game Event Value
	/// \param arg2 = Net Game Event Value
	/// \param arg3 = Net Game Event Value
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3);

	/// \brief Constructs a NetGameEvent
	///
	/// \param name = String
	/// \param arg1 = Net Game Event Value
	/// \param arg2 = Net Game Event Value
	/// \param arg3 = Net Game Event Value
	/// \param arg4 = Net Game Event Value
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3, const CL_NetGameEventValue &arg4);

	/// \brief Constructs a NetGameEvent
	///
	/// \param name = String
	/// \param arg1 = Net Game Event Value
	/// \param arg2 = Net Game Event Value
	/// \param arg3 = Net Game Event Value
	/// \param arg4 = Net Game Event Value
	/// \param arg5 = Net Game Event Value
	CL_NetGameEvent(const CL_String &name, const CL_NetGameEventValue &arg1, const CL_NetGameEventValue &arg2, const CL_NetGameEventValue &arg3, const CL_NetGameEventValue &arg4, const CL_NetGameEventValue &arg5);

	/// \brief Get Name
	///
	/// \return name
	CL_String get_name() const { return name; };

	unsigned int get_argument_count() const;

	/// \brief Get argument
	///
	/// \param index = value
	///
	/// \return Net Game Event Value
	CL_NetGameEventValue get_argument(unsigned int index) const;

	/// \brief Add argument
	///
	/// \param value = Net Game Event Value
	void add_argument(const CL_NetGameEventValue &value);

	/// \brief To string
	///
	/// \return String
	CL_String to_string() const;

private:

	/// \brief To string
	///
	/// \param v = Net Game Event Value
	///
	/// \return String
	CL_String to_string(const CL_NetGameEventValue &v) const;

	CL_String name;
	std::vector<CL_NetGameEventValue> arguments;
};

/// \}


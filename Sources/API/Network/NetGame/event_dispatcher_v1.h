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

/// \addtogroup clanNetwork_NetGame clanNetwork NetGame
/// \{

#pragma once

#include "../api_network.h"
#include "event.h"
#include <map>
#include "../../Core/Signals/callback_v2.h"

template<typename ContextParam>
/// \brief CL_NetGameEventDispatcher_v1
///
/// \xmlonly !group=Network/NetGame! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetGameEventDispatcher_v1
{
public:
	typedef CL_Callback_v2<const CL_NetGameEvent &, ContextParam> CallbackClass;

	CallbackClass &func_event(const CL_String &name) { return event_handlers[name]; }

	/// \brief Dispatch
	///
	/// \param game_event = Net Game Event
	/// \param context = Context Param
	///
	/// \return bool
	bool dispatch(const CL_NetGameEvent &game_event, ContextParam context);

private:
	std::map<CL_String, CallbackClass> event_handlers;
};

template<typename ContextParam>
bool CL_NetGameEventDispatcher_v1<ContextParam>::dispatch(const CL_NetGameEvent &game_event, ContextParam context)
{
	typename std::map<CL_String, CallbackClass>::iterator it;
	it = event_handlers.find(game_event.get_name());
	if (it != event_handlers.end() && !it->second.is_null())
	{
		it->second.invoke(game_event, context);
		return true;
	}
	else
	{
		return false;
	}
}

/// \}


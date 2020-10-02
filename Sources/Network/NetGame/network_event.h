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

#pragma once

#include "API/Network/NetGame/event.h"

namespace clan
{
	class NetGameConnection;

	class NetGameNetworkEvent
	{
	public:
		enum class Type
		{
			client_connected,
			event_received,
			client_disconnected
		};

		NetGameNetworkEvent(NetGameConnection *connection, Type type)
			: connection(connection), type(type), game_event(std::string())
		{
		}

		NetGameNetworkEvent(NetGameConnection *connection, Type type, const NetGameEvent &game_event)
			: connection(connection), type(type), game_event(game_event)
		{
		}

		NetGameNetworkEvent(NetGameConnection *connection, const NetGameEvent &game_event)
			: connection(connection), type(Type::event_received), game_event(game_event)
		{
		}

		NetGameConnection *connection;
		Type type;
		NetGameEvent game_event;
	};
}

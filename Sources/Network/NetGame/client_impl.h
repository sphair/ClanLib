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

#pragma once

#include "API/Core/System/keep_alive.h"
#include "API/Core/System/uniqueptr.h"

class CL_NetGameClient_Impl : public CL_KeepAliveObject
{
public:
	void process();

	CL_Mutex mutex;
	std::vector<CL_NetGameNetworkEvent> events;

	CL_UniquePtr<CL_NetGameConnection> connection;
	CL_Signal_v1<const CL_NetGameEvent &> sig_game_event_received;
	CL_Signal_v0 sig_game_connected;
	CL_Signal_v0 sig_game_disconnected;
};

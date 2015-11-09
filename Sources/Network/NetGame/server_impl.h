/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "API/Network/Socket/tcp_listen.h"
#include <memory>
#include <mutex>
#include <thread>

namespace clan
{
	class NetGameServer_Impl
	{
	public:
		void process();

		std::unique_ptr<TCPListen> tcp_listen;
		std::thread listen_thread;

		NetworkConditionVariable worker_event;
		std::mutex mutex;
		bool stop_flag = false;
		std::vector<NetGameConnection *> connections;
		std::vector<NetGameNetworkEvent> events;

		Signal<void(NetGameConnection *)> sig_game_client_connected;
		Signal<void(NetGameConnection *, const std::string &)> sig_game_client_disconnected;
		Signal<void(NetGameConnection *, const NetGameEvent &)> sig_game_event_received;
	};
}

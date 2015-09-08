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

#include "Network/precomp.h"
#include "API/Network/NetGame/connection.h"
#include "API/Network/NetGame/connection_site.h"
#include "network_event.h"
#include "network_data.h"
#include "connection_impl.h"

namespace clan
{
	NetGameConnection::NetGameConnection(NetGameConnectionSite *site, const TCPConnection &connection)
		: impl(new NetGameConnection_Impl)
	{
		impl->start(this, site, connection);
	}

	NetGameConnection::NetGameConnection(NetGameConnectionSite *site, const SocketName &socket_name)
		: impl(new NetGameConnection_Impl)
	{
		impl->start(this, site, socket_name);
	}

	NetGameConnection::~NetGameConnection()
	{
		delete impl;
	}

	void NetGameConnection::set_data(const std::string &name, void *new_data)
	{
		impl->set_data(name, new_data);
	}

	void *NetGameConnection::get_data(const std::string &name) const
	{
		return impl->get_data(name);
	}

	void NetGameConnection::send_event(const NetGameEvent &game_event)
	{
		impl->send_event(game_event);
	}

	void NetGameConnection::disconnect()
	{
		impl->disconnect();
	}

	SocketName NetGameConnection::get_remote_name() const
	{
		return impl->get_remote_name();
	}
}

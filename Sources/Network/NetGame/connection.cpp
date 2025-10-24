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

#include "Network/precomp.h"
#include "API/Network/NetGame/connection.h"
#include "API/Network/NetGame/connection_site.h"
#include "network_event.h"
#include "network_data.h"
#include "connection_impl.h"

CL_NetGameConnection::CL_NetGameConnection(CL_NetGameConnectionSite *site, const CL_TCPConnection &connection)
: impl(new CL_NetGameConnection_Impl())
{
	impl->start(this, site, connection);
}

CL_NetGameConnection::CL_NetGameConnection(CL_NetGameConnectionSite *site, const CL_SocketName &socket_name)
: impl(new CL_NetGameConnection_Impl())
{
	impl->start(this, site, socket_name);
}

CL_NetGameConnection::~CL_NetGameConnection()
{
}

void CL_NetGameConnection::set_data(const CL_StringRef &name, void *new_data)
{
	impl->set_data(name, new_data);
}

void *CL_NetGameConnection::get_data(const CL_StringRef &name) const
{
	return impl->get_data(name);
}

void CL_NetGameConnection::send_event(const CL_NetGameEvent &game_event)
{
	impl->send_event(game_event);
}

void CL_NetGameConnection::disconnect()
{
	impl->disconnect();
}


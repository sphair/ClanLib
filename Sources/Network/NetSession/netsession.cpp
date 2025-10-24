/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "API/Network/NetSession/netsession.h"
#include "netsession_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession construction:

CL_NetSession::CL_NetSession(const std::string &app_id)
: impl(0)
{
	impl = new CL_NetSession_Generic(app_id);
	impl->add_ref();
}

CL_NetSession::CL_NetSession(const CL_NetSession &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetSession::~CL_NetSession()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession attributes:

CL_NetGroup &CL_NetSession::get_all()
{
	return impl->all;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession signals:

CL_Signal_v1<CL_NetComputer &> &CL_NetSession::sig_computer_connected()
{
	return impl->sig_computer_connected;
}

CL_Signal_v1<CL_NetComputer &> &CL_NetSession::sig_computer_reconnected()
{
	return impl->sig_computer_reconnected;
}

CL_Signal_v1<CL_NetComputer &> &CL_NetSession::sig_computer_disconnected()
{
	return impl->sig_computer_disconnected;
}

CL_Signal_v1<CL_NetStream &> &CL_NetSession::sig_netstream_connect(const std::string &netstream)
{
	return impl->map_netstream_connect[netstream];
}

CL_Signal_v2<CL_NetPacket &, CL_NetComputer &> &CL_NetSession::sig_netpacket_receive(const std::string &packetchannel)
{
	return impl->map_netpacket_receive[packetchannel];
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession operators:

void CL_NetSession::start_listen(const std::string &port)
{
	impl->start_listen(port);
}

void CL_NetSession::stop_listen()
{
	impl->stop_listen();
}

CL_NetComputer CL_NetSession::connect(const CL_IPAddress &address)
{
	return impl->connect(address);
}

CL_NetComputer CL_NetSession::connect_async(const std::string &hostname, const std::string &port)
{
	return impl->connect_async(hostname, port);
}

void CL_NetSession::show_debug(bool enable)
{
	impl->show_debug = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession implementation:

CL_NetSession::CL_NetSession(CL_NetSession_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();
}

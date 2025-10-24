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

#include "Network/precomp.h"
#include "API/Network/NetSession/netsession.h"
#include "netsession_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession construction:

CL_NetSession::CL_NetSession(const CL_String &app_id)
: impl(new CL_NetSession_Impl)
{
	impl->app_id = app_id;
}

CL_NetSession::CL_NetSession(const CL_NetSession &copy)
: impl(copy.impl)
{
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

CL_Signal_v1<CL_NetStream &> &CL_NetSession::sig_netstream_connect(const CL_String &netstream)
{
	return impl->map_netstream_connect[netstream];
}

CL_Signal_v2<CL_DataBuffer &, CL_NetComputer &> &CL_NetSession::sig_netpacket_receive(const CL_String &packetchannel)
{
	return impl->map_netpacket_receive[packetchannel];
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession operators:

void CL_NetSession::start_listen(const CL_SocketName &local)
{
	CL_UDPSocket socket;
	socket.bind(local);
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->named_sockets.push_back(socket);
	impl->update_event.set();
}

void CL_NetSession::stop_listen()
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->named_sockets.clear();
	impl->update_event.set();
}

CL_NetComputer CL_NetSession::connect(const CL_SocketName &remote)
{
	return CL_NetComputer();
}

CL_NetComputer CL_NetSession::connect_async(const CL_SocketName &remote)
{
	return CL_NetComputer();
}

void CL_NetSession::show_debug(bool enable)
{
	impl->show_debug = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession implementation:

CL_NetSession::CL_NetSession(CL_SharedPtr<CL_NetSession_Impl> impl)
: impl(impl)
{
}

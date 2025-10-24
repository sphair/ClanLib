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

#pragma once


#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event.h"
#include "API/Core/Signals/signal_v1.h"
#include "API/Core/Signals/signal_v2.h"
#include "API/Network/NetSession/netgroup.h"
#include "API/Network/Socket/udp_socket.h"
#include "API/Network/Socket/socket_name.h"
#include <map>

class CL_NetComputer;
class CL_NetStream;
class CL_DataBuffer;
class CL_SocketName;
struct CL_NetSessionPacket;

class CL_NetSession_Impl
{
/// \name Construction
/// \{

public:
	CL_NetSession_Impl();

	~CL_NetSession_Impl();


/// \}
/// \name Attributes
/// \{

public:
	CL_String app_id;

	CL_NetGroup all;

	CL_Signal_v1<CL_NetComputer &> sig_computer_connected;

	CL_Signal_v1<CL_NetComputer &> sig_computer_reconnected;

	CL_Signal_v1<CL_NetComputer &> sig_computer_disconnected;

	CL_Signal_v1<CL_NetStream &> sig_netstream_connect;

	std::map<CL_String, CL_Signal_v1<CL_NetStream &> > map_netstream_connect;

	std::map<CL_String, CL_Signal_v2<CL_DataBuffer &, CL_NetComputer &> > map_netpacket_receive;

	bool show_debug;

	CL_Mutex mutex;

	std::vector<CL_UDPSocket> named_sockets;

	std::map<CL_SocketName, CL_NetComputer> computers;

	CL_Thread worker_thread;

	CL_Event stop_event;

	CL_Event update_event;


/// \}
/// \name Operations
/// \{

public:


/// \}
/// \name Implementation
/// \{

private:
	void worker_thread_main();

	void worker_thread_received_packet(const CL_NetSessionPacket &packet, const CL_SocketName &source);

	void worker_thread_received_hello(const CL_NetSessionPacket &packet, const CL_SocketName &source);

	void worker_thread_received_hello_ack(const CL_NetSessionPacket &packet, const CL_SocketName &source);

	void worker_thread_received_goodbye(const CL_NetSessionPacket &packet, const CL_SocketName &source);

	void worker_thread_received_goodbye_ack(const CL_NetSessionPacket &packet, const CL_SocketName &source);

	void worker_thread_received_message(const CL_NetSessionPacket &packet, const CL_SocketName &source);
/// \}
};



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

//! clanNetwork="NetSessions"
//! header=network.h

#ifndef header_netsession
#define header_netsession

#ifdef CL_API_DLL
#ifdef CL_NETWORK_EXPORT
#define CL_API_NETWORK __declspec(dllexport)
#else
#define CL_API_NETWORK __declspec(dllimport)
#endif
#else
#define CL_API_NETWORK
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../signals.h"
#include <string>
class CL_NetStream;
class CL_NetPacket;
class CL_NetComputer;
class CL_NetSession_Generic;
class CL_IPAddress;
class CL_NetGroup;

//: Net session class.
//- !group=Network/NetSessions!
//- !header=network.h!
class CL_API_NETWORK CL_NetSession
{
//! Construction:
public:
	//: Constructs a netsession.
	//param app_id: Application identifier. This must match any remote
	//param app_id: netsession connecting to/from this netsession.
	CL_NetSession(const std::string &app_id);

	CL_NetSession(const CL_NetSession &copy);

	virtual ~CL_NetSession();

//! Attributes:
public:
	//: Returns a list of all computers connected.
	CL_NetGroup &get_all();

//! Operations:
public:
	//: Start listening for connections.
	void start_listen(const std::string &listen_port);

	//: Stop listening for connections and disconnect everyone.
	void stop_listen();

	//: Connect to remote netsession.
	CL_NetComputer connect(const CL_IPAddress &address);

	//: Connect to remote netsession asyncronously.
	//- <p>The DNS lookup and connect is done in a worker thread, thus not
	//- blocking the calling thread. If the connection fails,
	//- CL_NetSession::sig_disconnected() is emitted.</p>
	CL_NetComputer connect_async(const std::string &hostname, const std::string &port);

	//: Enable or disable debug output.
	//- <p>This will output the netchannels each netpacket arrive in.</p>
	void show_debug(bool enable = true);

//! Signals:
public:
	//: Signal invoked when a computer connects to the netsession server.
	CL_Signal_v1<CL_NetComputer &> &sig_computer_connected();

	//: Signal invoked when a computer re-connects to the netsession server.
	//- <p>This happens when a CL_NetComputer instance has still not been destroyed
	//- since its last connect (allows you to recognize players dropping out, so
	//- they dont loose their score when they reconnect).</p>
	CL_Signal_v1<CL_NetComputer &> &sig_computer_reconnected();

	//: Signal invoked when a computer disconnects from the netsession server.
	CL_Signal_v1<CL_NetComputer &> &sig_computer_disconnected();

	//: Signal invoked when someone makes a connection to the specified
	//: netstream.
	CL_Signal_v1<CL_NetStream &> &sig_netstream_connect(const std::string &netstream);

	//: Signal invoked when someone sends a netpacket to the specified packet channel.
	CL_Signal_v2<CL_NetPacket &, CL_NetComputer &> &sig_netpacket_receive(const std::string &packet_channel);

//! Implementation:
public:
	CL_NetSession(CL_NetSession_Generic *impl);
private:
	CL_NetSession_Generic *impl;

	friend class CL_NetStream;
};

#endif

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

#ifndef header_netsession_generic
#define header_netsession_generic

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <string>
#include <map>
#include <list>
#include <queue>
#include "API/signals.h"
#include "API/Core/Math/number_pool.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/keep_alive.h"
#include "API/Network/NetSession/netcomputer.h"
#include "API/Network/NetSession/netgroup.h"
#include "API/Network/NetSession/netpacket.h"
#include "API/Network/Socket/socket.h"

class CL_NetStream;
class CL_NetStream_Generic;
class CL_NetComputer_Generic;

class CL_NetSession_Generic : public CL_KeepAlive
{
//! Construction:
public:
	CL_NetSession_Generic(const std::string &app_id);

	virtual ~CL_NetSession_Generic();

//! Attributes:
public:
	CL_Signal_v1<CL_NetComputer &> sig_computer_connected;

	CL_Signal_v1<CL_NetComputer &> sig_computer_reconnected;

	CL_Signal_v1<CL_NetComputer &> sig_computer_disconnected;

	std::map< std::string, CL_Signal_v1<CL_NetStream &> > map_netstream_connect;

	std::map< std::string, CL_Signal_v2<CL_NetPacket &, CL_NetComputer &> > map_netpacket_receive;

	//: UDP socket to send udp packets.
	CL_Socket udp_sock;

	//: trigger called when there is data to be send to clients.
	CL_EventTrigger udp_wakeup_server_trigger;

	//: trigger called when there is data to be send to server.
	CL_EventTrigger udp_wakeup_client_trigger;

	//: IP and Message address pair.
	typedef std::pair<CL_IPAddress, std::string> IPMassagePair;

	//: Queue of messages for client to be sent across unreliable UDP socket.
	std::queue<IPMassagePair> send_udp_client_queue;

	//: Queue of messages for server to be sent across unreliable UDP socket.
	std::queue<IPMassagePair> send_udp_server_queue;

	//: The unique number generator to generate the secret(magic) number (UDP stuff)
	CL_NumberPool secret_number_generator;

	//: Secret(magic) and IP address pair.
	typedef std::pair<unsigned short, std::string> SecretIPPair;

	//: List of CL_NetComputer_Generics that are pending for udp connection (basically to get the PORT to send to).
	std::map<SecretIPPair, CL_NetComputer_Generic *> udp_secret_ip_map;

	//: List of CL_NetComputer_Generics that already has udp connection ready (got the PORT to send to).
	std::map<CL_IPAddress, CL_NetComputer_Generic *> udp_ip_port_map;

	//: List of all computers, after the connect/disconnect signals have been emitted for them.
	CL_NetGroup all;

	//: List of connected computers.
	std::list<CL_NetComputer_Generic *> computers;

	//: Application Identifier.
	std::string app_id;

	//: Packet and Computer received pair.
	typedef std::pair<CL_NetPacket, CL_NetComputer> PacketCompPair;

	//: List of received netpackets on each channel.
	std::map< std::string, std::queue<PacketCompPair> > received_netpackets;
	
	//: NetStream and channel name pair.
	typedef std::pair<CL_NetStream_Generic *, std::string> NewStreamPair;
	
	//: List of incoming stream connection to be signalled.
	std::queue<NewStreamPair> new_streams;

	//: List of incoming connections to be signalled:
	std::queue<CL_NetComputer> new_connections;

	//: List of incoming connections to be signalled:
	std::queue<CL_NetComputer> reconnections;

	//: List of incoming disconnections to be signalled:
	std::queue<CL_NetComputer> disconnections;

	//: Syncronized access to member variables.
	CL_Mutex mutex;

	//: Show debug output if this is enabled.
	bool show_debug;

//! Operations:
public:
	void add_ref();

	int release_ref();

	CL_NetComputer connect(const CL_IPAddress &address);

	CL_NetComputer connect_async(const std::string &hostname, const std::string &port);

	void start_listen(const std::string &port);

	void stop_listen();

	void setup_udp_map(CL_NetComputer_Generic *computer, const std::string &address, unsigned int secret);

	void remove_udp_map(CL_NetComputer_Generic *computer);

	//: Called by CL_NetComputer_Generic's socket thread when a netpacket is received.
	void received_netpacket(const std::string &channel, const CL_NetPacket &packet, CL_NetComputer_Generic *comp);

//! Implementation:
private:
	virtual void keep_alive();

	void boot();

	void shutdown();

	void accept_thread();

	//: thread to send pending udp packets.
	void udp_thread();

	//: method called by udp_thread to dispatch udp message correctly(server)
	void dispatch_udp_messege_server(const CL_IPAddress &from, int bytes_available, char *buffer);

	//: method called by udp_thread to dispatch udp message correctly(client)
	void dispatch_udp_messege_client(const CL_IPAddress &from, int bytes_available, char *buffer);

	CL_Socket accept_tcp_socket;

	CL_Socket accept_udp_socket;

	CL_Thread listen_thread;

	CL_Thread send_udp_thread;

	CL_EventTrigger accept_shutdown_trigger;

	//: trigger to shutdown udp thread.
	CL_EventTrigger udp_shutdown_trigger;
	
	//: Flag to prevent recursive keep alive handling.
	bool in_keep_alive;

	// Reference counter.
	int ref;
};

#endif

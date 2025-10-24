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

#ifndef header_netcomputer_generic
#define header_netcomputer_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/System/mutex.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/event_trigger.h"
#include "API/Network/Socket/socket.h"
#include "API/Network/NetSession/netpacket.h"

#include <map>
#include <string>
#include <queue>

class CL_NetStream_Generic;
class CL_NetSession_Generic;
class CL_NetPacket_Generic;
class CL_NetPacket;
class CL_EventListener;

class CL_NetComputer_Generic
{
//! Construction:
public:
	//: Sync connect constructor.
	CL_NetComputer_Generic(
		const CL_Socket &tcp_sock,
		CL_NetSession_Generic *netsession,
		bool server,
		const std::string &server_port);

	//: ASync connect constructor.
	CL_NetComputer_Generic(
		const std::string &hostname,
		const std::string &port,
		CL_NetSession_Generic *netsession);

	virtual ~CL_NetComputer_Generic();

//! Attributes:
public:
	//: Return current reference count.
	int get_ref() const;

	CL_Thread const & get_thread_handle() const;

	//: TCP socket connection to the network computer.
	CL_Socket tcp_sock;

	//: If true, UDP connection has been established
	bool udp_available;

	//: UDP IP/Port pair for the remote network computer.
	CL_IPAddress udp_address;

	//; Cached dest address of remote computer.
	//: (we cache it because it seems linux forget about the dest peer when a
	//: socket is closed)
	CL_IPAddress address;

	//: True if computer has disconnected.
	bool disconnected;

	//: String describing reason why computer was disconnected.
	std::string disconnect_reason;

	//: Pointer to netsession owner of netcomputer.
	CL_NetSession_Generic *netsession;

	//: Syncronized data access to netcomputer.
	CL_Mutex mutex;

//! Operations:
public:
	//: Increase reference count.
	void add_ref();

	//: Decrease reference count. Delete object if reaching zero.
	int release_ref();

	//: Received udp packet.
	void received_udp_packet(CL_NetPacket &packet);

	//: Send packet to computer.
	void send_packet(const std::string &packet_channel, CL_NetPacket_Generic *packet);

	//: Send packet to computer using udp.
	void send_packet_udp(const std::string &packet_channel, CL_NetPacket_Generic *packet);

	//: Send stream connect packet to computer. Returns the channel_id of the new connection.
	int send_stream_connect(const std::string &stream_channel, CL_NetStream_Generic *stream);

	//: Send stream message to computer.
	void send_stream_message(unsigned int channel_id, const std::string &data);
	
	//: Send stream close message to computer.
	void send_stream_close(int channel_id);

	//: Disconnects from everything
	void disconnect(std::string reason);

	//: Shuts down the netcomputer instance.
	//- <p>Used by CL_NetSession_Generic to close down all connections when the netsession is destroyed.</p>
	void do_shutdown();

//! Implementation:
private:
	//: Send udp info to computer.
	void send_udp_info();

	//: Worker thread for this netcomputer. Basically calls update_socket() on activity.
	void worker_thread();

	//: Updates socket. Dispatches new available data and writes queued data to socket.
	void update_socket();

	//: Bytes left of current data transfer chunk.
	int data_left;

	//: Netstream receiving current data transfer.
	CL_NetStream_Generic *netstream;

	//: Netpacket receiving current data transfer.
	CL_NetPacket netpacket;

	//: Netpacket destination for current data transfer.
	int dest_netpacket_atom;

	//: Atom map used to map from strings to atom ints.
	std::map<std::string, int> local_atoms;

	//: Next free local atom.
	int local_atom_counter;

	//: Atom map used to map from atom ints to strings.
	std::map<int, std::string> remote_atoms;

	//: Queue of messages to be sent across reliable TCP socket.
	std::queue<std::string> send_tcp_queue;

	//: Data sent position in topmost send queue message.
	int send_pos;

	//: Reference counter.
	int ref;

	//: Basic message types used over socket.
	enum
	{
		cmd_create_atom       = 0,
		cmd_udp_info,
		cmd_udp_connect,
		cmd_netstream_connect = 32,
		cmd_netstream_msg,
		cmd_netstream_closed,
		cmd_netpacket_msg     = 64,
	};

	//: Asyncronous connect. If true, the worker thread will do the socket
	//: connect. Otherwise the connect has already been done from the main
	//: thread.
	bool async_boot;

	//: Asyncronous connect. Hostname to connect to.
	std::string async_hostname;

	//: Asyncrounous connect. Port used in connect.
	std::string async_port;

	//: Worker thread handle.
	CL_Thread thread;

	//: Trigger that will wake up sockets worker thread.
	CL_EventTrigger wakeup_trigger;

	//: List of channel connections currently active over this connection.
	std::map<int, CL_NetStream_Generic *> streams;
	
	//: Local stream channel ID counter.
	int channel_id_counter;
	
	//: If true, worker thread is asked to shut itself down.
	volatile bool shutdown;

	//: If true, this netcomputer generic object acts as the server in udp connect sequence.
	bool server;

	//: Port that server is listening on.
	std::string server_port;

	//: Connect secret to be sent to server in response to a cmd_udp_info.
	unsigned short udp_connect_secret;

	//: To allow CL_NetSession_Generic to access the message types for UDP connection
	friend class CL_NetSession_Generic;
};

#endif

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

//! clanNetwork="Sockets"
//! header=network.h

#ifndef header_socket
#define header_socket

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
#include "ip_address.h"
#include "inputsource_socket.h"
#include "outputsource_socket.h"

class CL_EventTrigger;
class CL_Socket_Generic;

//: This is the lowlevel raw socket interface in clanlib.
//- !group=Network/Sockets!
//- !header=network.h!
class CL_API_NETWORK CL_Socket
{
public:
	//: Type enum
	enum Type
	{
		tcp,
		udp
	};

	//: ShutdownHow enum
	enum ShutdownHow
	{
		shutdown_receive,
		shutdown_send
	};
	
	//: Which protocol to use
	enum Protocol
	{
		ipv4,
		ipv6
	};

//! Construction:
public:
	//: Constructs a null socket.
	CL_Socket();

	//: Constructs an attached socket.
	CL_Socket(int socket);

	//: Constructs a new socket using the specified protocol type.
	CL_Socket(Type type, Protocol proto = ipv4);

	//: Copy constructor.
	CL_Socket(const CL_Socket &copy);

	//: Socket destructor
	virtual ~CL_Socket();

//! Attributes:
public:
	//: Input source that operates on the socket.
	CL_InputSource_Socket input;

	//: Output source that operates on the socket.
	CL_OutputSource_Socket output;

	//: Returns the raw socket handle (for use with the lowlevel socket functions).
	int get_socket() const;

	//: Returns the maximum size of a message for message-oriented socket types (CL_Socket::udp).
	int get_max_msg_size() const;

	//: Returns an event trigger that triggers when socket has data ready to be read.
	CL_EventTrigger *get_read_trigger() const;

	//: Returns an event trigger that triggers when socket is ready for additional data.
	CL_EventTrigger *get_write_trigger() const;

	//: Returns an event trigger that triggers when an exception occurs on the socket.
	CL_EventTrigger *get_exception_trigger() const;

	//: Returns the socket name.
	CL_IPAddress get_source_address() const;

	//: Returns the name of the destination socket we are connected to.
	CL_IPAddress get_dest_address() const;

	//: Returns the list of broadcast addresses available to this socket. Works for UDP on IPv4 only.
	std::list<CL_IPAddress> get_broadcast_addresses(const std::string&) const;

//! Operations:
public:
	//: Copy operator.
	CL_Socket &operator =(const CL_Socket &other);

	//: Sets the socket blocking mode.
	void set_nonblocking(bool nonblocking = true);

	//: If enabled, don't delay send to coalesce packets.
	void set_nodelay(bool nodelay = true);

	//: If enabled, do broadcasting. Works for UDP on IPv4 only
	void set_broadcasting(bool broadcasting = true);

	//: Writes data to socket. Returns the amount that was written.
	int send(const void *data, int size);

	//: Writes data to socket, using the specified destination host.
	int send(const void *data, int size, const CL_IPAddress &dest);

	//: Writes data to socket. Returns the amount that was written.
	int send(const std::string &string);

	//: Push data back into the received data buffer.
	void push(const std::string &string);

	//: Reads data from the socket. Returns the amount that was read.
	int recv(void *data, int size);

	//: Reads data from the socket, storing the from address in the passed parameter.
	int recv(void *data, int size, CL_IPAddress &from);

	//: Initiate a connection on the socket.
	void connect(const CL_IPAddress &address);

	//: Shut down part of full-duplex connection.
	void shutdown(ShutdownHow how);

	//: Bind the socket to the specified address.
	void bind(const CL_IPAddress &address);

	//: Listen for connections on the socket.
	void listen(int backlog);

	//: Accept a connection on the socket.
	CL_Socket accept();

//! Signals:
public:
	//: This signal is invoked when the socket has data ready to be read.
	CL_Signal_v0 &sig_read_triggered();

	//: This signal is invoked when the socket has data ready to be written.
	CL_Signal_v0 &sig_write_triggered();

	//: This signal is invoked when an exception occurred on the socket.
	CL_Signal_v0 &sig_exception_triggered();
	
	//: This signal is invoked when the remote side drops the connection, either through timeout 
	//: or explicit shutdown
	CL_Signal_v0 &sig_disconnected();

//! Implementation:
public:
	CL_Socket(class CL_Socket_Generic *impl);
	CL_Socket_Generic *impl;
	Type proto;
};

#endif

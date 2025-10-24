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

#ifndef header_netcomputer
#define header_netcomputer

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

#include "../Socket/ip_address.h"
#include "netsession.h"

class CL_NetComputer_Generic;
class CL_Thread;

//: Class representing a computer in a network game.
//- !group=Network/NetSessions!
//- !header=network.h!
class CL_API_NETWORK CL_NetComputer
{
//! Construction:
public:
	//: Creates a netcomputer object.
	CL_NetComputer();

	CL_NetComputer(const CL_NetComputer &copy);

	//: Net Computer Destructor.
	virtual ~CL_NetComputer();

//! Attributes:
public:
	//: Returns the IP address (in network byte order) of the computer.
	CL_IPAddress get_address() const;

	//: Returns the netsession that the computer is attached to.
	CL_NetSession get_session();

	//: Returns true if computer is disconnected.
	bool is_disconnected() const;

	//: Returns the disconnect reason.
	const std::string &get_disconnect_reason() const;

	//: Returns true if objects are the same.
	bool operator == (const CL_NetComputer &other_instance) const;

	//: Returns true if the other netcomputer is less.
	//- <p>This is used for sorting purposes (eg. if you use a std::map<CL_NetComputer, Player>).</p>
	bool operator < (const CL_NetComputer &other_instance) const;

	//: Returns true if the other netcomputer is greater.
	//- <p>This is used for sorting purposes (eg. if you use a std::map<CL_NetComputer, Player>).</p>
	bool operator > (const CL_NetComputer &other_instance) const;

	//: Thread handle for this netcomputer (used for NetComputers' differentiation)
	CL_Thread const & get_thread_handle() const;

//! Operations:
public:
	//: Disconnects the computer from server.
	void disconnect(std::string reason = (std::string)"No reason given");

	//: Send packet to the specified channel.
	//- <p>set reliable to false to send with udp.</p>
	void send(const std::string &packet_channel, const CL_NetPacket &packet, bool reliable = true);

	//: Copy Operator.
	CL_NetComputer &operator = (const CL_NetComputer &other_instance);

//! Implementation:
public:
	CL_NetComputer(class CL_NetComputer_Generic *impl);

	CL_NetComputer_Generic *impl;
};

#endif

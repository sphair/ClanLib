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

#ifndef header_netpacket
#define header_netpacket

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

#include "inputsource_netpacket.h"
#include "outputsource_netpacket.h"
#include <string>
#include <list>

class CL_NetComputer;
class CL_NetPacket_Generic;

//: Net packet class.
//- !group=Network/NetSessions!
//- !header=network.h!
class CL_API_NETWORK CL_NetPacket
{
//! Construction:
public:
	//: Constructs a packet.
	//param data, size: Initial data for packet.
	CL_NetPacket();

	CL_NetPacket(const void *data, int size);

	CL_NetPacket(const std::string &data);

	CL_NetPacket(const CL_NetPacket &copy);

	// Destructor.
	~CL_NetPacket();

//! Attributes:
public:
	//: Returns a pointer to netpacket data.
	unsigned char *get_data();

	const unsigned char *get_data() const;

	//: Return size of netpacket data.
	int get_size() const;

	//: Input source interface for netpacket.
	CL_InputSource_NetPacket input;

	//: Output source interface for netpacket.
	CL_OutputSource_NetPacket output;

//! Operations:
public:
	//: Resize data. Does not destroy the old data in the packet.
	void resize(int new_size);

	// Copy assignment operator.
	CL_NetPacket &operator =(const CL_NetPacket &copy);

//! Implementation:
private:
	friend class CL_InputSource_NetPacket;
	friend class CL_OutputSource_NetPacket;
	friend class CL_NetComputer;

	CL_NetPacket_Generic *impl;
};

#endif

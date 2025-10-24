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

#ifndef header_netstream
#define header_netstream

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

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "inputsource_netstream.h"
#include "outputsource_netstream.h"
#include <string>

class CL_NetSession;
class CL_NetComputer;
class CL_EventTrigger;
class CL_NetStream_Generic;

//: Net stream class.
//- !group=Network/NetSessions!
//- !header=network.h!
class CL_API_NETWORK CL_NetStream
{
//! Construction:
public:
	//: Connects to a remote netstream.
	//param stream_channel: Channel identifier establishing connection to.
	//param dest: Remote computer on which to connect.
	CL_NetStream(
		const std::string &stream_channel,
		CL_NetComputer &dest);

	CL_NetStream(const CL_NetStream &copy);

	virtual ~CL_NetStream();

//! Attributes:
public:
	//: Input source interface for netstream.
	CL_InputSource_NetStream input;

	//: Output source interface for netstream.
	CL_OutputSource_NetStream output;

	//: Event trigger for reading.
	CL_EventTrigger *get_read_trigger();

	//: Event trigger for writing.
	CL_EventTrigger *get_write_trigger();

	//: Get remote computer
	CL_NetComputer get_computer();

//! Operations:
public:
	//: Copy constructor.
	CL_NetStream &operator =(const CL_NetStream &copy);

	//: Send data onto stream.
	int send(const void *data, int size);

	//: Receive data from stream.
	int recv(void *data, int size);

//! Implementation:
public:
	CL_NetStream(CL_NetStream_Generic *impl);

	CL_NetStream_Generic *impl;
};

#endif

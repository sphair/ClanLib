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

#ifndef header_netstream_generic
#define header_netstream_generic

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "API/Core/System/event_trigger.h"
#include "API/Core/System/mutex.h"
#include <queue>
#include <string>

class CL_NetComputer;
class CL_NetComputer_Generic;
class CL_NetSession_Generic;

class CL_NetStream_Generic
{
//! Construction:
public:
	//: Connect constructor.
	CL_NetStream_Generic(
		const std::string &netstream,
		CL_NetComputer &dest,
		CL_NetSession_Generic *session);

	//: Received constructor.
	CL_NetStream_Generic(
		int channel_id,
		CL_NetComputer_Generic *dest,
		CL_NetSession_Generic *session);

	~CL_NetStream_Generic();

//! Attributes:
public:
	//: Flag to be raised by netsession when all data has been written to a channel.
	CL_EventTrigger write_trigger;

	//: Flag to be raised by netsession when there is data to be read from the channel.
	CL_EventTrigger read_trigger;

	//: Receive buffer queue:
	std::queue<std::string> receive_queue;

	//: Syncronized access to member variables.
	CL_Mutex mutex;

	//: NetSession that stream is connected to.
	CL_NetSession_Generic *session;

	//: Computer the stream is communicating with.
	CL_NetComputer_Generic *computer;

	//: Stream channel identifier.
	int channel_id;
	
	//: True if connection was closed.
	bool closed;

	//: Returns the reference count of the netstream.
	int get_ref_count() const { return ref_count; }

//! Operations:
public:
	//: Increases the reference count.
	void add_ref();

	//: Decreases the reference count. If the count reaches zero, deletes this.
	int release_ref();

//! Implementation:
private:
	//: Reference counter.
	int ref_count;
};

#endif

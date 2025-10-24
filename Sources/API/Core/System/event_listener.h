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

//! clanCore="System"
//! header=core.h

#ifndef header_event_listener
#define header_event_listener

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

class CL_EventTrigger;
class CL_EventListener_Generic;

//: Event listening interace.
//- !group=Core/System!
//- !header=core.h!
//- <p>The event listener is used to collect a set of events, identified as CL_EventTrigger,
//- and wait for one of them to occur. This is analog to the low level C socket select()
//- function, except that this one operates on clanlib objects.</p>
class CL_API_CORE CL_EventListener
{
//! Construction:
public:
	//: Constructs an event listener object.
	//param trigger: Initial trigger to be added to listener.
	CL_EventListener();

	CL_EventListener(CL_EventTrigger *trigger);

	//: Event Listener destructor.
	virtual ~CL_EventListener();

//! Attributes:
public:

//! Operations:
public:
	//: Adds an event trigger to the listener.
	void add_trigger(CL_EventTrigger *trigger);

	//: Removes an event trigger from the listener.
	void remove_trigger(CL_EventTrigger *trigger);

	//: Removes all event triggers from the listener.
	void clear();

	//: Resets all event triggers added to the listener.
	void reset_all();

	//: Wait until one of the events triggers.
	//param timeout - The timeout is in milliseconds. If -1, waits infinitely.
	//- <p>If timeout = -1, this function will not timeout.</p>
	//returns: True if an event triggered, false if it timed out.
	bool wait(int timeout = -1);

//! Implementation:
private:
	CL_EventListener_Generic *impl;
};

#endif

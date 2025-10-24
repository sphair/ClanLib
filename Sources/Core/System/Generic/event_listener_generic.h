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

#ifndef header_event_listener_generic
#define header_event_listener_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/System/event_listener.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/sharedptr.h"

class CL_EventTrigger_Generic;

class CL_EventListener_Generic
{
//! Construction:
public:
	CL_EventListener_Generic(CL_EventListener *owner);

	virtual ~CL_EventListener_Generic();

//! Attributes:
public:

//! Operations:
public:
	//: Adds an event trigger to the listener.
	void add_trigger(const CL_EventTrigger *trigger);

	//: Removes an event trigger from the listener.
	void remove_trigger(const CL_EventTrigger *trigger);

	//: Removes all event triggers from the listener.
	void clear();

	//: Resets all event triggers added to the listener.
	void reset_all();

	//: Wait until one of the events triggers. The timeout is in milliseconds.
	//return: true if a event triggered, false if it timed out.
	virtual bool wait(int timeout)=0;

	std::list< CL_SharedPtr<CL_EventTrigger_Generic> > triggers;

//! Implementation:
private:
	CL_EventListener *owner;
};

#endif

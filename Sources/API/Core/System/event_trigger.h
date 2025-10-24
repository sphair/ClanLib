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

#ifndef header_event_trigger
#define header_event_trigger

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

#include "sharedptr.h"

class CL_EventListener;
class CL_EventTrigger_Generic;

//: ClanLib Event trigger.
//- !group=Core/System!
//- !header=core.h!
//- <p>This class represents an event that can be triggered.</p>
//- <p>Collect several triggers together using the CL_EventListener class, and then
//- call the CL_EventListener::wait function to sleep until one of the events
//- are triggered.</p>
class CL_API_CORE CL_EventTrigger
{
//! Construction:
public:
	//: Constructs an event trigger object.
	CL_EventTrigger();

	CL_EventTrigger(const CL_EventTrigger &copy);

	//: Event Trigger destructor.
	virtual ~CL_EventTrigger();

//! Attributes:
public:
	//: Returns true if the trigger flag has been raised.
	bool get_flag() const;

//! Operations:
public:
	//: Reset trigger flag.
	void reset() const;

	//: Raise trigger flag.
	void set_flag();

	//: Wait until the event triggers.
	//param timeout - The timeout is in milliseconds. If -1, waits infinitely.
	//- <p>If timeout = -1, this function will not timeout.</p>
	//returns: True if event triggered, false if it timed out.
	bool wait(int timeout = -1);

//! Implementation:
public:
	CL_SharedPtr<CL_EventTrigger_Generic> impl;
};

#endif

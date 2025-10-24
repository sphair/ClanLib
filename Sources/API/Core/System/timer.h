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

#ifndef header_timer
#define header_timer

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

#include "keep_alive.h"
#include "../../signals.h"

//: Timer signal emitter.
//- !group=Core/System!
//- !header=core.h!
//- <p>Use this class to emit a signal as a timer event.</p>
class CL_API_CORE CL_Timer : public CL_KeepAlive
{
//! Construction:
public:
	//: Constructs and registers the timer as a keep alive listener.
	//- <p>Note that it is disabled by default, so you need to enable() it.</p>
	//param interval: Interval in millisecs between each signal emit.
	CL_Timer(unsigned long interval = 1000);
	
//! Attributes:
public:
	//: Returns the current time interval.
	//return: The current time interval.
	unsigned long get_interval();

	//: Returns true if the timer is enabled, false otherwise.
	bool is_enabled();

//! Operations:
public:
	//: Sets a new interval for the timer.
	//param interval: Interval in millisecs between each signal emit.
	void set_interval(unsigned long interval);

	//: Call this function to enable the timer.
	void enable();
	
	//: Call this function to disable the timer.
	void disable();
	
//! Signals:
public:
	//: Signal emitted everytime the interval has elapsed.
	CL_Signal_v0 &sig_timer();
	
//! Implementation:
private:
	virtual void keep_alive();

	unsigned long interval;
	unsigned long last_time;
	bool enabled;

	CL_Signal_v0 signal_timer;
};

#endif

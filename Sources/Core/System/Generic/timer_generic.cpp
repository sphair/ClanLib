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

#include "Core/precomp.h"
#include "API/Core/System/timer.h"
#include "API/Core/System/system.h"

CL_Timer::CL_Timer(unsigned long _interval)
: interval(_interval), last_time(0), enabled(false)
{
}

void CL_Timer::keep_alive()
{
	if (!enabled)
		return;

	unsigned long cur = CL_System::get_time();
	if (last_time > cur)
		last_time = cur; // The counter has overflowed the variable - reset it
		
	while (cur >= last_time + interval)
	{
		sig_timer()();
		last_time += interval;
	}
}

void CL_Timer::enable()
{
	enabled = true;
	last_time = CL_System::get_time();
}

void CL_Timer::disable()
{
	enabled = false;
}

bool CL_Timer::is_enabled()
{
	return enabled;
}

void CL_Timer::set_interval(unsigned long interval)
{
	this->interval = interval;
	last_time = CL_System::get_time();
}

unsigned long CL_Timer::get_interval()
{
	return interval;
}

CL_Signal_v0 &CL_Timer::sig_timer()
{
	return signal_timer;
}

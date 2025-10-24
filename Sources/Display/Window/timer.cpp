/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Window/timer.h"
#include "timer_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Timer Construction:


CL_Timer::CL_Timer()
{

}

CL_Timer::CL_Timer(CL_DisplayWindow &window) : impl(new CL_Timer_Impl(window))
{
}

CL_Timer::~CL_Timer()
{
}

CL_Timer::CL_Timer(CL_TimerProvider *provider)
: impl(new CL_Timer_Impl(provider))
{
}

CL_Timer::CL_Timer(const CL_Timer &copy)
{
	impl = copy.impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Timer Attributes:

bool CL_Timer::is_repeating() const
{
	return impl->is_repeating();
}

unsigned int CL_Timer::get_timeout() const
{
	return impl->get_timeout();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Timer Events:
CL_Callback_v0 &CL_Timer::func_expired()
{
	return impl->func_expired();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Timer Operations:

void CL_Timer::start(unsigned int timeout, bool repeat)
{
	impl->start(timeout, repeat);
}

void CL_Timer::stop()
{
	impl->stop();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Timer Implementation:


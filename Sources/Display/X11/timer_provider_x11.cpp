/*
**  ClanLib SDK
**  Copyright (c) 1997-2006 The ClanLib Team
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
#include "API/Display/Window/display_window_message.h"
#include "timer_provider_x11.h"
#include "API/Display/Window/timer.h"
#include "../Window/display_window_impl.h"

unsigned int CL_TimerProvider_X11::handle_counter = 65536;

/////////////////////////////////////////////////////////////////////////////
// CL_TimerProvider_X11 Construction:

CL_TimerProvider_X11::CL_TimerProvider_X11(CL_DisplayWindow &disp_window)
: timer_handle(handle_counter++), repeat_flag(true), window(disp_window.get_provider()), running(false)
{
}

CL_TimerProvider_X11::~CL_TimerProvider_X11()
{
	stop();
}

/////////////////////////////////////////////////////////////////////////////
// CL_TimerProvider_X11 Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_TimerProvider_X11 Operations:

void CL_TimerProvider_X11::destroy()
{
	delete this;
}

void CL_TimerProvider_X11::start(unsigned int timeout, bool repeat)
{
	if (running)
	{
		stop();
	}

	repeat_flag = repeat;
	timeout_value = timeout;

	if (window)
	{
		window->set_timer(this);
		running = true;
	}else
	{
		throw CL_Exception(cl_text("Cannot start timer, window does not exist"));
	}
}

void CL_TimerProvider_X11::stop()
{
	if (running)
	{
		if (window)
		{
			window->kill_timer(this);
		}
		running = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_TimerProvider_X11 Implementation:


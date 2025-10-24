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
#include "API/Display/Window/display_window_message.h"
#include "timer_impl.h"
#ifdef WIN32
#include "../Win32/timer_provider_win32.h"
#else
#include "../X11/timer_provider_x11.h"
#endif


CL_Timer_Impl::CL_Timer_Impl() : provider(0)
{
}

CL_Timer_Impl::~CL_Timer_Impl()
{
	if (provider)
		provider->destroy();
}

CL_Timer_Impl::CL_Timer_Impl(CL_DisplayWindow &window) : provider(0)
{
	CL_DisplayWindowProvider *window_provider = window.get_provider();
	if (!window_provider)
	{
		throw CL_Exception(cl_text("Window provider is now valid"));
	}
	provider = window_provider->alloc_timer(window);
}

CL_Timer_Impl::CL_Timer_Impl(CL_TimerProvider *provider) : provider(provider)
{
}

bool CL_Timer_Impl::is_repeating() const
{
	if (provider==NULL)
		throw CL_Exception(cl_text("Unexpected call to - CL_Timer_Impl::is_repeating()"));

	return provider->is_repeating();
}

unsigned int CL_Timer_Impl::get_timeout() const
{
	if (provider==NULL)
		throw CL_Exception(cl_text("Unexpected call to - CL_Timer_Impl::get_timeout()"));

	return provider->get_timeout();
}

CL_Callback_v0 &CL_Timer_Impl::func_expired()
{
	if (provider==NULL)
		throw CL_Exception(cl_text("Unexpected call to - CL_Timer_Impl::func_expired()"));

	return provider->func_expired;
}

void CL_Timer_Impl::start(unsigned int timeout, bool repeat)
{
	if (provider==NULL)
		throw CL_Exception(cl_text("Unexpected call to - CL_Timer_Impl::start()"));

	provider->start(timeout, repeat);
}

void CL_Timer_Impl::stop()
{
	if (provider==NULL)
		throw CL_Exception(cl_text("Unexpected call to - CL_Timer_Impl::stop()"));
	provider->stop();
}


CL_TimerProvider *CL_Timer_Impl::get_provider()
{
	return provider;
}

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
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/timer_provider.h"
#include "API/Core/System/sharedptr.h"
#include "API/Core/Signals/callback_v0.h"

class CL_TimerProvider_X11 : public CL_TimerProvider
{
/// \name Construction
/// \{

public:
	CL_TimerProvider_X11(CL_DisplayWindow &disp_window);

	~CL_TimerProvider_X11();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the timer repeats until it is stopped
	bool is_repeating() const {return repeat_flag;}

	/// \brief Returns the current timeout. In milliseconds.
	unsigned int get_timeout() const {return timeout_value;}

	/// \brief Returns the timer unique id
	unsigned int get_id() const {return timer_handle;}

/// \}
/// \name Operations
/// \{

public:

	void destroy();

	void start(unsigned int timeout, bool repeat);

	void stop();

/// \}
/// \name Events
/// \{

public:
	CL_Callback_v0 func_expired;

/// \}
/// \name Implementation
/// \{

private:
	static unsigned int handle_counter;

	unsigned int timer_handle;
	bool repeat_flag;
	int timeout_value;
	bool running;

	CL_DisplayWindowProvider *window;
/// \}
};

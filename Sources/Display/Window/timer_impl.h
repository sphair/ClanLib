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

#pragma once

#include "API/Display/TargetProviders/timer_provider.h"

class CL_Timer_Impl
{
public:
	CL_Timer_Impl();

	~CL_Timer_Impl();

	CL_Timer_Impl(CL_DisplayWindow &window);

	CL_Timer_Impl(CL_TimerProvider *provider);

public:

	/// \brief Retrieves the timer provider.
	CL_TimerProvider *get_provider();

	/// \brief Returns true if the timer repeats until it is stopped
	bool is_repeating() const;

	/// \brief Returns the current timeout. In milliseconds.
	unsigned int get_timeout() const;

public:
	CL_Callback_v0 &func_expired();

public:
	/// \brief Starts the timer. Timeout in milliseconds.
	void start(unsigned int timeout, bool repeat=true);

	/// \brief Stop the timer.
	void stop();

private:
	CL_TimerProvider *provider;
};


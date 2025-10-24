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

/// \addtogroup clanDisplay_Window clanDisplay Window
/// \{


#pragma once


#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Signals/callback_v0.h"

class CL_Timer_Impl;
class CL_DisplayWindow;
class CL_TimerProvider;

/// \brief Timer class.
///
/// \xmlonly !group=Display/Window! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Timer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a timer object.

	CL_Timer();

	CL_Timer(CL_DisplayWindow &window);

	CL_Timer(CL_TimerProvider *provider);

	CL_Timer(const CL_Timer &copy);

	~CL_Timer();


/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the timer provider.
	CL_TimerProvider *get_provider();

	/// \brief Returns true if the timer repeats until it is stopped
	bool is_repeating() const;

	/// \brief Returns the current timeout. In milliseconds.
	unsigned int get_timeout() const;

/// \}
/// \name Events
/// \{

public:
	CL_Callback_v0 &func_expired();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Starts the timer. Timeout in milliseconds.
	void start(unsigned int timeout, bool repeat=true);

	/// \brief Stop the timer.
	void stop();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_Timer_Impl> impl;
/// \}
};




/// \}

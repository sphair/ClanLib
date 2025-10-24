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
**    Mark Page
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Signals/callback_v0.h"

/// \brief Interface for implementing a CL_Timer target.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_TimerProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_TimerProvider() { return; }


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the timer repeats until it is stopped
	virtual bool is_repeating() const = 0;

	/// \brief Returns the current timeout. In milliseconds.
	virtual unsigned int get_timeout() const = 0;

	virtual unsigned int get_id() const = 0;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the timer provider.
	virtual void destroy() = 0;

	/// \brief Starts the timer. Timeout in milliseconds.
	virtual void start(unsigned int timeout, bool repeat) = 0;

	/// \brief Stop the timer.
	virtual void stop() = 0;

/// \}
/// \name Events
/// \{

public:
	CL_Callback_v0 func_expired;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}

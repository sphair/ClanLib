/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/

#pragma once

#include "../api_core.h"
#include <memory>
#include "../Signals/callback.h"

namespace clan
{

class Timer_Impl;

/// \brief Timer class that invokes a callback on a specified interval
class CL_API_CORE Timer
{
/// \name Construction
/// \{
public:
	/// \brief Creates a timer object
	Timer();

	/// \brief Destroys the timer object
	virtual ~Timer();

	/// \brief Returns true if the timer repeats until it is stopped
	bool is_repeating() const;

	/// \brief Returns the current timeout. In milliseconds.
	unsigned int get_timeout() const;
/// \}

/// \name Events
/// \{
public:
	/// \brief Callback invoked every time the timer interval occurs
	Callback_v0 &func_expired();
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
	std::shared_ptr<Timer_Impl> impl;
/// \}
};

}

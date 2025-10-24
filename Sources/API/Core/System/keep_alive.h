/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "../Signals/callback_2.h"

class CL_Event;
class CL_KeepAliveObject;

/// \brief CL_KeepAlive application loop helper
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_KeepAlive
{
public:
	/// \brief Processes and dispatches keep alive events until the specified timeout period has passed
	///
	/// \param timeout = Timeout (ms). -1 = Wait forever
	static void process(int timeout = 0);

	/// \brief Function that polls the events
	///
	/// If this is not set, CL_Event::wait() is used.
	///
	/// \param events = The event objects to wait for
	/// \param timeout = Timeout (ms). -1 = Wait forever
	/// \return The event that triggered an event.\n
	///         -1 = Timeout
	///			events.size() = An external event was triggered
	static CL_Callback_2<int /*retval*/, const std::vector<CL_Event> &/*events*/, int /*timeout */ > &func_event_wait();

	/// \brief Returns all the current keep alive objects available for this thread
	static std::vector<CL_KeepAliveObject *> get_objects();
};

/// \brief Interface for objects participating in the thread keep-alive processing loop
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_KeepAliveObject
{
public:
	/// \brief Registers a keep alive object
	CL_KeepAliveObject();

	/// \brief Unregisters a keep alive object
	virtual ~CL_KeepAliveObject();

	/// \brief Returns a CL_Event object that is signaled when the keep alive object got data to process
	virtual CL_Event get_wakeup_event() = 0;

	/// \brief Called by CL_KeepAlive::process when the wakeup event is flagged
	virtual void process() = 0;
};

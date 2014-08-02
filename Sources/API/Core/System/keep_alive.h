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


#include <memory>
#include <functional>

namespace clan
{

class Event;
class KeepAliveObject;
class KeepAliveObject_Impl;

/// \brief KeepAlive application loop helper
class KeepAlive
{
public:
	/// \brief Processes and dispatches keep alive events until the specified timeout period has passed
	///
	/// \param timeout = Timeout (ms). -1 = Wait forever
	static void process(int timeout = 0);

	/// \brief Function that polls the events
	///
	/// If this is not set, Event::wait() is used.
	///
	/// \param events = The event objects to wait for
	/// \param timeout = Timeout (ms). -1 = Wait forever
	/// \return The event that triggered an event.\n
	///         -1 = Timeout
	///			events.size() = An external event was triggered
	static std::function<int /*retval*/(const std::vector<Event> &/*events*/, int /*timeout */)> &func_event_wait();
    
	/// \brief Function that gets called when KeepAliveObject objects are created.
    ///
    /// This callback is used to provide a thread identifier for the func_awake_thread callback.
    static std::function<void *()> &func_thread_id();
    
	/// \brief Function that gets called when KeepAliveObject::set_wakeup_event is called.
    ///
    /// This callback is used to provide a way to awaken a thread that needs to process keep alive events.
    static std::function<void(void *)> &func_awake_thread();

	/// \brief Returns all the current keep alive objects available for this thread
	static std::vector<KeepAliveObject *> get_objects();
};

/// \brief Interface for objects participating in the thread keep-alive processing loop
class KeepAliveObject
{
public:
	/// \brief Registers a keep alive object
	KeepAliveObject();

	/// \brief Unregisters a keep alive object
	virtual ~KeepAliveObject();

    /// \brief Informs the KeepAlive application loop this object got data to process
    void set_wakeup_event();

	/// \brief Called by KeepAlive::process when the wakeup event is flagged
	virtual void process() = 0;
    
private:
    std::shared_ptr<KeepAliveObject_Impl> impl;
    friend class KeepAlive;
};

}

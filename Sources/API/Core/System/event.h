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

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"
#include "sharedptr.h"
#include "mutex.h"
#include <vector>

class CL_EventProvider;
class CL_Event_Impl;

/// \brief OS level event.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_Event
{
/// \name Construction
/// \{

public:
	/// \brief Constructs an event object.
	CL_Event(bool manual_reset = true, bool initial_state = false);

	CL_Event(CL_EventProvider *event_provider);

	~CL_Event();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the event provider for this event.
	CL_EventProvider *get_event_provider() const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Wait for event to become flagged.
	///
	/// \return true = When was flagged. Else timeout
	bool wait(int timeout = -1);

	static int wait(int count, CL_Event const * const * events, int timeout = -1);

	static int wait(const std::vector<CL_Event *> &events, int timeout = -1);

	static int wait(const std::vector<CL_Event> &events, int timeout = -1);

	static int wait(CL_Event &event1, int timeout = -1);

	static int wait(CL_Event &event1, CL_Event &event2, int timeout = -1);

	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, int timeout = -1);

	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, int timeout = -1);

	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, int timeout = -1);

	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, int timeout = -1);

	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, CL_Event &event7, int timeout = -1);

	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, CL_Event &event7, CL_Event &event8, int timeout = -1);

	/// \brief Flag event.
	void set();

	/// \brief Reset flag.
	void reset();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_Event_Impl> impl;

/// \}
};


/// \}

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
*/

/// \addtogroup clanDisplay_Window clanDisplay Window
/// \{

#ifndef display_message_queue
#define display_message_queue

#include "../api_display.h"

class CL_Event;
class CL_DisplayWindowMessage;
class CL_DisplayMessageQueue_Provider;
class CL_DisplayMessageQueue_Impl;

/// \brief Display Message Queue class.
///
/// \xmlonly !group=Display/Window! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_DisplayMessageQueue
{
/// \name Operations
/// \{
public:
	/// \brief Make thread sleep until a message arrives on the queue or one of the events is triggered.
	///
	/// \return The index of the event that got flagged, -1 if the wait timed out and -2 if a message is available.
	static int wait(int count, CL_Event const * const * events, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param vector = std
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(const std::vector<CL_Event *> &events, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param vector = std
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(const std::vector<CL_Event> &events, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param event1 = Event
	/// \param timeout = value
	///
	/// \return int
	static int wait(CL_Event &event1, int timeout = -1);

	/// \brief Wait
	///
	/// \param event1 = Event
	/// \param event2 = Event
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(CL_Event &event1, CL_Event &event2, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param event1 = Event
	/// \param event2 = Event
	/// \param event3 = Event
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param event1 = Event
	/// \param event2 = Event
	/// \param event3 = Event
	/// \param event4 = Event
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param event1 = Event
	/// \param event2 = Event
	/// \param event3 = Event
	/// \param event4 = Event
	/// \param event5 = Event
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param event1 = Event
	/// \param event2 = Event
	/// \param event3 = Event
	/// \param event4 = Event
	/// \param event5 = Event
	/// \param event6 = Event
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param event1 = Event
	/// \param event2 = Event
	/// \param event3 = Event
	/// \param event4 = Event
	/// \param event5 = Event
	/// \param event6 = Event
	/// \param event7 = Event
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, CL_Event &event7, int timeout = -1, bool wait_all = false);

	/// \brief Wait
	///
	/// \param event1 = Event
	/// \param event2 = Event
	/// \param event3 = Event
	/// \param event4 = Event
	/// \param event5 = Event
	/// \param event6 = Event
	/// \param event7 = Event
	/// \param event8 = Event
	/// \param timeout = value
	/// \param wait_all = bool
	///
	/// \return int
	static int wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, CL_Event &event7, CL_Event &event8, int timeout = -1, bool wait_all = false);

	/// \brief Make thread sleep until a message arrives on the queue.
	static bool wait(int timeout = -1);

	/// \brief Get the first message from the queue.
	static CL_DisplayWindowMessage get_message();

	/// \brief Peek at the first message from the queue.
	static CL_DisplayWindowMessage peek_message(bool block);

	/// \brief Process a message.
	static void dispatch_message(const CL_DisplayWindowMessage &message);

	/// \brief Check for any messages
	static bool has_messages();

	/// \brief Process any messages (gets the messages then dispatches it) and processes any CL_KeepAlive objects
	static void process(int timeout = 0);

/// \}
/// \name Implementation
/// \{
private:
/// \}
};

#endif
/// \}

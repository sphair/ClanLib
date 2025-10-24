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


#include "Display/precomp.h"
#include "API/Display/Window/display_message_queue.h"
#include "API/Display/Window/display_window_message.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/TargetProviders/display_target_provider.h"
#include "API/Display/TargetProviders/display_message_queue_provider.h"
#include "API/Core/System/event.h"
#include "API/Core/System/keep_alive.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue operations:

int CL_DisplayMessageQueue::wait(const std::vector<CL_Event *> &events, int timeout, bool wait_all)
{
	if (events.size() > 0)
		return wait((int) events.size(), &events[0], timeout, wait_all);
	else
		return wait(0, 0, timeout, wait_all);
}

int CL_DisplayMessageQueue::wait(const std::vector<CL_Event> &events, int timeout, bool wait_all)
{
	std::vector<CL_Event>::size_type i, size;
	size = events.size();
	std::vector<CL_Event *> event_ptrs;
	event_ptrs.reserve(size);
	for (i = 0; i < size; i++)
		event_ptrs.push_back((CL_Event *) &events[i]);
	return wait(event_ptrs, timeout, wait_all);
}

int CL_DisplayMessageQueue::wait(CL_Event &event1, int timeout)
{
	CL_Event *events[1] = { &event1 };
	return wait(1, events, timeout, false);
}

int CL_DisplayMessageQueue::wait(CL_Event &event1, CL_Event &event2, int timeout, bool wait_all)
{
	CL_Event *events[2] = { &event1, &event2 };
	return wait(2, events, timeout, wait_all);
}

int CL_DisplayMessageQueue::wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, int timeout, bool wait_all)
{
	CL_Event *events[3] = { &event1, &event2, &event3 };
	return wait(3, events, timeout, wait_all);
}

int CL_DisplayMessageQueue::wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, int timeout, bool wait_all)
{
	CL_Event *events[4] = { &event1, &event2, &event3, &event4 };
	return wait(4, events, timeout, wait_all);
}

int CL_DisplayMessageQueue::wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, int timeout, bool wait_all)
{
	CL_Event *events[5] = { &event1, &event2, &event3, &event4, &event5 };
	return wait(5, events, timeout, wait_all);
}

int CL_DisplayMessageQueue::wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, int timeout, bool wait_all)
{
	CL_Event *events[6] = { &event1, &event2, &event3, &event4, &event5, &event6 };
	return wait(6, events, timeout, wait_all);
}

int CL_DisplayMessageQueue::wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, CL_Event &event7, int timeout, bool wait_all)
{
	CL_Event *events[7] = { &event1, &event2, &event3, &event4, &event5, &event6, &event7 };
	return wait(7, events, timeout, wait_all);
}

int CL_DisplayMessageQueue::wait(CL_Event &event1, CL_Event &event2, CL_Event &event3, CL_Event &event4, CL_Event &event5, CL_Event &event6, CL_Event &event7, CL_Event &event8, int timeout, bool wait_all)
{
	CL_Event *events[8] = { &event1, &event2, &event3, &event4, &event5, &event6, &event7, &event8 };
	return wait(8, events, timeout, wait_all);
}

bool CL_DisplayMessageQueue::wait(int timeout)
{
	return wait(0, 0, timeout) == -2;
}

int CL_DisplayMessageQueue::wait(int count, CL_Event const * const * events, int timeout, bool wait_all)
{
	return CL_Display::get_current_target().get_provider()->get_message_queue()->wait(count, events, timeout, wait_all);
}

CL_DisplayWindowMessage CL_DisplayMessageQueue::get_message()
{
	return CL_Display::get_current_target().get_provider()->get_message_queue()->get_message();
}

CL_DisplayWindowMessage CL_DisplayMessageQueue::peek_message(bool block)
{
	return CL_Display::get_current_target().get_provider()->get_message_queue()->peek_message(block);
}

void CL_DisplayMessageQueue::dispatch_message(const CL_DisplayWindowMessage &message)
{
	CL_Display::get_current_target().get_provider()->get_message_queue()->dispatch_message(message);
}

bool CL_DisplayMessageQueue::has_messages()
{
	return wait(0);
}

void CL_DisplayMessageQueue::process(int timeout)
{

	int time_start = CL_System::get_time();
	while (true)
	{
		int time_elapsed = CL_System::get_time() - time_start;
		int time_to_wait = timeout - time_elapsed;
		if (time_to_wait < 0)
			time_to_wait = 0;

		std::vector<CL_KeepAliveObject *> objects = CL_KeepAlive::get_objects();
		std::vector<CL_Event> events;
		for (std::vector<CL_KeepAliveObject *>::size_type i = 0; i < objects.size(); i++)
		{
			events.push_back(objects[i]->get_wakeup_event());
		}

		int wakeup_reason = wait(events, timeout);

		if (wakeup_reason >= 0)
		{
			objects[wakeup_reason]->process();
		}
		else if (wakeup_reason == -2)
		{
			do
			{
				CL_DisplayWindowMessage message = get_message();
				dispatch_message(message);
			}while (has_messages());		// There may be more than one message pending, so we have to process all of them here
		}
		else if (wakeup_reason == -1)
		{
			break;
		}

		if (time_to_wait == 0)
			break;
	}

/*
	while(has_messages())
	{
		CL_DisplayWindowMessage message = get_message();
		dispatch_message(message);
	}
*/
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue implementation:

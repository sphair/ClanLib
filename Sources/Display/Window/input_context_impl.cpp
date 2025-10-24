/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "input_context_impl.h"
#include "input_device_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext_Impl construction:

CL_InputContext_Impl::CL_InputContext_Impl()
{
}

CL_InputContext_Impl::~CL_InputContext_Impl()
{
	dispose();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext_Impl attributes:

CL_Mutex CL_InputContext_Impl::mutex;

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext_Impl operations:

void CL_InputContext_Impl::clear()
{
	throw_if_disposed();

	keyboards.clear();
	mice.clear();
	joysticks.clear();
	tablets.clear();
}

void CL_InputContext_Impl::add_keyboard(CL_InputDevice &keyboard)
{
	throw_if_disposed();

	CL_MutexSection mutex_lock(&mutex);
	keyboards.push_back(keyboard);
	keyboard.impl->input_contexts.push_back(input_context);
}

void CL_InputContext_Impl::add_mouse(CL_InputDevice &mouse)
{
	throw_if_disposed();

	CL_MutexSection mutex_lock(&mutex);
	mice.push_back(mouse);
	mouse.impl->input_contexts.push_back(input_context);
}

void CL_InputContext_Impl::add_joystick(CL_InputDevice &joystick)
{
	throw_if_disposed();

	CL_MutexSection mutex_lock(&mutex);
	joysticks.push_back(joystick);
	joystick.impl->input_contexts.push_back(input_context);
}

void CL_InputContext_Impl::add_tablet(CL_InputDevice &tablet)
{
	throw_if_disposed();

	CL_MutexSection mutex_lock(&mutex);
	tablets.push_back(tablet);
	tablet.impl->input_contexts.push_back(input_context);
}

void CL_InputContext_Impl::process_messages()
{
	throw_if_disposed();

	std::vector< std::pair<CL_InputEvent, CL_WeakPtr<CL_InputDevice_Impl> > >::size_type pos, size;

	// todo: process events from windowing system message queue

	// Fetch latest events received:
	CL_MutexSection mutex_lock(&mutex);
	std::vector< std::pair<CL_InputEvent, CL_WeakPtr<CL_InputDevice_Impl> > > cur_events  = events;
	events.clear();
	mutex_lock.unlock();

	// todo: sort received events by time

	size = cur_events.size();
	for (pos = 0; pos < size; pos++)
	{
		CL_InputEvent event = cur_events[pos].first;
		if (cur_events[pos].second.is_null())
			continue;
		event.device = CL_InputDevice(cur_events[pos].second);

		switch (event.type)
		{
		case CL_InputEvent::pressed:
			event.device.sig_key_down().invoke(event, input_state);
			break;
		case CL_InputEvent::released:
			event.device.sig_key_up().invoke(event, input_state);
			break;
		case CL_InputEvent::doubleclick:
			event.device.sig_key_dblclk().invoke(event, input_state);
			break;
		case CL_InputEvent::pointer_moved:
			event.device.sig_pointer_move().invoke(event, input_state);
			break;
		case CL_InputEvent::axis_moved:
			event.device.sig_axis_move().invoke(event, input_state);
			break;
		case CL_InputEvent::proximity_change:
			event.device.sig_proximity_change().invoke(event, input_state);
			break;
		default:	// Added to stop the compiler warning about "no_key" not handled in switch
			break;
		}
	}
}

void CL_InputContext_Impl::received_event(
	const CL_InputEvent &e,
	CL_WeakPtr<CL_InputDevice_Impl> &input_device)
{
	throw_if_disposed();

	events.push_back(
		std::pair<CL_InputEvent, CL_WeakPtr<CL_InputDevice_Impl> >(
			e,
			input_device));
}

bool CL_InputContext_Impl::poll(bool peek_only)
{
	throw_if_disposed();

	bool message_flag = false;

	poll_device(peek_only, joysticks, message_flag);
	poll_device(peek_only, mice, message_flag);
	poll_device(peek_only, keyboards, message_flag);
	poll_device(peek_only, tablets, message_flag);

	return message_flag;
}

void CL_InputContext_Impl::poll_device( bool peek_only, std::vector<CL_InputDevice> &device, bool &message_flag)
{
	throw_if_disposed();

	std::vector< CL_InputDevice >::size_type pos, size;
	size = device.size();
	for (pos = 0; pos < size; pos++)
	{
		if (device[pos].poll(peek_only))
		{
			message_flag = true;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputContext_Impl implementation:

void CL_InputContext_Impl::on_dispose()
{
	keyboards.clear();
	mice.clear();
	joysticks.clear();
	tablets.clear();
}
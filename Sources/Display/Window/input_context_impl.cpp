/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Display/Window/input_event.h"

namespace clan
{
	InputContext_Impl::InputContext_Impl()
	{
	}

	InputContext_Impl::~InputContext_Impl()
	{
		dispose();
	}

	std::recursive_mutex InputContext_Impl::mutex;

	void InputContext_Impl::clear()
	{
		throw_if_disposed();

		keyboards.clear();
		mice.clear();
		joysticks.clear();
	}

	void InputContext_Impl::add_keyboard(InputDevice &keyboard)
	{
		throw_if_disposed();

		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		keyboards.push_back(keyboard);
		keyboard.impl->input_contexts.push_back(input_context);
	}

	void InputContext_Impl::add_mouse(InputDevice &mouse)
	{
		throw_if_disposed();

		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		mice.push_back(mouse);
		mouse.impl->input_contexts.push_back(input_context);
	}

	void InputContext_Impl::add_joystick(InputDevice &joystick)
	{
		throw_if_disposed();

		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		joysticks.push_back(joystick);
		joystick.impl->input_contexts.push_back(input_context);
	}

	void InputContext_Impl::process_messages()
	{
		/*
		throw_if_disposed();

		std::vector< std::pair<InputEvent, std::weak_ptr<InputDevice_Impl> > >::size_type pos, size;

		// Fetch latest events received:
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		std::vector< std::pair<InputEvent, std::weak_ptr<InputDevice_Impl> > > cur_events = events;
		events.clear();
		mutex_lock.unlock();

		size = cur_events.size();
		for (pos = 0; pos < size; pos++)
		{
			if (is_disposed())	// Exit the function now if a previous input event has caused the input context to be disposed
				break;

			InputEvent event = cur_events[pos].first;
			if (cur_events[pos].second.expired())
				continue;
			event.device = InputDevice(cur_events[pos].second);

			switch (event.type)
			{
			case InputEvent::pressed:
				event.device.sig_key_down()(event);
				break;
			case InputEvent::released:
				event.device.sig_key_up()(event);
				break;
			case InputEvent::doubleclick:
				event.device.sig_key_dblclk()(event);
				break;
			case InputEvent::pointer_moved:
				event.device.sig_pointer_move()(event);
				break;
			case InputEvent::axis_moved:
				event.device.sig_axis_move()(event);
				break;
			case InputEvent::proximity_change:
				event.device.sig_proximity_change()(event);
				break;
			default:	// Added to stop the compiler warning about "no_key" not handled in switch
				break;
			}
		}
		*/
	}

	void InputContext_Impl::on_dispose()
	{
		keyboards.clear();
		mice.clear();
		joysticks.clear();
	}
}

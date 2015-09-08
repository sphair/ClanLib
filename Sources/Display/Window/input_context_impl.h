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

#pragma once

#include <vector>
#include "API/Display/Window/input_device.h"
#include "API/Core/System/disposable_object.h"
#include <mutex>

namespace clan
{
	class InputEvent;

	class InputContext_Impl : public DisposableObject
	{
	public:
		InputContext_Impl();
		~InputContext_Impl();

		static std::recursive_mutex mutex;

		std::vector<InputDevice> keyboards;
		std::vector<InputDevice> mice;
		std::vector<InputDevice> joysticks;
		std::vector<InputDevice> tablets;

		std::weak_ptr<InputContext_Impl> input_context;

		/// \brief Removes all devices from this input context.
		void clear();

		/// \brief Adds a new keyboard to the input context.
		void add_keyboard(InputDevice &keyboard);

		/// \brief Adds a new mouse to the input context.
		void add_mouse(InputDevice &mouse);

		/// \brief Adds a new joystick to the input context.
		void add_joystick(InputDevice &joystick);

		/// \brief Adds a new tablet to the input context.
		void add_tablet(InputDevice &tablet);

		/// \brief Process and dispatch messages to signals.
		void process_messages();

		/// \brief Called by input devices when events are received.
		void received_event(const InputEvent &e, std::weak_ptr<InputDevice_Impl> &input_device);

	private:
		void on_dispose() override;

		std::vector< std::pair<InputEvent, std::weak_ptr<InputDevice_Impl> > > events;
	};
}

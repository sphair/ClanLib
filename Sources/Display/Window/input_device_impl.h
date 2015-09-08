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
*/

#pragma once

#include "API/Core/Signals/signal.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "input_context_impl.h"

namespace clan
{
	class InputDevice_Impl
	{
	public:
		InputDevice_Impl()
			: provider(nullptr)
		{
			sc.connect(sig_provider_event, bind_member(this, &InputDevice_Impl::on_provider_event));
		}

		~InputDevice_Impl()
		{
			if (provider)
				delete provider;
		}

		void on_provider_event(const InputEvent &e)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(InputContext_Impl::mutex);
			std::vector< std::weak_ptr<InputContext_Impl> >::size_type pos, size;
			size = input_contexts.size();
			for (pos = 0; pos < size; pos++)
			{
				std::weak_ptr<InputContext_Impl> &ic = input_contexts[pos];
				if (!ic.expired())
					ic.lock()->received_event(e, input_device);
			}
		}

		std::vector< std::weak_ptr<InputContext_Impl> > input_contexts;

		std::weak_ptr<InputDevice_Impl> input_device;

		InputDeviceProvider *provider;

		SlotContainer sc;
		Signal<void(const InputEvent &)> sig_provider_event;
		Signal<void(const InputEvent &)> sig_key_down;
		Signal<void(const InputEvent &)> sig_key_up;
		Signal<void(const InputEvent &)> sig_pointer_move;
		Signal<void(const InputEvent &)> sig_axis_move;
		Signal<void(const InputEvent &)> sig_key_dblclk;
		Signal<void(const InputEvent &)> sig_proximity_change;
	};
}

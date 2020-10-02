/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

namespace clan
{
	class InputDevice_Impl : public std::enable_shared_from_this<InputDevice_Impl>
	{
	public:
		std::unique_ptr<InputDeviceProvider> provider;

		SlotContainer sc;
		Signal<void(const InputEvent &)> sig_key_down;
		Signal<void(const InputEvent &)> sig_key_up;
		Signal<void(const InputEvent &)> sig_pointer_move;
		Signal<void(const InputEvent &)> sig_axis_move;
		Signal<void(const InputEvent &)> sig_key_dblclk;
		Signal<void(const InputEvent &)> sig_proximity_change;
	};
}

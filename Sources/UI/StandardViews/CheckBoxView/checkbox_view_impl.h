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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

namespace clan
{

	class CheckBoxView_Impl
	{
	public:
		void on_pointer_press(PointerEvent &e);
		void on_pointer_release(PointerEvent &e);
		void update_state();

		CheckBoxView *checkbox = nullptr;
		bool _state_disabled = false;
		bool _state_hot = false;
		bool _state_pressed = false;

		bool _checked_state = false;
		std::function<void()> _func_state_changed;
	};
}


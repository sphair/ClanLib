/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
**    Artem Khomenko (add sticky property)
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/button_view.h"
#include "API/UI/StandardViews/image_view.h"
#include "button_view_impl.h"
#include "API/UI/Events/pointer_event.h"

namespace clan
{

	void ButtonViewImpl::update_state()
	{
		bool target_hot = false;
		bool target_disabled = false;
		bool target_pressed = false;

		if (_state_disabled)
		{
			target_disabled = true;
		}
		else if (_state_pressed)
		{
			target_pressed = true;
		}
		else if (_state_hot)
		{
			target_hot = true;
		}

		button->set_state_cascade("hot", target_hot);
		button->set_state_cascade("pressed", target_pressed);
		button->set_state_cascade("disabled", target_disabled);
	}

	void ButtonViewImpl::on_pointer_press(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		_state_pressed = true;
		update_state();
	}

	void ButtonViewImpl::on_pointer_release(PointerEvent &e)
	{
		if (_state_disabled)
			return;

		// If mouse released over component
		if (button->geometry().border_box().contains(e.pos(button) + button->geometry().content_box().get_top_left())) {
			if (_sticky) {
				_state_pressed = _state_pressed_previous;
				_state_pressed_previous = !_state_pressed_previous;
			}
			else
				_state_pressed = false;

			// User event handler need to get right state.
			update_state();

			if (_func_clicked)
				_func_clicked();
		}
		else {
			_state_hot = false;
			_state_pressed = _sticky ? !_state_pressed_previous : false;
			update_state();
		}
	}

}

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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/checkbox_view.h"
#include "API/UI/Style/style_property_parser.h"
#include "API/Display/2D/path.h"
#include "API/Display/System/timer.h"
#include "API/Display/2D/brush.h"
#include "API/UI/Events/pointer_event.h"
#include <algorithm>
#include "checkbox_view_impl.h"

namespace clan
{
	void CheckBoxView_Impl::update_state()
	{
		bool target_checked = false;
		bool target_checked_hot = false;
		bool target_checked_disabled = false;
		bool target_checked_pressed = false;
		bool target_unchecked = false;
		bool target_unchecked_hot = false;
		bool target_unchecked_disabled = false;
		bool target_unchecked_pressed = false;

		if (_checked_state)
		{
			if (_state_disabled)
			{
				target_checked_disabled = true;
			}
			else if (_state_pressed)
			{
				target_checked_pressed = true;
			}
			else if (_state_hot)
			{
				target_checked_hot = true;
			}
			else
			{
				target_checked = true;
			}
		}
		else
		{
			if (_state_disabled)
			{
				target_unchecked_disabled = true;
			}
			else if (_state_pressed)
			{
				target_unchecked_pressed = true;
			}
			else if (_state_hot)
			{
				target_unchecked_hot = true;
			}
			else
			{
				target_unchecked = true;
			}

		}

		checkbox->set_state_cascade("checked", target_checked);
		checkbox->set_state_cascade("checked_hot", target_checked_hot);
		checkbox->set_state_cascade("checked_pressed", target_checked_pressed);
		checkbox->set_state_cascade("checked_disabled", target_checked_disabled);
		//checkbox->set_state_cascade("unchecked", target_unchecked);	(The default state)
		checkbox->set_state_cascade("unchecked_hot", target_unchecked_hot);
		checkbox->set_state_cascade("unchecked_pressed", target_unchecked_pressed);
		checkbox->set_state_cascade("unchecked_disabled", target_unchecked_disabled);
		label->set_state_cascade("disabled", _state_disabled);

		// Update the font in accordance with the state.
		label->reset_font();

		// Fast draw the checkbox.
		checkbox->draw_without_layout();
	}

	void CheckBoxView_Impl::on_pointer_press(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		_state_pressed = true;
		update_state();
	}

	void CheckBoxView_Impl::on_pointer_release(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;

		_state_pressed = false;
		if (checkbox->geometry().border_box().contains(e.pos(checkbox) + checkbox->geometry().content_box().get_top_left()))	// Only allow click when mouse released over component
		{
			_checked_state = !_checked_state;
			if (_func_state_changed)
				_func_state_changed();
		}
		update_state();
	}

}


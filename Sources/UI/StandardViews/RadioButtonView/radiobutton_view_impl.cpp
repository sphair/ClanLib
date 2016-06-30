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
#include "API/UI/StandardViews/radiobutton_view.h"
#include "API/UI/Style/style_property_parser.h"
#include "API/Display/2D/path.h"
#include "API/Display/System/timer.h"
#include "API/Display/2D/brush.h"
#include "API/UI/Events/pointer_event.h"
#include <algorithm>
#include "radiobutton_view_impl.h"

namespace clan
{
	void RadioButtonView_Impl::update_state()
	{
		bool target_checked = false;
		bool target_checked_hot = false;
		bool target_checked_disabled = false;
		bool target_checked_pressed = false;
		bool target_unchecked = false;
		bool target_unchecked_hot = false;
		bool target_unchecked_disabled = false;
		bool target_unchecked_pressed = false;

		if (_state_selected)
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

		radio->set_state_cascade("checked", target_checked);
		radio->set_state_cascade("checked_hot", target_checked_hot);
		radio->set_state_cascade("checked_pressed", target_checked_pressed);
		radio->set_state_cascade("checked_disabled", target_checked_disabled);
		//radio->set_state_cascade("unchecked", target_unchecked);	(The default state)
		radio->set_state_cascade("unchecked_hot", target_unchecked_hot);
		radio->set_state_cascade("unchecked_pressed", target_unchecked_pressed);
		radio->set_state_cascade("unchecked_disabled", target_unchecked_disabled);

		// Draw changes.
		radio->draw_without_layout();
	}

	void RadioButtonView_Impl::on_pointer_press(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		_state_pressed = true;
		update_state();
	}

	void RadioButtonView_Impl::on_pointer_release(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;

		_state_pressed = false;
		if (!_state_selected)
		{
			if (radio->geometry().border_box().contains(e.pos(radio) + radio->geometry().content_box().get_top_left()))	// Only allow click when mouse released over component
			{
				_state_selected = true;
				release_group_checked();
				if (_func_selected)
					_func_selected();
			}
		}

		update_state();
	}

	void RadioButtonView_Impl::release_group_checked()
	{
		auto view = radio->parent();
		if (view)
		{
			std::vector<std::shared_ptr<View>> children_copy = view->children();
			for (auto &view : children_copy)
			{
				if (view.get() != radio)
				{
					RadioButtonView *alt_radio = dynamic_cast<RadioButtonView*>(view.get());
					if (alt_radio)
					{
						if (alt_radio->impl->_group == _group)
						{
							alt_radio->set_selected(false);
						}
					}
				}
			}
		}
	}
}


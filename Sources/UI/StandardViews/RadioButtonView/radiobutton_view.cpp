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
	RadioButtonView::RadioButtonView() : impl(new RadioButtonView_Impl())
	{
		impl->radio = this;

		slots.connect(sig_pointer_press(), impl.get(), &RadioButtonView_Impl::on_pointer_press);
		slots.connect(sig_pointer_release(), impl.get(), &RadioButtonView_Impl::on_pointer_release);

		slots.connect(sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_hot = true;  impl->update_state(); });
		slots.connect(sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_hot = false;  impl->update_state(); });
	}

	void RadioButtonView::set_disabled()
	{
		if (!impl->_state_disabled)
		{
			impl->_state_disabled = true;
			impl->update_state();
		}
	}
	void RadioButtonView::set_enabled()
	{
		if (impl->_state_disabled)
		{
			impl->_state_disabled = false;
			impl->update_state();
		}
	}

	bool RadioButtonView::disabled() const
	{
		return impl->_state_disabled;
	}

	int RadioButtonView::radio_id() const
	{
		return impl->_id;
	}

	bool RadioButtonView::selected() const
	{
		return impl->_state_selected;
	}

	std::string RadioButtonView::group_name() const
	{
		return impl->_group;
	}

	void RadioButtonView::set_radio_id(int id)
	{
		impl->_id = id;
	}

	void RadioButtonView::set_selected(bool selected)
	{
		if (impl->_state_selected != selected)
		{
			impl->_state_selected = selected;
			if (selected)
			{
				impl->release_group_checked();
			}
			impl->update_state();
		}
	}

	void RadioButtonView::set_group_name(const std::string &str)
	{
		impl->_group = str;
	}

	std::function<void()> &RadioButtonView::func_selected()
	{
		return impl->_func_selected;
	}

}


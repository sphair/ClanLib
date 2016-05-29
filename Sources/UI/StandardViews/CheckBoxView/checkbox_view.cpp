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
**    Artem Khomenko (add label property)
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
	CheckBoxView::CheckBoxView() : impl(new CheckBoxView_Impl())
	{
		impl->checkbox = this;

		impl->label = std::make_shared<LabelView>();
		add_child(impl->label);

		slots.connect(sig_pointer_press(), impl.get(), &CheckBoxView_Impl::on_pointer_press);
		slots.connect(sig_pointer_release(), impl.get(), &CheckBoxView_Impl::on_pointer_release);

		slots.connect(sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_hot = true;  impl->update_state(); });
		slots.connect(sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_hot = false;  impl->update_state(); });
		slots.connect(impl->label->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_hot = true;  impl->update_state(); });
		slots.connect(impl->label->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_hot = false;  impl->update_state(); });
	}

	void CheckBoxView::set_disabled()
	{
		if (!impl->_state_disabled)
		{
			impl->_state_disabled = true;
			impl->update_state();
		}
	}
	void CheckBoxView::set_enabled()
	{
		if (impl->_state_disabled)
		{
			impl->_state_disabled = false;
			impl->update_state();
		}
	}

	bool CheckBoxView::disabled() const
	{
		return impl->_state_disabled;
	}

	bool CheckBoxView::checked() const
	{
		return impl->_checked_state;
	}

	void CheckBoxView::set_check(bool checked_state)
	{
		if (impl->_checked_state != checked_state)
		{
			impl->_checked_state = checked_state;
			impl->update_state();
		}
	}

	std::shared_ptr<LabelView> CheckBoxView::label()
	{
		return impl->label;
	}

	std::function<void()> &CheckBoxView::func_state_changed()
	{
		return impl->_func_state_changed;
	}

}


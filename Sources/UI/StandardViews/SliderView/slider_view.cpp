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
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/slider_view.h"
#include "API/UI/Style/style_property_parser.h"
#include "API/Display/2D/path.h"
#include "API/Display/System/timer.h"
#include "API/Display/2D/brush.h"
#include "API/UI/Events/pointer_event.h"
#include <algorithm>
#include "slider_view_impl.h"

namespace clan
{
	SliderView::SliderView() : impl(std::make_shared<SliderViewImpl>())
	{
		impl->slider = this;

		impl->track = std::make_shared<View>();
		impl->thumb = std::make_shared<View>();

		add_child(impl->track);
		add_child(impl->thumb);
	
		slots.connect(sig_pointer_press(), impl.get(), &SliderViewImpl::on_pointer_track_press);
		slots.connect(sig_pointer_double_click(), impl.get(), &SliderViewImpl::on_pointer_track_press);
		slots.connect(sig_pointer_release(), impl.get(), &SliderViewImpl::on_pointer_track_release);

		slots.connect(impl->thumb->sig_pointer_press(), impl.get(), &SliderViewImpl::on_pointer_thumb_press);
		slots.connect(impl->thumb->sig_pointer_double_click(), impl.get(), &SliderViewImpl::on_pointer_thumb_press);
		slots.connect(impl->thumb->sig_pointer_release(), impl.get(), &SliderViewImpl::on_pointer_thumb_release);

		slots.connect(impl->thumb->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_hot = true;  impl->update_state(); });
		slots.connect(impl->thumb->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_hot = false;  impl->update_state(); });

		slots.connect(impl->thumb->sig_pointer_move(), impl.get(), &SliderViewImpl::on_pointer_move);

		slots.connect(sig_focus_gained(), impl.get(), &SliderViewImpl::on_focus_gained);
		slots.connect(sig_focus_lost(), impl.get(), &SliderViewImpl::on_focus_lost);
		slots.connect(sig_activated(), impl.get(), &SliderViewImpl::on_activated);
		slots.connect(sig_activated(), impl.get(), &SliderViewImpl::on_deactivated);

		impl->scroll_timer.func_expired() = clan::bind_member(impl.get(), &SliderViewImpl::scroll_timer_expired);

		set_vertical();
	}

	bool SliderView::disabled() const
	{
		return impl->_state_disabled;
	}

	void SliderView::set_disabled()
	{
		if (!impl->_state_disabled)
		{
			impl->_state_disabled = true;
			impl->update_state();
			impl->mouse_down_mode = SliderViewImpl::mouse_down_none;
			impl->scroll_timer.stop();
		}
	}
	void SliderView::set_enabled()
	{
		if (impl->_state_disabled)
		{
			impl->_state_disabled = false;
			impl->update_state();
		}
	}

	std::shared_ptr<View> SliderView::track() const
	{
		return impl->track;
	}

	std::shared_ptr<View> SliderView::thumb() const
	{
		return impl->thumb;
	}

	bool SliderView::vertical() const
	{
		return impl->_vertical;
	}

	bool SliderView::horizontal() const
	{
		return !impl->_vertical;
	}

	void SliderView::set_vertical()
	{
		impl->_vertical = true;
		impl->thumb->style()->set("position: absolute; top: 0px; left: auto");
		set_needs_layout();

	}

	void SliderView::set_horizontal()
	{
		impl->_vertical = false;
		impl->thumb->style()->set("position: absolute; left: 0px; top: auto");
		set_needs_layout();
	}

	int SliderView::max_position() const
	{
		return impl->_max_position;
	}
	int SliderView::position() const
	{
		return impl->_position;
	}
	int SliderView::min_position() const
	{
		return impl->_min_position;
	}

	void SliderView::set_max_position(int value)
	{
		int new_min = std::min(impl->_min_position, value);
		int new_max = value;
		int new_pos = std::max(std::min(value, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void SliderView::set_position(int value)
	{
		int new_pos = std::max(std::min(value, impl->_max_position), impl->_min_position);
		impl->update_pos(this, new_pos, impl->_min_position, impl->_max_position);
	}

	void SliderView::set_min_position(int value)
	{
		int new_min = value;
		int new_max = std::max(impl->_max_position, value);
		int new_pos = std::max(std::min(impl->_position, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void SliderView::set_tick_count(int tick_count)
	{
		impl->_tick_count = tick_count;
		set_needs_layout();
	}
	void SliderView::set_page_step(int page_step)
	{
		impl->_page_step = page_step;
		set_needs_layout();
	}

	void SliderView::set_lock_to_ticks(bool lock)
	{
		impl->_lock_to_ticks = lock;
	}

	void SliderView::layout_children(Canvas &canvas)
	{
		View::layout_children(canvas);

		// Thumb position in screen pixels.
		float thumb_pos = (float)impl->thumb_pos();

		if (vertical())
		{
			impl->thumb->style()->set("top: %1px;", thumb_pos);
		}
		else
		{
			impl->thumb->style()->set("left: %1px;", thumb_pos);
		}
	}

	std::function<void()> &SliderView::func_value_changed()
	{
		return impl->_func_value_changed;
	}

}

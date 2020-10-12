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
**    Mark Page
**    Artem Khomenko
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/scrollbar_view.h"
#include "API/UI/Style/style_property_parser.h"
#include "API/Display/2D/path.h"
#include "API/Display/System/timer.h"
#include "API/Display/2D/brush.h"
#include "API/UI/Events/pointer_event.h"
#include <algorithm>
#include "scrollbar_view_impl.h"

namespace clan
{
	// Minimum size of the thumb, needed for show the grip.
	float cMinThumbSizeForShowGrip = 30.0f;

	ScrollBarView::ScrollBarView(bool render_button_arrows) : impl(std::make_shared<ScrollBarViewImpl>())
	{
		impl->scrollbar = this;

		impl->button_decrement = std::make_shared<ScrollBarButtonView>(render_button_arrows);
		impl->button_increment = std::make_shared<ScrollBarButtonView>(render_button_arrows);
		impl->track = std::make_shared<View>();
		impl->thumb = std::make_shared<View>();
		impl->thumb_grip = std::make_shared<View>();
		auto spacer1 = std::make_shared<View>();
		auto spacer2 = std::make_shared<View>();

		add_child(impl->button_decrement);
		add_child(impl->track);
		add_child(impl->button_increment);

		impl->track->add_child(impl->thumb);

		impl->thumb->add_child(spacer1);
		impl->thumb->add_child(impl->thumb_grip);
		impl->thumb->add_child(spacer2);

		impl->button_decrement->style()->set("flex: 0 0 auto");
		impl->button_increment->style()->set("flex: 0 0 auto");
		impl->track->style()->set("flex: 1 1 auto");
		impl->thumb->style()->set("position: absolute");
		spacer1->style()->set("flex: 1 1 auto");
		spacer2->style()->set("flex: 1 1 auto");

		impl->button_decrement->style()->set("width: 17px; height: 17px");
		impl->button_increment->style()->set("width: 17px; height: 17px");

		slots.connect(impl->track->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_track_press);
		slots.connect(impl->track->sig_pointer_double_click(), impl.get(), &ScrollBarViewImpl::on_pointer_track_press);
		slots.connect(impl->track->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_track_release);

		slots.connect(impl->thumb->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_thumb_press);
		slots.connect(impl->thumb->sig_pointer_double_click(), impl.get(), &ScrollBarViewImpl::on_pointer_thumb_press);
		slots.connect(impl->thumb->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_thumb_release);

		slots.connect(impl->button_decrement->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_decrement_press);
		slots.connect(impl->button_decrement->sig_pointer_double_click(), impl.get(), &ScrollBarViewImpl::on_pointer_decrement_press);
		slots.connect(impl->button_decrement->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_decrement_release);
		slots.connect(impl->button_increment->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_increment_press);
		slots.connect(impl->button_increment->sig_pointer_double_click(), impl.get(), &ScrollBarViewImpl::on_pointer_increment_press);
		slots.connect(impl->button_increment->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_increment_release);

		slots.connect(impl->thumb->sig_pointer_move(), impl.get(), &ScrollBarViewImpl::on_pointer_move);

		slots.connect(sig_focus_gained(), impl.get(), &ScrollBarViewImpl::on_focus_gained);
		slots.connect(sig_focus_lost(), impl.get(), &ScrollBarViewImpl::on_focus_lost);
		slots.connect(sig_activated(), impl.get(), &ScrollBarViewImpl::on_activated);
		slots.connect(sig_activated(), impl.get(), &ScrollBarViewImpl::on_deactivated);

		slots.connect(impl->track->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_track_hot = true;  impl->update_track_state(); });
		slots.connect(impl->track->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_track_hot = false;  impl->update_track_state(); });
		slots.connect(impl->thumb->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_thumb_hot = true;  impl->update_thumb_state(); });
		slots.connect(impl->thumb->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_thumb_hot = false;  impl->update_thumb_state(); });
		slots.connect(impl->thumb_grip->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_thumb_hot = true;  impl->update_thumb_state(); });
		slots.connect(impl->thumb_grip->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_thumb_hot = false;  impl->update_thumb_state(); });
		slots.connect(spacer1->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_thumb_hot = true;  impl->update_thumb_state(); });
		slots.connect(spacer1->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_thumb_hot = false;  impl->update_thumb_state(); });
		slots.connect(spacer2->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_thumb_hot = true;  impl->update_thumb_state(); });
		slots.connect(spacer2->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_thumb_hot = false;  impl->update_thumb_state(); });
		slots.connect(impl->button_decrement->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_decrement_hot = true;  impl->update_decrement_state(); });
		slots.connect(impl->button_decrement->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_decrement_hot = false;  impl->update_decrement_state(); });
		slots.connect(impl->button_increment->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_increment_hot = true;  impl->update_increment_state(); });
		slots.connect(impl->button_increment->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_increment_hot = false;  impl->update_increment_state(); });

		impl->scroll_timer.func_expired() = clan::bind_member(impl.get(), &ScrollBarViewImpl::scroll_timer_expired);

		set_vertical();
	}

	std::shared_ptr<View> ScrollBarView::button_decrement() const
	{
		return impl->button_decrement;
	}

	std::shared_ptr<View> ScrollBarView::button_increment() const
	{
		return impl->button_increment;
	}

	std::shared_ptr<View> ScrollBarView::track() const
	{
		return impl->track;
	}

	std::shared_ptr<View> ScrollBarView::thumb() const
	{
		return impl->thumb;
	}

	std::shared_ptr<View> ScrollBarView::thumb_grip() const
	{
		return impl->thumb_grip;
	}

	bool ScrollBarView::vertical() const
	{
		return style_cascade().computed_value(PropertyHash::hash_flex_direction).is_keyword("column");
	}

	bool ScrollBarView::horizontal() const
	{
		return !vertical();
	}

	void ScrollBarView::set_vertical()
	{
		style()->set("flex-direction: column");
		impl->button_decrement->style()->set("flex-direction: column");
		impl->button_increment->style()->set("flex-direction: column");
		impl->track->style()->set("flex-direction: column");
		impl->thumb->style()->set("flex-direction: column");
		impl->thumb_grip->style()->set("flex-direction: column");
		impl->button_decrement->set_direction(ScrollBarButtonDirection::up);
		impl->button_increment->set_direction(ScrollBarButtonDirection::down);
	}

	void ScrollBarView::set_horizontal()
	{
		style()->set("flex-direction: row");
		impl->button_decrement->style()->set("flex-direction: row");
		impl->button_increment->style()->set("flex-direction: row");
		impl->track->style()->set("flex-direction: row");
		impl->thumb->style()->set("flex-direction: row");
		impl->thumb_grip->style()->set("flex-direction: row");
		impl->button_decrement->set_direction(ScrollBarButtonDirection::left);
		impl->button_increment->set_direction(ScrollBarButtonDirection::right);
	}

	double ScrollBarView::line_step() const
	{
		return impl->line_step;
	}

	double ScrollBarView::page_step() const
	{
		return impl->page_step;
	}

	void ScrollBarView::set_line_step(double value)
	{
		impl->line_step = value;
	}

	void ScrollBarView::set_page_step(double value)
	{
		impl->page_step = value;
	}

	double ScrollBarView::min_position() const
	{
		return impl->min_pos;
	}

	double ScrollBarView::max_position() const
	{
		return impl->max_pos;
	}

	double ScrollBarView::position() const
	{
		return impl->pos;
	}

	bool ScrollBarView::disabled() const
	{
		return impl->_state_disabled;
	}

	void ScrollBarView::set_disabled()
	{
		if (!impl->_state_disabled)
		{
			impl->_state_disabled = true;
			impl->update_thumb_state();
			impl->update_track_state();
			impl->update_increment_state();
			impl->update_decrement_state();

			impl->mouse_down_mode = ScrollBarViewImpl::mouse_down_none;
			impl->scroll_timer.stop();
		}
	}

	void ScrollBarView::set_enabled()
	{
		if (impl->_state_disabled)
		{
			impl->_state_disabled = false;
			impl->update_thumb_state();
			impl->update_track_state();
			impl->update_increment_state();
			impl->update_decrement_state();
		}
	}

	void ScrollBarView::set_min_position(double value)
	{
		double new_min = value;
		double new_max = std::max(impl->max_pos, value);
		double new_pos = std::max(std::min(impl->pos, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void ScrollBarView::set_max_position(double value)
	{
		double new_min = std::min(impl->min_pos, value);
		double new_max = value;
		double new_pos = std::max(std::min(impl->pos, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void ScrollBarView::set_range(double min_value, double max_value)
	{
		double new_min = min_value;
		double new_max = max_value;
		double new_pos = std::max(std::min(impl->pos, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void ScrollBarView::set_position(double value)
	{
		double new_pos = std::max(std::min(value, impl->max_pos), impl->min_pos);
		impl->update_pos(this, new_pos, impl->min_pos, impl->max_pos);
	}

	void ScrollBarView::layout_children(Canvas &canvas)
	{
		View::layout_children(canvas);

		// Update the CSS properties.
		auto track_geometry = impl->track->geometry();

		if (impl->min_pos == impl->max_pos || impl->page_step == 0.0)
		{
			impl->thumb->style()->set("left: 0; top: 0; width: %1px; height: %2px", track_geometry.content_width, track_geometry.content_height);
		}
		else
		{
			double thumb_pos = impl->thumb_pos();
			double thumb_length = impl->thumb_length();

			if (vertical())
				impl->thumb->style()->set("left: 0; top: %1px; width: %2px; height: %3px", (float)thumb_pos, track_geometry.content_width, (float)thumb_length);
			else
				impl->thumb->style()->set("left: %1px; top: 0; width: %2px; height: %3px", (float)thumb_pos, (float)thumb_length, track_geometry.content_height);

			// Grip is visible only if there enough space.
			impl->thumb_grip->set_hidden(thumb_length < cMinThumbSizeForShowGrip);
		}
	}

	Signal<void()> &ScrollBarView::sig_scroll()
	{
		return impl->sig_scroll;
	}

	void ScrollBarView::set_lock_to_line(bool lock)
	{
		impl->_lock_to_line = lock;
	}

}

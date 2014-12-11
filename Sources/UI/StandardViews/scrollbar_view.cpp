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

#include "UI/precomp.h"
#include "API/UI/StandardViews/scrollbar_view.h"
#include <algorithm>

namespace clan
{
	class ScrollBarViewImpl
	{
	public:
		std::shared_ptr<View> button_decrement;
		std::shared_ptr<View> button_increment;
		std::shared_ptr<View> track;
		std::shared_ptr<View> thumb;
		std::shared_ptr<View> thumb_grip;
		double min_pos = 0.0;
		double max_pos = 100.0;
		double pos = 0.0;
		double line_step = 1.0;
		double page_step = 25.0;
		Signal<void()> sig_scroll;

		void update_pos(ScrollBarView *view, double new_pos, double new_min, double new_max)
		{
			bool changed = new_min != min_pos || new_max != max_pos || new_pos != pos;
			if (changed)
			{
				min_pos = new_min;
				max_pos = new_max;
				pos = new_pos;
				view->set_needs_layout();
			}
		}
	};

	ScrollBarView::ScrollBarView() : impl(std::make_shared<ScrollBarViewImpl>())
	{
		impl->button_decrement = std::make_shared<View>();
		impl->button_increment = std::make_shared<View>();
		impl->track = std::make_shared<View>();
		impl->thumb = std::make_shared<View>();
		impl->thumb_grip = std::make_shared<View>();
		auto spacer1 = std::make_shared<View>();
		auto spacer2 = std::make_shared<View>();

		add_subview(impl->button_decrement);
		add_subview(impl->track);
		add_subview(impl->button_increment);

		impl->track->add_subview(impl->thumb);

		impl->thumb->add_subview(spacer1);
		impl->thumb->add_subview(impl->thumb_grip);
		impl->thumb->add_subview(spacer2);

		impl->button_decrement->box_style.set_flex(0.0f, 0.0f);
		impl->button_increment->box_style.set_flex(0.0f, 0.0f);
		impl->track->box_style.set_flex(1.0f, 1.0f);
		impl->thumb->box_style.set_absolute();
		spacer1->box_style.set_flex(1.0f, 1.0f);
		spacer2->box_style.set_flex(1.0f, 1.0f);

		impl->button_decrement->box_style.set_width(17.0f);
		impl->button_decrement->box_style.set_height(17.0f);
		impl->button_increment->box_style.set_width(17.0f);
		impl->button_increment->box_style.set_height(17.0f);

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
		return box_style.is_layout_vbox();
	}

	bool ScrollBarView::horizontal() const
	{
		return !vertical();
	}

	void ScrollBarView::set_vertical()
	{
		box_style.set_layout_vbox();
		impl->button_decrement->box_style.set_layout_vbox();
		impl->button_increment->box_style.set_layout_vbox();
		impl->track->box_style.set_layout_vbox();
		impl->thumb->box_style.set_layout_vbox();
		impl->thumb_grip->box_style.set_layout_vbox();
	}

	void ScrollBarView::set_horizontal()
	{
		box_style.set_layout_hbox();
		impl->button_decrement->box_style.set_layout_hbox();
		impl->button_increment->box_style.set_layout_hbox();
		impl->track->box_style.set_layout_hbox();
		impl->thumb->box_style.set_layout_hbox();
		impl->thumb_grip->box_style.set_layout_hbox();
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

	void ScrollBarView::set_min_position(double value)
	{
		double new_min = value;
		double new_max = std::max(impl->max_pos, value);
		double new_pos = std::max(std::min(value, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void ScrollBarView::set_max_position(double value)
	{
		double new_min = std::min(impl->min_pos, value);
		double new_max = value;
		double new_pos = std::max(std::min(value, new_max), new_min);
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

	void ScrollBarView::layout_subviews(Canvas &canvas)
	{
		View::layout_subviews(canvas);

		auto track_geometry = impl->track->geometry();

		if (impl->min_pos == impl->max_pos || impl->page_step == 0.0)
		{
			impl->thumb->box_style.set_left(0.0f);
			impl->thumb->box_style.set_top(0.0f);
			impl->thumb->box_style.set_width(track_geometry.content.get_width());
			impl->thumb->box_style.set_height(track_geometry.content.get_height());
		}
		else
		{
			double content_size = impl->max_pos - impl->min_pos + impl->page_step;
			double track_length = vertical() ? track_geometry.content_box().get_height() : track_geometry.content_box().get_width();
			double thumb_length = impl->page_step / content_size * track_length;

			thumb_length = std::min(std::max(thumb_length, 16.0), track_length);

			double t = (impl->pos - impl->min_pos) / (impl->max_pos - impl->min_pos);
			double thumb_pos = t * (track_length - thumb_length);

			if (vertical())
			{
				impl->thumb->box_style.set_left(0.0f);
				impl->thumb->box_style.set_top((float)thumb_pos);
				impl->thumb->box_style.set_width(track_geometry.content.get_width());
				impl->thumb->box_style.set_height((float)thumb_length);
			}
			else
			{
				impl->thumb->box_style.set_left((float)thumb_pos);
				impl->thumb->box_style.set_top(0.0f);
				impl->thumb->box_style.set_width((float)thumb_length);
				impl->thumb->box_style.set_height(track_geometry.content.get_height());
			}
		}

	}

	Signal<void()> &ScrollBarView::sig_scroll()
	{
		return impl->sig_scroll;
	}
}

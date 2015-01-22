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
#include "API/Display/2D/path.h"
#include "API/Display/System/timer.h"
#include "API/Display/2D/brush.h"
#include "API/UI/Events/pointer_event.h"
#include <algorithm>

namespace clan
{
	enum class ScrollBarButtonDirection
	{
		left,
		right,
		up,
		down
	};

	class ScrollBarButtonView : public View
	{
	public:
		ScrollBarButtonView()
		{
		}

		void render_content(Canvas &canvas) override
		{
			Rectf box = geometry().content_box().get_size();
			box.shrink(4.0f, 6.0f);

			Path path;
			path.set_fill_mode(PathFillMode::alternate);
			switch (direction)
			{
			case ScrollBarButtonDirection::left:
				path.move_to(box.right, box.top);
				path.line_to(box.right, box.bottom);
				path.line_to(box.left, box.top + box.get_height() * 0.5f);
				path.close();
				break;
			case ScrollBarButtonDirection::right:
				path.move_to(box.left, box.top);
				path.line_to(box.left, box.bottom);
				path.line_to(box.right, box.top + box.get_height() * 0.5f);
				path.close();
				break;
			case ScrollBarButtonDirection::up:
				path.move_to(box.left, box.bottom);
				path.line_to(box.right, box.bottom);
				path.line_to(box.left + box.get_width() * 0.5f, box.top);
				path.close();
				break;
			case ScrollBarButtonDirection::down:
				path.move_to(box.left, box.top);
				path.line_to(box.right, box.top);
				path.line_to(box.left + box.get_width() * 0.5f, box.bottom);
				path.close();
				break;
			}
			path.fill(canvas, Colorf(134, 137, 153));
		}

		void set_direction(ScrollBarButtonDirection new_dir)
		{
			if (new_dir != direction)
			{
				direction = new_dir;
				set_needs_render();
			}
		}

	private:
		ScrollBarButtonDirection direction = ScrollBarButtonDirection::left;
	};

	class ScrollBarViewImpl
	{
	public:

		void on_pointer_track_press(PointerEvent &e);
		void on_pointer_track_release(PointerEvent &e);
		void on_pointer_thumb_press(PointerEvent &e);
		void on_pointer_thumb_release(PointerEvent &e);
		void on_pointer_decrement_press(PointerEvent &e);
		void on_pointer_decrement_release(PointerEvent &e);
		void on_pointer_increment_press(PointerEvent &e);
		void on_pointer_increment_release(PointerEvent &e);

		void on_pointer_move(PointerEvent &e);
		void on_focus_gained(FocusChangeEvent &e);
		void on_focus_lost(FocusChangeEvent &e);
		void on_activated(ActivationChangeEvent &e);
		void on_deactivated(ActivationChangeEvent &e);

		void scroll_timer_expired();

		enum MouseDownMode
		{
			mouse_down_none,
			mouse_down_button_decr,
			mouse_down_button_incr,
			mouse_down_track_decr,
			mouse_down_track_incr,
			mouse_down_thumb_drag
		} mouse_down_mode = mouse_down_none;

		ScrollBarView *scrollbar = nullptr;

		std::shared_ptr<ScrollBarButtonView> button_decrement;
		std::shared_ptr<ScrollBarButtonView> button_increment;
		std::shared_ptr<View> track;
		std::shared_ptr<View> thumb;
		std::shared_ptr<View> thumb_grip;
		double min_pos = 0.0;
		double max_pos = 100.0;
		double pos = 0.0;
		double line_step = 1.0;
		double page_step = 25.0;
		double timer_step_size = 0.0;
		double timer_target_position = 0.0;
		double thumb_move_start_position = 0.0;
		Pointf mouse_drag_start_pos;
		Signal<void()> sig_scroll;
		Timer scroll_timer;

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
		impl->scrollbar = this;

		impl->button_decrement = std::make_shared<ScrollBarButtonView>();
		impl->button_increment = std::make_shared<ScrollBarButtonView>();
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

		slots.connect(impl->track->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_track_press);
		slots.connect(impl->track->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_track_release);

		//FIXME thumb grip works, but thumb doesn't
		slots.connect(impl->thumb_grip->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_thumb_press);
		slots.connect(impl->thumb_grip->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_thumb_release);
		slots.connect(impl->thumb->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_thumb_press);
		slots.connect(impl->thumb->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_thumb_release);

		slots.connect(impl->button_decrement->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_decrement_press);
		slots.connect(impl->button_decrement->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_decrement_release);
		slots.connect(impl->button_increment->sig_pointer_press(), impl.get(), &ScrollBarViewImpl::on_pointer_increment_press);
		slots.connect(impl->button_increment->sig_pointer_release(), impl.get(), &ScrollBarViewImpl::on_pointer_increment_release);

		//FIXME thumb grip works, but thumb doesn't
		slots.connect(impl->thumb_grip->sig_pointer_move(), impl.get(), &ScrollBarViewImpl::on_pointer_move);
		slots.connect(impl->thumb->sig_pointer_move(), impl.get(), &ScrollBarViewImpl::on_pointer_move);

		slots.connect(sig_focus_gained(), impl.get(), &ScrollBarViewImpl::on_focus_gained);
		slots.connect(sig_focus_lost(), impl.get(), &ScrollBarViewImpl::on_focus_lost);
		slots.connect(sig_activated(), impl.get(), &ScrollBarViewImpl::on_activated);
		slots.connect(sig_activated(), impl.get(), &ScrollBarViewImpl::on_deactivated);

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
		impl->button_decrement->set_direction(ScrollBarButtonDirection::up);
		impl->button_increment->set_direction(ScrollBarButtonDirection::down);
	}

	void ScrollBarView::set_horizontal()
	{
		box_style.set_layout_hbox();
		impl->button_decrement->box_style.set_layout_hbox();
		impl->button_increment->box_style.set_layout_hbox();
		impl->track->box_style.set_layout_hbox();
		impl->thumb->box_style.set_layout_hbox();
		impl->thumb_grip->box_style.set_layout_hbox();
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


	void ScrollBarViewImpl::on_focus_gained(FocusChangeEvent &e)
	{
	}

	void ScrollBarViewImpl::on_focus_lost(FocusChangeEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void ScrollBarViewImpl::on_activated(ActivationChangeEvent &e)
	{

	}

	void ScrollBarViewImpl::on_deactivated(ActivationChangeEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void ScrollBarViewImpl::on_pointer_track_press(PointerEvent &e)
	{
		float mouse_pos;
		Rectf thumb_geometry(thumb->geometry().content_box());
		float thumb_position;
		if (scrollbar->horizontal())
		{
			mouse_pos = e.pos(track.get()).x;
			thumb_position = thumb_geometry.left + thumb_geometry.get_width() / 2.0f;
			timer_target_position = min_pos + mouse_pos * ((max_pos - min_pos)) / (track->geometry().content_box().get_width());
		}
		else
		{
			mouse_pos = e.pos(track.get()).y;
			thumb_position = thumb_geometry.top + thumb_geometry.get_height() / 2.0f;
			timer_target_position = min_pos + mouse_pos * ((max_pos - min_pos)) / (track->geometry().content_box().get_height());
		}

		if (mouse_pos < thumb_position)
		{
			mouse_down_mode = mouse_down_track_decr;
			timer_step_size = -page_step;
		}
		else
		{
			mouse_down_mode = mouse_down_track_incr;
			timer_step_size = page_step;
		}

		scroll_timer_expired();

	}

	void ScrollBarViewImpl::on_pointer_track_release(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();

	}

	void ScrollBarViewImpl::on_pointer_thumb_press(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_thumb_drag;
		thumb_move_start_position = pos;
		mouse_drag_start_pos = e.pos(track.get());
	}

	void ScrollBarViewImpl::on_pointer_thumb_release(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_none;

	}

	void ScrollBarViewImpl::on_pointer_decrement_press(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_button_decr;
		timer_step_size = -line_step;
		scroll_timer_expired();
	}

	void ScrollBarViewImpl::on_pointer_decrement_release(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void ScrollBarViewImpl::on_pointer_increment_press(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_button_incr;
		timer_step_size = line_step;
		scroll_timer_expired();
	}

	void ScrollBarViewImpl::on_pointer_increment_release(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void ScrollBarViewImpl::on_pointer_move(PointerEvent &e)
	{
		if (mouse_down_mode != mouse_down_thumb_drag)
			return;

		Pointf mouse_pos(e.pos(track.get()));
		Rectf thumb_geometry(thumb->geometry().content_box());
		Rectf track_geometry(track->geometry().content_box());

		double last_position = pos;

		if (mouse_pos.x < -100 || mouse_pos.x > track_geometry.get_width() + 100 || mouse_pos.y < -100 || mouse_pos.y > track_geometry.get_height() + 100)
		{
			pos = thumb_move_start_position;
		}
		else
		{
			if (scrollbar->horizontal())
			{
				double delta = (mouse_pos.x - mouse_drag_start_pos.x);
				pos = thumb_move_start_position + delta * ((max_pos - min_pos)) / (track->geometry().content_box().get_width());
			}
			else
			{
				double delta = (mouse_pos.y - mouse_drag_start_pos.y);
				pos = thumb_move_start_position + delta * ((max_pos - min_pos)) / (track->geometry().content_box().get_height());
			}
		}
		if (pos > max_pos)
			pos = max_pos;
		if (pos < min_pos)
			pos = min_pos;

		if (last_position != pos)
		{
			sig_scroll();
			scrollbar->set_needs_layout();
		}

	}


	void ScrollBarViewImpl::scroll_timer_expired()
	{
		if ((mouse_down_mode == mouse_down_none) || (mouse_down_mode == mouse_down_thumb_drag))
			return;

		double last_position = pos;
		pos += timer_step_size;

		if (mouse_down_mode == mouse_down_track_decr)
		{
			if (pos < timer_target_position)
				pos = timer_target_position;
		}
		if (mouse_down_mode == mouse_down_track_incr)
		{
			if (pos > timer_target_position)
				pos = timer_target_position;
		}

		if (pos > max_pos)
			pos = max_pos;
		if (pos < min_pos)
			pos = min_pos;

		if (last_position != pos)
		{
			sig_scroll();
			scrollbar->set_needs_layout();
		}
		// FIXME - The scroll timer should work with "false" as a parameter. Well, ideally, this timer should be set to repeat outside of this function. But it shows a problem
		scroll_timer.start(100, true);

	}

}

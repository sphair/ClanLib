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

namespace clan
{
	class SliderViewImpl
	{
	public:

		void on_pointer_track_press(PointerEvent &e);
		void on_pointer_track_release(PointerEvent &e);
		void on_pointer_thumb_press(PointerEvent &e);
		void on_pointer_thumb_release(PointerEvent &e);

		void on_pointer_move(PointerEvent &e);
		void on_focus_gained(FocusChangeEvent &e);
		void on_focus_lost(FocusChangeEvent &e);
		void on_activated(ActivationChangeEvent &e);
		void on_deactivated(ActivationChangeEvent &e);

		void scroll_timer_expired();

		enum MouseDownMode
		{
			mouse_down_none,
			mouse_down_track_decr,
			mouse_down_track_incr,
			mouse_down_thumb_drag
		} mouse_down_mode = mouse_down_none;

		SliderView *slider = nullptr;

		std::shared_ptr<View> track;
		std::shared_ptr<View> thumb;
		Pointf mouse_drag_start_pos;
		Signal<void()> sig_value_changed;
		Timer scroll_timer;

		int _min_position = 0;
		int _max_position = 100;
		int _position = 50;
		bool _vertical = false;
		int _tick_count = 1;
		int _page_step = 1;
		bool _lock_to_ticks = false;

		int timer_step_size = 0;
		int timer_target_position = 0;
		int thumb_move_start_position = 0;

		void update_pos(SliderView *view, int new_pos, int new_min, int new_max);

	};

	void SliderViewImpl::update_pos(SliderView *view, int new_pos, int new_min, int new_max)
	{
		bool changed = new_min != _min_position || new_max != _max_position || new_pos != _position;
		if (changed)
		{
			_min_position = new_min;
			_max_position = new_max;
			_position = new_pos;
			view->set_needs_layout();
		}
	}
	SliderView::SliderView() : impl(std::make_shared<SliderViewImpl>())
	{
		impl->slider = this;

		impl->track = std::make_shared<View>();
		impl->thumb = std::make_shared<View>();

		add_subview(impl->track);
		add_subview(impl->thumb);
	
		slots.connect(sig_pointer_press(), impl.get(), &SliderViewImpl::on_pointer_track_press);
		slots.connect(sig_pointer_release(), impl.get(), &SliderViewImpl::on_pointer_track_release);

		slots.connect(impl->thumb->sig_pointer_press(), impl.get(), &SliderViewImpl::on_pointer_thumb_press);
		slots.connect(impl->thumb->sig_pointer_release(), impl.get(), &SliderViewImpl::on_pointer_thumb_release);

		slots.connect(impl->thumb->sig_pointer_move(), impl.get(), &SliderViewImpl::on_pointer_move);

		slots.connect(sig_focus_gained(), impl.get(), &SliderViewImpl::on_focus_gained);
		slots.connect(sig_focus_lost(), impl.get(), &SliderViewImpl::on_focus_lost);
		slots.connect(sig_activated(), impl.get(), &SliderViewImpl::on_activated);
		slots.connect(sig_activated(), impl.get(), &SliderViewImpl::on_deactivated);

		impl->scroll_timer.func_expired() = clan::bind_member(impl.get(), &SliderViewImpl::scroll_timer_expired);

		set_vertical();
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
		//TODO: Enable and fix this, keeping in mind what happens when you switch from vertical to horizontal

		//style()->set("flex-direction: column;");
		//impl->track->style()->set("flex: 1 1 main-size;");
		//impl->thumb->style()->set("position: absolute; height: 4px;  width: auto");
	}

	void SliderView::set_horizontal()
	{
		impl->_vertical = false;
		//TODO: Where is the best place to have these? In this function, in another function, in the application...
		style()->set("flex-direction: row;");
		impl->track->style()->set("flex: 1 1 main-size; height: 4px; margin: 8px 0px");
		impl->thumb->style()->set("position: absolute; width: 8px; height: 32px");
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
		int new_pos = std::max(std::min(value, new_max), new_min);
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

	void SliderView::layout_subviews(Canvas &canvas)
	{
		View::layout_subviews(canvas);

		auto track_geometry = impl->track->geometry();
		auto thumb_geometry = impl->thumb->geometry();

		float track_length = vertical() ? track_geometry.content_box().get_height() : track_geometry.content_box().get_width();
		float thumb_length = vertical() ? thumb_geometry.content_box().get_height() : thumb_geometry.content_box().get_width();;

		float t = (float) (impl->_position - impl->_min_position) / (float) (impl->_max_position - impl->_min_position);
		float thumb_pos = t * (track_length - thumb_length);

		if (vertical())
		{
			impl->thumb->style()->set("top: %1px;", thumb_pos);
		}
		else
		{
			impl->thumb->style()->set("left: %1px;", thumb_pos);
		}
	}

	Signal<void()> &SliderView::sig_value_changed()
	{
		return impl->sig_value_changed;
	}

	void SliderViewImpl::on_focus_gained(FocusChangeEvent &e)
	{
	}

	void SliderViewImpl::on_focus_lost(FocusChangeEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void SliderViewImpl::on_activated(ActivationChangeEvent &e)
	{

	}

	void SliderViewImpl::on_deactivated(ActivationChangeEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void SliderViewImpl::on_pointer_track_press(PointerEvent &e)
	{
		if (e.target() == thumb)	// Thumb control handled elsewhere
			return;

		float mouse_pos;
		Rectf thumb_geometry(thumb->geometry().content_box());
		float thumb_position;
		if (slider->horizontal())
		{
			mouse_pos = e.pos(track.get()).x;
			thumb_position = thumb_geometry.left + thumb_geometry.get_width() / 2.0f;
			timer_target_position = _min_position + mouse_pos * ((_max_position - _min_position)) / (track->geometry().content_box().get_width());
		}
		else
		{
			mouse_pos = e.pos(track.get()).y;
			thumb_position = thumb_geometry.top + thumb_geometry.get_height() / 2.0f;
			timer_target_position = _min_position + mouse_pos * ((_max_position - _min_position)) / (track->geometry().content_box().get_height());
		}

		if (mouse_pos < thumb_position)
		{
			mouse_down_mode = mouse_down_track_decr;
			timer_step_size = -_page_step;
		}
		else
		{
			mouse_down_mode = mouse_down_track_incr;
			timer_step_size = _page_step;
		}

		scroll_timer_expired();
	}

	void SliderViewImpl::on_pointer_track_release(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void SliderViewImpl::on_pointer_thumb_press(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_thumb_drag;
		thumb_move_start_position = _position;
		mouse_drag_start_pos = e.pos(track.get());
	}

	void SliderViewImpl::on_pointer_thumb_release(PointerEvent &e)
	{
		mouse_down_mode = mouse_down_none;
	}

	void SliderViewImpl::on_pointer_move(PointerEvent &e)
	{
		if (mouse_down_mode != mouse_down_thumb_drag)
			return;

		Pointf mouse_pos(e.pos(track.get()));
		Rectf track_geometry(track->geometry().content_box());

		int last_position = _position;

		if (mouse_pos.x < -100 || mouse_pos.x > track_geometry.get_width() + 100 || mouse_pos.y < -100 || mouse_pos.y > track_geometry.get_height() + 100)
		{
			_position = thumb_move_start_position;
		}
		else
		{
			if (slider->horizontal())
			{
				int delta = (mouse_pos.x - mouse_drag_start_pos.x);
				_position = thumb_move_start_position + (delta * (_max_position - _min_position)) / (track->geometry().content_box().get_width());
			}
			else
			{
				int delta = (mouse_pos.y - mouse_drag_start_pos.y);
				_position = thumb_move_start_position + (delta * (_max_position - _min_position)) / (track->geometry().content_box().get_height());
			}
		}
		if (_position > _max_position)
			_position = _max_position;
		if (_position < _min_position)
			_position = _min_position;

		if (_lock_to_ticks)
		{
			int remainder = (_position - _min_position) % _tick_count;
			_position = _position - remainder;
		}

		if (last_position != _position)
		{
			sig_value_changed();
			slider->set_needs_layout();
		}
	}
	
	void SliderViewImpl::scroll_timer_expired()
	{
		if ((mouse_down_mode == mouse_down_none) || (mouse_down_mode == mouse_down_thumb_drag))
			return;

		int last_position = _position;
		_position += timer_step_size;

		if (mouse_down_mode == mouse_down_track_decr)
		{
			if (_position < timer_target_position)
				_position = timer_target_position;
		}
		if (mouse_down_mode == mouse_down_track_incr)
		{
			if (_position > timer_target_position)
				_position = timer_target_position;
		}

		if (_position > _max_position)
			_position = _max_position;
		if (_position < _min_position)
			_position = _min_position;

		if (last_position != _position)
		{
			sig_value_changed();
			slider->set_needs_layout();
		}
		scroll_timer.start(100, false);

	}

}

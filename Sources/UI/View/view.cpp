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
#include "API/UI/View/view.h"
#include "API/UI/View/root_view.h"
#include "API/Display/2D/canvas.h"
#include "API/UI/Events/event.h"
#include "API/UI/Events/activation_change_event.h"
#include "API/UI/Events/close_event.h"
#include "API/UI/Events/focus_change_event.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/resize_event.h"
#include "API/UI/UIThread/ui_thread.h"
#include "view_impl.h"
#include "vbox_layout.h"
#include "hbox_layout.h"
#include <algorithm>

namespace clan
{
	View::View() : impl(new ViewImpl())
	{
		//box_style.set_style_changed(bind_member(this, &View::set_needs_layout));
	}

	View::~View()
	{
	}

	const StyleCascade &View::style_cascade() const
	{
		return impl->style_cascade;
	}

	const std::shared_ptr<Style> &View::style(const std::string &state) const
	{
		const auto it = impl->styles.find(state);
		if (it != impl->styles.end())
			return it->second;

		auto &style = impl->styles[state];
		style = std::make_shared<Style>();
		impl->update_style_cascade();
		return style;
	}

	bool View::state(const std::string &name) const
	{
		const auto it = impl->states.find(name);
		if (it != impl->states.end())
			return it->second.enabled;
		else
			return false;
	}
	
	void View::set_state(const std::string &name, bool value)
	{
		if (impl->states[name].enabled != value)
		{
			impl->states[name] = ViewImpl::StyleState(false, value);
			impl->update_style_cascade();
			set_needs_layout();
		}
	}
	void View::set_state_cascade(const std::string &name, bool value)
	{
		if (impl->states[name].enabled != value)
		{
			impl->states[name] = ViewImpl::StyleState(false, value);
			impl->update_style_cascade();
			set_needs_layout();
			impl->set_state_cascade_siblings(name, value);
		}
	}

	void ViewImpl::set_state_cascade_siblings(const std::string &name, bool value)
	{
		for (std::shared_ptr<View> &view : _subviews)
		{
			ViewImpl *impl = view->impl.get();
			if (impl->states[name].inherited)
			{
				impl->states[name] = ViewImpl::StyleState(true, value);
				impl->update_style_cascade();
				view->set_needs_layout();
				impl->set_state_cascade_siblings(name, value);
			}
		}
	}

	View *View::superview() const
	{
		return impl->_superview;
	}

	const std::vector<std::shared_ptr<View>> &View::subviews() const
	{
		return impl->_subviews;
	}

	void View::add_subview(const std::shared_ptr<View> &view)
	{
		if (view)
		{
			view->remove_from_super();

			impl->_subviews.push_back(view);
			view->impl->_superview = this;
			view->set_needs_layout();
			set_needs_layout();

			subview_added(view);
		}
	}

	void View::remove_from_super()
	{
		View *super = impl->_superview;
		if (super)
		{
			std::shared_ptr<View> view_ptr = shared_from_this();

			// To do: clear owner_view, focus_view, if it is this view or a child

			auto it = std::find_if(super->impl->_subviews.begin(), super->impl->_subviews.end(), [&](const std::shared_ptr<View> &view) { return view.get() == this; });
			if (it != super->impl->_subviews.end())
				super->impl->_subviews.erase(it);
			impl->_superview = nullptr;

			super->set_needs_layout();

			subview_removed(view_ptr);
		}
	}

	bool View::hidden() const
	{
		if (local_root())
		{
			return static_cast<const RootView *>(this)->root_hidden();
		}
		else
		{
			return impl->hidden;
		}
	}

	void View::set_hidden(bool value)
	{
		if (local_root())
		{
			return static_cast<RootView *>(this)->set_root_hidden(value);
		}
		else
		{
			if (value != impl->hidden)
			{
				impl->hidden = value;
				set_needs_layout();
			}
		}
	}

	bool View::is_static_position_and_visible() const
	{
		return style_cascade().computed_value("position").is_keyword("static") && !hidden();
	}

	bool View::needs_layout() const
	{
		return impl->_needs_layout;
	}

	void View::set_needs_layout()
	{
		impl->_needs_layout = true;
		impl->layout_cache.clear();

		View *super = superview();
		if (super)
			super->set_needs_layout();
		else
			set_needs_render();
	}

	Canvas View::get_canvas() const
	{
		if (local_root())
			return static_cast<const RootView *>(this)->get_root_canvas();

		View *super = superview();
		if (super)
			return super->get_canvas();
		else
			return Canvas();
	}

	void View::set_needs_render()
	{
		if (local_root())
		{
			static_cast<RootView *>(this)->set_root_needs_render();
		}
		else
		{
			View *super = superview();
			if (super)
				super->set_needs_render();
		}
	}

	const ViewGeometry &View::geometry() const
	{
		return impl->_geometry;
	}

	void View::set_geometry(const ViewGeometry &geometry)
	{
		if (impl->_geometry.content_box() != geometry.content_box())
		{
			impl->_geometry = geometry;
			set_needs_layout();
		}
	}

	bool View::render_exception_encountered() const
	{
		return impl->exception_encountered;
	}

	void View::clear_exception_encountered()
	{
		if (impl->exception_encountered)
		{
			impl->exception_encountered = false;
			set_needs_render();
		}
	}

	const Mat4f &View::view_transform() const
	{
		return impl->view_transform;
	}

	void View::set_view_transform(const Mat4f &transform)
	{
		impl->view_transform = transform;
		set_needs_render();
	}

	bool View::content_clipped() const
	{
		return impl->content_clipped;
	}

	void View::set_content_clipped(bool clipped)
	{
		if (impl->content_clipped != clipped)
		{
			impl->content_clipped = clipped;
			set_needs_render();
		}
	}

	float View::get_preferred_width(Canvas &canvas)
	{
		if (!impl->layout_cache.preferred_width_calculated)
		{
			impl->layout_cache.preferred_width = calculate_preferred_width(canvas);
			impl->layout_cache.preferred_width_calculated = true;
		}
		return impl->layout_cache.preferred_width;
	}

	float View::get_preferred_height(Canvas &canvas, float width)
	{
		auto it = impl->layout_cache.preferred_height.find(width);
		if (it != impl->layout_cache.preferred_height.end())
			return it->second;

		float height = calculate_preferred_height(canvas, width);
		impl->layout_cache.preferred_height[width] = height;
		return height;
	}

	float View::get_first_baseline_offset(Canvas &canvas, float width)
	{
		auto it = impl->layout_cache.first_baseline_offset.find(width);
		if (it != impl->layout_cache.first_baseline_offset.end())
			return it->second;

		float baseline_offset = calculate_first_baseline_offset(canvas, width);
		impl->layout_cache.first_baseline_offset[width] = baseline_offset;
		return baseline_offset;
	}

	float View::get_last_baseline_offset(Canvas &canvas, float width)
	{
		auto it = impl->layout_cache.last_baseline_offset.find(width);
		if (it != impl->layout_cache.last_baseline_offset.end())
			return it->second;

		float baseline_offset = calculate_last_baseline_offset(canvas, width);
		impl->layout_cache.last_baseline_offset[width] = baseline_offset;
		return baseline_offset;
	}

	float View::calculate_preferred_width(Canvas &canvas)
	{
		if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("column"))
			return VBoxLayout::get_preferred_width(canvas, this);
		else if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("row"))
			return HBoxLayout::get_preferred_width(canvas, this);
		else if (style_cascade().computed_value("width").is_keyword("auto"))
			return 0.0f;
		else
			return style_cascade().computed_value("width").number();
	}

	float View::calculate_preferred_height(Canvas &canvas, float width)
	{
		if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("column"))
			return VBoxLayout::get_preferred_height(canvas, this, width);
		else if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("row"))
			return HBoxLayout::get_preferred_height(canvas, this, width);
		else if (style_cascade().computed_value("height").is_keyword("auto"))
			return 0.0f;
		else
			return style_cascade().computed_value("height").number();
	}

	float View::calculate_first_baseline_offset(Canvas &canvas, float width)
	{
		if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("column"))
			return VBoxLayout::get_first_baseline_offset(canvas, this, width);
		else if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("row"))
			return HBoxLayout::get_first_baseline_offset(canvas, this, width);
		else
			return 0.0f;
	}

	float View::calculate_last_baseline_offset(Canvas &canvas, float width)
	{
		if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("column"))
			return VBoxLayout::get_last_baseline_offset(canvas, this, width);
		else if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("row"))
			return HBoxLayout::get_last_baseline_offset(canvas, this, width);
		else
			return 0.0f;
	}

	bool View::local_root() const
	{
		return dynamic_cast<const RootView *>(this) != nullptr;
	}

	void View::layout_subviews(Canvas &canvas)
	{
		if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("column"))
			VBoxLayout::layout_subviews(canvas, this);
		else if (style_cascade().computed_value("layout").is_keyword("flex") && style_cascade().computed_value("flex-direction").is_keyword("row"))
			HBoxLayout::layout_subviews(canvas, this);
	}

	View *View::root_view()
	{
		View *super = superview();
		if (super)
			return super->root_view();
		else
			return this;
	}

	const View *View::root_view() const
	{
		View *super = superview();
		if (super)
			return super->root_view();
		else
			return this;
	}

	View *View::owner_view() const
	{
		return root_view()->impl->_owner_view;
	}

	View *View::focus_view() const
	{
		return root_view()->impl->_focus_view;
	}

	std::shared_ptr<View> View::find_view_at(const Pointf &pos) const
	{
		for (unsigned int cnt = impl->_subviews.size(); cnt > 0; --cnt)	// Search the subviews in reverse order, as we want to search the view that was "last drawn" first
		{
			const std::shared_ptr<View> &child = impl->_subviews[cnt-1];
			if (child->geometry().border_box().contains(pos) && !child->hidden() && !child->local_root())
			{
				Pointf child_content_pos(pos.x - child->geometry().content_x, pos.y - child->geometry().content_y);
				child_content_pos = Vec2f(Mat4f::inverse(child->view_transform()) * Vec4f(child_content_pos, 0.0f, 1.0f));
				std::shared_ptr<View> view = child->find_view_at(child_content_pos);
				if (view)
					return view;
				else
					return child;
			}
		}

		return std::shared_ptr<View>();
	}

	FocusPolicy View::focus_policy() const
	{
		return impl->focus_policy;
	}

	void View::set_focus_policy(FocusPolicy policy)
	{
		impl->focus_policy = policy;
	}

	unsigned int View::tab_index() const
	{
		return impl->tab_index;
	}

	void View::set_tab_index(unsigned int index)
	{
		impl->tab_index = index;
	}

	void View::set_focus()
	{
		View *root = root_view();
		View *old_focus_view = root->impl->_focus_view;

		if (old_focus_view == this)
			return;

		root->impl->_focus_view = this;

		if (old_focus_view)
		{
			FocusChangeEvent focus_loss(FocusChangeType::lost);
			View::dispatch_event(old_focus_view, &focus_loss, true);
		}

		FocusChangeEvent focus_gain(FocusChangeType::gained);
		View::dispatch_event(this, &focus_gain, true);
	}

	void View::remove_focus()
	{
		View *root = root_view();
		View *old_focus_view = root->impl->_focus_view;

		root->impl->_focus_view = nullptr;

		if (old_focus_view)
		{
			FocusChangeEvent focus_loss(FocusChangeType::lost);
			View::dispatch_event(old_focus_view, &focus_loss, true);
		}
	}

	void View::prev_focus()
	{
		View *root = root_view();
		View *cur_focus = root->impl->_focus_view;
		View *prev_focus = nullptr;

		if (cur_focus)
		{
			prev_focus = cur_focus->impl->find_prev_with_tab_index(cur_focus->tab_index());
			if (prev_focus == nullptr)
				prev_focus = cur_focus->impl->find_prev_with_tab_index(root->impl->find_prev_tab_index(cur_focus->tab_index()));
		}
		else
		{
			prev_focus = root->impl->find_prev_with_tab_index(root->impl->find_prev_tab_index(0));
		}

		if (prev_focus)
			prev_focus->set_focus();
	}

	void View::next_focus()
	{
		View *root = root_view();
		View *cur_focus = root->impl->_focus_view;
		View *next_focus = nullptr;

		if (cur_focus)
		{
			next_focus = cur_focus->impl->find_next_with_tab_index(cur_focus->tab_index());
			if (next_focus == nullptr)
				next_focus = cur_focus->impl->find_next_with_tab_index(root->impl->find_next_tab_index(cur_focus->tab_index()));
		}
		else
		{
			next_focus = root->impl->find_next_with_tab_index(root->impl->find_next_tab_index(0));
		}

		if (next_focus)
			next_focus->set_focus();
	}

	void View::animate(float from, float to, const std::function<void(float)> &setter, int duration, const std::function<float(float)> &easing, std::function<void()> animation_end)
	{
		impl->animation_group.start(Animation(from, to, setter, duration, easing, animation_end));
	}

	void View::stop_animations()
	{
		impl->animation_group.stop();
	}

	void View::set_cursor(const CursorDescription &cursor)
	{
		if (impl->is_cursor_inherited || impl->cursor_desc != cursor)
		{
			impl->cursor_desc = cursor;
			impl->cursor = Cursor();
			impl->is_custom_cursor = true;
			impl->is_cursor_inherited = false;
		}
	}

	void View::set_cursor(StandardCursor type)
	{
		if (impl->is_cursor_inherited || impl->is_custom_cursor || impl->cursor_type != type)
		{
			impl->cursor_type = type;
			impl->cursor_desc = CursorDescription();
			impl->cursor = Cursor();
			impl->is_custom_cursor = false;
			impl->is_cursor_inherited = false;
		}
	}

	void View::set_inherit_cursor()
	{
		if (!impl->is_cursor_inherited)
		{
			impl->cursor_desc = CursorDescription();
			impl->cursor = Cursor();
			impl->is_custom_cursor = false;
			impl->is_cursor_inherited = true;
		}
	}

	void View::update_cursor(DisplayWindow &window)
	{
		if (impl->is_cursor_inherited)
		{
			View *super = superview();
			if (super)
				super->update_cursor(window);
			else
				window.set_cursor(StandardCursor::arrow);
		}
		else if (impl->is_custom_cursor)
		{
			if (impl->cursor.is_null())
			{
				impl->cursor = Cursor(window, impl->cursor_desc);
			}
			window.set_cursor(impl->cursor);
		}
		else
		{
			window.set_cursor(impl->cursor_type);
		}
	}

	Pointf View::to_screen_pos(const Pointf &pos)
	{
		if (local_root())
			return static_cast<RootView *>(this)->root_to_screen_pos(pos);

		if (superview())
			return superview()->to_screen_pos(geometry().content_box().get_top_left() + pos);
		else
			return Pointf();
	}

	Pointf View::from_screen_pos(const Pointf &pos)
	{
		if (local_root())
			return static_cast<RootView *>(this)->root_from_screen_pos(pos);

		if (superview())
			return superview()->from_screen_pos(pos) - geometry().content_box().get_top_left();
		else
			return Pointf();
	}

	Pointf View::to_root_pos(const Pointf &pos)
	{
		if (superview())
			return superview()->to_root_pos(geometry().content_box().get_top_left() + Vec2f(view_transform() * Vec4f(pos, 0.0f, 1.0f)));
		else
			return pos;
	}

	Pointf View::from_root_pos(const Pointf &pos)
	{
		if (superview())
			return superview()->from_root_pos(Vec2f(Mat4f::inverse(view_transform()) * Vec4f(pos, 0.0f, 1.0f)) - geometry().content_box().get_top_left());
		else
			return pos;
	}

	Signal<void(ActivationChangeEvent &)> &View::sig_activated(bool use_capture)
	{
		return impl->_sig_activated[use_capture ? 1 : 0];
	}

	Signal<void(ActivationChangeEvent &)> &View::sig_deactivated(bool use_capture)
	{
		return impl->_sig_deactivated[use_capture ? 1 : 0];
	}

	Signal<void(CloseEvent &)> &View::sig_close(bool use_capture)
	{
		return impl->_sig_close[use_capture ? 1 : 0];
	}

	Signal<void(ResizeEvent &)> &View::sig_resize(bool use_capture)
	{
		return impl->_sig_resize[use_capture ? 1 : 0];
	}

	Signal<void(FocusChangeEvent &)> &View::sig_focus_gained(bool use_capture)
	{
		return impl->_sig_focus_gained[use_capture ? 1 : 0];
	}

	Signal<void(FocusChangeEvent &)> &View::sig_focus_lost(bool use_capture)
	{
		return impl->_sig_focus_lost[use_capture ? 1 : 0];
	}

	Signal<void(PointerEvent &)> &View::sig_pointer_enter(bool use_capture)
	{
		return impl->_sig_pointer_enter[use_capture ? 1 : 0];
	}

	Signal<void(PointerEvent &)> &View::sig_pointer_leave(bool use_capture)
	{
		return impl->_sig_pointer_leave[use_capture ? 1 : 0];
	}

	Signal<void(PointerEvent &)> &View::sig_pointer_move(bool use_capture)
	{
		return impl->_sig_pointer_move[use_capture ? 1 : 0];
	}

	Signal<void(PointerEvent &)> &View::sig_pointer_press(bool use_capture)
	{
		return impl->_sig_pointer_press[use_capture ? 1 : 0];
	}

	Signal<void(PointerEvent &)> &View::sig_pointer_release(bool use_capture)
	{
		return impl->_sig_pointer_release[use_capture ? 1 : 0];
	}

	Signal<void(PointerEvent &)> &View::sig_pointer_double_click(bool use_capture)
	{
		return impl->_sig_pointer_double_click[use_capture ? 1 : 0];
	}

	Signal<void(PointerEvent &)> &View::sig_pointer_proximity_change(bool use_capture)
	{
		return impl->_sig_pointer_proximity_change[use_capture ? 1 : 0];
	}

	Signal<void(KeyEvent &)> &View::sig_key_press(bool use_capture)
	{
		return impl->_sig_key_press[use_capture ? 1 : 0];
	}

	Signal<void(KeyEvent &)> &View::sig_key_release(bool use_capture)
	{
		return impl->_sig_key_release[use_capture ? 1 : 0];
	}

	void View::dispatch_event(View *target, EventUI *e, bool no_propagation)
	{
		e->_target = target->shared_from_this();

		if (no_propagation)
		{
			e->_phase = EventUIPhase::at_target;
			e->_current_target = e->_target;
			e->_current_target->impl->process_event(e->_current_target.get(), e, true);
			if (!e->propagation_stopped())
				e->_current_target->impl->process_event(e->_current_target.get(), e, false);
		}
		else
		{
			target->impl->inverse_bubble(e);

			if (!e->propagation_stopped())
			{
				e->_phase = EventUIPhase::at_target;
				e->_current_target = e->_target;
				e->_current_target->impl->process_event(e->_current_target.get(), e, true);
				if (!e->propagation_stopped())
					e->_current_target->impl->process_event(e->_current_target.get(), e, false);

				while (e->_current_target && !e->propagation_stopped())
				{
					View *current_target_superview = e->_current_target->superview();
					e->_phase = EventUIPhase::bubbling;
					e->_current_target = current_target_superview ? current_target_superview->shared_from_this() : std::shared_ptr<View>();
					if (e->_current_target)
						e->_current_target->impl->process_event(e->_current_target.get(), e, false);
				}
			}
		}

		e->_current_target.reset();
		e->_phase = EventUIPhase::none;
	}

	/////////////////////////////////////////////////////////////////////////

	void ViewImpl::render(View *self, Canvas &canvas, ViewRenderLayer layer)
	{
		if (layer == ViewRenderLayer::background)
			style_cascade.render_background(canvas, _geometry);
		else if (layer == ViewRenderLayer::border)
			style_cascade.render_border(canvas, _geometry);

		Mat4f old_transform = canvas.get_transform();
		Pointf translate = _geometry.content_pos();
		canvas.set_transform(old_transform * Mat4f::translate(translate.x, translate.y, 0) * view_transform);

		bool clipped = content_clipped;
		if (clipped)
		{
			// Seems canvas cliprects are always in absolute coordinates - should this be changed?
			// Note: this code isn't correct for rotated transforms (plus canvas cliprect can only clip AABB)
			Vec4f tl_point = canvas.get_transform() * Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
			Vec4f br_point = canvas.get_transform() * Vec4f(_geometry.content_width, _geometry.content_height, 0.0f, 1.0f);
			canvas.push_cliprect(Rectf(std::min(tl_point.x, br_point.x), std::min(tl_point.y, br_point.y), std::max(tl_point.x, br_point.x), std::max(tl_point.y, br_point.y)));
		}

		if (layer == ViewRenderLayer::content)
		{
			if (!self->render_exception_encountered())
			{
				bool success = UIThread::try_catch([&]
				{
					self->render_content(canvas);
				});

				if (!success)
				{
					exception_encountered = true;
				}
			}

			if (self->render_exception_encountered())
			{
				canvas.set_transform(old_transform * Mat4f::translate(translate.x, translate.y, 0));
				canvas.fill_rect(0.0f, 0.0f, _geometry.content_width, _geometry.content_height, Colorf(1.0f, 0.2f, 0.2f, 0.5f));
				canvas.draw_line(0.0f, 0.0f, _geometry.content_width, _geometry.content_height, Colorf::black);
				canvas.draw_line(_geometry.content_width, 0.0f, 0.0f, _geometry.content_height, Colorf::black);
			}
		}

		Rectf clip_box = canvas.get_cliprect();
		for (std::shared_ptr<View> &view : _subviews)
		{
			if (!view->hidden() && !view->local_root())
			{
				// Note: this code isn't correct for rotated transforms (plus canvas cliprect can only clip AABB)
				Rect border_box = view->geometry().border_box();
				Vec4f tl_point = canvas.get_transform() * Vec4f(border_box.left, border_box.top, 0.0f, 1.0f);
				Vec4f br_point = canvas.get_transform() * Vec4f(border_box.right, border_box.bottom, 0.0f, 1.0f);
				Rectf transformed_border_box(std::min(tl_point.x, br_point.x), std::min(tl_point.y, br_point.y), std::max(tl_point.x, br_point.x), std::max(tl_point.y, br_point.y));
				if (clip_box.is_overlapped(transformed_border_box))
				{
					view->impl->render(view.get(), canvas, layer);
				}
			}
		}

		if (clipped)
			canvas.pop_cliprect();

		canvas.set_transform(old_transform);
	}

	void ViewImpl::update_style_cascade() const
	{
		std::vector<std::pair<Style *, size_t>> matches;

		for (auto it : styles)
		{
			auto &style_list = it.first;
			auto &style = it.second;

			auto style_classes = StringHelp::split_text(style_list, " ");

			bool match = true;
			for (const auto &state : style_classes)
			{
				auto search_it = states.find(state);
				if (search_it == states.end() || !search_it->second.enabled)
					match = false;
			}

			if (match)
				matches.push_back({ style.get(), style_classes.size() });
		}

		std::stable_sort(matches.begin(), matches.end(), [](const std::pair<Style *, size_t> &a, const std::pair<Style *, size_t> &b) { return a.second != b.second ? a.second > b.second : a.first > b.first; });

		style_cascade.cascade.clear();
		for (auto &match : matches)
			style_cascade.cascade.push_back(match.first);
	}

	void ViewImpl::process_event(View *self, EventUI *e, bool use_capture)
	{
		ActivationChangeEvent *activation_change = dynamic_cast<ActivationChangeEvent*>(e);
		CloseEvent *close = dynamic_cast<CloseEvent*>(e);
		ResizeEvent *resize = dynamic_cast<ResizeEvent*>(e);
		FocusChangeEvent *focus_change = dynamic_cast<FocusChangeEvent*>(e);
		PointerEvent *pointer = dynamic_cast<PointerEvent*>(e);
		KeyEvent *key = dynamic_cast<KeyEvent*>(e);

		if (activation_change)
		{
			switch (activation_change->type())
			{
			case ActivationChangeType::activated: self->sig_activated(use_capture)(*activation_change); break;
			case ActivationChangeType::deactivated: self->sig_deactivated(use_capture)(*activation_change); break;
			}
		}
		else if (close)
		{
			self->sig_close(use_capture)(*close);
		}
		else if (resize)
		{
			self->sig_resize(use_capture)(*resize);
		}
		else if (focus_change)
		{
			switch (focus_change->type())
			{
			case FocusChangeType::gained: self->sig_focus_gained(use_capture)(*focus_change); break;
			case FocusChangeType::lost: self->sig_focus_lost(use_capture)(*focus_change); break;
			}
		}
		else if (pointer)
		{
			switch (pointer->type())
			{
			case PointerEventType::enter: self->sig_pointer_enter(use_capture)(*pointer); break;
			case PointerEventType::leave: self->sig_pointer_leave(use_capture)(*pointer); break;
			case PointerEventType::move: self->sig_pointer_move(use_capture)(*pointer); break;
			case PointerEventType::press: self->sig_pointer_press(use_capture)(*pointer); break;
			case PointerEventType::release: self->sig_pointer_release(use_capture)(*pointer); break;
			case PointerEventType::double_click: self->sig_pointer_double_click(use_capture)(*pointer); break;
			case PointerEventType::promixity_change: self->sig_pointer_proximity_change(use_capture)(*pointer); break;
			case PointerEventType::none: break;
			}
		}
		else if (key)
		{
			switch (key->type())
			{
			case KeyEventType::none: break;
			case KeyEventType::press: self->sig_key_press(use_capture)(*key); break;
			case KeyEventType::release: self->sig_key_release(use_capture)(*key); break;
			}
		}
	}

	void ViewImpl::inverse_bubble(EventUI *e)
	{
		if (_superview)
		{
			std::shared_ptr<View> super = _superview->shared_from_this();
			super->impl->inverse_bubble(e);
			if (!e->propagation_stopped())
			{
				e->_phase = EventUIPhase::capturing;
				e->_current_target = super;
				if (e->_current_target)
					e->_current_target->impl->process_event(e->_current_target.get(), e, true);
			}
		}
	}

	unsigned int ViewImpl::find_next_tab_index(unsigned int start_index) const
	{
		unsigned int next_index = tab_index > start_index ? tab_index : 0;
		for (const auto &subview : _subviews)
		{
			if (subview->hidden()) continue;

			unsigned int next_subview_index = subview->impl->find_next_tab_index(start_index);
			if (next_subview_index != 0)
			{
				if (next_index != 0)
					next_index = clan::min(next_index, next_subview_index);
				else
					next_index = next_subview_index;
			}
		}
		return next_index;
	}

	unsigned int ViewImpl::find_prev_tab_index(unsigned int start_index) const
	{
		unsigned int index = find_prev_tab_index_helper(start_index);
		if (index == 0 && start_index > 0)
			index = find_highest_tab_index();
		return index;
	}

	unsigned int ViewImpl::find_prev_tab_index_helper(unsigned int start_index) const
	{
		unsigned int prev_index = tab_index < start_index ? tab_index : 0;
		for (const auto &subview : _subviews)
		{
			if (subview->hidden()) continue;

			unsigned int prev_subview_index = subview->impl->find_prev_tab_index_helper(start_index);
			if (prev_subview_index != 0)
			{
				if (prev_index != 0)
					prev_index = clan::max(prev_index, prev_subview_index);
				else
					prev_index = prev_subview_index;
			}
		}
		return prev_index;
	}

	unsigned int ViewImpl::find_highest_tab_index() const
	{
		unsigned int index = tab_index;
		for (const auto &subview : _subviews)
		{
			if (!subview->hidden())
				index = clan::max(subview->impl->find_highest_tab_index(), index);
		}
		return index;
	}

	View *ViewImpl::find_next_with_tab_index(unsigned int search_index, const ViewImpl *search_from, bool also_search_ancestors) const
	{
		bool search_from_found = search_from ? false : true;
		for (const auto &subview : _subviews)
		{
			if (subview->hidden())
				continue;

			if (search_from_found)
			{
				if (subview->tab_index() == search_index && subview->focus_policy() == FocusPolicy::accept)
				{
					return subview.get();
				}
				else
				{
					View *next = subview->impl->find_next_with_tab_index(search_index, nullptr, false);
					if (next)
						return next;
				}
			}
			else if (subview->impl.get() == search_from)
			{
				search_from_found = true;
			}
		}

		if (!also_search_ancestors || !_superview)
			return nullptr;

		return _superview->impl->find_next_with_tab_index(search_index, this, true);
	}

	View *ViewImpl::find_prev_with_tab_index(unsigned int search_index, const ViewImpl *search_from, bool also_search_ancestors) const
	{
		bool search_from_found = search_from ? false : true;
		for (auto it = _subviews.crbegin(); it != _subviews.crend(); ++it)
		{
			const auto &subview = *it;

			if (subview->hidden()) continue;

			if (search_from_found)
			{
				if (subview->tab_index() == search_index && subview->focus_policy() == FocusPolicy::accept)
				{
					return subview.get();
				}
				else
				{
					View *next = subview->impl->find_prev_with_tab_index(search_index, nullptr, false);
					if (next)
						return next;
				}
			}
			else if (subview->impl.get() == search_from)
			{
				search_from_found = true;
			}
		}

		if (!also_search_ancestors || !_superview)
			return nullptr;

		return _superview->impl->find_prev_with_tab_index(search_index, this, true);
	}

}

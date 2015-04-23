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

#pragma once

#include "../../Core/Math/rect.h"
#include "../../Core/Math/easing.h"
#include "../../Core/Signals/signal.h"
#include "../../UI/Events/event.h"
#include "../View/view_geometry.h"
#include "../Style/style.h"
#include "focus_policy.h"
#include <vector>
#include <memory>
#include <functional>

namespace clan
{
	class Style;
	class StyleCascade;
	class Canvas;
	class ActivationChangeEvent;
	class CloseEvent;
	class FocusChangeEvent;
	class PointerEvent;
	class ResizeEvent;
	class KeyEvent;
	class ViewImpl;
	class CursorDescription;
	enum class StandardCursor;
	class DisplayWindow;

	class View : public std::enable_shared_from_this<View>
	{
	public:
		View();
		~View();
		
		const StyleCascade &style_cascade() const;

		const std::shared_ptr<Style> &style(const std::string &state = std::string()) const;
		
		bool state(const std::string &name) const;
		void set_state(const std::string &name, bool value);

		/// Sets the state for this view and all siblings recursively, until a manually set state of the same name is found
		void set_state_cascade(const std::string &name, bool value);

		SlotContainer slots;

		View *superview() const;
		const std::vector<std::shared_ptr<View>> &subviews() const;

		void add_subview(const std::shared_ptr<View> &view);
		void remove_from_super();

		virtual bool hidden() const;
		virtual void set_hidden(bool value = true);

		bool is_static_position_and_visible() const;

		bool needs_layout() const;
		void set_needs_layout();

		const ViewGeometry &geometry() const;
		void set_geometry(const ViewGeometry &geometry);

		virtual Canvas get_canvas() const;

		virtual void set_needs_render();

		void render(Canvas &canvas);

		bool render_exception_encountered() const;
		void clear_exception_encountered();

		const Mat4f &view_transform() const;
		void set_view_transform(const Mat4f &transform);

		bool content_clipped() const;
		void set_content_clipped(bool clipped);

		virtual void render_content(Canvas &canvas) { }

		virtual float get_preferred_width(Canvas &canvas);
		virtual float get_preferred_height(Canvas &canvas, float width);
		virtual float get_first_baseline_offset(Canvas &canvas, float width);
		virtual float get_last_baseline_offset(Canvas &canvas, float width);

		virtual bool local_root();

		void layout(Canvas &canvas);
		virtual void layout_local();
		virtual void layout_subviews(Canvas &canvas);

		const View *root_view() const;
		View *root_view();
		View *owner_view() const;
		View *focus_view() const;
		View *proximity_view() const;

		std::shared_ptr<View> find_view_at(const Pointf &pos) const;

		FocusPolicy focus_policy() const;
		void set_focus_policy(FocusPolicy policy);

		unsigned int tab_index() const;
		void set_tab_index(unsigned int index);

		void set_focus();
		void remove_focus();
		bool has_focus() const { return focus_view() == this; }

		void prev_focus();
		void next_focus();

		void animate(float from, float to, const std::function<void(float)> &setter, int duration_ms = 400, const std::function<float(float)> &easing = Easing::linear, std::function<void()> animation_end = std::function<void()>());
		void stop_animations();

		void set_cursor(const CursorDescription &cursor);
		void set_cursor(StandardCursor type);
		void set_inherit_cursor();

		void dispatch_event(EventUI *e, bool no_propagation = false);

		Signal<void(ActivationChangeEvent &)> &sig_activated(bool use_capture = false);
		Signal<void(ActivationChangeEvent &)> &sig_deactivated(bool use_capture = false);
		Signal<void(CloseEvent &)> &sig_close(bool use_capture = false);
		Signal<void(ResizeEvent &)> &sig_resize(bool use_capture = false);
		Signal<void(FocusChangeEvent &)> &sig_focus_gained(bool use_capture = false);
		Signal<void(FocusChangeEvent &)> &sig_focus_lost(bool use_capture = false);
		Signal<void(PointerEvent &)> &sig_pointer_enter(bool use_capture = false);
		Signal<void(PointerEvent &)> &sig_pointer_leave(bool use_capture = false);
		Signal<void(PointerEvent &)> &sig_pointer_move(bool use_capture = false);
		Signal<void(PointerEvent &)> &sig_pointer_press(bool use_capture = false);
		Signal<void(PointerEvent &)> &sig_pointer_release(bool use_capture = false);
		Signal<void(PointerEvent &)> &sig_pointer_double_click(bool use_capture = false);
		Signal<void(PointerEvent &)> &sig_pointer_proximity_change(bool use_capture = false);
		Signal<void(KeyEvent &)> &sig_key_press(bool use_capture = false);
		Signal<void(KeyEvent &)> &sig_key_release(bool use_capture = false);

		void update_cursor(DisplayWindow &window);

		virtual Pointf to_screen_pos(const Pointf &pos);
		virtual Pointf from_screen_pos(const Pointf &pos);

		Pointf to_root_pos(const Pointf &pos);
		Pointf from_root_pos(const Pointf &pos);

	protected:
		virtual void subview_added(const std::shared_ptr<View> &view) { }
		virtual void subview_removed(const std::shared_ptr<View> &view) { }

		virtual void process_event(EventUI *e, bool use_capture);

	private:
		View(const View &) = delete;
		View &operator=(const View &) = delete;

		std::unique_ptr<ViewImpl> impl;

		friend class ViewImpl;
	};
}

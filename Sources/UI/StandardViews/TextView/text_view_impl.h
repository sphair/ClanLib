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
*/

#pragma once

#include "API/Core/Signals/signal.h"
#include "API/UI/Events/key_event.h"
#include "API/Display/System/timer.h"
#include "API/Display/Font/font.h"

namespace clan
{
	class TextViewSelection
	{
	public:
		void reset()
		{
			set_head_and_tail(Vec2i(), Vec2i());
		}

		void set_head_and_tail(Vec2i new_head, Vec2i new_tail)
		{
			if (selection_head != new_head || selection_tail != new_tail)
			{
				selection_head = new_head;
				selection_tail = new_tail;
				sig_selection_changed();
				if (view)
					view->set_needs_render();
			}
		}

		void set_head(Vec2i head)
		{
			set_head_and_tail(head, selection_tail);
		}

		void set_tail(Vec2i tail)
		{
			set_head_and_tail(selection_head, tail);
		}

		void set_view(View *new_view) { view = new_view; }

		bool is_head_start() const { return selection_head.y < selection_tail.y || (selection_head.y == selection_tail.y && selection_head.x <= selection_tail.x); }

		Vec2i start() const { return is_head_start() ? selection_head : selection_tail; }
		Vec2i end() const { return is_head_start() ? selection_tail : selection_head; }

		Signal<void()> sig_selection_changed;

	private:
		View *view = nullptr;
		Vec2i selection_head;
		Vec2i selection_tail;
	};

	class TextViewImpl
	{
	public:
		void on_key_press(KeyEvent &e);
		void on_key_release(KeyEvent &e);
		void on_pointer_press(PointerEvent &e);
		void on_pointer_release(PointerEvent &e);
		void on_pointer_move(PointerEvent &e);
		void on_focus_gained(FocusChangeEvent &e);
		void on_focus_lost(FocusChangeEvent &e);
		void on_activated(ActivationChangeEvent &e);
		void on_deactivated(ActivationChangeEvent &e);

		void select_all();
		void move_line(int direction, bool ctrl, bool shift, bool stay_on_line);
		void move(int direction, bool ctrl, bool shift, bool stay_on_line);
		void backspace();
		void del();
		void home(bool ctrl, bool shift);
		void end(bool ctrl, bool shift);
		void cut();
		void copy();
		void paste();
		void undo();
		void redo();
		void add(std::string new_text);

		void start_blink();
		void stop_blink();

		void save_undo();

		TextView *textfield = nullptr;

		TextAlignment alignment = TextAlignment::left;

		Font &get_font(Canvas &canvas);
		Font font; // Do not use directly. Use get_font.

		Size preferred_size = Size(20, 5);
		std::vector<std::string> text_lines;
		std::string placeholder;

		Signal<void(KeyEvent &)> sig_before_edit_changed;
		Signal<void(KeyEvent &)> sig_after_edit_changed;
		Signal<void(KeyEvent &)> sig_enter_pressed;

		bool readonly = false;
		bool cursor_drawing_enabled_when_parent_focused = false;

		TextViewSelection selection;
		Vec2f cursor_pos;

		Vec2f scroll_pos;

		bool cursor_blink_visible = false;
		Timer blink_timer;
		bool mouse_moves_left = false;
		Timer scroll_timer;
		bool ignore_mouse_events = false;
		bool mouse_selecting = false;

		struct UndoInfo
		{
			std::string text;
			size_t selection_start = 0;
			size_t selection_length = 0;
			size_t cursor_pos = 0;
		};

		std::vector<UndoInfo> undo_buffer;
		std::vector<UndoInfo> redo_buffer;
		bool needs_new_undo_step = true;

		static const std::string break_characters;

		std::vector<Rectf> last_measured_rects;

		std::string get_all_selected_text() const;

		std::string get_text_before_selection(size_t line_index) const;
		std::string get_selected_text(size_t line_index) const;
		std::string get_text_after_selection(size_t line_index) const;

		int find_next_break_character(int search_start, int line) const;
		int find_previous_break_character(int search_start, int line) const;

		Vec2i get_character_index(const Pointf &pos);
	};
}

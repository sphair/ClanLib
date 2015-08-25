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
#include "API/UI/StandardViews/text_view.h"
#include "API/UI/Style/style.h"
#include "API/UI/TopLevel/view_tree.h"
#include "API/UI/Events/pointer_event.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/2D/brush.h"
#include "API/Display/2D/pen.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/glyph_metrics.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Window/display_window.h"
#include "text_view_impl.h"
#include <algorithm>
#include <cmath>

namespace clan
{
	TextView::TextView() : impl(new TextViewImpl())
	{
		impl->textfield = this;
		impl->text_lines.resize(1);
		impl->selection.set_view(this);

		set_focus_policy(FocusPolicy::accept);
		set_cursor(StandardCursor::ibeam);
		set_content_clipped(true);

		slots.connect(sig_key_press(), impl.get(), &TextViewImpl::on_key_press);
		slots.connect(sig_key_release(), impl.get(), &TextViewImpl::on_key_release);
		slots.connect(sig_pointer_press(), impl.get(), &TextViewImpl::on_pointer_press);
		slots.connect(sig_pointer_release(), impl.get(), &TextViewImpl::on_pointer_release);
		slots.connect(sig_pointer_move(), impl.get(), &TextViewImpl::on_pointer_move);
		slots.connect(sig_focus_gained(), impl.get(), &TextViewImpl::on_focus_gained);
		slots.connect(sig_focus_lost(), impl.get(), &TextViewImpl::on_focus_lost);
		slots.connect(sig_activated(), impl.get(), &TextViewImpl::on_activated);
		slots.connect(sig_deactivated(), impl.get(), &TextViewImpl::on_deactivated);

		impl->scroll_timer.func_expired() = [&]()
		{
			if (impl->mouse_moves_left)
				impl->move(-1, false, false, true);
			else
				impl->move(1, false, false, true);
		};
	}

	TextView::~TextView()
	{
	}

	Size TextView::preferred_size() const
	{
		return impl->preferred_size;
	}

	void TextView::set_preferred_size(Size num_characters)
	{
		if (impl->preferred_size != num_characters)
		{
			impl->preferred_size = num_characters;
			set_needs_layout();
		}
	}

	std::string TextView::text() const
	{
		size_t length = 0;
		for (const auto &line : impl->text_lines) length += line.length() + 1;

		std::string all_text;
		all_text.reserve(length);
		for (const auto &line : impl->text_lines)
		{
			if (!all_text.empty())
				all_text.push_back('\n');
			all_text += line;
		}

		return all_text;
	}

	void TextView::set_text(const std::string &text)
	{
		impl->text_lines = StringHelp::split_text(text, "\n", false);
		if (impl->text_lines.empty())
			impl->text_lines.resize(1);

		impl->selection.reset();
		impl->cursor_pos = Vec2i();
		impl->scroll_pos = Vec2f();

		impl->undo_buffer.clear();
		impl->redo_buffer.clear();

		set_needs_render();
	}

	std::string TextView::placeholder() const
	{
		return impl->placeholder;
	}

	void TextView::set_placeholder(const std::string &value)
	{
		impl->placeholder = value;
		set_needs_render();
	}

	TextAlignment TextView::text_alignment() const
	{
		return impl->alignment;
	}

	void TextView::set_text_alignment(TextAlignment alignment)
	{
		if (impl->alignment != alignment)
		{
			impl->alignment = alignment;
			set_needs_render();
		}
	}

	bool TextView::is_read_only() const
	{
		return impl->readonly;
	}

	void TextView::set_read_only(bool value)
	{
		if (impl->readonly != value)
		{
			impl->readonly = value;
			set_needs_render();
		}
	}

	std::string TextView::selection() const
	{
		return impl->get_all_selected_text();
	}

	Vec2i TextView::selection_start() const
	{
		return impl->selection.start();
	}

	Vec2i TextView::selection_end() const
	{
		return impl->selection.end();
	}

	void TextView::set_selection(Vec2i head, Vec2i tail)
	{
		// Bounds check: (to do: should we throw an out of bounds exception instead?)
		head.y = std::max(std::min(head.y, (int)impl->text_lines.size() - 1), 0);
		tail.y = std::max(std::min(head.y, (int)impl->text_lines.size() - 1), 0);
		head.x = std::max(std::min(head.x, (int)impl->text_lines[head.y].size()), 0);
		tail.x = std::max(std::min(tail.x, (int)impl->text_lines[tail.y].size()), 0);

		impl->selection.set_head_and_tail(head, tail);
		impl->cursor_pos = tail;
		set_needs_render();
	}

	void TextView::clear_selection()
	{
		set_selection(Vec2i(), Vec2i());
	}

	void TextView::delete_selected_text()
	{
		if (impl->selection.start() != impl->selection.end())
			impl->del();
	}

	void TextView::select_all()
	{
		set_selection(Vec2i(0, 0), Vec2i(impl->text_lines.back().size(), impl->text_lines.size() - 1));
	}

	Vec2i TextView::cursor_pos() const
	{
		return impl->cursor_pos;
	}

	void TextView::set_cursor_pos(Vec2i pos)
	{
		impl->cursor_pos = pos;
		set_needs_render();
	}

	void TextView::set_cursor_drawing_enabled(bool value)
	{
		impl->cursor_drawing_enabled_when_parent_focused = value;
	}

	Signal<void(KeyEvent &)> &TextView::sig_before_edit_changed()
	{
		return impl->sig_before_edit_changed;
	}

	Signal<void(KeyEvent &)> &TextView::sig_after_edit_changed()
	{
		return impl->sig_after_edit_changed;
	}

	Signal<void()> &TextView::sig_selection_changed()
	{
		return impl->selection.sig_selection_changed;
	}

	Signal<void(KeyEvent &)> &TextView::sig_enter_pressed()
	{
		return impl->sig_enter_pressed;
	}

	void TextView::render_content(Canvas &canvas)
	{
		Font font = impl->get_font(canvas);
		FontMetrics font_metrics = font.get_font_metrics(canvas);
		float baseline = font_metrics.get_baseline_offset();
		float top_y = baseline - font_metrics.get_ascent();
		float bottom_y = baseline + font_metrics.get_descent();

		Colorf color = style_cascade().computed_value("color").color();

		float cursor_advance = canvas.grid_fit({ font.measure_text(canvas, impl->text_lines[impl->cursor_pos.y].substr(0, impl->cursor_pos.x)).advance.width, 0.0f }).x;

		// Keep cursor in view
		impl->scroll_pos.x = std::min(impl->scroll_pos.x, cursor_advance);
		impl->scroll_pos.x = std::max(impl->scroll_pos.x, cursor_advance - geometry().content_width + 1.0f);

		size_t line_start_y = -impl->scroll_pos.y;

		for (size_t line_index = 0; line_index < impl->text_lines.size(); line_index++)
		{
			std::string txt_before = impl->get_text_before_selection(line_index);
			std::string txt_selected = impl->get_selected_text(line_index);
			std::string txt_after = impl->get_text_after_selection(line_index);

			float advance_before = font.measure_text(canvas, txt_before).advance.width;
			float advance_selected = font.measure_text(canvas, txt_selected).advance.width;

			// Measure text for get_character_index()
			impl->last_measured_rects = font.get_character_indices(canvas, txt_before + txt_selected + txt_after);
			for (auto &box : impl->last_measured_rects)
				box.translate(-impl->scroll_pos.x, line_start_y);

			if (!txt_selected.empty())
			{
				Rectf selection_rect = Rectf(advance_before - impl->scroll_pos.x, top_y + line_start_y, advance_before + advance_selected - impl->scroll_pos.x, bottom_y + line_start_y);
				Path::rect(selection_rect).fill(canvas, focus_view() == this ? Brush::solid_rgb8(51, 153, 255) : Brush::solid_rgb8(200, 200, 200));
			}

			font.draw_text(canvas, -impl->scroll_pos.x, baseline + line_start_y, txt_before, color);
			font.draw_text(canvas, advance_before - impl->scroll_pos.x, baseline + line_start_y, txt_selected, focus_view() == this ? Colorf(255, 255, 255) : color);
			font.draw_text(canvas, advance_before + advance_selected - impl->scroll_pos.x, baseline + line_start_y, txt_after, color);

			line_start_y += font_metrics.get_line_height();
		}

		if (impl->cursor_blink_visible)
		{
			auto cursor_pos = canvas.grid_fit({ cursor_advance - impl->scroll_pos.x, top_y - impl->scroll_pos.y + font_metrics.get_line_height() * impl->cursor_pos.y });
			Path::rect(cursor_pos.x, cursor_pos.y, 1.0f, bottom_y - top_y).fill(canvas, Brush(color));
		}

		if (impl->text_lines.size() == 1 && impl->text_lines.front().empty())
		{
			color.r = color.r * 0.5f + 0.5f;
			color.g = color.g * 0.5f + 0.5f;
			color.b = color.b * 0.5f + 0.5f;
			font.draw_text(canvas, 0.0f, baseline, impl->placeholder, color);
		}
	}

	float TextView::calculate_preferred_width(Canvas &canvas)
	{
		if (style_cascade().computed_value("width").is_keyword("auto"))
		{
			Font font = impl->get_font(canvas);
			return font.measure_text(canvas, "X").advance.width * impl->preferred_size.width;
		}
		else
			return style_cascade().computed_value("width").number();
	}

	float TextView::calculate_preferred_height(Canvas &canvas, float width)
	{
		if (style_cascade().computed_value("height").is_keyword("auto"))
		{
			Font font = impl->get_font(canvas);
			return font.get_font_metrics(canvas).get_line_height() * impl->preferred_size.height;
		}
		else
			return style_cascade().computed_value("height").number();
	}

	float TextView::calculate_first_baseline_offset(Canvas &canvas, float width)
	{
		Font font = impl->get_font(canvas);
		return font.get_font_metrics(canvas).get_baseline_offset();
	}

	float TextView::calculate_last_baseline_offset(Canvas &canvas, float width)
	{
		return get_first_baseline_offset(canvas, width);
	}

	/////////////////////////////////////////////////////////////////////////

	Font &TextViewImpl::get_font(Canvas &canvas)
	{
		if (font.is_null())
			font = textfield->style_cascade().get_font(canvas);
		return font;
	}

	void TextViewImpl::start_blink()
	{
		blink_timer.func_expired() = [&]()
		{
			cursor_blink_visible = !cursor_blink_visible;
			textfield->set_needs_render();
		};
		blink_timer.start(500, true);

		cursor_blink_visible = true;
		textfield->set_needs_render();
	}

	void TextViewImpl::stop_blink()
	{
		blink_timer.stop();
		cursor_blink_visible = false;
		textfield->set_needs_render();
	}

	void TextViewImpl::on_focus_gained(FocusChangeEvent &e)
	{
		start_blink();
		cursor_blink_visible = true;
		textfield->set_needs_render();
		ignore_mouse_events = true;
	}

	void TextViewImpl::on_focus_lost(FocusChangeEvent &e)
	{
		if (mouse_selecting)
		{
			scroll_timer.stop();
			mouse_selecting = false;
		}
		stop_blink();
		selection.reset();
	}

	void TextViewImpl::on_activated(ActivationChangeEvent &e)
	{
		if (textfield->has_focus())
		{
			start_blink();
		}
	}

	void TextViewImpl::on_deactivated(ActivationChangeEvent &e)
	{
		if (mouse_selecting)
		{
			scroll_timer.stop();
			mouse_selecting = false;
		}
		stop_blink();
	}

	void TextViewImpl::on_key_press(KeyEvent &e)
	{
		if (e.key() == Key::key_return)
		{
			sig_enter_pressed(e);
			if (!e.default_prevented())
				add("\n");
			e.stop_propagation();
			return;
		}

		sig_before_edit_changed(e);
		if (e.default_prevented())
		{
			e.stop_propagation();
			return;
		}

		if (!readonly)
		{
			// Reset blinking
			start_blink();
		}

		if (e.key() == Key::key_return || e.key() == Key::escape || e.key() == Key::tab)
		{
			// Do not consume these.
			return;
		}
		else if (e.key() == Key::a && e.ctrl_down())
		{
			select_all();
			e.stop_propagation();
			return;
		}
		else if (e.key() == Key::c && e.ctrl_down())
		{
			copy();
			e.stop_propagation();
		}
		else if (e.key() == Key::up)
		{
			move_line(-1, e.ctrl_down(), e.shift_down(), false);
			e.stop_propagation();
		}
		else if (e.key() == Key::down)
		{
			move_line(1, e.ctrl_down(), e.shift_down(), false);
			e.stop_propagation();
		}
		else if (e.key() == Key::left)
		{
			move(-1, e.ctrl_down(), e.shift_down(), false);
			e.stop_propagation();
		}
		else if (e.key() == Key::right)
		{
			move(1, e.ctrl_down(), e.shift_down(), false);
			e.stop_propagation();
		}
		else if (e.key() == Key::backspace)
		{
			backspace();
			e.stop_propagation();
		}
		else if (e.key() == Key::key_delete)
		{
			del();
			e.stop_propagation();
		}
		else if (e.key() == Key::home)
		{
			home(e.ctrl_down(), e.shift_down());
			e.stop_propagation();
		}
		else if (e.key() == Key::end)
		{
			end(e.ctrl_down(), e.shift_down());
			e.stop_propagation();
		}
		else if (e.key() == Key::x && e.ctrl_down())
		{
			cut();
			e.stop_propagation();
		}
		else if (e.key() == Key::v && e.ctrl_down())
		{
			paste();
			e.stop_propagation();
		}
		else if (e.key() == Key::z && e.ctrl_down())
		{
			undo();
			e.stop_propagation();
		}
		else if (e.key() == Key::y && e.ctrl_down())
		{
			redo();
			e.stop_propagation();
		}
		else if ((!e.text().empty() && !(e.text().front() >= 0 && e.text().front() < 32) && (!e.alt_down() && !e.ctrl_down())) || (e.ctrl_down() && e.alt_down())) // Alt Gr translates to Ctrl+Alt sometimes!
		{
			add(e.text());
			e.stop_propagation();
		}

		sig_after_edit_changed(e);
	}

	void TextViewImpl::on_key_release(KeyEvent &e)
	{
	}

	void TextViewImpl::on_pointer_press(PointerEvent &e)
	{
		if (textfield->has_focus())
		{
			mouse_selecting = true;
			cursor_pos = get_character_index(e.pos(textfield));
			selection.set_head_and_tail(cursor_pos, cursor_pos);
		}
		else
		{
			textfield->set_focus();
		}

	}

	void TextViewImpl::on_pointer_release(PointerEvent &e)
	{
		if (!mouse_selecting)
			return;

		if (ignore_mouse_events) // This prevents text selection from changing from what was set when focus was gained.
		{
			ignore_mouse_events = false;
			mouse_selecting = false;
		}
		else
		{
			scroll_timer.stop();
			mouse_selecting = false;
			cursor_pos = get_character_index(e.pos(textfield));
			selection.set_tail(cursor_pos);
			textfield->set_focus();
			textfield->set_needs_render();
		}
	}

	void TextViewImpl::on_pointer_move(PointerEvent &e)
	{
		if (!mouse_selecting)
			return;
		if (ignore_mouse_events)
			return;

		Rectf content_rect = textfield->geometry().content_box();
		Vec2f pos = e.pos(textfield);
		if (pos.x < 0.0f || pos.x > content_rect.get_width())
		{
			if (pos.x < 0.0f)
				mouse_moves_left = true;
			else
				mouse_moves_left = false;

			if (!readonly)
				scroll_timer.start(50, true);
		}
		else
		{
			scroll_timer.stop();
			cursor_pos = get_character_index(pos);
			selection.set_tail(cursor_pos);
		}

	}

	void TextViewImpl::select_all()
	{
		selection.set_head_and_tail(Vec2i(), Vec2i(text_lines.back().size(), text_lines.size()));
	}

	void TextViewImpl::move_line(int steps, bool ctrl, bool shift, bool stay_on_line)
	{
		// To do: if ctrl is down it scrolls up and down instead of moving cursor
		// To do: allow cursor_pos.x to be out of bounds and only clamped into range when move or add/del/backspace is called

		if (ctrl)
			return;

		Vec2i pos = cursor_pos;

		if (steps > 0)
		{
			for (int i = 0; i < steps; i++)
			{
				if (pos.y + 1 != text_lines.size())
				{
					pos.y++;
					pos.x = std::min(pos.x, (int)text_lines[pos.y].size());
				}
			}
		}
		else
		{
			for (int i = 0; i < -steps; i++)
			{
				if (pos.y > 0)
				{
					pos.y--;
					pos.x = std::min(pos.x, (int)text_lines[pos.y].size());
				}
			}
		}

		if (shift)
		{
			if (selection.start() == selection.end())
				selection.set_head(cursor_pos);
			selection.set_tail(pos);
		}
		else
		{
			selection.reset();
		}

		cursor_pos = pos;
		needs_new_undo_step = true;
		textfield->set_needs_render();
	}

	void TextViewImpl::move(int steps, bool ctrl, bool shift, bool stay_on_line)
	{
		Vec2i pos = cursor_pos;

		// Jump over words if control is pressed.
		if (ctrl)
		{
			if (steps < 0)
			{
				if (!stay_on_line && pos.x == 0 && pos.y != 0)
				{
					pos.y--;
					pos.x = text_lines[pos.y].size();
				}
				pos.x = find_previous_break_character(pos.x, pos.y);
			}
			else
			{
				if (!stay_on_line && pos.x == text_lines[pos.y].size() && pos.y + 1 != text_lines.size())
				{
					pos.y++;
					pos.x = 0;
				}
				pos.x = find_next_break_character(pos.x, pos.y);
			}
		}
		else
		{
			UTF8_Reader utf8_reader(text_lines[pos.y].data(), text_lines[pos.y].length());
			utf8_reader.set_position(pos.x);

			if (steps > 0)
			{
				for (int i = 0; i < steps; i++)
				{
					if (!stay_on_line && utf8_reader.get_position() == text_lines[pos.y].size() && pos.y + 1 != text_lines.size())
					{
						pos.y++;
						utf8_reader = UTF8_Reader(text_lines[pos.y].data(), text_lines[pos.y].length());
						utf8_reader.set_position(0);
					}
					else
					{
						utf8_reader.next();
					}
				}
			}
			else if (steps < 0)
			{
				for (int i = 0; i < -steps; i++)
				{
					if (!stay_on_line && utf8_reader.get_position() == 0 && pos.y != 0)
					{
						pos.y--;
						utf8_reader = UTF8_Reader(text_lines[pos.y].data(), text_lines[pos.y].length());
						utf8_reader.set_position(text_lines[pos.y].length());
					}
					else
					{
						utf8_reader.prev();
					}
				}
			}

			pos.x = utf8_reader.get_position();
		}

		if (shift)
		{
			if (selection.start() == selection.end())
				selection.set_head(cursor_pos);
			selection.set_tail(pos);
		}
		else
		{
			selection.reset();
		}

		cursor_pos = pos;
		needs_new_undo_step = true;
		textfield->set_needs_render();
	}

	void TextViewImpl::home(bool ctrl, bool shift)
	{
		Vec2i pos = cursor_pos;

		if (ctrl)
			pos.y = 0;
		pos.x = 0;

		if (pos == cursor_pos)
			return;

		if (shift)
		{
			if (selection.start() == selection.end())
				selection.set_head(cursor_pos);
			selection.set_tail(pos);
		}
		else
		{
			selection.reset();
		}

		cursor_pos = pos;
		textfield->set_needs_render();
	}

	void TextViewImpl::end(bool ctrl, bool shift)
	{
		Vec2i pos = cursor_pos;

		if (ctrl)
			pos.y = text_lines.size() - 1;
		pos.x = text_lines[pos.y].size();

		if (pos == cursor_pos)
			return;

		if (shift)
		{
			if (selection.start() == selection.end())
				selection.set_head(cursor_pos);
			selection.set_tail(pos);
		}
		else
		{
			selection.reset();
		}

		cursor_pos = pos;
		textfield->set_needs_render();
	}

	void TextViewImpl::backspace()
	{
		if (selection.start() != selection.end())
		{
			del();
		}
		else if (cursor_pos.x > 0)
		{
			save_undo();

			UTF8_Reader utf8_reader(text_lines[cursor_pos.y].data(), text_lines[cursor_pos.y].length());
			utf8_reader.set_position(cursor_pos.x);
			utf8_reader.prev();
			int new_cursor_pos = utf8_reader.get_position();

			text_lines[cursor_pos.y].erase(text_lines[cursor_pos.y].begin() + new_cursor_pos, text_lines[cursor_pos.y].begin() + cursor_pos.x);
			cursor_pos.x = new_cursor_pos;

			textfield->set_needs_render();
		}
		else if (cursor_pos.y > 0)
		{
			save_undo();

			cursor_pos.y--;
			cursor_pos.x = text_lines[cursor_pos.y].length();

			text_lines[cursor_pos.y] += text_lines[cursor_pos.y + 1];
			text_lines.erase(text_lines.begin() + cursor_pos.y + 1);

			textfield->set_needs_render();
		}
	}

	void TextViewImpl::del()
	{
		if (selection.start() != selection.end())
		{
			save_undo();

			auto start = selection.start();
			auto end = selection.end();

			if (start.y == end.y)
			{
				text_lines[start.y].erase(text_lines[start.y].begin() + start.x, text_lines[start.y].begin() + end.x);
			}
			else
			{
				text_lines[start.y].erase(text_lines[start.y].begin() + start.x, text_lines[start.y].end());
				text_lines[end.y].erase(text_lines[end.y].begin(), text_lines[end.y].begin() + end.x);
				text_lines.erase(text_lines.begin() + start.y + 1, text_lines.begin() + end.y);
			}

			cursor_pos = start;
			selection.reset();

			textfield->set_needs_render();
		}
		else if (cursor_pos.x < text_lines[cursor_pos.y].length())
		{
			save_undo();

			UTF8_Reader utf8_reader(text_lines[cursor_pos.y].data(), text_lines[cursor_pos.y].length());
			utf8_reader.set_position(cursor_pos.x);
			text_lines[cursor_pos.y].erase(text_lines[cursor_pos.y].begin() + cursor_pos.x, text_lines[cursor_pos.y].begin() + cursor_pos.x + utf8_reader.get_char_length());

			textfield->set_needs_render();
		}
		else if (cursor_pos.y + 1 < text_lines.size())
		{
			save_undo();

			text_lines[cursor_pos.y] += text_lines[cursor_pos.y + 1];
			text_lines.erase(text_lines.begin() + cursor_pos.y + 1);

			textfield->set_needs_render();
		}
	}

	void TextViewImpl::cut()
	{
		copy();
		if (selection.start() == selection.end())
			select_all();
		del();
	}

	void TextViewImpl::copy()
	{
		ViewTree *tree = textfield->view_tree();
		if (tree)
		{
			DisplayWindow window = tree->get_display_window();
			if (!window.is_null())
			{
				if (selection.start() != selection.end())
					window.set_clipboard_text(get_all_selected_text());
				else
					window.set_clipboard_text(textfield->text());
			}
		}
	}

	void TextViewImpl::paste()
	{
		ViewTree *tree = textfield->view_tree();
		if (tree)
		{
			DisplayWindow window = tree->get_display_window();
			if (!window.is_null())
				add(window.get_clipboard_text());
		}
	}

	void TextViewImpl::undo()
	{
		/*
		if (undo_buffer.empty())
			return;

		UndoInfo info;
		info.text = text;
		info.cursor_pos = cursor_pos;
		info.selection_start = selection.start();
		info.selection_length = selection.length();
		redo_buffer.push_back(info);

		text = undo_buffer.back().text;
		cursor_pos = undo_buffer.back().cursor_pos;
		selection.set(undo_buffer.back().selection_start, undo_buffer.back().selection_length);

		undo_buffer.pop_back();

		textfield->set_needs_render();
		*/
	}

	void TextViewImpl::redo()
	{
		/*
		if (redo_buffer.empty())
			return;

		UndoInfo info;
		info.text = text;
		info.cursor_pos = cursor_pos;
		info.selection_start = selection.start();
		info.selection_length = selection.length();
		undo_buffer.push_back(info);

		text = redo_buffer.back().text;
		cursor_pos = redo_buffer.back().cursor_pos;
		selection.set(redo_buffer.back().selection_start, redo_buffer.back().selection_length);

		redo_buffer.pop_back();

		textfield->set_needs_render();
		*/
	}

	void TextViewImpl::save_undo()
	{
		/*
		redo_buffer.clear();

		if (undo_buffer.empty() || needs_new_undo_step)
		{
			UndoInfo info;
			info.text = text;
			info.cursor_pos = cursor_pos;
			info.selection_start = selection.start();
			info.selection_length = selection.length();
			undo_buffer.push_back(info);
			needs_new_undo_step = false;
		}
		*/
	}

	void TextViewImpl::add(std::string new_text)
	{
		if (selection.start() != selection.end())
			del();

		save_undo();

		size_t start = 0;
		while (true)
		{
			size_t end = new_text.find('\n', start);
			if (end == std::string::npos)
				end = new_text.size();

			text_lines[cursor_pos.y] = text_lines[cursor_pos.y].substr(0, cursor_pos.x) + new_text.substr(start, end - start) + text_lines[cursor_pos.y].substr(cursor_pos.x);
			cursor_pos.x += end - start;

			if (end == new_text.size())
				break;
			start = end + 1;

			text_lines.insert(text_lines.begin() + cursor_pos.y + 1, text_lines[cursor_pos.y].substr(cursor_pos.x));
			text_lines[cursor_pos.y].resize(cursor_pos.x);
			cursor_pos = Vec2i(0, cursor_pos.y + 1);
		}

		textfield->set_needs_render();
	}

	std::string TextViewImpl::get_all_selected_text() const
	{
		Vec2i start = selection.start();
		Vec2i end = selection.end();

		if (start.y == end.y)
		{
			return text_lines[start.y].substr(start.x, end.x - start.x);
		}
		else
		{
			size_t length = text_lines[start.y].length() - start.x + text_lines[end.y].length() - end.x + 1;
			for (auto y = start.y + 1; y < end.y; y++)
				length += text_lines[y].length() + 1;

			std::string result;
			result.reserve(length);
			result += text_lines[start.y].substr(start.x);
			result.push_back('\n');
			for (auto y = start.y + 1; y < end.y; y++)
			{
				result += text_lines[y];
				result.push_back('\n');
			}
			result += text_lines[end.y].substr(0, end.x);
			return result;
		}
	}

	std::string TextViewImpl::get_text_before_selection(size_t line_index) const
	{
		Vec2i start = selection.start();

		if ((size_t)start.y == line_index)
			return text_lines[line_index].substr(0, start.x);
		else if ((size_t)start.y > line_index)
			return text_lines[line_index];
		else
			return std::string();
	}

	std::string TextViewImpl::get_selected_text(size_t line_index) const
	{
		Vec2i start = selection.start();
		Vec2i end = selection.end();

		if (start.y == line_index && end.y == line_index)
			return text_lines[line_index].substr(start.x, end.x - start.x);
		else if ((size_t)start.y > line_index || (size_t)end.y < line_index)
			return std::string();
		else if (start.y == line_index)
			return text_lines[line_index].substr(start.x);
		else if (end.y == line_index)
			return text_lines[line_index].substr(0, end.x);
		else
			return text_lines[line_index];
	}

	std::string TextViewImpl::get_text_after_selection(size_t line_index) const
	{
		Vec2i end = selection.end();

		if ((size_t)end.y == line_index)
			return text_lines[line_index].substr(end.x);
		else if ((size_t)end.y < line_index)
			return text_lines[line_index];
		else
			return std::string();
	}

	int TextViewImpl::find_next_break_character(int search_start, int line) const
	{
		if (search_start == text_lines[line].size())
			return search_start;

		size_t pos = text_lines[line].find_first_of(break_characters, search_start + 1);
		if (pos == std::string::npos)
			return text_lines[line].size();
		return pos;
	}

	int TextViewImpl::find_previous_break_character(int search_start, int line) const
	{
		if (search_start == 0)
			return 0;
		size_t pos = text_lines[line].find_last_of(break_characters, search_start - 1);
		if (pos == std::string::npos)
			return 0;
		return pos;
	}

	Vec2i TextViewImpl::get_character_index(const Pointf &pos)
	{
		return Vec2i();
		/*
		if (last_measured_rects.empty())
			return Vec2i();

		for (unsigned int cnt = 0; cnt < last_measured_rects.size(); cnt++)
		{
			if ((last_measured_rects[cnt].left <= pos.x) && (last_measured_rects[cnt].right > pos.x))
				return cnt + 1;
		}
		if (last_measured_rects[0].left >= pos.x)
			return 0;
		return last_measured_rects.size();
		*/
	}

	const std::string TextViewImpl::break_characters = " ::;,.-";
}

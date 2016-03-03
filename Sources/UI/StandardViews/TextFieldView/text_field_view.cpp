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
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/text_field_view.h"
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
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/utf8_reader.h"
#include "text_field_view_impl.h"
#include <algorithm>
#include <cmath>

namespace clan
{
	TextFieldView::TextFieldView() : impl(new TextFieldViewImpl())
	{
		impl->textfield = this;
		impl->selection.set_view(this);

		set_focus_policy(FocusPolicy::accept);
		set_cursor(StandardCursor::ibeam);
		set_content_clipped(true);

		slots.connect(sig_key_press(), impl.get(), &TextFieldViewImpl::on_key_press);
		slots.connect(sig_key_release(), impl.get(), &TextFieldViewImpl::on_key_release);
		slots.connect(sig_pointer_press(), impl.get(), &TextFieldViewImpl::on_pointer_press);
		slots.connect(sig_pointer_release(), impl.get(), &TextFieldViewImpl::on_pointer_release);
		slots.connect(sig_pointer_move(), impl.get(), &TextFieldViewImpl::on_pointer_move);
		slots.connect(sig_focus_gained(), impl.get(), &TextFieldViewImpl::on_focus_gained);
		slots.connect(sig_focus_lost(), impl.get(), &TextFieldViewImpl::on_focus_lost);
		slots.connect(sig_activated(), impl.get(), &TextFieldViewImpl::on_activated);
		slots.connect(sig_deactivated(), impl.get(), &TextFieldViewImpl::on_deactivated);

		impl->scroll_timer.func_expired() = [&]()
		{
			if (impl->mouse_moves_left)
				impl->move(-1, false, false);
			else
				impl->move(1, false, false);
		};
	}

	TextFieldView::~TextFieldView()
	{
	}

	int TextFieldView::preferred_size() const
	{
		return impl->preferred_size;
	}

	void TextFieldView::set_preferred_size(int num_characters)
	{
		if (impl->preferred_size != num_characters)
		{
			impl->preferred_size = num_characters;
			set_needs_layout();
		}
	}

	std::string TextFieldView::text() const
	{
		return impl->text;
	}

	void TextFieldView::set_text(const std::string &text)
	{
		if (impl->lowercase)
			impl->text = StringHelp::text_to_lower(text);
		else if (impl->uppercase)
			impl->text = StringHelp::text_to_upper(text);
		else
			impl->text = text;

		impl->selection.reset();
		impl->cursor_pos = 0;
		impl->scroll_pos = 0.0f;

		impl->undo_buffer.clear();
		impl->redo_buffer.clear();

		set_needs_render();
	}

	std::string TextFieldView::placeholder() const
	{
		return impl->placeholder;
	}

	void TextFieldView::set_placeholder(const std::string &value)
	{
		impl->placeholder = value;
		set_needs_render();
	}

	TextAlignment TextFieldView::text_alignment() const
	{
		return impl->alignment;
	}

	void TextFieldView::set_text_alignment(TextAlignment alignment)
	{
		if (impl->alignment != alignment)
		{
			impl->alignment = alignment;
			set_needs_render();
		}
	}

	bool TextFieldView::is_read_only() const
	{
		return impl->readonly;
	}

	void TextFieldView::set_read_only(bool value)
	{
		if (impl->readonly != value)
		{
			impl->readonly = value;
			set_needs_render();
		}
	}

	bool TextFieldView::is_lowercase() const
	{
		return impl->lowercase;
	}

	void TextFieldView::set_lowercase(bool value)
	{
		if (impl->lowercase != value)
		{
			std::string old_text = text();
			impl->lowercase = value;
			set_text(old_text);
		}
	}

	bool TextFieldView::is_uppercase() const
	{
		return impl->uppercase;
	}

	void TextFieldView::set_uppercase(bool value)
	{
		if (impl->uppercase != value)
		{
			std::string old_text = text();
			impl->uppercase = value;
			set_text(old_text);
		}
	}

	bool TextFieldView::is_password_mode() const
	{
		return impl->password_mode;
	}

	void TextFieldView::set_password_mode(bool value)
	{
		if (impl->password_mode != value)
		{
			impl->password_mode = value;
			set_needs_render();
		}
	}

	int TextFieldView::max_length() const
	{
		return impl->max_length;
	}

	void TextFieldView::set_max_length(int length)
	{
		if (impl->max_length != length)
		{
			impl->max_length = length;
			set_needs_render();
		}
	}

	std::string TextFieldView::selection() const
	{
		return impl->get_selected_text();
	}

	size_t TextFieldView::selection_start() const
	{
		return impl->selection.start();
	}

	size_t TextFieldView::selection_length() const
	{
		return impl->selection.length();
	}

	void TextFieldView::set_selection(size_t start, size_t length)
	{
		start = std::min(start, impl->text.length());
		length = std::min(length, impl->text.length() - start);
		if (length == 0) start = 0;
		impl->selection.set(start, length);
		impl->cursor_pos = start + length;
		set_needs_render();
	}

	void TextFieldView::clear_selection()
	{
		set_selection(0, 0);
	}

	void TextFieldView::delete_selected_text()
	{
		if (impl->selection.length() > 0)
			impl->del();
	}

	void TextFieldView::select_all()
	{
		set_selection(0, impl->text.size());
	}

	void TextFieldView::set_select_all_on_focus_gain(bool value)
	{
		impl->select_all_on_focus_gain = value;
	}

	int TextFieldView::cursor_pos() const
	{
		return impl->cursor_pos;
	}

	void TextFieldView::set_cursor_pos(int pos)
	{
		impl->cursor_pos = pos;
		set_needs_render();
	}

	void TextFieldView::set_cursor_drawing_enabled(bool value)
	{
		impl->cursor_drawing_enabled_when_parent_focused = value;
	}

	int TextFieldView::text_int() const
	{
		return StringHelp::text_to_int(impl->text);
	}

	void TextFieldView::set_text(int number)
	{
		set_text(StringHelp::int_to_text(number));
	}

	float TextFieldView::text_float() const
	{
		return StringHelp::text_to_float(impl->text);
	}

	void TextFieldView::set_text(float number, int num_decimal_places)
	{
		set_text(StringHelp::float_to_text(number, num_decimal_places));
	}

	void TextFieldView::set_numeric_mode(bool enable, bool decimals)
	{
		impl->numeric_mode = enable;
		impl->numeric_mode_decimals = decimals;
	}

	void TextFieldView::set_input_mask(const std::string &mask)
	{
		impl->input_mask = mask;
	}

	void TextFieldView::set_decimal_character(const std::string &decimal_char)
	{
		impl->decimal_char = decimal_char;
	}

	Signal<void(KeyEvent &)> &TextFieldView::sig_before_edit_changed()
	{
		return impl->sig_before_edit_changed;
	}

	Signal<void(KeyEvent &)> &TextFieldView::sig_after_edit_changed()
	{
		return impl->sig_after_edit_changed;
	}

	Signal<void()> &TextFieldView::sig_selection_changed()
	{
		return impl->selection.sig_selection_changed;
	}

	Signal<void()> &TextFieldView::sig_enter_pressed()
	{
		return impl->sig_enter_pressed;
	}

	void TextFieldView::render_content(Canvas &canvas)
	{
		std::string txt_before = impl->get_text_before_selection();
		std::string txt_selected = impl->get_selected_text();
		std::string txt_after = impl->get_text_after_selection();

		if (impl->password_mode)
		{
			txt_before = impl->create_password(StringHelp::utf8_length(txt_before));
			txt_selected = impl->create_password(StringHelp::utf8_length(txt_selected));
			txt_after = impl->create_password(StringHelp::utf8_length(txt_after));
		}

		Font font = impl->get_font(canvas);

		float advance_before = font.measure_text(canvas, txt_before).advance.width;
		float advance_selected = font.measure_text(canvas, txt_selected).advance.width;
		float cursor_advance = canvas.grid_fit({ font.measure_text(canvas, impl->text.substr(0, impl->cursor_pos)).advance.width, 0.0f }).x;

		FontMetrics font_metrics = font.get_font_metrics(canvas);
		float baseline = font_metrics.get_baseline_offset();
		float top_y = baseline - font_metrics.get_ascent();
		float bottom_y = baseline + font_metrics.get_descent();

		// Keep cursor in view
		impl->scroll_pos = std::min(impl->scroll_pos, cursor_advance);
		impl->scroll_pos = std::max(impl->scroll_pos, cursor_advance - geometry().content_width + 1.0f);

		// Measure text for get_character_index()
		impl->last_measured_rects = font.get_character_indices(canvas, txt_before + txt_selected + txt_after);
		for (auto &box : impl->last_measured_rects)
			box.translate(-impl->scroll_pos, 0.0f);

		if (!txt_selected.empty())
		{
			Rectf selection_rect = Rectf(advance_before - impl->scroll_pos, top_y, advance_before + advance_selected - impl->scroll_pos, bottom_y);
			Path::rect(selection_rect).fill(canvas, focus_view() == this ? Brush::solid_rgb8(51, 153, 255) : Brush::solid_rgb8(200, 200, 200));
		}

		Colorf color = style_cascade().computed_value("color").color();
		font.draw_text(canvas, -impl->scroll_pos, baseline, txt_before, color);
		font.draw_text(canvas, advance_before - impl->scroll_pos, baseline, txt_selected, focus_view() == this ? Colorf(255, 255, 255) : color);
		font.draw_text(canvas, advance_before + advance_selected - impl->scroll_pos, baseline, txt_after, color);

		if (impl->cursor_blink_visible)
		{
			auto cursor_pos = canvas.grid_fit({ cursor_advance - impl->scroll_pos, top_y });
			Path::rect(cursor_pos.x, cursor_pos.y, 1.0f, bottom_y - top_y).fill(canvas, Brush(color));
		}

		if (impl->text.empty())
		{
			color.r = color.r * 0.5f + 0.5f;
			color.g = color.g * 0.5f + 0.5f;
			color.b = color.b * 0.5f + 0.5f;
			font.draw_text(canvas, 0.0f, baseline, impl->placeholder, color);
		}
	}

	float TextFieldView::calculate_preferred_width(Canvas &canvas)
	{
		Font font = impl->get_font(canvas);
		return font.measure_text(canvas, "X").advance.width * impl->preferred_size;
	}

	float TextFieldView::calculate_preferred_height(Canvas &canvas, float width)
	{
		Font font = impl->get_font(canvas);
		return font.get_font_metrics(canvas).get_line_height();
	}

	float TextFieldView::calculate_first_baseline_offset(Canvas &canvas, float width)
	{
		Font font = impl->get_font(canvas);
		return font.get_font_metrics(canvas).get_baseline_offset();
	}

	float TextFieldView::calculate_last_baseline_offset(Canvas &canvas, float width)
	{
		return first_baseline_offset(canvas, width);
	}

	/////////////////////////////////////////////////////////////////////////

	const Font &TextFieldViewImpl::get_font(Canvas &canvas)
	{
		if (!font)
			font = textfield->style_cascade().font(canvas);
		return font;
	}

	void TextFieldViewImpl::start_blink()
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

	void TextFieldViewImpl::stop_blink()
	{
		blink_timer.stop();
		cursor_blink_visible = false;
		textfield->set_needs_render();
	}

	void TextFieldViewImpl::on_focus_gained(FocusChangeEvent &e)
	{
		start_blink();
		cursor_blink_visible = true;
		textfield->set_needs_render();
		ignore_mouse_events = true;
		if (select_all_on_focus_gain) select_all();
	}

	void TextFieldViewImpl::on_focus_lost(FocusChangeEvent &e)
	{
		if (mouse_selecting)
		{
			scroll_timer.stop();
			mouse_selecting = false;
		}
		stop_blink();
		selection.reset();
	}

	void TextFieldViewImpl::on_activated(ActivationChangeEvent &e)
	{
		if (textfield->has_focus())
		{
			start_blink();
		}
	}

	void TextFieldViewImpl::on_deactivated(ActivationChangeEvent &e)
	{
		if (mouse_selecting)
		{
			scroll_timer.stop();
			mouse_selecting = false;
		}
		stop_blink();
	}

	void TextFieldViewImpl::on_key_press(KeyEvent &e)
	{
		if (e.key() == Key::key_return)
		{
			sig_enter_pressed();
			e.stop_propagation();
			return;
		}

		sig_before_edit_changed(e);
		if (e.propagation_stopped())
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
		else if (e.key() == Key::left)
		{
			move(-1, e.ctrl_down(), e.shift_down());
			e.stop_propagation();
		}
		else if (e.key() == Key::right)
		{
			move(1, e.ctrl_down(), e.shift_down());
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
			home(e.shift_down());
			e.stop_propagation();
		}
		else if (e.key() == Key::end)
		{
			end(e.shift_down());
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

	void TextFieldViewImpl::on_key_release(KeyEvent &e)
	{
	}

	void TextFieldViewImpl::on_pointer_press(PointerEvent &e)
	{
		if (textfield->has_focus())
		{
			mouse_selecting = true;
			cursor_pos = get_character_index(e.pos(textfield).x);
			selection.set(cursor_pos, 0);
		}
		else
		{
			textfield->set_focus();
		}

	}

	void TextFieldViewImpl::on_pointer_release(PointerEvent &e)
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
			cursor_pos = get_character_index(e.pos(textfield).x);
			selection.set_tail(cursor_pos);
			textfield->set_focus();
			textfield->set_needs_render();
		}
	}

	void TextFieldViewImpl::on_pointer_move(PointerEvent &e)
	{
		if (!mouse_selecting)
			return;
		if (ignore_mouse_events)
			return;

		Rect content_rect = textfield->geometry().content_box();
		int xpos = e.pos(textfield).x;
		if (xpos < 0 || xpos > content_rect.get_width())
		{
			if (xpos < 0)
				mouse_moves_left = true;
			else
				mouse_moves_left = false;

			if (!readonly)
				scroll_timer.start(50, true);
		}
		else
		{
			scroll_timer.stop();
			cursor_pos = get_character_index(xpos);
			selection.set_tail(cursor_pos);
		}

	}

	void TextFieldViewImpl::select_all()
	{
		selection.set(0, text.size());
	}

	void TextFieldViewImpl::move(int steps, bool ctrl, bool shift)
	{
		int pos = cursor_pos;

		// Jump over words if control is pressed.
		if (ctrl)
		{
			if (steps < 0)
				steps = find_previous_break_character(pos - 1) - pos;
			else
				steps = find_next_break_character(pos + 1) - pos;

			pos += steps;
			if (pos < 0)
				pos = 0;
			if (pos >(int)text.size())
				pos = text.size();
		}
		else
		{
			UTF8_Reader utf8_reader(text.data(), text.length());
			utf8_reader.set_position(pos);
			if (steps > 0)
			{
				for (int i = 0; i < steps; i++)
					utf8_reader.next();
			}
			else if (steps < 0)
			{
				for (int i = 0; i < -steps; i++)
					utf8_reader.prev();
			}

			pos = utf8_reader.get_position();
		}

		if (shift)
		{
			if (selection.length() == 0)
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

	void TextFieldViewImpl::home(bool shift)
	{
		if (cursor_pos == 0)
			return;

		if (shift)
		{
			if (selection.length() == 0)
				selection.set_head(cursor_pos);
			selection.set_tail(0);
		}
		else
		{
			selection.reset();
		}

		cursor_pos = 0;
		textfield->set_needs_render();
	}

	void TextFieldViewImpl::end(bool shift)
	{
		if (cursor_pos == text.size())
			return;

		if (shift)
		{
			if (selection.length() == 0)
				selection.set_head(cursor_pos);
			selection.set_tail(text.size());
		}
		else
		{
			selection.reset();
		}

		cursor_pos = text.size();
		textfield->set_needs_render();
	}

	void TextFieldViewImpl::backspace()
	{
		if (selection.length() > 0)
		{
			del();
		}
		else if (cursor_pos > 0)
		{
			save_undo();

			UTF8_Reader utf8_reader(text.data(), text.length());
			utf8_reader.set_position(cursor_pos);
			utf8_reader.prev();
			int new_cursor_pos = utf8_reader.get_position();

			text.erase(text.begin() + new_cursor_pos, text.begin() + cursor_pos);
			cursor_pos = new_cursor_pos;

			textfield->set_needs_render();
		}
	}

	void TextFieldViewImpl::del()
	{
		if (selection.length() > 0)
		{
			save_undo();

			cursor_pos = selection.start();
			text.erase(text.begin() + selection.start(), text.begin() + selection.end());
			selection.reset();

			textfield->set_needs_render();
		}
		else if (cursor_pos < text.length())
		{
			save_undo();

			UTF8_Reader utf8_reader(text.data(), text.length());
			utf8_reader.set_position(cursor_pos);
			text.erase(text.begin() + cursor_pos, text.begin() + cursor_pos + utf8_reader.get_char_length());

			textfield->set_needs_render();
		}
	}

	void TextFieldViewImpl::cut()
	{
		copy();
		if (selection.length() == 0)
			selection.set(0, text.length());
		del();
	}

	void TextFieldViewImpl::copy()
	{
		if (!password_mode)
		{
			ViewTree *tree = textfield->view_tree();
			if (tree)
			{
				DisplayWindow window = tree->display_window();
				if (window)
				{
					if (selection.length() > 0)
						window.set_clipboard_text(get_selected_text());
					else
						window.set_clipboard_text(text);
				}
			}
		}
	}

	void TextFieldViewImpl::paste()
	{
		ViewTree *tree = textfield->view_tree();
		if (tree)
		{
			DisplayWindow window = tree->display_window();
			if (window)
				add(window.get_clipboard_text());
		}
	}

	void TextFieldViewImpl::undo()
	{
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
	}

	void TextFieldViewImpl::redo()
	{
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
	}

	void TextFieldViewImpl::save_undo()
	{
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
	}

	void TextFieldViewImpl::add(std::string new_text)
	{
		if (selection.length() > 0)
			del();

		bool accepts_input = false;
		if (numeric_mode)
		{
			// '-' can only be added once, and only as the first character.
			if (new_text == "-" && cursor_pos == 0 && text.find("-") == std::string::npos)
			{
				accepts_input = true;
			}
			else if (numeric_mode_decimals && new_text == decimal_char && cursor_pos > 0) // add decimal char 
			{
				if (text.find(decimal_char) == std::string::npos) // allow only one decimal char.
				{
					accepts_input = true;
				}
			}
			else if (numeric_mode_characters.find(new_text) != std::string::npos) // 0-9
			{
				accepts_input = true;
			}
		}
		else
		{
			if (!input_mask.empty())
				accepts_input = input_mask_accepts_input(new_text);
			else
				accepts_input = true;
		}

		if (!accepts_input)
			return;

		if (max_length >= 0 && StringHelp::utf8_length(text) + StringHelp::utf8_length(new_text) > (size_t)max_length) // To do: clip rather than fully reject
			return;

		save_undo();

		if (lowercase)
			new_text = StringHelp::text_to_lower(new_text);
		else if (uppercase)
			new_text = StringHelp::text_to_upper(new_text);

		text = text.substr(0, cursor_pos) + new_text + text.substr(cursor_pos);
		cursor_pos += new_text.size();

		textfield->set_needs_render();
	}

	std::string TextFieldViewImpl::get_text_before_selection() const
	{
		size_t start = std::min(selection.start(), text.length());
		return text.substr(0, start);
	}

	std::string TextFieldViewImpl::get_selected_text() const
	{
		size_t start = std::min(selection.start(), text.length());
		size_t length = std::min(selection.length(), text.length() - start);
		return text.substr(start, length);
	}

	std::string TextFieldViewImpl::get_text_after_selection() const
	{
		size_t start = std::min(selection.start(), text.length());
		size_t length = std::min(selection.length(), text.length() - start);
		return text.substr(start + length);
	}

	std::string TextFieldViewImpl::create_password(std::string::size_type num_letters) const
	{
		return std::string(num_letters, '*');
	}

	bool TextFieldViewImpl::input_mask_accepts_input(const std::string &str) const
	{
		return str.find_first_not_of(input_mask) == std::string::npos;
	}

	int TextFieldViewImpl::find_next_break_character(int search_start) const
	{
		if (search_start >= int(text.size()) - 1)
			return text.size();

		int pos = text.find_first_of(break_characters, search_start);
		if (pos == std::string::npos)
			return text.size();
		return pos;
	}

	int TextFieldViewImpl::find_previous_break_character(int search_start) const
	{
		if (search_start <= 0)
			return 0;
		int pos = text.find_last_of(break_characters, search_start);
		if (pos == std::string::npos)
			return 0;
		return pos;
	}


	unsigned int TextFieldViewImpl::get_character_index(int mouse_x_wincoords)
	{
		int mouse_x = mouse_x_wincoords;

		if (last_measured_rects.empty())
			return 0;

		for (unsigned int cnt = 0; cnt < last_measured_rects.size(); cnt++)
		{
			if ((last_measured_rects[cnt].left <= mouse_x) && (last_measured_rects[cnt].right > mouse_x))
				return cnt + 1;
		}
		if (last_measured_rects[0].left >= mouse_x)
			return 0;
		return last_measured_rects.size();
	}

	Size TextFieldViewImpl::get_visual_text_size(Canvas &canvas, int pos, int npos)
	{
		Font font = get_font(canvas);

		return password_mode ? 
			Size(font.measure_text(canvas, create_password(StringHelp::utf8_length(text.substr(pos, npos)))).bbox_size) :
			Size(font.measure_text(canvas, text.substr(pos, npos)).bbox_size);
	}
	const std::string TextFieldViewImpl::numeric_mode_characters = "0123456789";
	const std::string TextFieldViewImpl::break_characters = " ::;,.-";
}

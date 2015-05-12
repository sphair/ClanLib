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
#include "API/UI/StandardViews/text_field_view.h"
#include "API/UI/Style/style.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/2D/brush.h"
#include "API/Display/2D/pen.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/glyph_metrics.h"
#include "API/Display/Font/font_metrics.h"
#include "text_field_view_impl.h"
#include <algorithm>
#include <cmath>
#include "API/Display/Render/shared_gc_data.h"
#include "API/UI/StandardViews/window_view.h"
#include "API/UI/Events/pointer_event.h"

namespace clan
{
	TextFieldView::TextFieldView() : impl(new TextFieldViewImpl())
	{
		impl->textfield = this;

		set_focus_policy(FocusPolicy::accept);
		set_cursor(StandardCursor::ibeam);

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

		impl->selection_start = 0;
		impl->selection_length = 0;
		impl->cursor_pos = impl->text.size();
		impl->scroll_pos = 0.0f;

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
		return impl->selection_start;
	}

	size_t TextFieldView::selection_length() const
	{
		return impl->selection_length;
	}

	void TextFieldView::set_selection(size_t start, size_t length)
	{
		start = std::min(start, impl->text.length());
		length = std::min(length, impl->text.length() - start);
		if (length == 0) start = 0;
		if (impl->selection_start != start || impl->selection_length != length)
		{
			impl->selection_start = start;
			impl->selection_length = length;
			impl->cursor_pos = start + length;
			set_needs_render();
		}
	}

	void TextFieldView::clear_selection()
	{
		set_selection(0, 0);
	}

	void TextFieldView::delete_selected_text()
	{
		if (impl->selection_length > 0)
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
		return impl->sig_selection_changed;
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

		FontMetrics font_metrics = font.get_font_metrics(canvas);
		float baseline = font_metrics.get_baseline_offset();
		float top_y = baseline - font_metrics.get_ascent();
		float bottom_y = baseline + font_metrics.get_descent();

		// Measure text for get_character_index()
		impl->last_measured_rects = font.get_character_indices(canvas, txt_before + txt_selected + txt_after);

		if (!txt_selected.empty())
		{
			Rectf selection_rect = Rectf(advance_before, top_y, advance_before + advance_selected, bottom_y);
			Path::rect(selection_rect).fill(canvas, focus_view() == this ? Brush::solid_rgb8(51, 153, 255) : Brush::solid_rgb8(200, 200, 200));
		}

		Colorf color = style_cascade().computed_value("color").color();
		font.draw_text(canvas, 0.0f, baseline, txt_before, color);
		font.draw_text(canvas, advance_before, baseline, txt_selected, focus_view() == this ? Colorf(255, 255, 255) : color);
		font.draw_text(canvas, advance_before + advance_selected, baseline, txt_after, color);

		float cursor_advance = std::round(font.measure_text(canvas, impl->text.substr(0, impl->cursor_pos)).advance.width);

		if (impl->cursor_blink_visible)
			Path::rect(cursor_advance, top_y, 1.0f, bottom_y - top_y).fill(canvas, Brush(color));
	}

	float TextFieldView::calculate_preferred_width(Canvas &canvas)
	{
		if (style_cascade().computed_value("width").is_keyword("auto"))
		{
			Font font = impl->get_font(canvas);
			return font.measure_text(canvas, impl->text).advance.width;
		}
		else
			return style_cascade().computed_value("width").number();
	}

	float TextFieldView::calculate_preferred_height(Canvas &canvas, float width)
	{
		if (style_cascade().computed_value("height").is_keyword("auto"))
		{
			Font font = impl->get_font(canvas);
			return font.get_font_metrics(canvas).get_line_height();
		}
		else
			return style_cascade().computed_value("height").number();
	}

	float TextFieldView::calculate_first_baseline_offset(Canvas &canvas, float width)
	{
		Font font = impl->get_font(canvas);
		return font.get_font_metrics(canvas).get_baseline_offset();
	}

	float TextFieldView::calculate_last_baseline_offset(Canvas &canvas, float width)
	{
		return get_first_baseline_offset(canvas, width);
	}

	/////////////////////////////////////////////////////////////////////////

	Font &TextFieldViewImpl::get_font(Canvas &canvas)
	{
		if (font.is_null())
			font = textfield->style_cascade().get_font(canvas);
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
		set_text_selection(0, 0);
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
			set_text_selection(cursor_pos, 0);
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
			int sel_end = get_character_index(e.pos(textfield).x);
			selection_length = sel_end - selection_start;
			cursor_pos = sel_end;
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
			selection_length = cursor_pos - selection_start;
			textfield->set_needs_render();
		}

	}

	void TextFieldViewImpl::select_all()
	{
		set_text_selection(0, text.size());
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
			select_to(pos);
		else
			set_text_selection(0, 0);

		cursor_pos = pos;
		textfield->set_needs_render();
	}

	void TextFieldViewImpl::select_to(size_t pos)
	{
		size_t anchor = cursor_pos;
		if (selection_length > 0)
		{
			if (cursor_pos == selection_start)
			{
				anchor = selection_start + selection_length;
			}
			else
			{
				anchor = selection_start;
			}
		}

		if (pos < anchor)
		{
			set_text_selection(pos, anchor - pos);
		}
		else
		{
			set_text_selection(anchor, pos - anchor);
		}
	}

	void TextFieldViewImpl::home(bool shift)
	{
		if (cursor_pos == 0)
			return;

		if (shift)
		{
			select_to(0);
		}
		else
		{
			set_text_selection(0, 0);
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
			select_to(text.size());
		}
		else
		{
			set_text_selection(0, 0);
		}

		cursor_pos = text.size();
		textfield->set_needs_render();
	}

	void TextFieldViewImpl::backspace()
	{
		if (selection_length > 0)
		{
			del();
		}
		else if (cursor_pos > 0)
		{
			undo_info.first_erase = false;
			if (undo_info.first_text_insert)
			{
				undo_info.undo_text = text;
				undo_info.first_text_insert = false;
			}

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
		if (selection_length > 0)
		{
			size_t start = selection_start;
			size_t length = selection_length;
			set_text_selection(0, 0);

			undo_info.first_erase = false;
			if (undo_info.first_text_insert)
			{
				undo_info.undo_text = text;
				undo_info.first_text_insert = false;
			}

			cursor_pos = start;
			text.erase(text.begin() + start, text.begin() + start + length);

			textfield->set_needs_render();
		}
		else if (cursor_pos < text.length())
		{
			undo_info.first_erase = false;
			if (undo_info.first_text_insert)
			{
				undo_info.undo_text = text;
				undo_info.first_text_insert = false;
			}

			UTF8_Reader utf8_reader(text.data(), text.length());
			utf8_reader.set_position(cursor_pos);
			text.erase(text.begin() + cursor_pos, text.begin() + cursor_pos + utf8_reader.get_char_length());

			textfield->set_needs_render();
		}
	}

	void TextFieldViewImpl::cut()
	{
		copy();
		if (selection_length == 0)
			set_text_selection(0, text.length());
		del();
	}

	void TextFieldViewImpl::copy()
	{
		if (!password_mode)
		{
			WindowView *window_view = dynamic_cast<WindowView*>(textfield->root_view());
			if (window_view)
			{
				if (selection_length > 0)
					window_view->get_display_window().set_clipboard_text(get_selected_text());
				else
					window_view->get_display_window().set_clipboard_text(text);
			}
		}
	}

	void TextFieldViewImpl::paste()
	{
		WindowView *window_view = dynamic_cast<WindowView*>(textfield->root_view());
		if (window_view)
		{
			add(window_view->get_display_window().get_clipboard_text());
		}
	}

	void TextFieldViewImpl::undo()
	{
		if (!readonly)
		{
			set_text_selection(0, 0);

			std::string tmp = undo_info.undo_text;
			undo_info.undo_text = text;
			text = tmp;

			cursor_pos = std::min(cursor_pos, text.size());

			textfield->set_needs_render();
		}
	}

	void TextFieldViewImpl::add(std::string new_text)
	{
		if (selection_length > 0)
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

		undo_info.first_erase = false;
		if (undo_info.first_text_insert)
		{
			undo_info.undo_text = text;
			undo_info.first_text_insert = false;
		}

		if (lowercase)
			new_text = StringHelp::text_to_lower(new_text);
		else if (uppercase)
			new_text = StringHelp::text_to_upper(new_text);

		text = text.substr(0, cursor_pos) + new_text + text.substr(cursor_pos);
		cursor_pos += new_text.size();

		textfield->set_needs_render();
	}

	void TextFieldViewImpl::set_text_selection(size_t start, size_t length)
	{
		start = std::min(start, text.length());
		length = std::min(length, text.length() - start);
		//if (length == 0) start = 0;
		if (selection_length != length || selection_start != start)
		{
			selection_start = start;
			selection_length = length;
			textfield->set_needs_render();

			sig_selection_changed();
		}
	}

	std::string TextFieldViewImpl::get_text_before_selection() const
	{
		size_t start = std::min(selection_start, text.length());
		return text.substr(0, start);
	}

	std::string TextFieldViewImpl::get_selected_text() const
	{
		size_t start = std::min(selection_start, text.length());
		size_t length = std::min(selection_length, text.length() - start);
		return text.substr(start, length);
	}

	std::string TextFieldViewImpl::get_text_after_selection() const
	{
		size_t start = std::min(selection_start, text.length());
		size_t length = std::min(selection_length, text.length() - start);
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

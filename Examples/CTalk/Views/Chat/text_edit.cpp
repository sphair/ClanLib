
#include "precomp.h"
#include "text_edit.h"

TextEdit::TextEdit(CL_GUIComponent *parent)
: CL_GUIComponent(parent), mouse_down(false), cursor_pos(0), cursor_visible(false), blink_duration(500), selection_start(0), selection_length(0)
{
	set_type_name("textedit");
/*
	CL_GUIConsumedKeys consumed_keys;
	consumed_keys.set_consumed(CL_GUIConsumedKeys::key_arrows);
	consumed_keys.set_consumed(CL_GUIConsumedKeys::key_characters);
	consumed_keys.set_consumed(CL_GUIConsumedKeys::key_space);
	set_consumed_keys(consumed_keys);
*/
	func_render().set(this, &TextEdit::on_render);
	func_resized().set(this, &TextEdit::on_resized);
	func_focus_gained().set(this, &TextEdit::on_focus_gained);
	func_focus_lost().set(this, &TextEdit::on_focus_lost);
	func_pointer_enter().set(this, &TextEdit::on_pointer_enter);
	func_pointer_exit().set(this, &TextEdit::on_pointer_exit);
	func_input_pressed().set(this, &TextEdit::on_input_pressed);
	func_input_released().set(this, &TextEdit::on_input_released);
	func_input_doubleclick().set(this, &TextEdit::on_input_doubleclick);
	func_input_pointer_moved().set(this, &TextEdit::on_input_pointer_moved);

	cursor_timer.func_expired().set(this, &TextEdit::on_blink_cursor);

	part_background = CL_GUIThemePart(this);
	part_cursor = CL_GUIThemePart(this, "cursor");
	part_background.set_state("normal", true);
	part_cursor.set_state("blink_off", true);

	on_resized();
}

CL_String TextEdit::get_text() const
{
	return text;
}

void TextEdit::set_text(const CL_String &new_text)
{
	text = new_text;
	selection_start = 0;
	selection_length = 0;
	cursor_pos = text.length();
	populate_span();
}

size_t TextEdit::get_cursor_pos() const
{
	return cursor_pos;
}

int TextEdit::get_text_height(int width)
{
	CL_Rect contentbox = part_background.get_content_box(CL_Size(width, 100));
	CL_GraphicContext gc = get_gc();
	span.layout(gc, contentbox.get_width());
	CL_Size s = span.get_size();
	contentbox.bottom = contentbox.top+s.height;
	CL_Rect renderbox = part_background.get_render_box(contentbox);
	return renderbox.get_height();
}

void TextEdit::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	CL_Rect renderbox = get_size();
	CL_Rect contentbox = part_background.get_content_box(renderbox);
	part_background.render_box(gc, renderbox, clip_rect);
	span.set_position(contentbox.get_top_left());
	span.draw_layout(gc);
}

void TextEdit::on_resized()
{
	CL_Rect renderbox = get_size();
	CL_Rect contentbox = part_background.get_content_box(renderbox);
	CL_GraphicContext gc = get_gc();
	span.layout(gc, contentbox.get_width());
	request_repaint();
}

bool TextEdit::on_focus_gained()
{
	cursor_timer.start(blink_duration);
	cursor_visible = true;
	request_repaint();
	return true;
}

bool TextEdit::on_focus_lost()
{
	cursor_timer.stop();
	cursor_visible = false;
	request_repaint();
	return true;
}

bool TextEdit::on_pointer_enter()
{
	set_cursor(cl_cursor_ibeam);
	return true;
}

bool TextEdit::on_pointer_exit()
{
	set_cursor(cl_cursor_arrow);
	return true;
}

bool TextEdit::on_input_pressed(const CL_InputEvent &input_event)
{
	if (filter_event(input_event))
		return true;

	switch (input_event.device.get_type())
	{
	case CL_InputDevice::keyboard:
		return on_input_keyboard_pressed(input_event);
	case CL_InputDevice::pointer:
		return on_input_pointer_pressed(input_event);
	default:
		return false;
	}
}

bool TextEdit::on_input_released(const CL_InputEvent &input_event)
{
	if (filter_event(input_event))
		return true;

	switch (input_event.device.get_type())
	{
	case CL_InputDevice::pointer:
		return on_input_pointer_released(input_event);
	default:
		return false;
	}
}

bool TextEdit::on_input_keyboard_pressed(const CL_InputEvent &e)
{
	show_cursor_on_activity();

	switch (e.id)
	{
	case CL_KEY_ENTER:
	case CL_KEY_ESCAPE:
	case CL_KEY_TAB:
		return false;
	case CL_KEY_LEFT:
		move(-1, e);
		return true;
	case CL_KEY_RIGHT:
		move(1, e);
		return true;
	case CL_KEY_BACKSPACE:
		backspace(e);
		return true;
	case CL_KEY_DELETE:
		del(e);
		return true;
	case CL_KEY_HOME:
		home(e);
		return true;
	case CL_KEY_END:
		end(e);
		return true;
	}

	if (e.ctrl)
	{
		switch (e.id)
		{
		case CL_KEY_A:
			select_all();
			return true;
		case CL_KEY_C:
			copy_to_clipboard();
			return true;
		case CL_KEY_X:
			cut_to_clipboard();
			return true;
		case CL_KEY_V:
			paste_from_clipboard();
			return true;
		}
	}
	else
	{
		if (!e.str.empty() && !(e.str[0] >= 0 && e.str[0] < 32))
		{
			insert_text(e.str);
			return true;
		}
	}

	return false;
}

bool TextEdit::on_input_pointer_pressed(const CL_InputEvent &input_event)
{
	set_focus();
	if (input_event.id == CL_MOUSE_LEFT)
	{
		capture_mouse(true);
		mouse_down = true;
	}

	return true;
}

bool TextEdit::on_input_pointer_released(const CL_InputEvent &input_event)
{
	if (input_event.id == CL_MOUSE_LEFT)
	{
		capture_mouse(false);
		mouse_down = false;
	}

	return true;
}

bool TextEdit::on_input_doubleclick(const CL_InputEvent &input_event)
{
	if (filter_event(input_event))
		return true;

	return true;
}

bool TextEdit::on_input_pointer_moved(const CL_InputEvent &input_event)
{
	if (filter_event(input_event))
		return true;

	return true;
}

void TextEdit::on_blink_cursor()
{
	cursor_visible = !cursor_visible;
	part_cursor.set_state("blink_on", cursor_visible);
	part_cursor.set_state("blink_off", !cursor_visible);
	request_repaint();
}

void TextEdit::populate_span()
{
	span.clear();
	span.add_text(text, part_background.get_font(), CL_Colorf::black);
	span.set_selection_range(selection_start, selection_start + selection_length);
	on_resized();
}

void TextEdit::show_cursor_on_activity()
{
	cursor_timer.start(blink_duration);
	if (!cursor_visible)
	{
		cursor_visible = true;
		request_repaint();
	}
}

void TextEdit::move(int steps, const CL_InputEvent &e)
{
	if (e.shift && selection_length == 0)
		selection_start = cursor_pos;

	// Jump over words if control is pressed.
	if (e.ctrl)
	{
		if (steps < 0)
			steps = find_previous_break_character(cursor_pos-1) - cursor_pos;
		else 
			steps = find_next_break_character(cursor_pos+1) - cursor_pos;
	}

	cursor_pos += steps;
	if (cursor_pos < 0)
		cursor_pos = 0;
	if (cursor_pos > (int)text.size())
		cursor_pos = text.size();

	if (e.shift)
	{
		selection_length = cursor_pos - selection_start;
	}
	else
	{
		// Clear the selection if a cursor key is pressed but shift isn't down. 
		selection_start = -1;
		selection_length = 0;
	}

	span.set_selection_range(selection_start, selection_start+selection_length);
	request_repaint();
}

void TextEdit::backspace(const CL_InputEvent &e)
{
	if (selection_length > 0)
	{
		erase_selection();
	}
	else if (cursor_pos > 0)
	{
		text.erase(text.begin()+(cursor_pos-1));
		cursor_pos--;
		populate_span();
	}

}

void TextEdit::del(const CL_InputEvent &e)
{
	if (selection_length > 0)
	{
		erase_selection();
	}
	else if (cursor_pos < text.length())
	{
		text.erase(text.begin()+cursor_pos);
		populate_span();
	}
}

void TextEdit::home(const CL_InputEvent &e)
{
	selection_start = 0;
	selection_length = 0;
	cursor_pos = 0;
}

void TextEdit::end(const CL_InputEvent &e)
{
	selection_start = 0;
	selection_length = 0;
	cursor_pos = text.length();
}

void TextEdit::select_all()
{
	selection_start = 0;
	selection_length = text.length();
	span.set_selection_range(selection_start, selection_start+selection_length);
	request_repaint();
}

void TextEdit::copy_to_clipboard()
{
	get_display_window().set_clipboard_text(text.substr(selection_start, selection_length));
}

void TextEdit::cut_to_clipboard()
{
	copy_to_clipboard();
	erase_selection();
}

void TextEdit::paste_from_clipboard()
{
	erase_selection();
	insert_text(get_display_window().get_clipboard_text());
}

void TextEdit::insert_text(const CL_String &new_text)
{
	erase_selection();
	text.insert(cursor_pos, new_text);
	cursor_pos += new_text.length();
	populate_span();
}

CL_String TextEdit::break_characters = cl_text(" ::;,.-");

int TextEdit::find_next_break_character(int search_start)
{
	if (search_start >= int(text.size())-1)
		return text.size();

	int pos = text.find_first_of(break_characters, search_start);
	if (pos == CL_String::npos)
		return text.size();
	return pos;
}

int TextEdit::find_previous_break_character(int search_start)
{
	if (search_start <= 0)
		return 0;
	int pos = text.find_last_of(break_characters, search_start);
	if (pos == CL_String::npos)
		return 0;
	return pos;
}

void TextEdit::erase_selection()
{
	if (selection_length > 0)
	{
		text.erase(text.begin()+selection_start, text.begin()+selection_start+selection_length);
		selection_start = 0;
		selection_length = 0;
		if (cursor_pos > text.length())
			cursor_pos = text.length();
	}
}

bool TextEdit::filter_event(const CL_InputEvent &e)
{
	// The 'unhandled event' design seem to be broken in CL_LineEdit (which we emulate here),
	// so we change it to instead mean 'first chance' on handling event.
	if (!cb_unhandled_event.is_null())
	{
		bool consumed = cb_unhandled_event.invoke(e);
		if (consumed)
			return true;
	}
	
	return false;
}

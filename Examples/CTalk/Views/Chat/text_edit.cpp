
#include "precomp.h"
#include "text_edit.h"

TextEdit::TextEdit(CL_GUIComponent *parent)
: CL_GUIComponent(parent), mouse_down(false), cursor_pos(0), cursor_visible(false), blink_duration(500)
{
	set_type_name("channeltopic");
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

	on_resized();
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
		backspace();
		return true;
	case CL_KEY_DELETE:
		del();
		return true;
	case CL_KEY_HOME:
		home();
		return true;
	case CL_KEY_END:
		end();
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
	return true;
}

bool TextEdit::on_input_pointer_moved(const CL_InputEvent &input_event)
{
	return true;
}

void TextEdit::on_blink_cursor()
{
	cursor_visible = !cursor_visible;
	request_repaint();
}

void TextEdit::populate_span()
{
	span.clear();
	span.add_text(text, part_background.get_font(), CL_Colorf::black);
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

void TextEdit::move(int direction, const CL_InputEvent &e)
{

}

void TextEdit::backspace()
{

}

void TextEdit::del()
{

}

void TextEdit::home()
{

}

void TextEdit::end()
{

}

void TextEdit::select_all()
{

}

void TextEdit::copy_to_clipboard()
{

}

void TextEdit::cut_to_clipboard()
{

}

void TextEdit::paste_from_clipboard()
{

}

void TextEdit::insert_text(const CL_String &text)
{

}

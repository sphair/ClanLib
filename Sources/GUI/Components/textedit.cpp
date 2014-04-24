/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include <algorithm>
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/Components/textedit.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/display_window.h"
#include "../gui_css_strings.h"
#include "textedit_impl.h"
#include "API/Display/2D/canvas.h"

#ifndef WIN32
#include "stdlib.h"
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// TextEdit Construction:

TextEdit::TextEdit(GUIComponent *parent)
: GUIComponent(parent, CssStr::TextEdit::type_name), impl(new TextEdit_Impl)
{
	set_focus_policy(focus_local);

	func_render().set(impl.get(), &TextEdit_Impl::on_render);
	func_process_message().set(impl.get(), &TextEdit_Impl::on_process_message);
	func_resized().set(impl.get(), &TextEdit_Impl::on_resized);
	func_enablemode_changed().set(impl.get(), &TextEdit_Impl::on_enable_changed);

	impl->textedit = this;
	impl->timer.func_expired().set(impl.get(), &TextEdit_Impl::on_timer_expired);

	impl->scroll_timer.func_expired().set(impl.get(), &TextEdit_Impl::on_scroll_timer_expired);
	impl->create_components();
	impl->create_parts();
}

TextEdit::~TextEdit()
{
}

/////////////////////////////////////////////////////////////////////////////
// TextEdit Attributes:

TextEdit *TextEdit::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	TextEdit *object = NULL;
	if (reference_component)
		object = dynamic_cast<TextEdit*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find TextEdit named item: %1", id));

	return object;
}

bool TextEdit::is_read_only() const
{
	return impl->readonly;
}

bool TextEdit::is_lowercase() const
{
	return impl->lowercase;
}

bool TextEdit::is_uppercase() const
{
	return impl->uppercase;
}

int TextEdit::get_max_length() const
{
	return impl->max_length;
}

std::string TextEdit::get_line_text(int line) const
{
	if (line >= 0 && line < (int)impl->lines.size())
		return impl->lines[line].text;
	else
		return std::string();
}

std::string TextEdit::get_text() const
{
	std::string::size_type size = 0;
	for (size_t i = 0; i < impl->lines.size(); i++)
		size += impl->lines[i].text.size();
	size += impl->lines.size() - 1;

	std::string text;
	text.reserve(size);

	for (size_t i = 0; i < impl->lines.size(); i++)
	{
		if (i > 0)
			text.push_back('\n');
		text += impl->lines[i].text;
	}

	return text;
}

int TextEdit::get_line_count() const
{
	return impl->lines.size();
}

std::string TextEdit::get_selection() const
{
	std::string::size_type offset = impl->to_offset(impl->selection_start);
	int start = min(offset, offset + impl->selection_length);
	return get_text().substr(start, abs(impl->selection_length));
}

int TextEdit::get_selection_start() const
{
	return impl->to_offset(impl->selection_start);
}

int TextEdit::get_selection_length() const
{
	return impl->selection_length;
}

int TextEdit::get_cursor_pos() const
{
	return impl->to_offset(impl->cursor_pos);
}

int TextEdit::get_cursor_line_number() const
{
	return impl->cursor_pos.y;
}

/////////////////////////////////////////////////////////////////////////////
// TextEdit Operations:

void TextEdit::select_all()
{
	std::string::size_type size = 0;
	for (size_t i = 0; i < impl->lines.size(); i++)
		size += impl->lines[i].text.size();
	size += impl->lines.size() - 1;
	set_selection(0, size);
}

void TextEdit::set_read_only(bool enable)
{
	if (impl->readonly != enable)
	{
		impl->readonly = enable;
		request_repaint();
	}
}

void TextEdit::set_lowercase(bool enable)
{
	if (impl->lowercase != enable)
	{
		impl->lowercase = enable;
		request_repaint();
	}
}

void TextEdit::set_uppercase(bool enable)
{
	if (impl->uppercase != enable)
	{
		impl->uppercase = enable;
		request_repaint();
	}
}

void TextEdit::set_max_length(int length)
{
	if (impl->max_length != length)
	{
		impl->max_length = length;

		std::string::size_type size = 0;
		for (size_t i = 0; i < impl->lines.size(); i++)
			size += impl->lines[i].text.size();
		size += impl->lines.size() - 1;

		if ((int)size > length)
		{
			InputEvent no_event;
			if (!impl->func_before_edit_changed.is_null())
				impl->func_before_edit_changed.invoke(no_event);
			set_selection(length, size - length);
			delete_selected_text();
			if (!impl->func_after_edit_changed.is_null())
				impl->func_after_edit_changed.invoke(no_event);
		}
		request_repaint();
	}
}

void TextEdit::set_text(const std::string &text)
{
	impl->lines.clear();
	std::string::size_type start = 0;
	std::string::size_type end = text.find('\n');
	while (end != std::string::npos)
	{
		TextEdit_Impl::Line line;
		line.text = text.substr(start, end - start);
		impl->lines.push_back(line);
		start = end + 1;
		end = text.find('\n', start);
	}
	TextEdit_Impl::Line line;
	line.text = text.substr(start);
	impl->lines.push_back(line);

	impl->clip_start_offset = 0;
	set_cursor_pos(0);
	clear_selection();
	request_repaint();
}

void TextEdit::add_text(const std::string &text)
{
	std::string::size_type start = 0;
	std::string::size_type end = text.find('\n');
	while (end != std::string::npos)
	{
		TextEdit_Impl::Line line;
		line.text = text.substr(start, end - start);
		impl->lines.push_back(line);
		start = end + 1;
		end = text.find('\n', start);
	}
	TextEdit_Impl::Line line;
	line.text = text.substr(start);
	impl->lines.push_back(line);

//	impl->clip_start_offset = 0;
//	set_cursor_pos(0);
	clear_selection();
	request_repaint();
}

void TextEdit::set_selection(int pos, int length)
{
	impl->selection_start = Vec2i(pos, 0);
	impl->selection_length = length;
	request_repaint();
}

void TextEdit::clear_selection()
{
	set_selection(0,0);
	request_repaint();
}

void TextEdit::delete_selected_text()
{
	if (get_selection_length() == 0)
		return;

	std::string::size_type offset = impl->to_offset(impl->selection_start);
	int start = min(offset, offset + impl->selection_length);
	int length = abs(impl->selection_length);

	clear_selection();
	std::string text = get_text();
	set_text(text.substr(0, start) + text.substr(start + length));
	set_cursor_pos(start);
}

void TextEdit::set_cursor_pos(int pos)
{
	impl->cursor_pos = impl->from_offset(pos);
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// TextEdit Events:

Callback<void(InputEvent &)> &TextEdit::func_before_edit_changed()
{
	return impl->func_before_edit_changed;
}

Callback<void(InputEvent &)> &TextEdit::func_after_edit_changed()
{
	return impl->func_after_edit_changed;
}

Callback<void()> &TextEdit::func_selection_changed()
{
	return impl->func_selection_changed;
}

Callback<void()> &TextEdit::func_focus_gained()
{
	return impl->func_focus_gained;
}

Callback<void()> & TextEdit::func_focus_lost()
{
	return impl->func_focus_lost;
}

Callback<void()> &TextEdit::func_enter_pressed()
{
	return impl->func_enter_pressed;
}

void TextEdit::set_input_mask( const std::string &mask )
{
	impl->input_mask = mask;
}

void TextEdit::set_cursor_drawing_enabled(bool enable)
{
	impl->cursor_drawing_enabled_when_parent_focused = enable;

	if (!impl->readonly)
		impl->timer.start(500);
}

void TextEdit::set_select_all_on_focus_gain(bool enable)
{
	impl->select_all_on_focus_gain = enable;
}

/////////////////////////////////////////////////////////////////////////////
// TextEdit Implementation:

void TextEdit_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!textedit->is_enabled())
		return;

	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		
		InputEvent &e = input_msg->input_event;

		if (e.device.get_type() == InputDevice::keyboard)
		{
			if (!readonly && e.type == InputEvent::pressed &&
				(e.id == keycode_enter || e.id == keycode_return || e.id == keycode_numpad_enter))
			{
				if (!func_enter_pressed.is_null())
				{
					func_enter_pressed.invoke();
				}
				else
				{
					textedit->clear_selection();
					insert_text(cursor_pos, "\n");
					textedit->set_cursor_pos(textedit->get_cursor_pos() + 1);
				}
				msg->consumed = true;
				return;
			}

			if (!func_before_edit_changed.is_null())
			{
				func_before_edit_changed.invoke(e);
				if (e.type == InputEvent::no_key)
				{
					// If the 'func_before_edit_changed' callback sets e.type to 'no_key',
					// the input event is to be ignored.
					return;
				}
			}

			if (!readonly)	// Do not flash cursor when readonly
			{
				cursor_blink_visible = true;
				timer.start(500); // don't blink cursor when moving or typing.
			}

			if (e.type == InputEvent::pressed) // || e.repeat_count > 1) 
			{
				if (e.id == keycode_enter || e.id == keycode_escape || e.id == keycode_tab || e.id == keycode_numpad_enter)
				{
					// Do not consume these.
					return;
				}
				else if (e.id == keycode_a && e.ctrl)
				{
					// select all
					textedit->select_all();
					msg->consumed = true;
				}
				else if (e.id == keycode_c && e.ctrl)
				{
					std::string str = textedit->get_selection();
					textedit->get_gui_manager().set_clipboard_text(str);
					msg->consumed = true;
				}
				else if (readonly)
				{
					// Do not consume messages on read only component (only allow CTRL-A and CTRL-C)
					return;
				}
				else if (e.id == keycode_up)
				{
					if (e.shift && selection_length == 0)
						selection_start = cursor_pos;

					if (cursor_pos.y > 0)
					{
						cursor_pos.y--;
						cursor_pos.x = min(lines[cursor_pos.y].text.size(), (size_t)cursor_pos.x);
					}

					if (e.shift)
					{
						selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					}
					else
					{
						// Clear the selection if a cursor key is pressed but shift isn't down. 
						selection_start = Vec2i(0,0);
						selection_length = 0;
					}
					move_vertical_scroll();
					textedit->request_repaint();
					undo_info.first_text_insert = true;

					msg->consumed = true;
				}
				else if (e.id == keycode_down)
				{
					if (e.shift && selection_length == 0)
						selection_start = cursor_pos;

					if (cursor_pos.y < lines.size() - 1)
					{
						cursor_pos.y++;
						cursor_pos.x = min(lines[cursor_pos.y].text.size(), (size_t)cursor_pos.x);
					}

					if (e.shift)
					{
						selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					}
					else
					{
						// Clear the selection if a cursor key is pressed but shift isn't down. 
						selection_start = Vec2i(0,0);
						selection_length = 0;
					}
					move_vertical_scroll();

					textedit->request_repaint();
					undo_info.first_text_insert = true;

					msg->consumed = true;
				}
				else if (e.id == keycode_left)
				{
					move(-1, e.shift, e.ctrl);
					msg->consumed = true;
				}
				else if (e.id == keycode_right)
				{
					move(1, e.shift, e.ctrl);
					msg->consumed = true;
				}
				else if (e.id == keycode_backspace)
				{
					backspace();
					msg->consumed = true;
				}
				else if (e.id == keycode_delete)
				{
					del();
					msg->consumed = true;
				}
				else if (e.id == keycode_home)
				{
					if (e.ctrl)
						cursor_pos = Vec2i(0, 0);
					else
						cursor_pos.x = 0;
					if (e.shift)
						selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					else
						textedit->clear_selection();
					textedit->request_repaint();
					msg->consumed = true;
					move_vertical_scroll();
				}
				else if (e.id == keycode_end)
				{
					if (e.ctrl)
						cursor_pos = Vec2i(lines.back().text.length(), lines.size() - 1);
					else
						cursor_pos.x = lines[cursor_pos.y].text.size();

					if (e.shift)
						selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					else
						textedit->clear_selection();
					textedit->request_repaint();
					msg->consumed = true;
				}
				else if (e.id == keycode_x && e.ctrl)
				{
					std::string str = textedit->get_selection();
					textedit->delete_selected_text();
					textedit->get_gui_manager().set_clipboard_text(str);
					msg->consumed = true;
				}
				else if (e.id == keycode_v && e.ctrl)
				{
					std::string str = textedit->get_gui_manager().get_clipboard_text();
					std::string::const_iterator end_str = std::remove(str.begin(), str.end(), '\r');
					str.resize(end_str - str.begin());
					textedit->delete_selected_text();

					if (input_mask.empty())
					{
						insert_text(cursor_pos, str);
						textedit->set_cursor_pos(textedit->get_cursor_pos() + str.length());
					}
					else
					{
						if (input_mask_accepts_input(cursor_pos, str))
						{
							insert_text(cursor_pos, str);
							textedit->set_cursor_pos(textedit->get_cursor_pos() + str.length());
						}
					}
					move_vertical_scroll();
					msg->consumed = true;
				}
				else if (e.id == keycode_shift)
				{
					if (selection_length == 0)
						selection_start = cursor_pos;
					msg->consumed = true;
				}
				else if ((!e.str.empty() && !(e.str[0] >= 0 && e.str[0] < 32) && (!e.alt && !e.ctrl)) || (e.ctrl && e.alt)) // Alt Gr translates to Ctrl+Alt sometimes!
				{
					textedit->delete_selected_text();
					textedit->clear_selection();
					if (input_mask.empty())
					{
						// not in any special mode, just insert the string.
						insert_text(cursor_pos, e.str);
						cursor_pos.x += e.str.size();
					}
					else
					{
						if (input_mask_accepts_input(cursor_pos, e.str))
						{
							insert_text(cursor_pos, e.str);
							cursor_pos.x += e.str.size();
						}
					}
					msg->consumed = true;
				}
			}
			else if (e.type == InputEvent::released) 
			{
				// undo
				if (e.ctrl && e.id == keycode_z) 
				{
					if (!readonly)
					{
						std::string tmp = undo_info.undo_text;
						undo_info.undo_text = textedit->get_text();
						textedit->set_text(tmp);
						msg->consumed = true;
					}
				}
			}

			if (e.type == InputEvent::pressed && !func_after_edit_changed.is_null())
			{
				func_after_edit_changed.invoke(e);
			}

		}
		else if (e.device.get_type() == InputDevice::pointer)
		{
			if (e.type == InputEvent::pressed && e.id == mouse_left)
			{
				textedit->capture_mouse(true);
				mouse_selecting = true;
				cursor_pos = get_character_index(e.mouse_pos);
				selection_start = cursor_pos;
				selection_length = 0;

				textedit->request_repaint();
				msg->consumed = true;
			}
			if (mouse_selecting && e.type == InputEvent::released && e.id == mouse_left)
			{
				if (ignore_mouse_events) // This prevents text selection from changing from what was set when focus was gained.
				{
					textedit->capture_mouse(false);
					ignore_mouse_events = false;
					mouse_selecting = false;
				}
				else
				{
					scroll_timer.stop();
					textedit->capture_mouse(false);
					mouse_selecting = false;
					Vec2i sel_end = get_character_index(e.mouse_pos);
					selection_length = to_offset(sel_end) - to_offset(selection_start);
					cursor_pos = sel_end;
					textedit->set_focus();
					textedit->request_repaint();
				}
				msg->consumed = true;
			}
			if (e.type == InputEvent::pointer_moved && mouse_selecting && !ignore_mouse_events)
			{
				if (e.mouse_pos.x < content_rect.left || e.mouse_pos.x > content_rect.right)
				{
					if (e.mouse_pos.x < content_rect.left)
						mouse_moves_left = true;
					else 
						mouse_moves_left = false;

					if (!readonly)
						scroll_timer.start(50, true);
				}
				else
				{
					scroll_timer.stop();
					cursor_pos = get_character_index(e.mouse_pos);
					selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					textedit->request_repaint();
				}
				msg->consumed = true;
			}
		}
	}
	std::shared_ptr<GUIMessage_FocusChange> focus_change_msg = std::dynamic_pointer_cast<GUIMessage_FocusChange>(msg);
	if (focus_change_msg)
	{
		

		if (focus_change_msg->focus_type == GUIMessage_FocusChange::gained_focus)
		{
			if (textedit->get_focus_policy() == GUIComponent::focus_parent)
			{
				textedit->get_parent_component()->set_focus();
				if (!readonly)
					timer.start(500);
				return;
			}

			if (!readonly)
				timer.start(500);
			if (select_all_on_focus_gain)
				textedit->select_all();
			ignore_mouse_events = true;
			part_selection.set_pseudo_class(CssStr::unfocused, false);
			cursor_pos.y = lines.size() - 1;
			cursor_pos.x = lines[cursor_pos.y].text.length();

			textedit->request_repaint();

			if (!func_focus_gained.is_null())
				func_focus_gained.invoke();
		}
		else if (focus_change_msg->focus_type == GUIMessage_FocusChange::losing_focus)
		{
			timer.stop();
			textedit->clear_selection();
			part_selection.set_pseudo_class(CssStr::unfocused, true);

			textedit->request_repaint();

			if (!func_focus_lost.is_null())
				func_focus_lost.invoke();
		}
	}
	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
		{
			textedit->set_cursor(cursor_ibeam);
		}
		else
		{
			textedit->set_cursor(cursor_arrow);
		}
	}
}

void TextEdit_Impl::create_parts()
{
	part_selection = GUIThemePart(textedit, CssStr::TextEdit::part_selection);
	part_cursor = GUIThemePart(textedit, CssStr::TextEdit::part_cursor);

	bool enabled = textedit->is_enabled();

	textedit->set_pseudo_class(CssStr::hover, false);
	textedit->set_pseudo_class(CssStr::normal, enabled);
	textedit->set_pseudo_class(CssStr::disabled, !enabled);

	part_cursor.set_pseudo_class(CssStr::normal, enabled);
	part_cursor.set_pseudo_class(CssStr::disabled, !enabled);

	part_selection.set_pseudo_class(CssStr::normal, enabled);
	part_selection.set_pseudo_class(CssStr::disabled, !enabled);

	on_resized();	//TODO: Is this required?
}

void TextEdit_Impl::create_components()
{
        vert_scrollbar = new ScrollBar(textedit);
	vert_scrollbar->func_scroll().set(this, &TextEdit_Impl::on_vertical_scroll);
	vert_scrollbar->set_visible(false);
	vert_scrollbar->set_vertical();
}

void TextEdit_Impl::on_vertical_scroll()
{
	
}

void TextEdit_Impl::update_vertical_scroll()
{
       	Rect rect(
		content_rect.get_width()-vert_scrollbar->get_content_box().get_width(),
		content_rect.top, 
		content_rect.get_width(),
		content_rect.get_height());

	vert_scrollbar->set_geometry(rect);

	size_t total_height = get_total_line_height();
	int height_per_line = max((size_t)1,total_height / max((size_t)1,lines.size()));
	bool visible = total_height > content_rect.get_height();
	vert_scrollbar->calculate_ranges(content_rect.get_height() / height_per_line, total_height/height_per_line);
	vert_scrollbar->set_line_step(1);
	vert_scrollbar->set_visible(visible);


	if (visible == false)
	  vert_scrollbar->set_position(0);

}

void TextEdit_Impl::move_vertical_scroll()
{
	size_t total_height = get_total_line_height();
	int height_per_line = max((size_t)1,total_height / max((size_t)1,lines.size()));
	int lines_fit = content_rect.get_height() / height_per_line;
	if(cursor_pos.y >= vert_scrollbar->get_position() + lines_fit)
	{
		vert_scrollbar->set_position(cursor_pos.y -  lines_fit +1);
	}
	else if(cursor_pos.y < vert_scrollbar->get_position())
	{
		vert_scrollbar->set_position(cursor_pos.y);
	}
}


int TextEdit_Impl::get_total_line_height()
{
        int total = 0;
        for(std::vector<Line>::const_iterator iter = lines.begin(); iter != lines.end(); iter++)
        {
		total += iter->layout.get_size().height;
        }
	return total;
}

void TextEdit_Impl::move(int steps, bool shift, bool ctrl)
{
	if (shift && selection_length == 0)
		selection_start = cursor_pos;

	// Jump over words if control is pressed.
	if (ctrl)
	{
		if (steps < 0 && cursor_pos.x == 0 && cursor_pos.y > 0)
		{
			cursor_pos.x = lines[cursor_pos.y - 1].text.size();
			cursor_pos.y--;
		}
		else if (steps > 0 && cursor_pos.x == lines[cursor_pos.y].text.size() && cursor_pos.y + 1 < lines.size())
		{
			cursor_pos.x = 0;
			cursor_pos.y++;
		}

		Vec2i new_pos;
		if (steps < 0)
			new_pos = find_previous_break_character(cursor_pos);
		else 
			new_pos = find_next_break_character(cursor_pos);

		cursor_pos = new_pos;
	}
	else if (steps < 0 && cursor_pos.x == 0 && cursor_pos.y > 0)
	{
		cursor_pos.x = lines[cursor_pos.y - 1].text.size();
		cursor_pos.y--;
	}
	else if (steps > 0 && cursor_pos.x == lines[cursor_pos.y].text.size() && cursor_pos.y + 1 < lines.size())
	{
		cursor_pos.x = 0;
		cursor_pos.y++;
	}
	else
	{
		UTF8_Reader utf8_reader(lines[cursor_pos.y].text.data(), lines[cursor_pos.y].text.length());
		utf8_reader.set_position(cursor_pos.x);
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

		cursor_pos.x = utf8_reader.get_position();
	}

	if (shift)
	{
		selection_length = to_offset(cursor_pos) - to_offset(selection_start);
	}
	else
	{
		// Clear the selection if a cursor key is pressed but shift isn't down. 
		selection_start = Vec2i(0,0);
		selection_length = 0;
	}

	
	move_vertical_scroll();
	textedit->request_repaint();

	undo_info.first_text_insert = true;
}

std::string TextEdit_Impl::break_characters = " ::;,.-";

Vec2i TextEdit_Impl::find_next_break_character(Vec2i search_start)
{
	search_start.x++;
	if (search_start.x >= int(lines[search_start.y].text.size())-1)
		return Vec2i(lines[search_start.y].text.size(), search_start.y);

	int pos = lines[search_start.y].text.find_first_of(break_characters, search_start.x);
	if (pos == std::string::npos)
		return Vec2i(lines[search_start.y].text.size(), search_start.y);
	return Vec2i(pos, search_start.y);
}

Vec2i TextEdit_Impl::find_previous_break_character(Vec2i search_start)
{
	search_start.x--;
	if (search_start.x <= 0)
		return Vec2i(0, search_start.y);
	int pos = lines[search_start.y].text.find_last_of(break_characters, search_start.x);
	if (pos == std::string::npos)
		return Vec2i(0, search_start.y);
	return Vec2i(pos, search_start.y);
}

void TextEdit_Impl::insert_text(Vec2i pos, const std::string &str)
{
	undo_info.first_erase = false;
	if (undo_info.first_text_insert)
	{
		undo_info.undo_text = textedit->get_text();
		undo_info.first_text_insert = false;
	}

	// checking if insert exceeds max length
	if(to_offset(Vec2i(lines[lines.size()-1].text.size(), lines.size()-1)) + str.length() > max_length)
	{
		return;
	}

	std::string::size_type start = 0;
	while (true)
	{
		std::string::size_type next_newline = str.find('\n', start);

		lines[pos.y].text.insert(pos.x, str.substr(start, next_newline - start));
		lines[pos.y].invalidated = true;

		if (next_newline == std::string::npos)
			break;

		pos.x += next_newline - start;

		Line line;
		line.text = lines[pos.y].text.substr(pos.x);
		lines.insert(lines.begin() + pos.y + 1, line);
		lines[pos.y].text = lines[pos.y].text.substr(0, pos.x);
		lines[pos.y].invalidated = true;
		pos = Vec2i(0, pos.y + 1);

		start = next_newline + 1;
	}

	move_vertical_scroll();

	textedit->request_repaint();
}

void TextEdit_Impl::backspace()
{
	if (undo_info.first_erase)
	{
		undo_info.first_erase = false;
		undo_info.undo_text = textedit->get_text();
	}

	if (textedit->get_selection_length() != 0)
	{
		textedit->delete_selected_text();
		textedit->clear_selection();
		textedit->request_repaint();
	}
	else
	{
		if (cursor_pos.x > 0)
		{
			UTF8_Reader utf8_reader(lines[cursor_pos.y].text.data(), lines[cursor_pos.y].text.length());
			utf8_reader.set_position(cursor_pos.x);
			utf8_reader.prev();
			int length = utf8_reader.get_char_length();
			lines[cursor_pos.y].text.erase(cursor_pos.x-length, length);
			lines[cursor_pos.y].invalidated = true;
			cursor_pos.x -= length;
			textedit->request_repaint();
		}
		else if (cursor_pos.y > 0)
		{
			selection_start = Vec2i(lines[cursor_pos.y - 1].text.length(), cursor_pos.y - 1);
			selection_length = 1;
			textedit->delete_selected_text();
		}
	}
	move_vertical_scroll();
}

void TextEdit_Impl::del()
{
	if (undo_info.first_erase)
	{
		undo_info.first_erase = false;
		undo_info.undo_text = textedit->get_text();
	}

	if (textedit->get_selection_length() != 0)
	{
		textedit->delete_selected_text();
		textedit->clear_selection();
		textedit->request_repaint();
	}
	else
	{
		if (cursor_pos.x < (int)lines[cursor_pos.y].text.size())
		{
			UTF8_Reader utf8_reader(lines[cursor_pos.y].text.data(), lines[cursor_pos.y].text.length());
			utf8_reader.set_position(cursor_pos.x);
			int length = utf8_reader.get_char_length();
			lines[cursor_pos.y].text.erase(cursor_pos.x,length);
			lines[cursor_pos.y].invalidated = true;
			textedit->request_repaint();
		}
		else if (cursor_pos.y + 1 < lines.size())
		{
			selection_start = Vec2i(lines[cursor_pos.y].text.length(), cursor_pos.y);
			selection_length = 1;
			textedit->delete_selected_text();
		}
	}
	move_vertical_scroll();
}


void TextEdit_Impl::on_timer_expired()
{
	if (textedit == 0 || textedit->is_visible() == false)
	{
		timer.stop();
		return;
	}

	if (cursor_blink_visible)
		timer.start(500);
	else
		timer.start(500);

	cursor_blink_visible = !cursor_blink_visible;
	textedit->request_repaint();
}

void TextEdit_Impl::on_resized()
{
	Canvas canvas = textedit->get_canvas();

	vertical_text_align = textedit->get_vertical_text_align(canvas);

	clip_start_offset = 0;
	update_vertical_scroll();
}

void TextEdit_Impl::on_scroll_timer_expired()
{
	if (mouse_moves_left)
		move(-1,false, false);
	else
		move(1, false, false);
}

void TextEdit_Impl::on_enable_changed()
{
	create_parts();

	bool enabled = textedit->is_enabled();

	if (!enabled)
	{
		cursor_blink_visible = false;
		timer.stop();
	}
	textedit->request_repaint();
}

bool TextEdit_Impl::input_mask_accepts_input(Vec2i cursor_pos, const std::string &str)
{
	return str.find_first_not_of(input_mask) == std::string::npos;
}

std::string::size_type TextEdit_Impl::to_offset(Vec2i pos) const
{
	if (pos.y < lines.size())
	{
		std::string::size_type offset = 0;
		for (int line = 0; line < pos.y; line++)
		{
			offset += lines[line].text.size() + 1;
		}
		return offset + min((size_t)pos.x, lines[pos.y].text.size());
	}
	else
	{
		std::string::size_type offset = 0;
		for (size_t line = 0; line < lines.size(); line++)
		{
			offset += lines[line].text.size() + 1;
		}
		return offset - 1;
	}
}

Vec2i TextEdit_Impl::from_offset(std::string::size_type offset) const
{
	int line_offset = 0;
	for (int line = 0; line < lines.size(); line++)
	{
		if (offset <= line_offset + lines[line].text.size())
		{
			return Vec2i(offset - line_offset, line);
		}
		line_offset += lines[line].text.size() + 1;
	}
	return Vec2i(lines.back().text.size(), lines.size() - 1);
}

int TextEdit::get_total_height()
{
	Canvas canvas = get_canvas();
	impl->layout_lines(canvas);
	if (!impl->lines.empty())
	{
		return impl->lines.back().box.bottom + impl->textedit->get_content_shrink_box().bottom;
	}
	else
	{
		return impl->textedit->get_height();
	}
}

void TextEdit_Impl::layout_lines(Canvas &canvas)
{
	Rect g = textedit->get_size();
	Rect content_box = textedit->get_content_box();

	Vec2i sel_start;
	Vec2i sel_end;
	if (selection_length > 0)
	{
		sel_start = selection_start;
		sel_end = from_offset(to_offset(selection_start) + selection_length);
	}
	else if (selection_length < 0)
	{
		sel_start = from_offset(to_offset(selection_start) + selection_length);
		sel_end = selection_start;
	}

	Point draw_pos = content_box.get_top_left();
	for (size_t i = vert_scrollbar->get_position(); i < lines.size(); i++)
	{
		Line &line = lines[i];
		if (line.invalidated)
		{
			line.layout.clear();
			if (!line.text.empty())
				line.layout.add_text(line.text, textedit->get_font(), Colorf::black);
			else
				line.layout.add_text(StringHelp::wchar_to_utf8(0xa0),  textedit->get_font(), Colorf::black); // Draw one NBSP character to get the correct height
			line.layout.layout(canvas, content_box.get_width());
			line.box = Rect(draw_pos, line.layout.get_size());
			line.invalidated = false;
		}

		if (sel_start != sel_end && sel_start.y <= i && sel_end.y >= i)
		{
			line.layout.set_selection_range(sel_start.y < i ? 0 : sel_start.x, sel_end.y > i ? line.text.size() : sel_end.x);
		}
		else
		{
			line.layout.set_selection_range(0,0);
		}

		line.layout.hide_cursor();
		if (textedit->has_focus() || (textedit->get_focus_policy() == GUIComponent::focus_parent && cursor_drawing_enabled_when_parent_focused))
		{
			if (cursor_blink_visible && cursor_pos.y == i)
			{
				line.layout.set_cursor_pos(cursor_pos.x);
				line.layout.show_cursor();
			}
		}

		line.box.translate(draw_pos - line.box.get_top_left());
		line.layout.set_position(line.box.get_top_left());

		draw_pos = line.box.get_bottom_left();
	}
	update_vertical_scroll();
}

void TextEdit_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	layout_lines(canvas);
	Rect g = textedit->get_size();
	Rect content_box = textedit->get_content_box();

	textedit->set_cliprect(canvas, content_box);
	for (size_t i = vert_scrollbar->get_position(); i < lines.size(); i++)
		lines[i].layout.draw_layout(canvas);
	textedit->reset_cliprect(canvas);
}

Vec2i TextEdit_Impl::get_character_index(Point mouse_wincoords)
{
	Canvas canvas = textedit->get_canvas();
	for (size_t i = 0; i < lines.size(); i++)
	{
		Line &line = lines[i];
		if (line.box.top <= mouse_wincoords.y && line.box.bottom > mouse_wincoords.y)
		{
			SpanLayout::HitTestResult result = line.layout.hit_test(canvas, mouse_wincoords);
			switch (result.type)
			{
			case SpanLayout::HitTestResult::inside:
				return Vec2i(clamp(result.offset, 0, line.text.size()), i);
			case SpanLayout::HitTestResult::outside_left:
				return Vec2i(0, i);
			case SpanLayout::HitTestResult::outside_right:
				return Vec2i(line.text.size(), i);
			}
		}
	}

	return Vec2i(lines.back().text.size(), lines.size() - 1);
}

}

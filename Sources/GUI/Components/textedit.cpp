/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/textedit.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/draw.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/Window/display_window.h"
#include "../gui_css_strings.h"
#include "textedit_impl.h"

#ifndef WIN32
#include "stdlib.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Construction:

CL_TextEdit::CL_TextEdit(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_TextEdit_Impl)
{
	set_type_name(CssStr::TextEdit::type_name);
	set_focus_policy(focus_local);

	func_render().set(impl.get(), &CL_TextEdit_Impl::on_render);
	func_process_message().set(impl.get(), &CL_TextEdit_Impl::on_process_message);
	func_style_changed().set(impl.get(), &CL_TextEdit_Impl::on_style_changed);
	func_resized().set(impl.get(), &CL_TextEdit_Impl::on_resized);
	func_enablemode_changed().set(impl.get(), &CL_TextEdit_Impl::on_enable_changed);

	impl->textedit = this;
	impl->timer.func_expired().set(impl.get(), &CL_TextEdit_Impl::on_timer_expired);

	impl->scroll_timer.func_expired().set(impl.get(), &CL_TextEdit_Impl::on_scroll_timer_expired);
	impl->create_parts();

	set_type_name("textedit");
	impl->textedit = this;
	func_process_message().set(impl.get(), &CL_TextEdit_Impl::on_process_message);
	func_render().set(impl.get(), &CL_TextEdit_Impl::on_render);
}

CL_TextEdit::~CL_TextEdit()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Attributes:

CL_TextEdit *CL_TextEdit::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_TextEdit *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_TextEdit*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_TextEdit named item: %1", id));

	return object;
}

bool CL_TextEdit::is_read_only() const
{
	return impl->readonly;
}

bool CL_TextEdit::is_lowercase() const
{
	return impl->lowercase;
}

bool CL_TextEdit::is_uppercase() const
{
	return impl->uppercase;
}

int CL_TextEdit::get_max_length() const
{
	return impl->max_length;
}

CL_String CL_TextEdit::get_text() const
{
	CL_String::size_type size = 0;
	for (size_t i = 0; i < impl->lines.size(); i++)
		size += impl->lines[i].text.size();
	size += impl->lines.size() - 1;

	CL_String text;
	text.reserve(size);

	for (size_t i = 0; i < impl->lines.size(); i++)
	{
		if (i > 0)
			text.push_back('\n');
		text += impl->lines[i].text;
	}

	return text;
}

CL_String CL_TextEdit::get_selection() const
{
	CL_String::size_type offset = impl->to_offset(impl->selection_start);
	int start = cl_min(offset, offset + impl->selection_length);
	return get_text().substr(start, abs(impl->selection_length));
}

int CL_TextEdit::get_selection_start() const
{
	return impl->to_offset(impl->selection_start);
}

int CL_TextEdit::get_selection_length() const
{
	return impl->selection_length;
}

int CL_TextEdit::get_cursor_pos() const
{
	return impl->to_offset(impl->cursor_pos);
}

CL_Size CL_TextEdit::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Operations:

void CL_TextEdit::select_all()
{
	CL_String::size_type size = 0;
	for (size_t i = 0; i < impl->lines.size(); i++)
		size += impl->lines[i].text.size();
	size += impl->lines.size() - 1;
	set_selection(0, size);
}

void CL_TextEdit::set_read_only(bool enable)
{
	if (impl->readonly != enable)
	{
		impl->readonly = enable;
		request_repaint();
	}
}

void CL_TextEdit::set_lowercase(bool enable)
{
	if (impl->lowercase != enable)
	{
		impl->lowercase = enable;
		request_repaint();
	}
}

void CL_TextEdit::set_uppercase(bool enable)
{
	if (impl->uppercase != enable)
	{
		impl->uppercase = enable;
		request_repaint();
	}
}

void CL_TextEdit::set_max_length(int length)
{
	if (impl->max_length != length)
	{
		impl->max_length = length;

		CL_String::size_type size = 0;
		for (size_t i = 0; i < impl->lines.size(); i++)
			size += impl->lines[i].text.size();
		size += impl->lines.size() - 1;

		if ((int)size > length)
		{
			CL_InputEvent no_event;
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

void CL_TextEdit::set_text(const CL_StringRef &text)
{
	impl->lines.clear();
	CL_String::size_type start = 0;
	CL_String::size_type end = text.find('\n');
	while (end != CL_String::npos)
	{
		CL_TextEdit_Impl::Line line;
		line.text = text.substr(start, end - start);
		impl->lines.push_back(line);
		start = end + 1;
		end = text.find('\n', start);
	}
	CL_TextEdit_Impl::Line line;
	line.text = text.substr(start);
	impl->lines.push_back(line);

	impl->clip_start_offset = 0;
	set_cursor_pos(0);
	clear_selection();
	request_repaint();
}

void CL_TextEdit::set_selection(int pos, int length)
{
	impl->selection_start = pos;
	impl->selection_length = length;
	request_repaint();
}

void CL_TextEdit::clear_selection()
{
	set_selection(0,0);
	request_repaint();
}

void CL_TextEdit::delete_selected_text()
{
	if (get_selection_length() == 0)
		return;

	CL_String::size_type offset = impl->to_offset(impl->selection_start);
	int start = cl_min(offset, offset + impl->selection_length);
	int length = abs(impl->selection_length);

	clear_selection();
	CL_String text = get_text();
	set_text(text.substr(0, start) + text.substr(start + length));
	set_cursor_pos(start);
}

void CL_TextEdit::set_cursor_pos(int pos)
{
	impl->cursor_pos = impl->from_offset(pos);
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Events:

CL_Callback_v1<CL_InputEvent &> &CL_TextEdit::func_before_edit_changed()
{
	return impl->func_before_edit_changed;
}

CL_Callback_v1<CL_InputEvent &> &CL_TextEdit::func_after_edit_changed()
{
	return impl->func_after_edit_changed;
}

CL_Callback_v0 &CL_TextEdit::func_selection_changed()
{
	return impl->func_selection_changed;
}

CL_Callback_v0 &CL_TextEdit::func_focus_gained()
{
	return impl->func_focus_gained;
}

CL_Callback_v0 & CL_TextEdit::func_focus_lost()
{
	return impl->func_focus_lost;
}

CL_Callback_v0 &CL_TextEdit::func_enter_pressed()
{
	return impl->func_enter_pressed;
}

void CL_TextEdit::set_input_mask( const CL_StringRef &mask )
{
	impl->input_mask = mask;
}

void CL_TextEdit::set_cursor_drawing_enabled(bool enable)
{
	impl->cursor_drawing_enabled_when_parent_focused = enable;

	if (!impl->readonly)
		impl->timer.start(500);
}

void CL_TextEdit::set_select_all_on_focus_gain(bool enable)
{
	impl->select_all_on_focus_gain = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Implementation:

void CL_TextEdit_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (!textedit->is_enabled())
		return;

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.device.get_type() == CL_InputDevice::keyboard)
		{
			if (e.type == CL_InputEvent::pressed &&
				(e.id == CL_KEY_ENTER || e.id == CL_KEY_RETURN || e.id == CL_KEY_NUMPAD_ENTER))
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
				msg.set_consumed();
				return;
			}

			if (!func_before_edit_changed.is_null())
			{
				func_before_edit_changed.invoke(e);
				if (e.type == CL_InputEvent::no_key)
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

			if (e.type == CL_InputEvent::pressed) // || e.repeat_count > 1) 
			{
				if (e.id == CL_KEY_ENTER || e.id == CL_KEY_ESCAPE || e.id == CL_KEY_TAB || e.id == CL_KEY_NUMPAD_ENTER)
				{
					// Do not consume these.
					return;
				}
				else if (e.id == CL_KEY_A && e.ctrl)
				{
					// select all
					textedit->select_all();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_C && e.ctrl)
				{
					CL_String str = textedit->get_selection();
					textedit->get_gui_manager().set_clipboard_text(str);
					msg.set_consumed();
				}
				else if (readonly)
				{
					// Do not consume messages on read only component (only allow CTRL-A and CTRL-C)
					return;
				}
				else if (e.id == CL_KEY_UP)
				{
					if (e.shift && selection_length == 0)
						selection_start = cursor_pos;

					if (cursor_pos.y > 0)
					{
						cursor_pos.y--;
						cursor_pos.x = cl_min(lines[cursor_pos.y].text.size(), cursor_pos.x);
					}

					if (e.shift)
					{
						selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					}
					else
					{
						// Clear the selection if a cursor key is pressed but shift isn't down. 
						selection_start = CL_Vec2i(0,0);
						selection_length = 0;
					}

					textedit->request_repaint();
					undo_info.first_text_insert = true;

					msg.set_consumed();
				}
				else if (e.id == CL_KEY_DOWN)
				{
					if (e.shift && selection_length == 0)
						selection_start = cursor_pos;

					if (cursor_pos.y < lines.size() - 1)
					{
						cursor_pos.y++;
						cursor_pos.x = cl_min(lines[cursor_pos.y].text.size(), cursor_pos.x);
					}

					if (e.shift)
					{
						selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					}
					else
					{
						// Clear the selection if a cursor key is pressed but shift isn't down. 
						selection_start = CL_Vec2i(0,0);
						selection_length = 0;
					}

					textedit->request_repaint();
					undo_info.first_text_insert = true;

					msg.set_consumed();
				}
				else if (e.id == CL_KEY_LEFT)
				{
					move(-1, e);
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_RIGHT)
				{
					move(1, e);
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_BACKSPACE)
				{
					backspace();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_DELETE)
				{
					del();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_HOME)
				{
					if (e.ctrl)
						cursor_pos = CL_Vec2i(0, 0);
					else
						cursor_pos.x = 0;
					if (e.shift)
						selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					else
						textedit->clear_selection();
					textedit->request_repaint();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_END)
				{
					if (e.ctrl)
						cursor_pos = CL_Vec2i(lines.back().text.length(), lines.size() - 1);
					else
						cursor_pos.x = lines[cursor_pos.y].text.size();

					if (e.shift)
						selection_length = to_offset(cursor_pos) - to_offset(selection_start);
					else
						textedit->clear_selection();
					textedit->request_repaint();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_X && e.ctrl)
				{
					CL_String str = textedit->get_selection();
					textedit->delete_selected_text();
					textedit->get_gui_manager().set_clipboard_text(str);
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_V && e.ctrl)
				{
					CL_String str = textedit->get_gui_manager().get_clipboard_text();
					CL_String::const_iterator end_str = std::remove(str.begin(), str.end(), '\r');
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

					msg.set_consumed();
				}
				else if (e.id == CL_KEY_SHIFT)
				{
					if (selection_length == 0)
						selection_start = cursor_pos;
					msg.set_consumed();
				}
				else if (!e.str.empty() && !(e.str[0] >= 0 && e.str[0] < 32) && (!e.alt && !e.ctrl) || (e.ctrl && e.alt)) // Alt Gr translates to Ctrl+Alt sometimes!
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
					msg.set_consumed();
				}
			}
			else if (e.type == CL_InputEvent::released) 
			{
				// undo
				if (e.ctrl && e.id == CL_KEY_Z) 
				{
					if (!readonly)
					{
						CL_String tmp = undo_info.undo_text;
						undo_info.undo_text = textedit->get_text();
						textedit->set_text(tmp);
						msg.set_consumed();
					}
				}
			}

			if (e.type == CL_InputEvent::pressed && !func_after_edit_changed.is_null())
			{
				func_after_edit_changed.invoke(e);
			}

		}
		else if (e.device.get_type() == CL_InputDevice::pointer)
		{
			if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
			{
				textedit->capture_mouse(true);
				mouse_selecting = true;
				cursor_pos = get_character_index(e.mouse_pos);
				selection_start = cursor_pos;
				selection_length = 0;

				textedit->request_repaint();
				msg.set_consumed();
			}
			if (mouse_selecting && e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
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
					CL_Vec2i sel_end = get_character_index(e.mouse_pos);
					selection_length = to_offset(sel_end) - to_offset(selection_start);
					cursor_pos = sel_end;
					textedit->set_focus();
					textedit->request_repaint();
				}
				msg.set_consumed();
			}
			if (e.type == CL_InputEvent::pointer_moved && mouse_selecting && !ignore_mouse_events)
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
				msg.set_consumed();
			}
		}
	}
	else if (msg.is_type(CL_GUIMessage_FocusChange::get_type_name()))
	{
		CL_GUIMessage_FocusChange fmsg(msg);

		if (fmsg.get_focus_type() == CL_GUIMessage_FocusChange::gained_focus)
		{
			if (textedit->get_focus_policy() == CL_GUIComponent::focus_parent)
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
			part_selection.set_state(CssStr::unfocused, false);
			cursor_pos.y = lines.size() - 1;
			cursor_pos.x = lines[cursor_pos.y].text.length();

			textedit->request_repaint();

			if (!func_focus_gained.is_null())
				func_focus_gained.invoke();
		}
		else if (fmsg.get_focus_type() == CL_GUIMessage_FocusChange::losing_focus)
		{
			timer.stop();
			textedit->clear_selection();
			part_selection.set_state(CssStr::unfocused, true);

			textedit->request_repaint();

			if (!func_focus_lost.is_null())
				func_focus_lost.invoke();
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer_msg(msg);
		if (pointer_msg.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
		{
			textedit->set_cursor(cl_cursor_ibeam);
		}
		else
		{
			textedit->set_cursor(cl_cursor_arrow);
		}
	}

}

void CL_TextEdit_Impl::on_style_changed()
{
	create_parts();
}

void CL_TextEdit_Impl::create_parts()
{
	part_component = CL_GUIThemePart(textedit);
	part_selection = CL_GUIThemePart(textedit, CssStr::TextEdit::part_selection);
	part_cursor = CL_GUIThemePart(textedit, CssStr::TextEdit::part_cursor);

	bool enabled = textedit->is_enabled();

	part_component.set_state(CssStr::hot, false);
	part_component.set_state(CssStr::normal, enabled);
	part_component.set_state(CssStr::disabled, !enabled);

	text_color = part_component.get_property(prop_text_color);

	part_cursor.set_state(CssStr::normal, enabled);
	part_cursor.set_state(CssStr::disabled, !enabled);

	part_selection.set_state(CssStr::normal, enabled);
	part_selection.set_state(CssStr::disabled, !enabled);

	on_resized();	//TODO: Is this required?

}

void CL_TextEdit_Impl::move(int steps, CL_InputEvent &e)
{
	if (e.shift && selection_length == 0)
		selection_start = cursor_pos;

	// Jump over words if control is pressed.
	if (e.ctrl)
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

		CL_Vec2i new_pos;
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
		CL_UTF8_Reader utf8_reader(lines[cursor_pos.y].text);
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

	if (e.shift)
	{
		selection_length = to_offset(cursor_pos) - to_offset(selection_start);
	}
	else
	{
		// Clear the selection if a cursor key is pressed but shift isn't down. 
		selection_start = CL_Vec2i(0,0);
		selection_length = 0;
	}

	textedit->request_repaint();

	undo_info.first_text_insert = true;
}

CL_String CL_TextEdit_Impl::break_characters = " ::;,.-";

CL_Vec2i CL_TextEdit_Impl::find_next_break_character(CL_Vec2i search_start)
{
	search_start.x++;
	if (search_start.x >= int(lines[search_start.y].text.size())-1)
		return CL_Vec2i(lines[search_start.y].text.size(), search_start.y);

	int pos = lines[search_start.y].text.find_first_of(break_characters, search_start.x);
	if (pos == CL_String::npos)
		return CL_Vec2i(lines[search_start.y].text.size(), search_start.y);
	return CL_Vec2i(pos, search_start.y);
}

CL_Vec2i CL_TextEdit_Impl::find_previous_break_character(CL_Vec2i search_start)
{
	search_start.x--;
	if (search_start.x <= 0)
		return CL_Vec2i(0, search_start.y);
	int pos = lines[search_start.y].text.find_last_of(break_characters, search_start.x);
	if (pos == CL_String::npos)
		return CL_Vec2i(0, search_start.y);
	return CL_Vec2i(pos, search_start.y);
}

void CL_TextEdit_Impl::insert_text(CL_Vec2i pos, const CL_StringRef &str)
{
	undo_info.first_erase = false;
	if (undo_info.first_text_insert)
	{
		undo_info.undo_text = textedit->get_text();
		undo_info.first_text_insert = false;
	}

	// checking if insert exceeds max length
	if(to_offset(CL_Vec2i(lines[lines.size()-1].text.size(), lines.size()-1)) + str.length() > max_length)
	{
		return;
	}

	CL_String::size_type start = 0;
	while (true)
	{
		CL_String::size_type next_newline = str.find('\n', start);

		lines[pos.y].text.insert(pos.x, str.substr(start, next_newline - start));
		lines[pos.y].invalidated = true;

		if (next_newline == CL_String::npos)
			break;

		pos.x += next_newline - start;

		Line line;
		line.text = lines[pos.y].text.substr(pos.x);
		lines.insert(lines.begin() + pos.y + 1, line);
		lines[pos.y].text = lines[pos.y].text.substr(0, pos.x);
		lines[pos.y].invalidated = true;
		pos = CL_Vec2i(0, pos.y + 1);

		start = next_newline + 1;
	}

	textedit->request_repaint();
}

void CL_TextEdit_Impl::backspace()
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
			CL_UTF8_Reader utf8_reader(lines[cursor_pos.y].text);
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
			selection_start = CL_Vec2i(lines[cursor_pos.y - 1].text.length(), cursor_pos.y - 1);
			selection_length = 1;
			textedit->delete_selected_text();
		}
	}
}

void CL_TextEdit_Impl::del()
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
			CL_UTF8_Reader utf8_reader(lines[cursor_pos.y].text);
			utf8_reader.set_position(cursor_pos.x);
			int length = utf8_reader.get_char_length();
			lines[cursor_pos.y].text.erase(cursor_pos.x,length);
			lines[cursor_pos.y].invalidated = true;
			textedit->request_repaint();
		}
		else if (cursor_pos.y + 1 < lines.size())
		{
			selection_start = CL_Vec2i(lines[cursor_pos.y].text.length(), cursor_pos.y);
			selection_length = 1;
			textedit->delete_selected_text();
		}
	}
}


void CL_TextEdit_Impl::on_timer_expired()
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

void CL_TextEdit_Impl::on_resized()
{
	content_rect = part_component.get_content_box(textedit->get_size());

	CL_GraphicContext &gc = textedit->get_gc();
	CL_Font font = part_component.get_font();

	vertical_text_align = part_component.get_vertical_text_align(gc, font, content_rect);

	clip_start_offset = 0;
}


void CL_TextEdit_Impl::on_scroll_timer_expired()
{
	CL_GUIMessage_Input msg;
	CL_InputEvent event;
	msg.set_event(event);

	if (mouse_moves_left)
		move(-1,event);
	else
		move(1, event);
}

void CL_TextEdit_Impl::on_enable_changed()
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

bool CL_TextEdit_Impl::input_mask_accepts_input(CL_Vec2i cursor_pos, const CL_StringRef &str)
{
	return str.find_first_not_of(input_mask) == CL_StringData8::npos;
}

CL_String::size_type CL_TextEdit_Impl::to_offset(CL_Vec2i pos) const
{
	if (pos.y < lines.size())
	{
		CL_String::size_type offset = 0;
		for (int line = 0; line < pos.y; line++)
		{
			offset += lines[line].text.size() + 1;
		}
		return offset + cl_min(pos.x, lines[pos.y].text.size());
	}
	else
	{
		CL_String::size_type offset = 0;
		for (size_t line = 0; line < lines.size(); line++)
		{
			offset += lines[line].text.size() + 1;
		}
		return offset - 1;
	}
}

CL_Vec2i CL_TextEdit_Impl::from_offset(CL_String::size_type offset) const
{
	int line_offset = 0;
	for (int line = 0; line < lines.size(); line++)
	{
		if (offset <= line_offset + lines[line].text.size())
		{
			return CL_Vec2i(offset - line_offset, line);
		}
		line_offset += lines[line].text.size() + 1;
	}
	return CL_Vec2i(lines.back().text.size(), lines.size() - 1);
}

void CL_TextEdit_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect g = textedit->get_size();
	part_component.render_box(gc, g, update_rect);

	CL_Rect content_box = part_component.get_content_box(g);
	CL_Font font = part_component.get_font();

	CL_Vec2i sel_start;
	CL_Vec2i sel_end;
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

	textedit->set_cliprect(gc, content_box);

	CL_Point draw_pos = content_box.get_top_left();
	for (size_t i = 0; i < lines.size(); i++)
	{
		Line &line = lines[i];
		if (line.invalidated)
		{
			line.layout.clear();
			if (!line.text.empty())
				line.layout.add_text(line.text, font, CL_Colorf::black);
			else
				line.layout.add_text(CL_StringHelp::wchar_to_utf8(0xa0), font, CL_Colorf::black); // Draw one NBSP character to get the correct height
			line.layout.layout(gc, content_box.get_width());
			line.box = CL_Rect(draw_pos, line.layout.get_size());
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
		if (textedit->has_focus() || (textedit->get_focus_policy() == CL_GUIComponent::focus_parent && cursor_drawing_enabled_when_parent_focused))
		{
			if (cursor_blink_visible && cursor_pos.y == i)
			{
				line.layout.set_cursor_pos(cursor_pos.x);
				line.layout.show_cursor();
			}
		}

		line.box.translate(draw_pos - line.box.get_top_left());
		line.layout.set_position(line.box.get_top_left());
		line.layout.draw_layout(gc);

		draw_pos = line.box.get_bottom_left();
	}

	textedit->reset_cliprect(gc);
}

CL_Vec2i CL_TextEdit_Impl::get_character_index(CL_Point mouse_wincoords)
{
	CL_GraphicContext gc = textedit->get_gc();
	for (size_t i = 0; i < lines.size(); i++)
	{
		Line &line = lines[i];
		if (line.box.top <= mouse_wincoords.y && line.box.bottom > mouse_wincoords.y)
		{
			CL_SpanLayout::HitTestResult result = line.layout.hit_test(gc, mouse_wincoords);
			switch (result.type)
			{
			case CL_SpanLayout::HitTestResult::inside:
				return CL_Vec2i(cl_clamp(result.offset, 0, line.text.size()), i);
			case CL_SpanLayout::HitTestResult::outside_left:
				return CL_Vec2i(0, i);
			case CL_SpanLayout::HitTestResult::outside_right:
				return CL_Vec2i(line.text.size(), i);
			}
		}
	}

	return CL_Vec2i(lines.back().text.size(), lines.size() - 1);
}

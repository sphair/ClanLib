/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/GUI/Components/lineedit.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/draw.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/Window/display_window.h"
#include "../gui_css_strings.h"
#include "lineedit_impl.h"

#ifndef WIN32
#include "stdlib.h"
#endif

const CL_StringRef numeric_mode_characters = "0123456789";

/////////////////////////////////////////////////////////////////////////////
// CL_LineEdit Construction:

CL_LineEdit::CL_LineEdit(CL_GUIComponent *parent)
: CL_GUIComponent(parent),
  impl(new CL_LineEdit_Impl)
{
	set_type_name(CssStr::LineEdit::type_name);
	set_focus_policy(focus_local);

	func_render().set(impl.get(), &CL_LineEdit_Impl::on_render);
	func_process_message().set(impl.get(), &CL_LineEdit_Impl::on_process_message);
	func_style_changed().set(impl.get(), &CL_LineEdit_Impl::on_style_changed);
	func_resized().set(impl.get(), &CL_LineEdit_Impl::on_resized);
	func_enablemode_changed().set(impl.get(), &CL_LineEdit_Impl::on_enable_changed);

	impl->lineedit = this;
	impl->timer.func_expired().set(impl.get(), &CL_LineEdit_Impl::on_timer_expired);

	impl->scroll_timer.func_expired().set(impl.get(), &CL_LineEdit_Impl::on_scroll_timer_expired);
	impl->create_parts();
}

CL_LineEdit::~CL_LineEdit()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_LineEdit Attributes:

CL_LineEdit *CL_LineEdit::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_LineEdit *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_LineEdit*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_LineEdit named item: %1", id));

	return object;
}

bool CL_LineEdit::is_read_only() const
{
	return impl->readonly;
}

CL_LineEdit::Alignment CL_LineEdit::get_alignment() const
{
	return align_left;
}

bool CL_LineEdit::is_lowercase() const
{
	return impl->lowercase;
}

bool CL_LineEdit::is_uppercase() const
{
	return impl->uppercase;
}

bool CL_LineEdit::is_password_mode() const
{
	return impl->password_mode;
}

int CL_LineEdit::get_max_length() const
{
	return impl->max_length;
}

CL_StringRef CL_LineEdit::get_text() const
{
	return impl->text;
}

int CL_LineEdit::get_text_int() const
{
	return CL_StringHelp::text_to_int(impl->text);
}

float CL_LineEdit::get_text_float() const
{
	return CL_StringHelp::text_to_float(impl->text);
}

CL_String CL_LineEdit::get_selection() const
{
	int start = cl_min(impl->selection_start, impl->selection_start + impl->selection_length);
	return impl->text.substr(start, abs(impl->selection_length));
}

int CL_LineEdit::get_selection_start() const
{
	return impl->selection_start;
}

int CL_LineEdit::get_selection_length() const
{
	return impl->selection_length;
}

int CL_LineEdit::get_cursor_pos() const
{
	return impl->cursor_pos;
}

CL_Size CL_LineEdit::get_text_size()
{
	CL_GraphicContext &gc = get_gc();
	CL_Font font = impl->part_component.get_font();
	return impl->get_visual_text_size(gc, font);
}

CL_Size CL_LineEdit::get_text_size(const CL_String &str)
{
	CL_GraphicContext &gc = get_gc();
	CL_Font font = impl->part_component.get_font();
	CL_Size text_size = font.get_text_size(gc, str);
	return text_size;
}

CL_Size CL_LineEdit::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_LineEdit Operations:

void CL_LineEdit::select_all()
{
	impl->selection_start = 0;
	impl->selection_length = impl->text.size();
	request_repaint();
}

void CL_LineEdit::set_read_only(bool enable)
{
	if (impl->readonly != enable)
	{
		impl->readonly = enable;
		request_repaint();
	}
}

void CL_LineEdit::set_alignment(Alignment alignment)
{
	if (impl->alignment != alignment)
	{
		impl->alignment = alignment;
		request_repaint();
	}
}

void CL_LineEdit::set_lowercase(bool enable)
{
	if (impl->lowercase != enable)
	{
		impl->lowercase = enable;
		request_repaint();
	}
}

void CL_LineEdit::set_uppercase(bool enable)
{
	if (impl->uppercase != enable)
	{
		impl->uppercase = enable;
		request_repaint();
	}
}

void CL_LineEdit::set_password_mode(bool enable)
{
	if (impl->password_mode != enable)
	{
		impl->password_mode = enable;
		request_repaint();
	}
}

void CL_LineEdit::set_max_length(int length)
{
	if (impl->max_length != length)
	{
		impl->max_length = length;
		if ((int)impl->text.length() > length)
		{
			CL_InputEvent no_event;
			if (!impl->func_before_edit_changed.is_null())
				impl->func_before_edit_changed.invoke(no_event);
			impl->text = impl->text.substr(0, length);
			if (!impl->func_after_edit_changed.is_null())
				impl->func_after_edit_changed.invoke(no_event);
		}
		request_repaint();
	}
}

void CL_LineEdit::set_text(const CL_StringRef &text)
{
	impl->text = text;
	impl->clip_start_offset = 0;
	impl->update_text_clipping();
	set_cursor_pos(impl->text.size());
	clear_selection();
	request_repaint();
}

void CL_LineEdit::set_text(int number)
{
	impl->text = CL_StringHelp::int_to_text(number);
	impl->clip_start_offset = 0;
	impl->update_text_clipping();
	set_cursor_pos(impl->text.size());
	clear_selection();
	request_repaint();
}

void CL_LineEdit::set_text(float number)
{
	impl->text = CL_StringHelp::float_to_text(number);
	impl->clip_start_offset = 0;
	impl->update_text_clipping();
	set_cursor_pos(impl->text.size());
	clear_selection();
	request_repaint();
}


void CL_LineEdit::set_selection(int pos, int length)
{
	impl->selection_start = pos;
	impl->selection_length = length;
	request_repaint();
}

void CL_LineEdit::clear_selection()
{
	set_selection(0,0);
	request_repaint();
}

void CL_LineEdit::delete_selected_text()
{
	if (get_selection_length() == 0)
		return;

	int sel_start = impl->selection_start;
	int sel_end = impl->selection_start + impl->selection_length;
	if (sel_start > sel_end)
		std::swap(sel_start, sel_end);

	impl->text = impl->text.substr(0, sel_start) + impl->text.substr(sel_end, impl->text.size());
	impl->cursor_pos = sel_start;
	clear_selection();
	request_repaint();
}

void CL_LineEdit::set_cursor_pos(int pos)
{
	impl->cursor_pos = pos;
	impl->update_text_clipping();
	request_repaint();
}

void CL_LineEdit::resize_to_fit(int max_width)
{
	CL_GraphicContext &gc = get_gc();
	CL_Font font = impl->part_component.get_font();

	CL_Rect g = get_geometry();
	CL_Rect rect_content = impl->part_component.get_content_box(g);
	CL_Size text_size = impl->get_visual_text_size(gc, font);
	rect_content.set_size(CL_Size(text_size.width+1, rect_content.get_height()));

	g.set_size(impl->part_component.get_render_box(rect_content).get_size());
	set_geometry(g);

	impl->clip_start_offset = 0;

	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// CL_LineEdit Events:

CL_Callback_v1<CL_InputEvent &> &CL_LineEdit::func_before_edit_changed()
{
	return impl->func_before_edit_changed;
}

CL_Callback_v1<CL_InputEvent &> &CL_LineEdit::func_after_edit_changed()
{
	return impl->func_after_edit_changed;
}

CL_Callback_v0 &CL_LineEdit::func_selection_changed()
{
	return impl->func_selection_changed;
}

CL_Callback_v0 &CL_LineEdit::func_focus_gained()
{
	return impl->func_focus_gained;
}

CL_Callback_v0 & CL_LineEdit::func_focus_lost()
{
	return impl->func_focus_lost;
}

CL_Callback_v0 &CL_LineEdit::func_enter_pressed()
{
	return impl->func_enter_pressed;
}

void CL_LineEdit::set_input_mask( const CL_StringRef &mask )
{
	impl->input_mask = mask;
}

void CL_LineEdit::set_numeric_mode( bool enable, bool decimals)
{
	impl->numeric_mode = enable;
	impl->numeric_mode_decimals = decimals;
}

void CL_LineEdit::set_decimal_character( const CL_StringRef &decimal_char )
{
	impl->decimal_char = decimal_char;
}

void CL_LineEdit::set_cursor_drawing_enabled(bool enable)
{
	impl->cursor_drawing_enabled_when_parent_focused = enable;

	if (!impl->readonly)
		impl->timer.start(500);
}

void CL_LineEdit::set_select_all_on_focus_gain(bool enable)
{
	impl->select_all_on_focus_gain = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_LineEdit Implementation:

void CL_LineEdit_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (!lineedit->is_enabled())
		return;

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.device.get_type() == CL_InputDevice::keyboard)
		{
			if (!func_enter_pressed.is_null() && 
				e.type == CL_InputEvent::pressed &&
				(e.id == CL_KEY_ENTER || e.id == CL_KEY_RETURN))
			{
				func_enter_pressed.invoke();
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
				if (e.id == CL_KEY_ENTER || e.id == CL_KEY_ESCAPE || e.id == CL_KEY_TAB)
				{
					// Do not consume these.
					return;
				}
				else if (e.id == CL_KEY_A && e.ctrl)
				{
					// select all
					selection_start = 0;
					selection_length = text.size();
					cursor_pos = selection_length;
					update_text_clipping();
					lineedit->request_repaint();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_C && e.ctrl)
				{
					if(!password_mode)	// Do not allow copying the password to clipboard
					{
						CL_String str = lineedit->get_selection();
						lineedit->get_gui_manager().set_clipboard_text(str);
						msg.set_consumed();
					}
				}
				else if (readonly)
				{
					// Do not consume messages on read only component (only allow CTRL-A and CTRL-C)
					return;
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
					update_text_clipping();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_DELETE)
				{
					del();
					update_text_clipping();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_HOME)
				{
					selection_start = cursor_pos;
					cursor_pos = 0;
					if (e.shift)
						selection_length = -selection_start;
					else
						lineedit->clear_selection();
					update_text_clipping();
					lineedit->request_repaint();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_END)
				{
					selection_start = cursor_pos;
					cursor_pos = text.size();
					if (e.shift)
						selection_length = text.size() - selection_start;
					else
						lineedit->clear_selection();
					update_text_clipping();
					lineedit->request_repaint();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_X && e.ctrl)
				{
					CL_String str = lineedit->get_selection();
					lineedit->delete_selected_text();
					lineedit->get_gui_manager().set_clipboard_text(str);
					update_text_clipping();	
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_V && e.ctrl)
				{
					CL_String str = lineedit->get_gui_manager().get_clipboard_text();
					lineedit->delete_selected_text();
					insert_text(cursor_pos, str);
					lineedit->set_cursor_pos(cursor_pos + str.length());
					update_text_clipping();
					msg.set_consumed();
				}
				else if (e.id == CL_KEY_SHIFT)
				{
					if (selection_start == -1)
					{
						selection_start = cursor_pos;
						selection_length = 0;
					}
					msg.set_consumed();
				}
				else if (!e.str.empty() && !(e.str[0] >= 0 && e.str[0] < 32) && (!e.alt && !e.ctrl) || (e.ctrl && e.alt)) // Alt Gr translates to Ctrl+Alt sometimes!
				{
					lineedit->delete_selected_text();
					lineedit->clear_selection();
					if (input_mask.empty())
					{
						if (numeric_mode)
						{
							// '-' can only be added once, and only as the first character.
							if (e.str == "-" && cursor_pos == 0 && text.find("-") == CL_StringRef::npos) 
							{
								insert_text(cursor_pos, e.str);
								cursor_pos += e.str.size();
							}
							else if (numeric_mode_decimals && e.str == decimal_char && cursor_pos > 0) // add decimal char 
							{
								if (text.find(decimal_char) == CL_StringRef::npos) // allow only one decimal char.
								{
									insert_text(cursor_pos, e.str);
									cursor_pos += e.str.size();
								}
							}
							else if (numeric_mode_characters.find(e.str) != CL_StringRef::npos) // 0-9
							{
								insert_text(cursor_pos, e.str);
								cursor_pos += e.str.size();
							}
						}
						else
						{
							// not in any special mode, just insert the string.
							insert_text(cursor_pos, e.str);
							cursor_pos += e.str.size();
						}
					}
					else
					{
						if (input_mask_accepts_input(cursor_pos, e.str))
						{
							insert_text(cursor_pos, e.str);
							cursor_pos += e.str.size();
						}
					}
					update_text_clipping();
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
						undo_info.undo_text = lineedit->get_text();
						lineedit->set_text(tmp);
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
				if (lineedit->has_focus())
				{
					lineedit->capture_mouse(true);
					mouse_selecting = true;
					cursor_pos = get_character_index(e.mouse_pos.x);
					selection_start = cursor_pos;
					selection_length = 0;
				}
				else
				{
					lineedit->set_focus();
				}
				lineedit->request_repaint();
				msg.set_consumed();
			}
			if (mouse_selecting && e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
			{
				if (ignore_mouse_events) // This prevents text selection from changing from what was set when focus was gained.
				{
					lineedit->capture_mouse(false);
					ignore_mouse_events = false;
					mouse_selecting = false;
				}
				else
				{
					scroll_timer.stop();
					lineedit->capture_mouse(false);
					mouse_selecting = false;
					int sel_end = get_character_index(e.mouse_pos.x);
					selection_length = sel_end - selection_start;
					cursor_pos = sel_end;
					lineedit->set_focus();
					lineedit->request_repaint();
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
					cursor_pos = get_character_index(e.mouse_pos.x);
					selection_length = cursor_pos - selection_start;
					lineedit->request_repaint();
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
			if (lineedit->get_focus_policy() == CL_GUIComponent::focus_parent)
			{
				lineedit->get_parent_component()->set_focus();
				if (!readonly)
					timer.start(500);
				return;
			}

			if (!readonly)
				timer.start(500);
			if (select_all_on_focus_gain)
				lineedit->select_all();
			ignore_mouse_events = true;
			part_selection.set_state(CssStr::unfocused, false);
			cursor_pos = text.length();

			lineedit->request_repaint();

			if (!func_focus_gained.is_null())
				func_focus_gained.invoke();
		}
		else if (fmsg.get_focus_type() == CL_GUIMessage_FocusChange::losing_focus)
		{
			timer.stop();
			lineedit->clear_selection();
			part_selection.set_state(CssStr::unfocused, true);

			lineedit->request_repaint();

			if (!func_focus_lost.is_null())
				func_focus_lost.invoke();
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer_msg(msg);
		if (pointer_msg.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
		{
			lineedit->set_cursor(cl_cursor_ibeam);
		}
		else
		{
			lineedit->set_cursor(cl_cursor_arrow);
		}
	}

}

void CL_LineEdit_Impl::on_style_changed()
{
	create_parts();
}

void CL_LineEdit_Impl::create_parts()
{
	part_component = CL_GUIThemePart(lineedit);
	part_selection = CL_GUIThemePart(lineedit, CssStr::LineEdit::part_selection);
	part_cursor = CL_GUIThemePart(lineedit, CssStr::LineEdit::part_cursor);

	bool enabled = lineedit->is_enabled();

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

void CL_LineEdit_Impl::move(int steps, CL_InputEvent &e)
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

		cursor_pos += steps;
		if (cursor_pos < 0)
			cursor_pos = 0;
		if (cursor_pos > (int)text.size())
			cursor_pos = text.size();
	}
	else
	{
		CL_UTF8_Reader utf8_reader(text);
		utf8_reader.set_position(cursor_pos);
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

		cursor_pos = utf8_reader.get_position();
	}

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

	update_text_clipping();

	lineedit->request_repaint();

	undo_info.first_text_insert = true;
}

void CL_LineEdit_Impl::insert_text(int pos, const CL_StringRef &str)
{
	undo_info.first_erase = false;
	if (undo_info.first_text_insert)
	{
		undo_info.undo_text = lineedit->get_text();
		undo_info.first_text_insert = false;
	}

	// checking if insert exceeds max length
	if(text.length() + str.length() > max_length)
	{
		return;
	}
	text.insert(pos, str);

	update_text_clipping();
	lineedit->request_repaint();
}

void CL_LineEdit_Impl::backspace()
{
	if (undo_info.first_erase)
	{
		undo_info.first_erase = false;
		undo_info.undo_text = lineedit->get_text();
	}

	if (lineedit->get_selection_length() != 0)
	{
		lineedit->delete_selected_text();
		lineedit->clear_selection();
		lineedit->request_repaint();
	}
	else
	{
		if (cursor_pos > 0)
		{
			CL_UTF8_Reader utf8_reader(text);
			utf8_reader.set_position(cursor_pos);
			utf8_reader.prev();
			int length = utf8_reader.get_char_length();
			text.erase(cursor_pos-length, length);
			cursor_pos -= length;
			lineedit->request_repaint();
		}
	}
}

void CL_LineEdit_Impl::del()
{
	if (undo_info.first_erase)
	{
		undo_info.first_erase = false;
		undo_info.undo_text = lineedit->get_text();
	}

	if (lineedit->get_selection_length() != 0)
	{
		lineedit->delete_selected_text();
		lineedit->clear_selection();
		lineedit->request_repaint();
	}
	else
	{
		if (cursor_pos < (int)text.size())
		{
			CL_UTF8_Reader utf8_reader(text);
			utf8_reader.set_position(cursor_pos);
			int length = utf8_reader.get_char_length();
			text.erase(cursor_pos,length);
			lineedit->request_repaint();
		}
	}
}

int CL_LineEdit_Impl::get_character_index(int mouse_x_wincoords)
{
	if (text.size() <= 1 )
	{
		return text.size();
	}

	CL_GraphicContext &gc = lineedit->get_gc();
	CL_Font font = part_component.get_font();
	CL_UTF8_Reader utf8_reader(text);

	int mouse_x = mouse_x_wincoords - content_rect.left ;

	int seek_start = clip_start_offset;
	int seek_end = text.size();
	int seek_center = (seek_start + seek_end) / 2;

	while (true)
	{
		utf8_reader.set_position(seek_center);
		utf8_reader.move_to_leadbyte();
		if (seek_center != utf8_reader.get_position())
			utf8_reader.next();
		seek_center = utf8_reader.get_position();

		utf8_reader.set_position(seek_start);
		utf8_reader.next();
		if (utf8_reader.get_position() == seek_end)
			break;

		CL_Size text_size = get_visual_text_size(gc, font, clip_start_offset, seek_center - clip_start_offset);

		if (text_size.width > mouse_x)
			seek_end = seek_center;
		else
			seek_start = seek_center;
		seek_center = (seek_start+seek_end)/2;
	}

	CL_Size text_size = get_visual_text_size(gc, font, clip_start_offset, seek_center - clip_start_offset);

	utf8_reader.set_position(seek_center);
	utf8_reader.next();
	if (utf8_reader.is_end() && mouse_x > text_size.width)
		seek_center = text.size();

	return seek_center;
}

void CL_LineEdit_Impl::update_text_clipping()
{
	CL_GraphicContext &gc = lineedit->get_gc();
	CL_Font font = part_component.get_font();

	CL_Size text_size = get_visual_text_size(gc, font, clip_start_offset, text.size() - clip_start_offset);

	if (cursor_pos < clip_start_offset)
		clip_start_offset = cursor_pos;

	CL_Rect cursor_rect = get_cursor_rect();

	CL_UTF8_Reader utf8_reader(text);
	while (cursor_rect.right > content_rect.right)
	{
		utf8_reader.set_position(clip_start_offset);
		utf8_reader.next();
		clip_start_offset = utf8_reader.get_position();
		if (clip_start_offset == text.size())
			break;
		cursor_rect = get_cursor_rect();
	}

	// Get number of chars of current text fitting in the lineedit.
	int search_upper = text.size();
	int search_lower = clip_start_offset;

	while (true)
	{
		int midpoint = (search_lower + search_upper) / 2;

		utf8_reader.set_position(midpoint);
		utf8_reader.move_to_leadbyte();
		if (midpoint != utf8_reader.get_position())
			utf8_reader.next();
		midpoint = utf8_reader.get_position();

		if (midpoint == search_lower || midpoint == search_upper)
			break;

		CL_Size midpoint_size = get_visual_text_size(gc, font, clip_start_offset, midpoint-clip_start_offset);

		if (content_rect.right < midpoint_size.width)
			search_upper = midpoint;
		else
			search_lower = midpoint;
	}
	clip_end_offset = search_upper;

	if (cursor_rect.left < content_rect.left)
	{
		clip_start_offset = cursor_pos;
	}
}

CL_Rect CL_LineEdit_Impl::get_cursor_rect()
{
	CL_GraphicContext &gc = lineedit->get_gc();
	CL_Font font = part_component.get_font();

	CL_Rect cursor_rect;

	int substr_end = cursor_pos - clip_start_offset;
	if (substr_end < 0)
		substr_end = 0;

	CL_String clipped_text;

	if ( password_mode )
	{
		// If we are in password mode, we gonna return the right characters
		clipped_text = create_password(substr_end);
	}
	else
	{
		clipped_text = text.substr(clip_start_offset, substr_end);
	}

	CL_Size text_size_before_cursor = font.get_text_size(gc, clipped_text);

	cursor_rect.left = content_rect.left + text_size_before_cursor.width;
	cursor_rect.right = cursor_rect.left + part_cursor.get_preferred_width();

	cursor_rect.top = vertical_text_align.top;
	cursor_rect.bottom = vertical_text_align.bottom;

	return cursor_rect;
}

CL_Rect CL_LineEdit_Impl::get_selection_rect()
{
	CL_GraphicContext &gc = lineedit->get_gc();

	// text before selection:
	CL_Font font = part_component.get_font();

	CL_String txt_before = get_visible_text_before_selection();
	CL_Size text_size_before_selection = font.get_text_size(gc, txt_before);

	// selection text:
	font = part_selection.get_font();
	CL_String txt_selected = get_visible_selected_text();
	CL_Size text_size_selection = font.get_text_size(gc, txt_selected);

	CL_Rect selection_rect;
	selection_rect.left = content_rect.left + text_size_before_selection.width;
	selection_rect.right = selection_rect.left + text_size_selection.width;
	selection_rect.top = vertical_text_align.top;
	selection_rect.bottom = vertical_text_align.bottom;

	return selection_rect;
}

CL_String CL_LineEdit_Impl::break_characters = " ::;,.-";

int CL_LineEdit_Impl::find_next_break_character(int search_start)
{
	if (search_start >= int(text.size())-1)
		return text.size();

	int pos = text.find_first_of(break_characters, search_start);
	if (pos == CL_String::npos)
		return text.size();
	return pos;
}

int CL_LineEdit_Impl::find_previous_break_character(int search_start)
{
	if (search_start <= 0)
		return 0;
	int pos = text.find_last_of(break_characters, search_start);
	if (pos == CL_String::npos)
		return 0;
	return pos;
}

void CL_LineEdit_Impl::on_timer_expired()
{
	if (lineedit == 0 || lineedit->is_visible() == false)
	{
		timer.stop();
		return;
	}

	if (cursor_blink_visible)
		timer.start(500);
	else
		timer.start(500);

	cursor_blink_visible = !cursor_blink_visible;
	lineedit->request_repaint();
}

void CL_LineEdit_Impl::on_resized()
{
	content_rect = part_component.get_content_box(lineedit->get_size());

	CL_GraphicContext &gc = lineedit->get_gc();
	CL_Font font = part_component.get_font();

	vertical_text_align = part_component.get_vertical_text_align(gc, font, content_rect);

	clip_start_offset = 0;
	cursor_pos = 0;
	update_text_clipping();
}

CL_String CL_LineEdit_Impl::get_visible_text_before_selection()
{
	int sel_start = cl_min(selection_start, selection_start+selection_length);
	int start = cl_min(sel_start, clip_start_offset);
	
	if (start < clip_start_offset)
		return CL_String();

	int end = cl_min(sel_start, clip_end_offset);

	// If we are in password mode, we gonna return the right characters
	if ( password_mode )
		return create_password(end-start);

	return text.substr(start, end-start);
}

CL_String CL_LineEdit_Impl::get_visible_selected_text()
{
	if (selection_length == 0)
		return CL_String();

	int sel_start = cl_min(selection_start, selection_start+selection_length);
	int sel_end = cl_max(selection_start, selection_start + selection_length);
	int end = cl_min(clip_end_offset, sel_end);
	int start = cl_max(clip_start_offset, sel_start);

	if (start > end)
		return CL_String();

	if (start == end)
		return CL_String();

	// If we are in password mode, we gonna return the right characters
	if ( password_mode )
		return create_password(end-start);

	return text.substr(start, end-start);
}

CL_String CL_LineEdit_Impl::get_visible_text_after_selection()
{
	// returns the whole visible string if there is no selection.

	int sel_end = cl_max(selection_start, selection_start + selection_length);
	int start = cl_max(clip_start_offset, sel_end);

	int end = clip_end_offset;
	if (start > end)
		return CL_String();

	if (clip_end_offset == sel_end)
		return CL_String();

	if (sel_end <= 0)
		return CL_String();
	else
	{
		// If we are in password mode, we gonna return the right characters
		if ( password_mode )
			return create_password(end-start);

		return text.substr(start, end-start);
	}
}

void CL_LineEdit_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect g = lineedit->get_size();
	part_component.render_box(gc, g, update_rect);

	CL_Font font = part_component.get_font();

	CL_String txt_before = get_visible_text_before_selection();
	CL_String txt_selected = get_visible_selected_text();
	CL_String txt_after = get_visible_text_after_selection();

	if (txt_before.empty() && txt_selected.empty() && txt_after.empty())
	{

		// If we are in password mode, we gonna return the right characters
		if ( password_mode )
		{
			txt_after = create_password(clip_end_offset-clip_start_offset);
		}
		else
		{
			txt_after = text.substr(clip_start_offset, clip_end_offset-clip_start_offset);
		}
	}

	CL_Size size_before = font.get_text_size(gc, txt_before);
	CL_Size size_selected = font.get_text_size(gc, txt_selected);

	// Draw text before selection
	if (!txt_before.empty())
	{
		CL_Rect text_rect = content_rect;
		text_rect.top = g.top;
		text_rect.bottom = g.bottom;
		part_component.render_text(gc, txt_before, text_rect, update_rect);
	}
	if (!txt_selected.empty())
	{
		// Draw selection box.
		CL_Rect selection_rect = get_selection_rect();
		part_selection.render_box(gc, selection_rect, update_rect);

		CL_Rect text_rect = content_rect;
		text_rect.left += (size_before.width);
		text_rect.top = g.top;
		text_rect.bottom = g.bottom;
		part_selection.render_text(gc, txt_selected, text_rect, update_rect);
	}
	if (!txt_after.empty())
	{
		CL_Rect text_rect = content_rect;
		text_rect.left += (size_before.width + size_selected.width);
		text_rect.top = g.top;
		text_rect.bottom = g.bottom;
		part_component.render_text(gc, txt_after, text_rect, update_rect);
	}

	// draw cursor
	if (lineedit->has_focus() || (lineedit->get_focus_policy() == CL_GUIComponent::focus_parent && cursor_drawing_enabled_when_parent_focused))
	{
		if (cursor_blink_visible)
		{
			CL_Rect cursor_rect = get_cursor_rect();
			part_cursor.render_box(gc, cursor_rect, update_rect);
		}
	}
}

void CL_LineEdit_Impl::on_scroll_timer_expired()
{
	CL_GUIMessage_Input msg;
	CL_InputEvent event;
	msg.set_event(event);

	if (mouse_moves_left)
		move(-1,event);
	else
		move(1, event);
}

void CL_LineEdit_Impl::on_enable_changed()
{
	create_parts();

	bool enabled = lineedit->is_enabled();

	if (!enabled)
	{
		cursor_blink_visible = false;
		timer.stop();
	}
	lineedit->request_repaint();
}

bool CL_LineEdit_Impl::input_mask_accepts_input(int cursor_pos, const CL_StringRef &str)
{
	throw CL_Exception("CL_LineEdit_Impl::input_mask_accepts_input - not implemented!");
	return true;
}

CL_String CL_LineEdit_Impl::create_password(CL_String::size_type num_letters) const
{
	return CL_String(num_letters, '*');
}

CL_Size CL_LineEdit_Impl::get_visual_text_size(CL_GraphicContext &gc, CL_Font &font, int pos, int npos) const
{
	return password_mode ? font.get_text_size(gc, create_password(npos)) : font.get_text_size(gc, text.substr(pos, npos));
}

CL_Size CL_LineEdit_Impl::get_visual_text_size(CL_GraphicContext &gc, CL_Font &font) const
{
	return password_mode ? font.get_text_size(gc, create_password(text.size())) : font.get_text_size(gc, text);
}


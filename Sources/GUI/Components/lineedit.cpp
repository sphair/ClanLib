/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/lineedit.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Window/display_window.h"
#include "../gui_css_strings.h"
#include "lineedit_impl.h"
#include "API/Display/2D/canvas.h"

#ifndef WIN32
#include "stdlib.h"
#endif

namespace clan
{

const std::string LineEdit_Impl::numeric_mode_characters = "0123456789";

/////////////////////////////////////////////////////////////////////////////
// LineEdit Construction:

LineEdit::LineEdit(GUIComponent *parent)
: GUIComponent(parent, CssStr::LineEdit::type_name),
  impl(new LineEdit_Impl)
{
	set_focus_policy(focus_local);

	func_render().set(impl.get(), &LineEdit_Impl::on_render);
	func_process_message().set(impl.get(), &LineEdit_Impl::on_process_message);
	func_resized().set(impl.get(), &LineEdit_Impl::on_resized);
	func_enablemode_changed().set(impl.get(), &LineEdit_Impl::on_enable_changed);

	impl->lineedit = this;
	impl->timer.func_expired().set(impl.get(), &LineEdit_Impl::on_timer_expired);

	impl->scroll_timer.func_expired().set(impl.get(), &LineEdit_Impl::on_scroll_timer_expired);
	impl->create_parts();
}

LineEdit::~LineEdit()
{
}

/////////////////////////////////////////////////////////////////////////////
// LineEdit Attributes:

LineEdit *LineEdit::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	LineEdit *object = NULL;
	if (reference_component)
		object = dynamic_cast<LineEdit*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find LineEdit named item: %1", id));

	return object;
}

bool LineEdit::is_read_only() const
{
	return impl->readonly;
}

LineEdit::Alignment LineEdit::get_alignment() const
{
	return align_left;
}

bool LineEdit::is_lowercase() const
{
	return impl->lowercase;
}

bool LineEdit::is_uppercase() const
{
	return impl->uppercase;
}

bool LineEdit::is_password_mode() const
{
	return impl->password_mode;
}

int LineEdit::get_max_length() const
{
	return impl->max_length;
}

std::string LineEdit::get_text() const
{
	return impl->text;
}

int LineEdit::get_text_int() const
{
	return StringHelp::text_to_int(impl->text);
}

float LineEdit::get_text_float() const
{
	return StringHelp::text_to_float(impl->text);
}

std::string LineEdit::get_selection() const
{
	int start = min(impl->selection_start, impl->selection_start + impl->selection_length);
	return impl->text.substr(start, abs(impl->selection_length));
}

int LineEdit::get_selection_start() const
{
	return impl->selection_start;
}

int LineEdit::get_selection_length() const
{
	return impl->selection_length;
}

int LineEdit::get_cursor_pos() const
{
	return impl->cursor_pos;
}

Size LineEdit::get_text_size()
{
	Canvas canvas = get_canvas();
	return impl->get_visual_text_size(canvas);
}

Size LineEdit::get_text_size(const std::string &str)
{
	Canvas canvas = get_canvas();
	return get_render_text_size(canvas, str);;
}

float LineEdit::get_preferred_content_width()
{
	return get_text_size().width;
}

float LineEdit::get_preferred_content_height(float width)
{
	return get_text_size().height;
}

/////////////////////////////////////////////////////////////////////////////
// LineEdit Operations:

void LineEdit::select_all()
{
	impl->set_text_selection(0, impl->text.size());
	request_repaint();
}

void LineEdit::set_read_only(bool enable)
{
	if (impl->readonly != enable)
	{
		impl->readonly = enable;
		request_repaint();
	}
}

void LineEdit::set_alignment(Alignment alignment)
{
	if (impl->alignment != alignment)
	{
		impl->alignment = alignment;
		request_repaint();
	}
}

void LineEdit::set_lowercase(bool enable)
{
	if (impl->lowercase != enable)
	{
		impl->lowercase = enable;
		impl->text = StringHelp::text_to_lower(impl->text);
		request_repaint();
	}
}

void LineEdit::set_uppercase(bool enable)
{
	if (impl->uppercase != enable)
	{
		impl->uppercase = enable;
		impl->text = StringHelp::text_to_upper(impl->text);
		request_repaint();
	}
}

void LineEdit::set_password_mode(bool enable)
{
	if (impl->password_mode != enable)
	{
		impl->password_mode = enable;
		request_repaint();
	}
}

void LineEdit::set_max_length(int length)
{
	if (impl->max_length != length)
	{
		impl->max_length = length;
		if ((int)impl->text.length() > length)
		{
			InputEvent no_event;
			if (!impl->func_before_edit_changed.is_null())
				impl->func_before_edit_changed.invoke(no_event);
			impl->text = impl->text.substr(0, length);
			if (!impl->func_after_edit_changed.is_null())
				impl->func_after_edit_changed.invoke(no_event);
		}
		request_repaint();
	}
}

void LineEdit::set_text(const std::string &text)
{
	if(impl->lowercase)
		impl->text = StringHelp::text_to_lower(text);
	else if(impl->uppercase)
		impl->text = StringHelp::text_to_upper(text);
	else
		impl->text = text;

	impl->clip_start_offset = 0;
	impl->update_text_clipping();
	set_cursor_pos(impl->text.size());
	impl->set_text_selection(0, 0);
	request_repaint();
}

void LineEdit::set_text(int number)
{
	impl->text = StringHelp::int_to_text(number);
	impl->clip_start_offset = 0;
	impl->update_text_clipping();
	set_cursor_pos(impl->text.size());
	impl->set_text_selection(0, 0);
	request_repaint();
}

void LineEdit::set_text(float number, int num_decimal_places)
{
	impl->text = StringHelp::float_to_text(number, num_decimal_places);
	impl->clip_start_offset = 0;
	impl->update_text_clipping();
	set_cursor_pos(impl->text.size());
	impl->set_text_selection(0, 0);
	request_repaint();
}

void LineEdit::set_selection(int pos, int length)
{
	//don't call func_selection_changed() here, because this
	//member is for public usage
	impl->selection_start = pos;
	impl->selection_length = length;
	request_repaint();
}

void LineEdit::clear_selection()
{
	//don't call func_selection_changed() here, because this
	//member is for public usage
	set_selection(0, 0);
	request_repaint();
}

void LineEdit::delete_selected_text()
{
	if (get_selection_length() == 0)
		return;

	int sel_start = impl->selection_start;
	int sel_end = impl->selection_start + impl->selection_length;
	if (sel_start > sel_end)
		std::swap(sel_start, sel_end);

	impl->text = impl->text.substr(0, sel_start) + impl->text.substr(sel_end, impl->text.size());
	impl->cursor_pos = sel_start;
	impl->set_text_selection(0, 0);
	int old_pos = get_cursor_pos();
	set_cursor_pos(0);
	set_cursor_pos(old_pos);
}

void LineEdit::set_cursor_pos(int pos)
{
	impl->cursor_pos = pos;
	impl->update_text_clipping();
	request_repaint();
}

void LineEdit::resize_to_fit(int max_width)
{
	Canvas canvas = get_canvas();

	Rect g = get_geometry();
	Rect rect_content = impl->lineedit->get_content_box();
	Size text_size = impl->get_visual_text_size(canvas);
	rect_content.set_size(Size(text_size.width+1, rect_content.get_height()));

	g.set_size(impl->lineedit->get_size());
	set_geometry(g);

	impl->clip_start_offset = 0;

	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// LineEdit Events:

Callback_v1<InputEvent &> &LineEdit::func_before_edit_changed()
{
	return impl->func_before_edit_changed;
}

Callback_v1<InputEvent &> &LineEdit::func_after_edit_changed()
{
	return impl->func_after_edit_changed;
}

Callback_v0 &LineEdit::func_selection_changed()
{
	return impl->func_selection_changed;
}

Callback_v0 &LineEdit::func_focus_gained()
{
	return impl->func_focus_gained;
}

Callback_v0 & LineEdit::func_focus_lost()
{
	return impl->func_focus_lost;
}

Callback_v0 &LineEdit::func_enter_pressed()
{
	return impl->func_enter_pressed;
}

void LineEdit::set_input_mask( const std::string &mask )
{
	impl->input_mask = mask;
}

void LineEdit::set_numeric_mode( bool enable, bool decimals)
{
	impl->numeric_mode = enable;
	impl->numeric_mode_decimals = decimals;
}

void LineEdit::set_decimal_character( const std::string &decimal_char )
{
	impl->decimal_char = decimal_char;
}

void LineEdit::set_cursor_drawing_enabled(bool enable)
{
	impl->cursor_drawing_enabled_when_parent_focused = enable;

	if (!impl->readonly)
		impl->timer.start(500);
}

void LineEdit::set_select_all_on_focus_gain(bool enable)
{
	impl->select_all_on_focus_gain = enable;
}

/////////////////////////////////////////////////////////////////////////////
// LineEdit Implementation:

void LineEdit_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!lineedit->is_enabled())
		return;

	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		
		InputEvent &e = input_msg->input_event;

		if (e.device.get_type() == InputDevice::keyboard)
		{
			if (!func_enter_pressed.is_null() && 
				e.type == InputEvent::pressed &&
				(e.id == keycode_enter || e.id == keycode_return || e.id == keycode_numpad_enter))
			{
				func_enter_pressed.invoke();
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
					set_text_selection(0, text.size());
					cursor_pos = selection_length;
					update_text_clipping();
					lineedit->request_repaint();
					msg->consumed = true;
				}
				else if (e.id == keycode_c && e.ctrl)
				{
					if(!password_mode)	// Do not allow copying the password to clipboard
					{
						std::string str = lineedit->get_selection();
						lineedit->get_gui_manager().set_clipboard_text(str);
						msg->consumed = true;
					}
				}
				else if (readonly)
				{
					// Do not consume messages on read only component (only allow CTRL-A and CTRL-C)
					return;
				}
				else if (e.id == keycode_left)
				{
					move(-1, e.ctrl, e.shift);
					msg->consumed = true;
				}
				else if (e.id == keycode_right)
				{
					move(1, e.ctrl, e.shift);
					msg->consumed = true;
				}
				else if (e.id == keycode_backspace)
				{
					backspace();
					update_text_clipping();
					msg->consumed = true;
				}
				else if (e.id == keycode_delete)
				{
					del();
					update_text_clipping();
					msg->consumed = true;
				}
				else if (e.id == keycode_home)
				{
					set_selection_start(cursor_pos);
					cursor_pos = 0;
					if (e.shift)
						set_selection_length(-selection_start);
					else
						set_text_selection(0, 0);
					update_text_clipping();
					lineedit->request_repaint();
					msg->consumed = true;
				}
				else if (e.id == keycode_end)
				{
					set_selection_start(cursor_pos);
					cursor_pos = text.size();
					if (e.shift)
						set_selection_length(text.size() - selection_start);
					else
						set_text_selection(0, 0);
					update_text_clipping();
					lineedit->request_repaint();
					msg->consumed = true;
				}
				else if (e.id == keycode_x && e.ctrl)
				{
					std::string str = lineedit->get_selection();
					lineedit->delete_selected_text();
					lineedit->get_gui_manager().set_clipboard_text(str);
					update_text_clipping();	
					msg->consumed = true;
				}
				else if (e.id == keycode_v && e.ctrl)
				{
					std::string str = lineedit->get_gui_manager().get_clipboard_text();
					std::string::const_iterator end_str = std::remove(str.begin(), str.end(), '\n');
					str.resize(end_str - str.begin());
					end_str = std::remove(str.begin(), str.end(), '\r');
					str.resize(end_str - str.begin());
					lineedit->delete_selected_text();

					if (input_mask.empty())
					{
						if (numeric_mode)
						{
							std::string present_text = lineedit->get_text();

							bool present_minus = present_text.find('-') != std::string::npos;
							bool str_minus = str.find('-') != std::string::npos;

							if(!present_minus || !str_minus)
							{
								if((!present_minus && !str_minus) || //if no minus found
									(str_minus && cursor_pos == 0 && str[0] == '-') || //if there's minus in text to paste
									(present_minus && cursor_pos > 0)) //if there's minus in the beginning of control's text
								{
									if(numeric_mode_decimals)
									{
										std::string::size_type decimal_point_pos;
										if((decimal_point_pos = str.find_first_not_of(numeric_mode_characters, str[0] == '-' ? 1 : 0)) == std::string::npos) //no decimal char inside string to paste
										{ //we don't look at the position of decimal char inside of text in the textbox, if it's present
											if(insert_text(cursor_pos, str))
												lineedit->set_cursor_pos(cursor_pos + str.length());
										}
										else
										{
											if(present_text.find(decimal_char) == std::string::npos &&
												str[decimal_point_pos] == decimal_char[0] &&
												str.find_first_not_of(numeric_mode_characters, decimal_point_pos + 1) == std::string::npos) //allow only one decimal char in the string to paste
											{
												if(insert_text(cursor_pos, str))
													lineedit->set_cursor_pos(cursor_pos + str.length());
											}
										}
									}
									else
									{
										if(str.find_first_not_of(numeric_mode_characters, str[0] == '-' ? 1 : 0) == std::string::npos)
										{
											if(insert_text(cursor_pos, str))
												lineedit->set_cursor_pos(cursor_pos + str.length());
										}
									}
								}
							}
						}
						else
						{
							if(insert_text(cursor_pos, str))
								lineedit->set_cursor_pos(cursor_pos + str.length());
						}
					}
					else
					{
						if (input_mask_accepts_input(cursor_pos, str))
						{
							if(insert_text(cursor_pos, str))
								lineedit->set_cursor_pos(cursor_pos + str.length());
						}
					}

					update_text_clipping();
					msg->consumed = true;
				}
				else if (e.id == keycode_shift)
				{
					if (selection_start == -1)
						set_text_selection(cursor_pos, 0);

					msg->consumed = true;
				}
				else if ((!e.str.empty() && !(e.str[0] >= 0 && e.str[0] < 32) && (!e.alt && !e.ctrl)) || (e.ctrl && e.alt)) // Alt Gr translates to Ctrl+Alt sometimes!
				{
					lineedit->delete_selected_text();
					if (input_mask.empty())
					{
						if (numeric_mode)
						{
							// '-' can only be added once, and only as the first character.
							if (e.str == "-" && cursor_pos == 0 && text.find("-") == std::string::npos) 
							{
								if(insert_text(cursor_pos, e.str))
									cursor_pos += e.str.size();
							}
							else if (numeric_mode_decimals && e.str == decimal_char && cursor_pos > 0) // add decimal char 
							{
								if (text.find(decimal_char) == std::string::npos) // allow only one decimal char.
								{
									if(insert_text(cursor_pos, e.str))
										cursor_pos += e.str.size();
								}
							}
							else if (numeric_mode_characters.find(e.str) != std::string::npos) // 0-9
							{
								if(insert_text(cursor_pos, e.str))
									cursor_pos += e.str.size();
							}
						}
						else
						{
							// not in any special mode, just insert the string.
							if(insert_text(cursor_pos, e.str))
								cursor_pos += e.str.size();
						}
					}
					else
					{
						if (input_mask_accepts_input(cursor_pos, e.str))
						{
							if(insert_text(cursor_pos, e.str))
								cursor_pos += e.str.size();
						}
					}
					update_text_clipping();
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
						undo_info.undo_text = lineedit->get_text();
						lineedit->set_text(tmp);
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
				if (lineedit->has_focus())
				{
					lineedit->capture_mouse(true);
					mouse_selecting = true;
					cursor_pos = get_character_index(e.mouse_pos.x);
					set_text_selection(cursor_pos, 0);
				}
				else
				{
					lineedit->set_focus();
				}
				lineedit->request_repaint();
				msg->consumed = true;
			}
			if (mouse_selecting && e.type == InputEvent::released && e.id == mouse_left)
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
					set_selection_length(sel_end - selection_start);
					cursor_pos = sel_end;
					lineedit->set_focus();
					lineedit->request_repaint();
				}
				msg->consumed = true;
			}
			if (e.type == InputEvent::pointer_moved && mouse_selecting && !ignore_mouse_events)
			{
				Rect content_rect = lineedit->get_content_box();
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
					set_selection_length(cursor_pos - selection_start);
					lineedit->request_repaint();
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
			if (lineedit->get_focus_policy() == GUIComponent::focus_parent)
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
			part_selection.set_pseudo_class(CssStr::unfocused, false);
			cursor_pos = text.length();

			lineedit->request_repaint();

			if (!func_focus_gained.is_null())
				func_focus_gained.invoke();
		}
		else if (focus_change_msg->focus_type == GUIMessage_FocusChange::losing_focus)
		{
			timer.stop();
			set_text_selection(0, 0);
			part_selection.set_pseudo_class(CssStr::unfocused, true);

			lineedit->request_repaint();

			if (!func_focus_lost.is_null())
				func_focus_lost.invoke();
		}
	}
	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
		{
			lineedit->set_cursor(cursor_ibeam);
		}
		else
		{
			lineedit->set_cursor(cursor_arrow);
		}
	}
}


void LineEdit_Impl::create_parts()
{
	part_selection = GUIThemePart(lineedit, CssStr::LineEdit::part_selection);
	part_cursor = GUIThemePart(lineedit, CssStr::LineEdit::part_cursor);

	bool enabled = lineedit->is_enabled();

	lineedit->set_pseudo_class(CssStr::hot, false);
	lineedit->set_pseudo_class(CssStr::normal, enabled);
	lineedit->set_pseudo_class(CssStr::disabled, !enabled);

	part_cursor.set_pseudo_class(CssStr::normal, enabled);
	part_cursor.set_pseudo_class(CssStr::disabled, !enabled);

	part_selection.set_pseudo_class(CssStr::normal, enabled);
	part_selection.set_pseudo_class(CssStr::disabled, !enabled);

	on_resized();	//TODO: Is this required?
}

void LineEdit_Impl::move(int steps, bool ctrl, bool shift)
{
	if (shift && selection_length == 0)
		set_selection_start(cursor_pos);

	// Jump over words if control is pressed.
	if (ctrl)
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
		UTF8_Reader utf8_reader(text.data(), text.length());
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

	
	// Clear the selection if a cursor key is pressed but shift isn't down. 
	if (shift)
		set_selection_length(cursor_pos - selection_start);
	else
		set_text_selection(-1, 0);

	update_text_clipping();

	lineedit->request_repaint();

	undo_info.first_text_insert = true;
}

bool LineEdit_Impl::insert_text(int pos, const std::string &str)
{
	undo_info.first_erase = false;
	if (undo_info.first_text_insert)
	{
		undo_info.undo_text = lineedit->get_text();
		undo_info.first_text_insert = false;
	}

	// checking if insert exceeds max length
	if(StringHelp::utf8_length(text) + StringHelp::utf8_length(str) > max_length)
	{
		return false;
	}

	if(lowercase)
		text.insert(pos, StringHelp::text_to_lower(str));
	else if(uppercase)
		text.insert(pos, StringHelp::text_to_upper(str));
	else
		text.insert(pos, str);

	update_text_clipping();
	lineedit->request_repaint();
	return true;
}

void LineEdit_Impl::backspace()
{
	if (undo_info.first_erase)
	{
		undo_info.first_erase = false;
		undo_info.undo_text = lineedit->get_text();
	}

	if (lineedit->get_selection_length() != 0)
	{
		lineedit->delete_selected_text();
		lineedit->request_repaint();
	}
	else
	{
		if (cursor_pos > 0)
		{
			UTF8_Reader utf8_reader(text.data(), text.length());
			utf8_reader.set_position(cursor_pos);
			utf8_reader.prev();
			int length = utf8_reader.get_char_length();
			text.erase(cursor_pos-length, length);
			cursor_pos -= length;
			lineedit->request_repaint();
		}
	}

	int old_pos = lineedit->get_cursor_pos();
	lineedit->set_cursor_pos(0);
	lineedit->set_cursor_pos(old_pos);
}

void LineEdit_Impl::del()
{
	if (undo_info.first_erase)
	{
		undo_info.first_erase = false;
		undo_info.undo_text = lineedit->get_text();
	}

	if (lineedit->get_selection_length() != 0)
	{
		lineedit->delete_selected_text();
		lineedit->request_repaint();
	}
	else
	{
		if (cursor_pos < (int)text.size())
		{
			UTF8_Reader utf8_reader(text.data(), text.length());
			utf8_reader.set_position(cursor_pos);
			int length = utf8_reader.get_char_length();
			text.erase(cursor_pos,length);
			lineedit->request_repaint();
		}
	}
}

int LineEdit_Impl::get_character_index(int mouse_x_wincoords)
{
	if (text.size() <= 1 )
	{
		return text.size();
	}

	Canvas canvas = lineedit->get_canvas();
	UTF8_Reader utf8_reader(text.data(), text.length());

	Rect content_rect = lineedit->get_content_box();
	int mouse_x = mouse_x_wincoords - content_rect.left ;

	int seek_start = clip_start_offset;
	int seek_end = text.size();
	int seek_center = (seek_start + seek_end) / 2;

	//fast search
	while (true)
	{
		utf8_reader.set_position(seek_center);
		utf8_reader.move_to_leadbyte();

		seek_center = utf8_reader.get_position();

		Size text_size = get_visual_text_size(canvas, clip_start_offset, seek_center - clip_start_offset);

		if (text_size.width > mouse_x)
			seek_end = seek_center;
		else
			seek_start = seek_center;

		if(seek_end - seek_start < 7)
			break; //go to accurate search

		seek_center = (seek_start + seek_end) / 2;
	}

	utf8_reader.set_position(seek_start);
	utf8_reader.move_to_leadbyte();

	//accurate search
	while (true)
	{
		seek_center = utf8_reader.get_position();

		Size text_size = get_visual_text_size(canvas, clip_start_offset, seek_center - clip_start_offset);
		if (text_size.width > mouse_x || utf8_reader.is_end())
			break;

		utf8_reader.next();
	}

	return seek_center;
}

void LineEdit_Impl::update_text_clipping()
{
	Canvas canvas = lineedit->get_canvas();

	Size text_size = get_visual_text_size(canvas, clip_start_offset, text.size() - clip_start_offset);

	if (cursor_pos < clip_start_offset)
		clip_start_offset = cursor_pos;

	Rect cursor_rect = get_cursor_rect();

	UTF8_Reader utf8_reader(text.data(), text.length());
	Rect content_rect = lineedit->get_content_box();
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

		Size midpoint_size = get_visual_text_size(canvas, clip_start_offset, midpoint-clip_start_offset);

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

Rect LineEdit_Impl::get_cursor_rect()
{
	Canvas canvas = lineedit->get_canvas();

	Rect cursor_rect;

	int substr_end = cursor_pos - clip_start_offset;
	if (substr_end < 0)
		substr_end = 0;

	std::string clipped_text = text.substr(clip_start_offset, substr_end);

	if ( password_mode )
	{
		// If we are in password mode, we gonna return the right characters
		clipped_text = create_password(StringHelp::utf8_length(clipped_text));
	}

	Size text_size_before_cursor = lineedit->get_render_text_size(canvas, clipped_text);

	Rect content_rect = lineedit->get_content_box();
	cursor_rect.left = content_rect.left + text_size_before_cursor.width;
	cursor_rect.right = cursor_rect.left + part_cursor.get_css_width();

	cursor_rect.top = vertical_text_align.top;
	cursor_rect.bottom = vertical_text_align.bottom;

	return cursor_rect;
}

Rect LineEdit_Impl::get_selection_rect()
{
	Canvas canvas = lineedit->get_canvas();

	// text before selection:

	std::string txt_before = get_visible_text_before_selection();
	Size text_size_before_selection = lineedit->get_render_text_size(canvas, txt_before);

	// selection text:
	std::string txt_selected = get_visible_selected_text();
	Size text_size_selection = part_selection.get_render_text_size(canvas, txt_selected);

	Rect selection_rect;
	Rect content_rect = lineedit->get_content_box();
	selection_rect.left = content_rect.left + text_size_before_selection.width;
	selection_rect.right = selection_rect.left + text_size_selection.width;
	selection_rect.top = vertical_text_align.top;
	selection_rect.bottom = vertical_text_align.bottom;

	return selection_rect;
}

std::string LineEdit_Impl::break_characters = " ::;,.-";

int LineEdit_Impl::find_next_break_character(int search_start)
{
	if (search_start >= int(text.size())-1)
		return text.size();

	int pos = text.find_first_of(break_characters, search_start);
	if (pos == std::string::npos)
		return text.size();
	return pos;
}

int LineEdit_Impl::find_previous_break_character(int search_start)
{
	if (search_start <= 0)
		return 0;
	int pos = text.find_last_of(break_characters, search_start);
	if (pos == std::string::npos)
		return 0;
	return pos;
}

void LineEdit_Impl::on_timer_expired()
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

void LineEdit_Impl::on_resized()
{
	Canvas canvas = lineedit->get_canvas();

	vertical_text_align = lineedit->get_vertical_text_align(canvas);

	clip_start_offset = 0;
	cursor_pos = 0;
	update_text_clipping();
}

std::string LineEdit_Impl::get_visible_text_before_selection()
{
	std::string ret;
	int sel_start = min(selection_start, selection_start+selection_length);
	int start = min(sel_start, clip_start_offset);
	
	if (start < clip_start_offset)
		return ret;

	int end = min(sel_start, clip_end_offset);

	ret = text.substr(start, end - start);

	// If we are in password mode, we gonna return the right characters
	if ( password_mode )
		ret = create_password(StringHelp::utf8_length(ret));

	return ret;
}

std::string LineEdit_Impl::get_visible_selected_text()
{
	std::string ret;

	if (selection_length == 0)
		return ret;

	int sel_start = min(selection_start, selection_start+selection_length);
	int sel_end = max(selection_start, selection_start + selection_length);
	int end = min(clip_end_offset, sel_end);
	int start = max(clip_start_offset, sel_start);

	if (start > end)
		return ret;

	if (start == end)
		return ret;

	ret = text.substr(start, end - start);

	// If we are in password mode, we gonna return the right characters
	if ( password_mode )
		ret = create_password(StringHelp::utf8_length(ret));

	return ret;
}

void LineEdit_Impl::set_selection_start(int start)
{
	if(!func_selection_changed.is_null() && selection_length && selection_start != start)
		func_selection_changed.invoke();

	selection_start = start;
}

void LineEdit_Impl::set_selection_length(int length)
{
	if(!func_selection_changed.is_null() && selection_length != length)
		func_selection_changed.invoke();

	selection_length = length;
}

void LineEdit_Impl::set_text_selection(int start, int length)
{
	if(!func_selection_changed.is_null() && (selection_length != length || (selection_length && selection_start != start)))
		func_selection_changed.invoke();
	
	selection_start = start;
	selection_length = length;
}

std::string LineEdit_Impl::get_visible_text_after_selection()
{
	// returns the whole visible string if there is no selection.
	std::string ret;

	int sel_end = max(selection_start, selection_start + selection_length);
	int start = max(clip_start_offset, sel_end);

	int end = clip_end_offset;
	if (start > end)
		return ret;

	if (clip_end_offset == sel_end)
		return ret;

	if (sel_end <= 0)
		return ret;
	else
	{
		ret = text.substr(start, end - start);
		// If we are in password mode, we gonna return the right characters
		if ( password_mode )
			ret = create_password(StringHelp::utf8_length(ret));

		return ret;
	}
}

void LineEdit_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect g = lineedit->get_size();

	std::string txt_before = get_visible_text_before_selection();
	std::string txt_selected = get_visible_selected_text();
	std::string txt_after = get_visible_text_after_selection();

	if (txt_before.empty() && txt_selected.empty() && txt_after.empty())
	{
		txt_after = text.substr(clip_start_offset, clip_end_offset - clip_start_offset);

		// If we are in password mode, we gonna return the right characters
		if ( password_mode )
			txt_after = create_password(StringHelp::utf8_length(txt_after));
	}

	Size size_before = lineedit->get_render_text_size(canvas, txt_before);
	Size size_selected = part_selection.get_render_text_size(canvas, txt_selected);
	Rect content_rect = lineedit->get_content_box();

	// Draw text before selection
	if (!txt_before.empty())
	{
		lineedit->render_text(canvas, txt_before, 0, lineedit->get_vertical_text_align(canvas).baseline);
	}
	if (!txt_selected.empty())
	{
		// Draw selection box.
		Rect selection_rect = get_selection_rect();
		part_selection.render_box(canvas, selection_rect);

		Rect text_rect = content_rect;
		text_rect.left += (size_before.width);
		text_rect.top = g.top;
		text_rect.bottom = g.bottom;
		part_selection.render_text(canvas, txt_selected, text_rect, part_selection.get_vertical_text_align(canvas, text_rect).baseline);
	}
	if (!txt_after.empty())
	{
		Rect text_rect = content_rect;
		text_rect.left += (size_before.width + size_selected.width);
		text_rect.top = g.top;
		text_rect.bottom = g.bottom;
		lineedit->render_text(canvas, txt_after, size_before.width + size_selected.width, lineedit->get_vertical_text_align(canvas).baseline);
	}

	// draw cursor
	if (lineedit->has_focus() || (lineedit->get_focus_policy() == GUIComponent::focus_parent && cursor_drawing_enabled_when_parent_focused))
	{
		if (cursor_blink_visible)
		{
			Rect cursor_rect = get_cursor_rect();
			part_cursor.render_box(canvas, cursor_rect);
		}
	}
}

void LineEdit_Impl::on_scroll_timer_expired()
{
	if (mouse_moves_left)
		move(-1,false, false);
	else
		move(1, false, false);
}

void LineEdit_Impl::on_enable_changed()
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

bool LineEdit_Impl::input_mask_accepts_input(int cursor_pos, const std::string &str)
{
	return str.find_first_not_of(input_mask) == std::string::npos;
}

std::string LineEdit_Impl::create_password(std::string::size_type num_letters) const
{
	return std::string(num_letters, '*');
}

Size LineEdit_Impl::get_visual_text_size(Canvas &canvas, int pos, int npos) const
{
	return password_mode ? lineedit->get_render_text_size(canvas, create_password(StringHelp::utf8_length(text.substr(pos, npos)))) : lineedit->get_render_text_size(canvas, text.substr(pos, npos));
}

Size LineEdit_Impl::get_visual_text_size(Canvas &canvas) const
{
	return password_mode ? lineedit->get_render_text_size(canvas, create_password(StringHelp::utf8_length(text))) : lineedit->get_render_text_size(canvas, text);
}

}

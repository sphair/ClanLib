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
**    Magnus Norddahl
*/
#include "GameIDE/precomp.h"
#include "scintilla_editor_impl.h"
#include "scintilla_editor.h"

namespace clan
{

ScintillaEditor_Impl::ScintillaEditor_Impl(ScintillaEditor *component)
: component(component), is_mouse_captured(false), scrollbar_vert(0), scrollbar_horz(0)
{
	component->func_render().set(this, &ScintillaEditor_Impl::on_render);
	component->func_resized().set(this, &ScintillaEditor_Impl::on_resized);
	component->func_input_pressed().set(this, &ScintillaEditor_Impl::on_input_pressed);
	component->func_input_released().set(this, &ScintillaEditor_Impl::on_input_released);
	component->func_input_doubleclick().set(this, &ScintillaEditor_Impl::on_input_doubleclick);
	component->func_input_pointer_moved().set(this, &ScintillaEditor_Impl::on_input_pointer_moved);
	component->func_focus_gained().set(this, &ScintillaEditor_Impl::on_focus_gained);
	component->func_focus_lost().set(this, &ScintillaEditor_Impl::on_focus_lost);
	component->set_focus_policy(GUIComponent::focus_local);
	scrollbar_vert = new ScrollBar(component);
	scrollbar_horz = new ScrollBar(component);
	scrollbar_vert->func_scroll().set(this, &ScintillaEditor_Impl::on_scrollbar_vert_scroll);
	scrollbar_vert->set_vertical();
	scrollbar_horz->func_scroll().set(this, &ScintillaEditor_Impl::on_scrollbar_horz_scroll);
	scrollbar_horz->set_horizontal();
	wMain = this;
}

ScintillaEditor_Impl::~ScintillaEditor_Impl()
{
}

int ScintillaEditor_Impl::to_int_color(const Colorf &c)
{
	Color ci = c;
	return ci.get_argb8();
}

Colorf ScintillaEditor_Impl::from_int_color(int color)
{
	Color ci((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
	return Colorf(ci);
}

void ScintillaEditor_Impl::Initialise()
{
}

void ScintillaEditor_Impl::Finalise()
{
	ScintillaBase::Finalise();
}

void ScintillaEditor_Impl::CreateCallTipWindow(Scintilla::PRectangle rc)
{
}

void ScintillaEditor_Impl::AddToPopUp(const char *label, int cmd, bool enabled)
{
}

sptr_t ScintillaEditor_Impl::WndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam)
{
	return ScintillaBase::WndProc(iMessage, wParam, lParam);
}

sptr_t ScintillaEditor_Impl::DefWndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam)
{
	return 0;
}

void ScintillaEditor_Impl::ScrollText(int linesToMove)
{
	component->request_repaint();
}

void ScintillaEditor_Impl::SetVerticalScrollPos()
{
	scrollbar_vert->set_position(topLine);
}

void ScintillaEditor_Impl::SetHorizontalScrollPos()
{
	scrollbar_horz->set_position(xOffset);
}

bool ScintillaEditor_Impl::ModifyScrollBars(int nMax, int nPage)
{
	bool modified = false;

	// Todo: use !verticalScrollBarVisible
	// Todo: use !horizontalScrollBarVisible || (wrapState != eWrapNone)

	if (scrollbar_vert->get_max() != nMax || scrollbar_vert->get_page_step() != nPage)
	{
		scrollbar_vert->set_ranges(0, max(nMax-nPage+1, 1), 1, max(nPage, 1));
		modified = true;
	}

	Scintilla::PRectangle rcText = GetTextRectangle();
	int page_width = rcText.Width();
	if (scrollbar_horz->get_max() != scrollWidth || scrollbar_horz->get_page_step() != page_width)
	{
		scrollbar_horz->set_ranges(0, max(scrollWidth-page_width+1, 1), 8, max(page_width, 1));
		if (scrollWidth < page_width)
		{
			HorizontalScrollTo(0);
		}
		modified = true;
	}

	return modified;
}

void ScintillaEditor_Impl::Copy()
{
	component->get_display_window().set_clipboard_text(component->get_selection_text());
}

void ScintillaEditor_Impl::Paste()
{
	component->add_text(component->get_display_window().get_clipboard_text());
}

void ScintillaEditor_Impl::ClaimSelection()
{
}

void ScintillaEditor_Impl::NotifyChange()
{
	if (!cb_change.is_null())
		cb_change.invoke();
}

void ScintillaEditor_Impl::NotifyParent(Scintilla::SCNotification scn)
{
	switch (scn.nmhdr.code)
	{
	case SCN_STYLENEEDED:
		if (!cb_style_needed.is_null()) cb_style_needed.invoke(); break;
	case SCN_CHARADDED:
		if (!cb_char_added.is_null()) cb_char_added.invoke(); break;
	case SCN_SAVEPOINTREACHED:
		if (!cb_save_point_reached.is_null()) cb_save_point_reached.invoke(); break;
	case SCN_SAVEPOINTLEFT:
		if (!cb_save_point_left.is_null()) cb_save_point_left.invoke(); break;
	case SCN_MODIFYATTEMPTRO:
		if (!cb_modify_attempt_read_only.is_null()) cb_modify_attempt_read_only.invoke(); break;
	case SCN_KEY:
		if (!cb_key.is_null()) cb_key.invoke(); break;
	case SCN_DOUBLECLICK:
		if (!cb_double_click.is_null()) cb_double_click.invoke(); break;
	case SCN_UPDATEUI:
		if (!cb_update_ui.is_null()) cb_update_ui.invoke(); break;
	case SCN_MODIFIED:
		if (!cb_modified.is_null()) cb_modified.invoke(); break;
	case SCN_MACRORECORD:
		if (!cb_macro_record.is_null()) cb_macro_record.invoke(record_string(scn.message, scn.wParam, scn.lParam)); break;
	case SCN_MARGINCLICK:
		if (!cb_margin_click.is_null()) cb_margin_click.invoke((scn.modifiers & SCI_SHIFT) != 0, (scn.modifiers & SCI_CTRL) != 0, (scn.modifiers & SCI_ALT) != 0, scn.position, scn.margin); break;
	case SCN_NEEDSHOWN:
		if (!cb_need_shown.is_null())
			cb_need_shown.invoke(scn.position, scn.length);
		else
			for (int line = scn.position; line < scn.position + scn.length; line++)
				ensure_visible(line);
		break;
	case SCN_PAINTED:
		if (!cb_painted.is_null()) cb_painted.invoke(); break;
	case SCN_USERLISTSELECTION:
		if (!cb_userlist_selection.is_null()) cb_userlist_selection.invoke(); break;
	case SCN_DWELLSTART:
		if (!cb_dwell_start.is_null()) cb_dwell_start.invoke(scn.position, Point(scn.x, scn.y)); break;
	case SCN_DWELLEND:
		if (!cb_dwell_end.is_null()) cb_dwell_end.invoke(scn.position, Point(scn.x, scn.y)); break;
	case SCN_ZOOM:
		if (!cb_zoom.is_null()) cb_zoom.invoke(); break;
	case SCN_HOTSPOTCLICK:
		if (!cb_hotspot_click.is_null()) cb_hotspot_click.invoke(); break;
	case SCN_HOTSPOTDOUBLECLICK:
		if (!cb_hotspot_double_click.is_null()) cb_hotspot_double_click.invoke(); break;
	case SCN_HOTSPOTRELEASECLICK:
		if (!cb_hotspot_release_click.is_null()) cb_hotspot_release_click.invoke(); break;
	case SCN_INDICATORCLICK:
		if (!cb_indicator_click.is_null()) cb_indicator_click.invoke(); break;
	case SCN_INDICATORRELEASE:
		if (!cb_indicator_release.is_null()) cb_indicator_release.invoke(); break;
	case SCN_CALLTIPCLICK:
		if (!cb_calltip_click.is_null()) cb_calltip_click.invoke(); break;
	case SCN_AUTOCSELECTION:
		if (!cb_autocomplete_selection.is_null()) cb_autocomplete_selection.invoke(); break;
	case SCN_AUTOCCANCELLED:
		if (!cb_autocomplete_cancelled.is_null()) cb_autocomplete_cancelled.invoke(); break;
	case SCN_AUTOCCHARDELETED:
		if (!cb_autocomplete_char_deleted.is_null()) cb_autocomplete_char_deleted.invoke(); break;
	default:
		break;
	}
}

void ScintillaEditor_Impl::CopyToClipboard(const Scintilla::SelectionText &selectedText)
{
	component->get_display_window().set_clipboard_text(std::string(selectedText.s, selectedText.len));
}

void ScintillaEditor_Impl::SetTicking(bool on)
{
}

void ScintillaEditor_Impl::SetMouseCapture(bool on)
{
	is_mouse_captured = on;
	component->capture_mouse(on);
}

void ScintillaEditor_Impl::play_record(const std::string &str)
{
	std::string::size_type pos = str.find_first_of(':');
	unsigned int message;
	if (pos == std::string::npos)
		message = StringHelp::text_to_uint(str);
	else
		message = StringHelp::text_to_uint(str.substr(0, pos));

	switch (message)
	{
	case SCI_REPLACESEL:
		if (pos != std::string::npos)
			send_message(message, 0, reinterpret_cast<sptr_t>(str.c_str() + pos + 1));
		break;
	case SCI_ADDTEXT:
	case SCI_APPENDTEXT:
		if (pos != std::string::npos)
			send_message(message, str.length() - pos - 1, reinterpret_cast<sptr_t>(str.data() + pos + 1));
		break;
	case SCI_INSERTTEXT:
	case SCI_SEARCHNEXT:
	case SCI_SEARCHPREV:
		if (pos != std::string::npos)
		{
			std::string::size_type pos2 = str.find_first_not_of(':', pos + 1);
			if (pos2 != std::string::npos)
			{
				send_message(message, StringHelp::text_to_uint(str.substr(pos + 1, pos2 - pos - 1)), reinterpret_cast<sptr_t>(str.data() + pos2 + 1));
			}
		}
		break;
	case SCI_GOTOLINE:
	case SCI_GOTOPOS:
		send_message(message, StringHelp::text_to_uint(str.substr(pos + 1)), 0);
		break;

	case SCI_SEARCHANCHOR:
	case SCI_CLEARALL:
	case SCI_SELECTALL:
	case SCI_CUT:
	case SCI_COPY:
	case SCI_PASTE:
	case SCI_CLEAR:
	case SCI_LINEDOWN:
	case SCI_LINEDOWNEXTEND:
	case SCI_PARADOWN:
	case SCI_PARADOWNEXTEND:
	case SCI_LINEUP:
	case SCI_LINEUPEXTEND:
	case SCI_PARAUP:
	case SCI_PARAUPEXTEND:
	case SCI_CHARLEFT:
	case SCI_CHARLEFTEXTEND:
	case SCI_CHARRIGHT:
	case SCI_CHARRIGHTEXTEND:
	case SCI_WORDLEFT:
	case SCI_WORDLEFTEXTEND:
	case SCI_WORDRIGHT:
	case SCI_WORDRIGHTEXTEND:
	case SCI_WORDPARTLEFT:
	case SCI_WORDPARTLEFTEXTEND:
	case SCI_WORDPARTRIGHT:
	case SCI_WORDPARTRIGHTEXTEND:
	case SCI_WORDLEFTEND:
	case SCI_WORDLEFTENDEXTEND:
	case SCI_WORDRIGHTEND:
	case SCI_WORDRIGHTENDEXTEND:
	case SCI_HOME:
	case SCI_HOMEEXTEND:
	case SCI_LINEEND:
	case SCI_LINEENDEXTEND:
	case SCI_HOMEWRAP:
	case SCI_HOMEWRAPEXTEND:
	case SCI_LINEENDWRAP:
	case SCI_LINEENDWRAPEXTEND:
	case SCI_DOCUMENTSTART:
	case SCI_DOCUMENTSTARTEXTEND:
	case SCI_DOCUMENTEND:
	case SCI_DOCUMENTENDEXTEND:
	case SCI_STUTTEREDPAGEUP:
	case SCI_STUTTEREDPAGEUPEXTEND:
	case SCI_STUTTEREDPAGEDOWN:
	case SCI_STUTTEREDPAGEDOWNEXTEND:
	case SCI_PAGEUP:
	case SCI_PAGEUPEXTEND:
	case SCI_PAGEDOWN:
	case SCI_PAGEDOWNEXTEND:
	case SCI_EDITTOGGLEOVERTYPE:
	case SCI_CANCEL:
	case SCI_DELETEBACK:
	case SCI_TAB:
	case SCI_BACKTAB:
	case SCI_FORMFEED:
	case SCI_VCHOME:
	case SCI_VCHOMEEXTEND:
	case SCI_VCHOMEWRAP:
	case SCI_VCHOMEWRAPEXTEND:
	case SCI_DELWORDLEFT:
	case SCI_DELWORDRIGHT:
	case SCI_DELWORDRIGHTEND:
	case SCI_DELLINELEFT:
	case SCI_DELLINERIGHT:
	case SCI_LINECOPY:
	case SCI_LINECUT:
	case SCI_LINEDELETE:
	case SCI_LINETRANSPOSE:
	case SCI_LINEDUPLICATE:
	case SCI_LOWERCASE:
	case SCI_UPPERCASE:
	case SCI_LINESCROLLDOWN:
	case SCI_LINESCROLLUP:
	case SCI_DELETEBACKNOTLINE:
	case SCI_HOMEDISPLAY:
	case SCI_HOMEDISPLAYEXTEND:
	case SCI_LINEENDDISPLAY:
	case SCI_LINEENDDISPLAYEXTEND:
	case SCI_SETSELECTIONMODE:
	case SCI_LINEDOWNRECTEXTEND:
	case SCI_LINEUPRECTEXTEND:
	case SCI_CHARLEFTRECTEXTEND:
	case SCI_CHARRIGHTRECTEXTEND:
	case SCI_HOMERECTEXTEND:
	case SCI_VCHOMERECTEXTEND:
	case SCI_LINEENDRECTEXTEND:
	case SCI_PAGEUPRECTEXTEND:
	case SCI_PAGEDOWNRECTEXTEND:
	case SCI_SELECTIONDUPLICATE:
	case SCI_COPYALLOWLINE:
	case SCI_VERTICALCENTRECARET:
	case SCI_MOVESELECTEDLINESUP:
	case SCI_MOVESELECTEDLINESDOWN:
	case SCI_SCROLLTOSTART:
	case SCI_SCROLLTOEND:
		send_message(message, 0, 0);
	default:
		break;
	}
}

std::string ScintillaEditor_Impl::record_string(unsigned int message, uptr_t wparam, sptr_t lparam)
{
	switch (message)
	{
	case SCI_REPLACESEL:
		return StringHelp::uint_to_text(message) + ":" + reinterpret_cast<char*>(lparam);
	case SCI_ADDTEXT:
	case SCI_APPENDTEXT:
		return StringHelp::uint_to_text(message) + ":" + std::string(reinterpret_cast<char*>(lparam), wparam);
	case SCI_INSERTTEXT:
	case SCI_SEARCHNEXT:
	case SCI_SEARCHPREV:
		return StringHelp::uint_to_text(message) + ":" + StringHelp::uint_to_text(wparam) + ":" + reinterpret_cast<char*>(lparam);
	case SCI_GOTOLINE:
	case SCI_GOTOPOS:
		return StringHelp::uint_to_text(message) + ":" + StringHelp::uint_to_text(wparam);

	case SCI_SEARCHANCHOR:
	case SCI_CLEARALL:
	case SCI_SELECTALL:
	case SCI_CUT:
	case SCI_COPY:
	case SCI_PASTE:
	case SCI_CLEAR:
	case SCI_LINEDOWN:
	case SCI_LINEDOWNEXTEND:
	case SCI_PARADOWN:
	case SCI_PARADOWNEXTEND:
	case SCI_LINEUP:
	case SCI_LINEUPEXTEND:
	case SCI_PARAUP:
	case SCI_PARAUPEXTEND:
	case SCI_CHARLEFT:
	case SCI_CHARLEFTEXTEND:
	case SCI_CHARRIGHT:
	case SCI_CHARRIGHTEXTEND:
	case SCI_WORDLEFT:
	case SCI_WORDLEFTEXTEND:
	case SCI_WORDRIGHT:
	case SCI_WORDRIGHTEXTEND:
	case SCI_WORDPARTLEFT:
	case SCI_WORDPARTLEFTEXTEND:
	case SCI_WORDPARTRIGHT:
	case SCI_WORDPARTRIGHTEXTEND:
	case SCI_WORDLEFTEND:
	case SCI_WORDLEFTENDEXTEND:
	case SCI_WORDRIGHTEND:
	case SCI_WORDRIGHTENDEXTEND:
	case SCI_HOME:
	case SCI_HOMEEXTEND:
	case SCI_LINEEND:
	case SCI_LINEENDEXTEND:
	case SCI_HOMEWRAP:
	case SCI_HOMEWRAPEXTEND:
	case SCI_LINEENDWRAP:
	case SCI_LINEENDWRAPEXTEND:
	case SCI_DOCUMENTSTART:
	case SCI_DOCUMENTSTARTEXTEND:
	case SCI_DOCUMENTEND:
	case SCI_DOCUMENTENDEXTEND:
	case SCI_STUTTEREDPAGEUP:
	case SCI_STUTTEREDPAGEUPEXTEND:
	case SCI_STUTTEREDPAGEDOWN:
	case SCI_STUTTEREDPAGEDOWNEXTEND:
	case SCI_PAGEUP:
	case SCI_PAGEUPEXTEND:
	case SCI_PAGEDOWN:
	case SCI_PAGEDOWNEXTEND:
	case SCI_EDITTOGGLEOVERTYPE:
	case SCI_CANCEL:
	case SCI_DELETEBACK:
	case SCI_TAB:
	case SCI_BACKTAB:
	case SCI_FORMFEED:
	case SCI_VCHOME:
	case SCI_VCHOMEEXTEND:
	case SCI_VCHOMEWRAP:
	case SCI_VCHOMEWRAPEXTEND:
	case SCI_DELWORDLEFT:
	case SCI_DELWORDRIGHT:
	case SCI_DELWORDRIGHTEND:
	case SCI_DELLINELEFT:
	case SCI_DELLINERIGHT:
	case SCI_LINECOPY:
	case SCI_LINECUT:
	case SCI_LINEDELETE:
	case SCI_LINETRANSPOSE:
	case SCI_LINEDUPLICATE:
	case SCI_LOWERCASE:
	case SCI_UPPERCASE:
	case SCI_LINESCROLLDOWN:
	case SCI_LINESCROLLUP:
	case SCI_DELETEBACKNOTLINE:
	case SCI_HOMEDISPLAY:
	case SCI_HOMEDISPLAYEXTEND:
	case SCI_LINEENDDISPLAY:
	case SCI_LINEENDDISPLAYEXTEND:
	case SCI_SETSELECTIONMODE:
	case SCI_LINEDOWNRECTEXTEND:
	case SCI_LINEUPRECTEXTEND:
	case SCI_CHARLEFTRECTEXTEND:
	case SCI_CHARRIGHTRECTEXTEND:
	case SCI_HOMERECTEXTEND:
	case SCI_VCHOMERECTEXTEND:
	case SCI_LINEENDRECTEXTEND:
	case SCI_PAGEUPRECTEXTEND:
	case SCI_PAGEDOWNRECTEXTEND:
	case SCI_SELECTIONDUPLICATE:
	case SCI_COPYALLOWLINE:
	case SCI_VERTICALCENTRECARET:
	case SCI_MOVESELECTEDLINESUP:
	case SCI_MOVESELECTEDLINESDOWN:
	case SCI_SCROLLTOSTART:
	case SCI_SCROLLTOEND:
		return StringHelp::uint_to_text(message);

	default:
		// Unknown macro command.  This should not happen unless scintilla was upgraded to a new version where new commands were introduced.
		std::terminate();
		return std::string();
	}
}

void ScintillaEditor_Impl::on_render(Canvas &canvas, const Rect &update_box)
{
	component->set_cliprect(canvas, update_box);
	ScintillaSurface surface(this, canvas);
	Scintilla::PRectangle sci_update_box(update_box.left, update_box.top, update_box.right, update_box.bottom);
	Paint(&surface, sci_update_box);
	component->reset_cliprect(canvas);
}

void ScintillaEditor_Impl::on_resized()
{
	Rect box = component->get_size();
	component->request_repaint();
	Rect vert_box(box.right - scrollbar_vert->get_width(), box.top, box.right, box.bottom - scrollbar_horz->get_height());
	Rect horz_box(box.left, box.bottom - scrollbar_horz->get_height(), box.right - scrollbar_vert->get_width(), box.bottom);
	client_box = Rect(box.left, box.top, max(vert_box.left, box.left), max(horz_box.top, box.top));
	scrollbar_vert->set_geometry(vert_box);
	scrollbar_horz->set_geometry(horz_box);
	ChangeSize();
}

void ScintillaEditor_Impl::on_scrollbar_vert_scroll()
{
	ScrollTo(scrollbar_vert->get_position());
}

void ScintillaEditor_Impl::on_scrollbar_horz_scroll()
{
	HorizontalScrollTo(scrollbar_horz->get_position());
}

bool ScintillaEditor_Impl::on_input_pressed(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		component->set_focus(true);
		int cur_time = System::get_time(); // Not really the right way, but..
		ButtonDown(Scintilla::Point(e.mouse_pos.x, e.mouse_pos.y), cur_time, e.shift, e.ctrl, e.alt);
		return true;
	}
	else if (e.id == mouse_right)
	{
		component->set_focus(true);
		Scintilla::Point pt(e.mouse_pos.x, e.mouse_pos.y);
		if (!PointInSelection(pt))
		{
			CancelModes();
			SetEmptySelection(PositionFromLocation(pt));
		}
		return true;
	}
	else if (e.id == mouse_wheel_up)
	{
		if (e.ctrl)
			KeyCommand(SCI_ZOOMIN);
		else
			ScrollTo(topLine - 3);
		return true;
	}
	else if (e.id == mouse_wheel_down)
	{
		if (e.ctrl)
			KeyCommand(SCI_ZOOMOUT);
		else
			ScrollTo(topLine + 3);
		return true;
	}
	else if (e.id == keycode_a && e.ctrl && !e.alt)
	{
		component->select_all();
		return true;
	}
	else if (e.id == keycode_z && e.ctrl && !e.alt)
	{
		component->undo();
		return true;
	}
	else if (e.id == keycode_y && e.ctrl && !e.alt)
	{
		component->redo();
		return true;
	}
	else if (e.id == keycode_c && e.ctrl && !e.alt)
	{
		component->copy();
		return true;
	}
	else if (e.id == keycode_x && e.ctrl && !e.alt)
	{
		component->cut();
		return true;
	}
	else if (e.id == keycode_v && e.ctrl && !e.alt)
	{
		component->paste();
		return true;
	}
	else
	{
		int sci_command_key = 0;
		switch (e.id)
		{
		case keycode_down:       sci_command_key = SCK_DOWN; break;
		case keycode_up:         sci_command_key = SCK_UP; break;
		case keycode_left:       sci_command_key = SCK_LEFT; break;
		case keycode_right:      sci_command_key = SCK_RIGHT; break;
		case keycode_home:       sci_command_key = SCK_HOME; break;
		case keycode_end:        sci_command_key = SCK_END; break;
		case keycode_prior:      sci_command_key = SCK_PRIOR; break;
		case keycode_next:       sci_command_key = SCK_NEXT; break;
		case keycode_delete:     sci_command_key = SCK_DELETE; break;
		case keycode_insert:     sci_command_key = SCK_INSERT; break;
		case keycode_escape:     sci_command_key = SCK_ESCAPE; break;
		case keycode_backspace:  sci_command_key = SCK_BACK; break;
		case keycode_tab:        sci_command_key = SCK_TAB; break;
		case keycode_return:     sci_command_key = SCK_RETURN; break;
		case keycode_add:        sci_command_key = SCK_ADD; break;
		case keycode_subtract:   sci_command_key = SCK_SUBTRACT; break;
		case keycode_divide:     sci_command_key = SCK_DIVIDE; break;
		case keycode_lwin:       sci_command_key = SCK_WIN; break;
		case keycode_rwin:       sci_command_key = SCK_RWIN; break;
		case keycode_apps:       sci_command_key = SCK_MENU; break;
		default:
			if (!e.str.empty() && ((unsigned char)e.str[0]) >= 32)
			{
				AddCharUTF((char*)e.str.c_str(), e.str.length());
				return true;
			}
			else
			{
				return false;
			}
		}
		bool lastKeyDownConsumed = false;
		int ret = KeyDown(sci_command_key, e.shift, e.ctrl, e.alt, &lastKeyDownConsumed);
		return lastKeyDownConsumed || ret != 0;
	}
}

bool ScintillaEditor_Impl::on_input_released(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		int cur_time = System::get_time(); // Not really the right way, but..
		ButtonUp(Scintilla::Point(e.mouse_pos.x, e.mouse_pos.y), cur_time, e.ctrl);
		return true;
	}
	else if (e.id == mouse_right)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ScintillaEditor_Impl::on_input_doubleclick(const InputEvent &e)
{
	// To do: Seems scintilla does its own double-click calculations - we might have to simulate it.
	return on_input_pressed(e);
}

bool ScintillaEditor_Impl::on_input_pointer_moved(const InputEvent &e)
{
	ButtonMove(Scintilla::Point(e.mouse_pos.x, e.mouse_pos.y));
	// To do: track leave and call MouseLeave();
	return true;
}

bool ScintillaEditor_Impl::on_focus_gained()
{
	SetFocusState(true);
	return true;
}

bool ScintillaEditor_Impl::on_focus_lost()
{
	SetFocusState(false);
	return true;
}

bool ScintillaEditor_Impl::HaveMouseCapture()
{
	return is_mouse_captured;
}

Canvas ScintillaEditor_Impl::sci_get_canvas()
{
	return component->get_canvas();
}

void ScintillaEditor_Impl::sci_set_cliprect(Canvas &canvas, Rect box)
{
	return component->set_cliprect(canvas, box.clip(client_box));
}

void ScintillaEditor_Impl::Window_Destroy()
{
}

bool ScintillaEditor_Impl::Window_HasFocus()
{
	return component->has_focus();
}

Scintilla::PRectangle ScintillaEditor_Impl::Window_GetPosition()
{
	Rect geometry = component->get_geometry();
	return Scintilla::PRectangle(geometry.left, geometry.top, geometry.right, geometry.bottom);
}

void ScintillaEditor_Impl::Window_SetPosition(Scintilla::PRectangle rc)
{
	component->set_geometry(Rect(rc.left, rc.top, rc.right, rc.bottom));
}

void ScintillaEditor_Impl::Window_SetPositionRelative(Scintilla::PRectangle rc, ScintillaWindowHandle *relativeTo)
{
	Scintilla::PRectangle rect = relativeTo->Window_GetPosition();
	component->set_geometry(Rect(rc.left, rc.top, rc.right, rc.bottom).translate(rect.left, rect.top));
}

Scintilla::PRectangle ScintillaEditor_Impl::Window_GetClientPosition()
{
	Rect box = client_box;
	return Scintilla::PRectangle(box.left, box.top, box.right, box.bottom);
}

void ScintillaEditor_Impl::Window_Show(bool show)
{
	component->set_visible(show);
}

void ScintillaEditor_Impl::Window_InvalidateAll()
{
	component->request_repaint();
}

void ScintillaEditor_Impl::Window_InvalidateRectangle(Scintilla::PRectangle rc)
{
	component->request_repaint(Rect(rc.left, rc.top, rc.right, rc.bottom));
}

void ScintillaEditor_Impl::Window_SetFont(Scintilla::Font &font)
{
}

void ScintillaEditor_Impl::Window_SetCursor(Scintilla::Window::Cursor curs)
{
	switch (curs)
	{
	case Scintilla::Window::cursorInvalid:
		component->set_cursor(cursor_no);
		break;
	case Scintilla::Window::cursorText:
		component->set_cursor(cursor_ibeam);
		break;
	case Scintilla::Window::cursorUp:
		component->set_cursor(cursor_uparrow);
		break;
	case Scintilla::Window::cursorWait:
		component->set_cursor(cursor_wait);
		break;
	case Scintilla::Window::cursorHoriz:
		component->set_cursor(cursor_size_we);
		break;
	case Scintilla::Window::cursorVert:
		component->set_cursor(cursor_size_ns);
		break;
	case Scintilla::Window::cursorHand:
		component->set_cursor(cursor_hand);
		break;
	case Scintilla::Window::cursorReverseArrow:
		// To do: load our own cursor for this
	case Scintilla::Window::cursorArrow:
	default:
		component->set_cursor(cursor_arrow);
		break;
	}
}

void ScintillaEditor_Impl::Window_SetTitle(const char *s)
{
}

Scintilla::PRectangle ScintillaEditor_Impl::Window_GetMonitorRect(Scintilla::Point pt)
{
	ScreenInfo screen_info;
	int primary_screen_index = 0;
	std::vector<Rect> rects = screen_info.get_screen_geometries(primary_screen_index);
	return Scintilla::PRectangle(rects[primary_screen_index].left, rects[primary_screen_index].top, rects[primary_screen_index].right, rects[primary_screen_index].bottom);
}

/////////////////////////////////////////////////////////////////////////////
// Platform.h objects:

Scintilla::ColourDesired Scintilla::Platform::Chrome()
{
	return ScintillaEditor_Impl::to_int_color(Colorf::gray90);
}

Scintilla::ColourDesired Scintilla::Platform::ChromeHighlight()
{
	return ScintillaEditor_Impl::to_int_color(Colorf::whitesmoke);
}

const char *Scintilla::Platform::DefaultFont()
{
	return "Segoe UI";
}

int Scintilla::Platform::DefaultFontSize()
{
	return 10;
}

unsigned int Scintilla::Platform::DoubleClickTime()
{
#ifdef WIN32
	return GetDoubleClickTime();
#else
	return 400;
#endif
}

bool Scintilla::Platform::MouseButtonBounce()
{
	return false;
}

int Scintilla::Platform::Minimum(int a, int b)
{
	return min(a, b);
}

int Scintilla::Platform::Maximum(int a, int b)
{
	return max(a, b);
}

void Scintilla::Platform::DebugPrintf(const char *format, ...)
{
}

void Scintilla::Platform::Assert(const char *c, const char *file, int line)
{
	std::terminate();
}

int Scintilla::Platform::Clamp(int val, int minVal, int maxVal)
{
	return clamp(val, minVal, maxVal);
}

/*
void Platform::DebugDisplay(const char *s)
{
}

bool Platform::IsKeyDown(int key)
{
	return false;
}

long Platform::SendScintilla(WindowID w, unsigned int msg, unsigned long wParam, long lParam)
{
	return 0;
}

long Platform::SendScintillaPointer(WindowID w, unsigned int msg, unsigned long wParam, void *lParam)
{
	return 0;
}

bool Platform::IsDBCSLeadByte(int codePage, char ch)
{
	return false;
}

int Platform::DBCSCharLength(int codePage, const char *s)
{
	return 0;
}

int Platform::DBCSCharMaxLength()
{
	return 0;
}

bool Platform::ShowAssertionPopUps(bool assertionPopUps_)
{
	return false;
}
*/

/////////////////////////////////////////////////////////////////////////////

Scintilla::ElapsedTime::ElapsedTime()
{
}

/////////////////////////////////////////////////////////////////////////////

Scintilla::Point Scintilla::Point::FromLong(long lpoint)
{
	return Scintilla::Point(static_cast<short>(lpoint&255), static_cast<short>((lpoint>>8)&255));
}

/////////////////////////////////////////////////////////////////////////////

Scintilla::Palette::Palette()
{
	used = 0;
	allowRealization = false;
	hpal = 0;
	size = 100;
	entries = new ColourPair[size];
}

Scintilla::Palette::~Palette()
{
	Release();
	delete []entries;
	entries = 0;
}

void Scintilla::Palette::Release()
{
	used = 0;
	// if (hpal)
	//	::DeleteObject(hpal);
	hpal = 0;
	delete []entries;
	size = 100;
	entries = new ColourPair[size];
}

/**
 * This method either adds a colour to the list of wanted colours (want==true)
 * or retrieves the allocated colour back to the ColourPair.
 * This is one method to make it easier to keep the code for wanting and retrieving in sync.
 */
void Scintilla::Palette::WantFind(ColourPair &cp, bool want)
{
	if (want)
	{
		for (int i=0; i < used; i++)
		{
			if (entries[i].desired == cp.desired)
				return;
		}

		if (used >= size)
		{
			int sizeNew = size * 2;
			ColourPair *entriesNew = new ColourPair[sizeNew];
			for (int j=0; j<size; j++) {
				entriesNew[j] = entries[j];
			}
			delete []entries;
			entries = entriesNew;
			size = sizeNew;
		}

		entries[used].desired = cp.desired;
		entries[used].allocated.Set(cp.desired.AsLong());
		used++;
	}
	else
	{
		for (int i=0; i < used; i++)
		{
			if (entries[i].desired == cp.desired)
			{
				cp.allocated = entries[i].allocated;
				return;
			}
		}
		cp.allocated.Set(cp.desired.AsLong());
	}
}

void Scintilla::Palette::Allocate(Window &)
{
}

/////////////////////////////////////////////////////////////////////////////

Scintilla::Font::Font()
: fid(0)
{
}

Scintilla::Font::~Font()
{
}

void Scintilla::Font::Create(const char *faceName, int characterSet, int size, bool bold, bool italic, int extraFontFlag)
{
	SetID(new ScintillaFontHandle(faceName, characterSet, size, bold, italic, extraFontFlag));
}

void Scintilla::Font::Release()
{
	delete static_cast<ScintillaFontHandle*>(GetID());
}

/////////////////////////////////////////////////////////////////////////////

Scintilla::Menu::Menu()
: mid(0)
{
}

void Scintilla::Menu::CreatePopUp()
{
	//Destroy();
	//mid = ::CreatePopupMenu();
}

void Scintilla::Menu::Destroy()
{
	//if (mid)
	//	::DestroyMenu(reinterpret_cast<HMENU>(mid));
	//mid = 0;
}

void Scintilla::Menu::Show(Point pt, Window &w)
{
	/*::TrackPopupMenu(reinterpret_cast<HMENU>(mid),
		0, pt.x - 4, pt.y, 0,
		reinterpret_cast<HWND>(w.GetID()), NULL);*/
	Destroy();
}

/////////////////////////////////////////////////////////////////////////////

Scintilla::Window::~Window()
{
}

void Scintilla::Window::Destroy()
{
	delete reinterpret_cast<ScintillaWindowHandle*>(GetID());
}

bool Scintilla::Window::HasFocus()
{
	return reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_HasFocus();
}

Scintilla::PRectangle Scintilla::Window::GetPosition()
{
	return reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_GetPosition();
}

void Scintilla::Window::SetPosition(PRectangle rc)
{
	reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_SetPosition(rc);
}

void Scintilla::Window::SetPositionRelative(PRectangle rc, Window relativeTo)
{
	reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_SetPositionRelative(rc, reinterpret_cast<ScintillaWindowHandle*>(relativeTo.GetID()));
}

Scintilla::PRectangle Scintilla::Window::GetClientPosition()
{
	return reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_GetClientPosition();
}

void Scintilla::Window::Show(bool show)
{
	reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_Show(show);
}

void Scintilla::Window::InvalidateAll()
{
	reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_InvalidateAll();
}

void Scintilla::Window::InvalidateRectangle(PRectangle rc)
{
	reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_InvalidateRectangle(rc);
}

void Scintilla::Window::SetFont(Font &font)
{
	reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_SetFont(font);
}

void Scintilla::Window::SetCursor(Cursor curs)
{
	reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_SetCursor(curs);
}

void Scintilla::Window::SetTitle(const char *s)
{
	reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_SetTitle(s);
}

Scintilla::PRectangle Scintilla::Window::GetMonitorRect(Point pt)
{
	return reinterpret_cast<ScintillaWindowHandle*>(GetID())->Window_GetMonitorRect(pt);
}

/////////////////////////////////////////////////////////////////////////////

Scintilla::Surface *Scintilla::Surface::Allocate()
{
	return new ScintillaSurface();
}

ScintillaSurface::ScintillaSurface()
: component(0)
{
	pen_color = Color::black;
}

ScintillaSurface::ScintillaSurface(ScintillaWindowHandle *component, Canvas &canvas)
: component(component), canvas(canvas)
{
	pen_color = Color::black;
}

void ScintillaSurface::Init(Scintilla::WindowID wid)
{
	component = (ScintillaWindowHandle*)wid;
	canvas = component->sci_get_canvas();
}

void ScintillaSurface::Init(Scintilla::SurfaceID sid, Scintilla::WindowID wid)
{
	// We should not get any Inits from this function since we never give Scintilla any SurfaceID
	std::terminate();
}

void ScintillaSurface::InitPixMap(int width, int height, Surface *surface_, Scintilla::WindowID wid)
{
	// To do: fix this - scintilla is trying to render to an offscreen texture
	component = (ScintillaWindowHandle*)wid;
	canvas = static_cast<ScintillaSurface*>(surface_)->canvas;
}

void ScintillaSurface::Release()
{
}

bool ScintillaSurface::Initialised()
{
	return component != 0;
}

void ScintillaSurface::PenColour(Scintilla::ColourAllocated fore)
{
	unsigned int c = (unsigned int)fore.AsLong();
	pen_color = Color((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff, 255/*(c >> 24) & 0xff*/);
}

int ScintillaSurface::LogPixelsY()
{
	return 96;
}

int ScintillaSurface::DeviceHeightFont(int points)
{
	return (points * 96 + 36) / 72;
}

void ScintillaSurface::MoveTo(int x_, int y_)
{
	pen_pos = Point(x_, y_);
}

void ScintillaSurface::LineTo(int x_, int y_)
{
	canvas.draw_line((float)pen_pos.x, (float)pen_pos.y, (float)x_, (float)y_, (Colorf)pen_color);
}

void ScintillaSurface::Polygon(Scintilla::Point *pts, int npts, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back)
{
}

void ScintillaSurface::RectangleDraw(Scintilla::PRectangle rc, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back)
{
	unsigned int c = (unsigned int)fore.AsLong();
	Color fore_color((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff, 255/*(c >> 24) & 0xff*/);

	c = (unsigned int)back.AsLong();
	Color back_color((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff, 255/*(c >> 24) & 0xff*/);

	canvas.fill_rect((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom, (Colorf)back_color);
	canvas.draw_box((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom, (Colorf)fore_color);
}

void ScintillaSurface::FillRectangle(Scintilla::PRectangle rc, Scintilla::ColourAllocated back)
{
	unsigned int c = (unsigned int)back.AsLong();
	Color back_color((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff, 255/*(c >> 24) & 0xff*/);

	canvas.fill_rect((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom, (Colorf)back_color);
}

void ScintillaSurface::FillRectangle(Scintilla::PRectangle rc, Surface &surfacePattern)
{
}

void ScintillaSurface::RoundedRectangle(Scintilla::PRectangle rc, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back)
{
}

void ScintillaSurface::AlphaRectangle(Scintilla::PRectangle rc, int cornerSize, Scintilla::ColourAllocated fill, int alphaFill, Scintilla::ColourAllocated outline, int alphaOutline, int flags)
{
}

void ScintillaSurface::DrawRGBAImage(Scintilla::PRectangle rc, int width, int height, const unsigned char *pixelsImage)
{
}

void ScintillaSurface::Ellipse(Scintilla::PRectangle rc, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back)
{
}

void ScintillaSurface::Copy(Scintilla::PRectangle rc, Scintilla::Point from, Surface &surfaceSource)
{
}

void ScintillaSurface::DrawTextNoClip(Scintilla::PRectangle rc, Scintilla::Font &font_, int ybase, const char *s, int len, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back)
{
	unsigned int c = (unsigned int)fore.AsLong();
	Color fore_color((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff, 255/*(c >> 24) & 0xff*/);

	c = (unsigned int)back.AsLong();
	Color back_color((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff, 255/*(c >> 24) & 0xff*/);

	canvas.fill_rect((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom, (Colorf)back_color);

	Font &f = ScintillaFontHandle::from_handle(font_)->get_font(canvas);
	f.draw_text(canvas, (float)rc.left, (float)ybase, std::string(s, len), (Colorf)fore_color);
}

void ScintillaSurface::DrawTextClipped(Scintilla::PRectangle rc, Scintilla::Font &font_, int ybase, const char *s, int len, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back)
{
	Rect last_clip = canvas.get_cliprect();
	component->sci_set_cliprect(canvas, Rect(rc.left, rc.top, rc.right, rc.bottom));
	DrawTextNoClip(rc, font_, ybase, s, len, fore, back);
	canvas.set_cliprect(last_clip);
}

void ScintillaSurface::DrawTextTransparent(Scintilla::PRectangle rc, Scintilla::Font &font_, int ybase, const char *s, int len, Scintilla::ColourAllocated fore)
{
	unsigned int c = (unsigned int)fore.AsLong();
	Color fore_color((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff, 255/*(c >> 24) & 0xff*/);

	Font &f = ScintillaFontHandle::from_handle(font_)->get_font(canvas);
	f.draw_text(canvas, (float)rc.left, (float)ybase, std::string(s, len), (Colorf)fore_color);
}

void ScintillaSurface::MeasureWidths(Scintilla::Font &font_, const char *s, int len, int *positions)
{
	Font &f = ScintillaFontHandle::from_handle(font_)->get_font(canvas);
	int x = 0;
	int index = 0;
	UTF8_Reader reader(s, len);
	while (!reader.is_end())
	{
		unsigned int utf32 = reader.get_char();
		x += f.get_glyph_size(canvas, utf32).width;
		int char_len = reader.get_char_length();
		for (int i = 0; i < char_len; i++)
			positions[index++] = x;
		reader.next();
	}
}

int ScintillaSurface::WidthText(Scintilla::Font &font_, const char *s, int len)
{
	return ScintillaFontHandle::from_handle(font_)->get_font(canvas).get_text_size(canvas, std::string(s, len)).width;
}

int ScintillaSurface::WidthChar(Scintilla::Font &font_, char ch)
{
	unsigned int utf32 = (unsigned char)ch;
	return ScintillaFontHandle::from_handle(font_)->get_font(canvas).get_glyph_size(canvas, utf32).width;
}

int ScintillaSurface::Ascent(Scintilla::Font &font_)
{
	return (int)ScintillaFontHandle::from_handle(font_)->get_font(canvas).get_font_metrics().get_ascent();
}

int ScintillaSurface::Descent(Scintilla::Font &font_)
{
	return (int)ScintillaFontHandle::from_handle(font_)->get_font(canvas).get_font_metrics().get_descent();
}

int ScintillaSurface::InternalLeading(Scintilla::Font &font_)
{
	return (int)ScintillaFontHandle::from_handle(font_)->get_font(canvas).get_font_metrics().get_internal_leading();
}

int ScintillaSurface::ExternalLeading(Scintilla::Font &font_)
{
	return (int)ScintillaFontHandle::from_handle(font_)->get_font(canvas).get_font_metrics().get_external_leading();
}

int ScintillaSurface::Height(Scintilla::Font &font_)
{
	return (int)ScintillaFontHandle::from_handle(font_)->get_font(canvas).get_font_metrics().get_height();
}

int ScintillaSurface::AverageCharWidth(Scintilla::Font &font_)
{
	return (int)ScintillaFontHandle::from_handle(font_)->get_font(canvas).get_font_metrics().get_average_character_width();
}

int ScintillaSurface::SetPalette(Scintilla::Palette *pal, bool inBackGround)
{
	return 0;
}

void ScintillaSurface::SetClip(Scintilla::PRectangle rc)
{
	component->sci_set_cliprect(canvas, Rect(rc.left, rc.top, rc.right, rc.bottom));
}

void ScintillaSurface::FlushCachedState()
{
}

void ScintillaSurface::SetUnicodeMode(bool unicodeMode_)
{
}

void ScintillaSurface::SetDBCSMode(int codePage)
{
}

/////////////////////////////////////////////////////////////////////////////

Scintilla::ListBox *Scintilla::ListBox::Allocate()
{
	return new ScintillaListBox();
}

Scintilla::ListBox::ListBox()
{
}

Scintilla::ListBox::~ListBox()
{
}

void ScintillaListBox::SetFont(Scintilla::Font &font)
{
}

void ScintillaListBox::Create(Window &parent, int ctrlID, Scintilla::Point location, int lineHeight_, bool unicodeMode_)
{
	Window::operator=(this);
}

void ScintillaListBox::SetAverageCharWidth(int width)
{
}

void ScintillaListBox::SetVisibleRows(int rows)
{
}

int ScintillaListBox::GetVisibleRows() const
{
	return 0;
}

Scintilla::PRectangle ScintillaListBox::GetDesiredRect()
{
	return Scintilla::PRectangle();
}

int ScintillaListBox::CaretFromEdge()
{
	return 0;
}

void ScintillaListBox::Clear()
{
}

void ScintillaListBox::Append(char *s, int type)
{
}

int ScintillaListBox::Length()
{
	return 0;
}

void ScintillaListBox::Select(int n)
{
}

int ScintillaListBox::GetSelection()
{
	return 0;
}

int ScintillaListBox::Find(const char *prefix)
{
	return 0;
}

void ScintillaListBox::GetValue(int n, char *value, int len)
{
}

void ScintillaListBox::RegisterImage(int type, const char *xpm_data)
{
}

void ScintillaListBox::RegisterRGBAImage(int type, int width, int height, const unsigned char *pixelsImage)
{
}

void ScintillaListBox::ClearRegisteredImages()
{
}

void ScintillaListBox::SetDoubleClickAction(Scintilla::CallBackAction, void *)
{
}

void ScintillaListBox::SetList(const char* list, char separator, char typesep)
{
}

}


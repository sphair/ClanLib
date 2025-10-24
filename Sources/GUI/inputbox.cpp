/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "API/Core/XML/dom_element.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/System/clanstring.h"
#include "inputbox_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_InputBox::CL_InputBox(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_InputBox_Generic(this, std::string(), false, false, 0);
	get_style_manager()->connect_styles("inputbox", this);
	find_preferred_size();
}

CL_InputBox::CL_InputBox(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_InputBox_Generic(this, text, false, false, 0);
	get_style_manager()->connect_styles("inputbox", this);
	find_preferred_size();
}

CL_InputBox::CL_InputBox(
	const CL_Rect &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_InputBox_Generic(this, text, false, false, 0);
	get_style_manager()->connect_styles("inputbox", this);
}

CL_InputBox::CL_InputBox(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_InputBox_Generic(this, "", false, false, 0);
	get_style_manager()->connect_styles("inputbox", this);
}

CL_InputBox::~CL_InputBox()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_InputBox::get_length() const
{
	return impl->text.size();
}

const std::string &CL_InputBox::get_text() const
{
	return impl->text;
}

std::string CL_InputBox::get_marked_text() const
{
	return impl->get_marked_text();
}

bool CL_InputBox::has_marked_text() const
{
	return (get_selection_length() > 0);
}

int CL_InputBox::get_selection_start() const
{
	return impl->get_selection_start();
}

int CL_InputBox::get_selection_length() const
{
	return impl->get_selection_length();
}

int CL_InputBox::get_max_length() const
{
	return impl->max_length;
}

bool CL_InputBox::in_password_mode() const
{
	return impl->password_mode;
}

bool CL_InputBox::is_read_only() const
{
	return impl->read_only;
}

int CL_InputBox::get_cursor_position() const
{
	return impl->cursor_position;
}

bool CL_InputBox::is_edited() const
{
	return impl->edited;
}

/////////////////////////////////////////////////////////////////////////////
// Added by AlexeyKharkov:

void CL_InputBox::on_input_down(const CL_InputEvent &key)
{
	impl->on_input_down(key);
}
void CL_InputBox::on_input_up(const CL_InputEvent &key)
{
	impl->on_input_up(key);
}

void CL_InputBox::on_mouse_Ldown( int mouse_pos )
{
	impl->on_mouse_Ldown( mouse_pos );
}
void CL_InputBox::on_mouse_Lup( int mouse_pos )
{
	impl->on_mouse_Lup( mouse_pos );
}
void CL_InputBox::on_mouse_move( int mouse_pos )
{
	impl->on_mouse_move( mouse_pos );
}

void CL_InputBox::on_lost_focus()
{
	impl->on_lost_focus();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_InputBox::set_text(const std::string &text)
{
	impl->set_text(text);
}

void CL_InputBox::set_text(int number)
{
	impl->set_text(CL_String::format("%1", number));
}

void CL_InputBox::set_text(double number)
{
	impl->set_text(CL_String::format("%1", number));
}

void CL_InputBox::set_max_length(int length)
{
	impl->set_max_length(length);
}

void CL_InputBox::set_password_mode(bool enable)
{
	impl->password_mode = enable;
}

void CL_InputBox::set_read_only(bool enable)
{
	impl->read_only = enable;
}

void CL_InputBox::select_all()
{
	impl->select_all();
}

void CL_InputBox::deselect()
{
	impl->deselect();
}

void CL_InputBox::set_selection(int start, int length)
{
	impl->set_selection(start, length);
}

void CL_InputBox::set_cursor_position(int pos)
{
	impl->set_cursor_position(pos);
}

void CL_InputBox::clear()
{
	impl->set_text("");
}

void CL_InputBox::backspace()
{
	impl->backspace();
}

void CL_InputBox::del()
{
	impl->del();
}

void CL_InputBox::cut()
{
	impl->cut();
}

void CL_InputBox::move_cursor(int delta, bool mark)
{
	impl->move_cursor(delta, mark);
}

void CL_InputBox::move_cursor_word(int delta, bool mark)
{
	impl->move_cursor_word(delta, mark);
}

void CL_InputBox::home(bool mark)
{
	impl->home(mark);
}

void CL_InputBox::end(bool mark)
{
	impl->end(mark);
}

void CL_InputBox::set_edited(bool on)
{
	impl->edited = on;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const std::string &> &CL_InputBox::sig_changed()
{
	return impl->sig_changed;
}

CL_Signal_v0 &CL_InputBox::sig_return_pressed()
{
	return impl->sig_return_pressed;
}

CL_Signal_v0 &CL_InputBox::sig_activity()
{
	return impl->sig_activity;
}

CL_Signal_v2<char &, bool &> &CL_InputBox::sig_validate_character()
{
	return impl->sig_validate_character;
}

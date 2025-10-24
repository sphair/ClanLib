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
#include "API/GUI/button.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "button_generic.h"
#include "component_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Button::CL_Button(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, std::string(), false);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::CL_Button(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, text, false);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::CL_Button(
	const CL_Rect &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, text, false);
	get_style_manager()->connect_styles("button", this);
}

CL_Button::CL_Button(
	const CL_Point &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y, 0, 0), parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, text, false);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::CL_Button(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, std::string(), false);
	get_style_manager()->connect_styles("button", this);
}

CL_Button::CL_Button(
	const CL_Point &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y, 0, 0), parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, std::string(), false);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::~CL_Button()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_Button::get_text() const
{
	return impl->text;
}

bool CL_Button::is_toggle_button() const
{
	return impl->toggle_mode;
}

bool CL_Button::is_down() const
{
	return impl->toggled;
}

bool CL_Button::is_drawn_down() const
{
	return impl->visually_down;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Button::set_text(const std::string &text)
{
	impl->text = text;
}

void CL_Button::set_toggle_mode(bool toggle)
{
	impl->toggle_mode = toggle;
}

void CL_Button::set_down(bool enable)
{
	impl->toggled = enable;
	impl->visually_down = enable;
}

void CL_Button::toggle()
{
	impl->toggled = !impl->toggled;
	impl->visually_down = impl->toggled;
	impl->sig_toggled(impl->toggled);
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<bool> &CL_Button::sig_toggled()
{
	return impl->sig_toggled;
}

CL_Signal_v0 &CL_Button::sig_clicked()
{
	return impl->sig_clicked;
}

CL_Signal_v0 &CL_Button::sig_pressed()
{
	return impl->sig_pressed;
}

CL_Signal_v0 &CL_Button::sig_released()
{
	return impl->sig_released;
}

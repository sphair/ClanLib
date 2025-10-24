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
#include "API/GUI/window.h"
#include "API/GUI/stylemanager.h"
#include "API/GUI/gui_manager.h"
#include "API/Core/XML/dom_element.h"
#include "window_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction

CL_Window::CL_Window(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, std::string(), no_buttons);
	get_style_manager()->connect_styles("window", this);
	find_preferred_size();
}

CL_Window::CL_Window(
	int button_enum, 
	CL_Component *parent, 
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, std::string(), button_enum);
	get_style_manager()->connect_styles("window", this);
	find_preferred_size();
}

CL_Window::CL_Window(
	const CL_Rect &pos,
	const std::string &title,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, title, no_buttons);
	get_style_manager()->connect_styles("window", this);
}

CL_Window::CL_Window(
	const CL_Rect &pos,
	const std::string &title,
	int button_enum, 
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, title, button_enum);
	get_style_manager()->connect_styles("window", this);
}

CL_Window::CL_Window(
	const std::string &title,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, title, no_buttons);
	get_style_manager()->connect_styles("window", this);
	find_preferred_size();
}

CL_Window::CL_Window(
	const std::string &title,
	int button_enum, 
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, title, button_enum);
	get_style_manager()->connect_styles("window", this);
	find_preferred_size();
}

CL_Window::~CL_Window()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_Window::get_title() const
{
	return impl->title;
}

CL_Window::TitlebarButtons CL_Window::get_titlebar_buttons_enum() const
{
	return (CL_Window::TitlebarButtons)impl->titlebar_buttons;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Window::set_title(const std::string &text)
{
	impl->title = text;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v0 &CL_Window::sig_close_button_clicked()
{
	return impl->sig_close_button_clicked;
}

CL_Signal_v0 &CL_Window::sig_minimize_button_clicked()
{
	return impl->sig_minimize_button_clicked;
}

CL_Signal_v0 &CL_Window::sig_maximize_button_clicked()
{
	return impl->sig_maximize_button_clicked;
}

CL_Signal_v0 &CL_Window::sig_help_button_clicked()
{
	return impl->sig_help_button_clicked;
}

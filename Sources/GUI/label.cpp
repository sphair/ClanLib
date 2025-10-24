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
#include "API/GUI/label.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "label_generic.h"
#include "component_generic.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Label::CL_Label(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	init("");
	find_preferred_size();
}

CL_Label::CL_Label(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	init(text);
	find_preferred_size();
}

CL_Label::CL_Label(
	const CL_Point &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y, 0, 0), parent, style), impl(NULL)
{
	init(text);
	find_preferred_size();
}

CL_Label::CL_Label(
	const CL_Rect &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	init(text);
}

void CL_Label::init(const std::string &text)
{
	impl = new CL_Label_Generic(this, text);
	get_style_manager()->connect_styles("label", this);
	set_focusable(false);
}

CL_Label::~CL_Label()
{
	if (impl->font == NULL)
		delete impl->font;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_Label::get_text() const
{
	return impl->text;
}

CL_Origin CL_Label::get_alignment() const
{
	return impl->alignment;
}

CL_Font* CL_Label::get_font()
{
	return impl->font;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Label::set_text(const std::string &text)
{
	impl->text = text;
}

void CL_Label::set_text(int number)
{
	impl->text = CL_String::from_int(number);
}

void CL_Label::set_text(double number)
{
	impl->text = CL_String::from_double(number);
}

void CL_Label::clear()
{
	impl->text = "";
}

void CL_Label::set_alignment(CL_Origin alignment)
{
	impl->alignment = alignment;
}

void CL_Label::set_font(const CL_Font& f)
{
	impl->font = new CL_Font(f);
	impl->sig_font_change();
}

/////////////////////////////////////////////////////////////////////////////
// Signals:
CL_Signal_v0 &CL_Label::sig_font_change()
{
	return impl->sig_font_change;
}

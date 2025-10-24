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
#include "API/GUI/menu_item.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "menu_item_generic.h"
#include "component_generic.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_MenuItem::CL_MenuItem(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_MenuItem_Generic(this, text);
	get_style_manager()->connect_styles("menu_item", this);

	set_focusable(false);

	find_preferred_size();
}

CL_MenuItem::~CL_MenuItem()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_MenuItem::get_text() const
{
	return impl->text;
}

bool CL_MenuItem::is_toggling() const
{
	return impl->toggling;
}

bool CL_MenuItem::is_selected() const
{
	return impl->selected;
}

bool CL_MenuItem::use_icon() const
{
	return impl->use_icon;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_MenuItem::set_toggling(bool toggle)
{
	impl->toggling = toggle;
}

void CL_MenuItem::set_use_icon(bool use_icon)
{
	impl->use_icon = use_icon;
}

void CL_MenuItem::set_selected(bool selected)
{
	impl->selected = selected;
}

void CL_MenuItem::set_text(const std::string &text)
{
	impl->text = text;
}

void CL_MenuItem::set_text(int number)
{
	impl->text = CL_String::from_int(number);
}

void CL_MenuItem::set_text(double number)
{
	impl->text = CL_String::from_double(number);
}

void CL_MenuItem::clear()
{
	impl->text = "";
}

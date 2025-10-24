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
#include "API/GUI/radiobutton.h"
#include "API/GUI/component.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RadioButton::CL_RadioButton(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("radiobutton", this);
	find_preferred_size();
}

CL_RadioButton::CL_RadioButton(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(text, parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("radiobutton", this);
	find_preferred_size();
}

CL_RadioButton::CL_RadioButton(
	const CL_Point &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(pos, text, parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("radiobutton", this);
	find_preferred_size();
}

CL_RadioButton::~CL_RadioButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_RadioButton::is_checked() const
{
	return is_down();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_RadioButton::set_checked(bool check)
{
	set_down(check);
}

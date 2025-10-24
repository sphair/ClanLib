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
#include "menu_item_generic.h"
#include "API/Core/XML/dom_element.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_MenuItem_Generic::CL_MenuItem_Generic(
	CL_MenuItem *self,
	const std::string &_text)
:
	toggling(false),
	selected(false),
	use_icon(true),
	menu_item(self)
{
	text = _text;

	slots.connect( menu_item->sig_set_options(), this, &CL_MenuItem_Generic::on_set_options);
	slots.connect( menu_item->sig_mouse_down(), this, &CL_MenuItem_Generic::on_mouse_down);
}

void CL_MenuItem_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("text"))
		text = options.get_attribute("text");
}

void CL_MenuItem_Generic::on_mouse_down(const CL_InputEvent &event)
{
	if( toggling && menu_item->get_parent()->is_enabled(false) )
		selected = !selected;
}

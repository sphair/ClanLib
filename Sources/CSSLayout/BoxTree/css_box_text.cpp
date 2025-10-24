/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "CSSLayout/precomp.h"
#include "css_box_text.h"
#include "css_box_element.h"

void CL_CSSBoxText::set_text(const CL_String &new_text)
{
	text = new_text;
	processed_text = text;
	processed_selection_start = selection_start;
	processed_selection_end = selection_end;
}

const CL_CSSBoxElement *CL_CSSBoxText::get_parent_element() const
{
	const CL_CSSBoxNode *cur = this;
	cur = cur->get_parent();
	while (cur)
	{
		const CL_CSSBoxElement *element = dynamic_cast<const CL_CSSBoxElement*>(cur);
		if (element)
			return element;
		cur = cur->get_parent();
	}
	throw CL_Exception("CSSBoxText node has no element parent!");
}

const CL_CSSBoxProperties &CL_CSSBoxText::get_properties() const
{
	return get_parent_element()->computed_properties;
}

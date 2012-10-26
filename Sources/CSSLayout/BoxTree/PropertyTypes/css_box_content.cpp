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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/PropertyTypes/css_box_content.h"

namespace clan
{

CSSBoxContent::CSSBoxContent()
: type(type_normal)
{
}

void CSSBoxContent::compute(const CSSBoxContent *parent, CSSResourceCache *layout, float em_size, float ex_size, bool before_or_after_pseudo_element)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			str = parent->str;
		}
		else
		{
			type = type_normal;
		}
	}
/*
	if (before_or_after_pseudo_element)
	{
		if (type == type_normal)
			type = type_none;

		// To do: For URI values, the absolute URI; for attr() values, the resulting string.
	}
	else
	{
		type = type_normal;
	}
*/
}

std::string CSSBoxContent::to_string() const
{
	switch (type)
	{
	default:
	case type_normal:
		return "normal";
	case type_none:
		return "none";
	case type_string:
		return "string";
	case type_uri:
		return "uri";
	case type_counter:
		return "counter";
	case type_attr:
		return "attr";
	case type_open_quote:
		return "open-quote";
	case type_close_quote:
		return "close-quote";
	case type_no_open_quote:
		return "no-open-quote";
	case type_no_close_quote:
		return "no-close-quote";
	case type_inherit:
		return "inherit";
	}
}

}

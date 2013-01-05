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
#include "API/CSSLayout/PropertyValues/css_value_quotes.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueQuotes::CSSValueQuotes()
: type(type_inherit)
{
}

void CSSValueQuotes::compute(const CSSValueQuotes *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			strings = parent->strings;
		}
		else
		{
			type = type_strings;
			strings = layout->get_default_quotes();
		}
	}
}

std::string CSSValueQuotes::to_string() const
{
	switch (type)
	{
	default:
	case type_strings:
		{
			std::string s;
			for (size_t i = 0; i < strings.size(); i++)
			{
				if (i > 0)
					s += ", ";
				s += string_format("\"%1\"", strings[i]);
			}
			return s;
		}
	case type_none:
		return "none";
	case type_inherit:
		return "inherit";
	}
}

}

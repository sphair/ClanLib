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
#include "API/CSSLayout/PropertyTypes/css_box_font_weight.h"

namespace clan
{

CSSBoxFontWeight::CSSBoxFontWeight()
: type(type_inherit)
{
}

void CSSBoxFontWeight::compute(const CSSBoxFontWeight *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
			type = parent->type;
		else
			type = type_normal;
	}

	// CSS 2.1 does not specify how the computed value of font-weight is represented internally or externally. 
}

std::string CSSBoxFontWeight::to_string() const
{
	switch (type)
	{
	default:
	case type_normal:
		return "normal";
	case type_bold:
		return "bold";
	case type_bolder:
		return "bolder";
	case type_lighter:
		return "lighter";
	case type_100:
		return "100";
	case type_200:
		return "200";
	case type_300:
		return "300";
	case type_400:
		return "400";
	case type_500:
		return "500";
	case type_600:
		return "600";
	case type_700:
		return "700";
	case type_800:
		return "800";
	case type_900:
		return "900";
	case type_inherit:
		return "inherit";
	}
}

}

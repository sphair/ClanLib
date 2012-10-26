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
#include "API/CSSLayout/PropertyTypes/css_box_margin_width.h"
#include "../../css_resource_cache.h"

namespace clan
{

CSSBoxMarginWidth::CSSBoxMarginWidth()
: type(type_length), length(0, CSSBoxLength::type_px), percentage(0.0f)
{
}

void CSSBoxMarginWidth::compute(const CSSBoxMarginWidth *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			length = parent->length;
			percentage = parent->percentage;
		}
		else
		{
			type = type_length;
			length = CSSBoxLength(0.0f, CSSBoxLength::type_px);
		}
	}

	if (type == type_length)
		length = layout->compute_length(length, em_size, ex_size);
}

std::string CSSBoxMarginWidth::to_string() const
{
	switch (type)
	{
	default:
	case type_length:
		return length.to_string();
	case type_percentage:
		return StringHelp::float_to_text(percentage) + "%";
	case type_auto:
		return "auto";
	case type_inherit:
		return "inherit";
	}
}

}

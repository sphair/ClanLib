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
#include "API/CSSLayout/PropertyTypes/css_box_border_width.h"
#include "API/CSSLayout/PropertyTypes/css_box_border_style.h"
#include "../../css_resource_cache.h"

CL_CSSBoxBorderWidth::CL_CSSBoxBorderWidth()
: type(type_medium)
{
}

void CL_CSSBoxBorderWidth::compute(const CL_CSSBoxBorderWidth *parent, CL_CSSResourceCache *layout, float em_size, float ex_size, const CL_CSSBoxBorderStyle &style)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			length = parent->length;
		}
		else
		{
			type = type_medium;
		}
	}

	switch (type)
	{
	case type_thin:
		type = type_length;
		length = CL_CSSBoxLength(1.0f, CL_CSSBoxLength::type_px);
		break;

	case type_medium:
		type = type_length;
		length = CL_CSSBoxLength(2.0f, CL_CSSBoxLength::type_px);
		break;

	case type_thick:
		type = type_length;
		length = CL_CSSBoxLength(3.0f, CL_CSSBoxLength::type_px);
		break;
	default:
		break;
	}

	if (style.type == CL_CSSBoxBorderStyle::type_hidden || style.type == CL_CSSBoxBorderStyle::type_none)
	{
		type = type_length;
		length = CL_CSSBoxLength(0.0f, CL_CSSBoxLength::type_px);
	}

	if (type == type_length)
	{
		length = layout->compute_length(length, em_size, ex_size);
	}
}

CL_String CL_CSSBoxBorderWidth::to_string() const
{
	switch (type)
	{
	default:
	case type_thin:
		return "thin";
	case type_medium:
		return "medium";
	case type_thick:
		return "thick";
	case type_length:
		return length.to_string();
	case type_inherit:
		return "inherit";
	}
}
/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "css_box_background_position.h"
#include "../../css_resource_cache.h"

CL_CSSBoxBackgroundPosition::CL_CSSBoxBackgroundPosition()
: type(type_value), type_x(type1_percentage), type_y(type2_percentage), percentage_x(0.0f), percentage_y(0.0f)
{
}

void CL_CSSBoxBackgroundPosition::compute(const CL_CSSBoxBackgroundPosition *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			type_x = parent->type_x;
			type_y = parent->type_y;
			length_x = parent->length_x;
			length_y = parent->length_y;
			percentage_x = parent->percentage_x;
			percentage_y = parent->percentage_y;
		}
		else
		{
			type = type_value;
			type_x = type1_percentage;
			percentage_x = 0.0f;
			type_y = type2_percentage;
			percentage_y = 0.0f;
		}
	}

	switch (type_x)
	{
	case type1_left:
		type_x = type1_percentage;
		percentage_x = 0.0f;
		break;

	case type1_center:
		type_x = type1_percentage;
		percentage_x = 50.0f;
		break;

	case type1_right:
		type_x = type1_percentage;
		percentage_x = 100.0f;
		break;

	case type1_length:
		length_x = layout->compute_length(length_x, em_size, ex_size);
		break;
	}

	switch (type_y)
	{
	case type2_top:
		type_y = type2_percentage;
		percentage_y = 0.0f;
		break;

	case type2_center:
		type_y = type2_percentage;
		percentage_y = 50.0f;
		break;

	case type2_bottom:
		type_y = type2_percentage;
		percentage_y = 100.0f;
		break;

	case type1_length:
		length_y = layout->compute_length(length_y, em_size, ex_size);
		break;
	}
}

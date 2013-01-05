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
#include "API/CSSLayout/PropertyTypes/css_box_background_position.h"
#include "../../css_resource_cache.h"

namespace clan
{

CSSBoxBackgroundPosition::CSSBoxBackgroundPosition()
: type(type_value)
{
	positions.push_back(Position());
}

void CSSBoxBackgroundPosition::compute(const CSSBoxBackgroundPosition *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			positions = parent->positions;
		}
		else
		{
			type = type_value;
			positions.clear();
			positions.push_back(Position());
		}
	}

	for (size_t i = 0; i < positions.size(); i++)
	{
		switch (positions[i].type_x)
		{
		case type1_left:
			positions[i].type_x = type1_percentage;
			positions[i].percentage_x = 0.0f;
			break;

		case type1_center:
			positions[i].type_x = type1_percentage;
			positions[i].percentage_x = 50.0f;
			break;

		case type1_right:
			positions[i].type_x = type1_percentage;
			positions[i].percentage_x = 100.0f;
			break;

		case type1_length:
			positions[i].length_x = layout->compute_length(positions[i].length_x, em_size, ex_size);
			break;
		}

		switch (positions[i].type_y)
		{
		case type2_top:
			positions[i].type_y = type2_percentage;
			positions[i].percentage_y = 0.0f;
			break;

		case type2_center:
			positions[i].type_y = type2_percentage;
			positions[i].percentage_y = 50.0f;
			break;

		case type2_bottom:
			positions[i].type_y = type2_percentage;
			positions[i].percentage_y = 100.0f;
			break;

		case type1_length:
			positions[i].length_y = layout->compute_length(positions[i].length_y, em_size, ex_size);
			break;
		}
	}
}

std::string CSSBoxBackgroundPosition::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	std::string s;
	for (size_t i = 0; i < positions.size(); i++)
	{
		if (i > 0)
			s += ", ";
		switch (positions[i].type_x)
		{
		case type1_left:
			s += "left";
			break;
		case type1_center:
			s += "center";
			break;
		case type1_right:
			s += "right";
			break;
		case type1_percentage:
			s += StringHelp::float_to_text(positions[i].percentage_x) + "%";
			break;
		case type1_length:
			s += positions[i].length_x.to_string();
			break;
		}
		s += " ";
		switch (positions[i].type_y)
		{
		case type2_top:
			s += "top";
			break;
		case type2_center:
			s += "center";
			break;
		case type2_bottom:
			s += "bottom";
			break;
		case type2_percentage:
			s += StringHelp::float_to_text(positions[i].percentage_y) + "%";
			break;
		case type2_length:
			s += positions[i].length_y.to_string();
			break;
		}
	}
	return s;
}

}

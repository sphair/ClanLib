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

#pragma once

#include "../css_box_property.h"

class CL_CSSBoxBorderImageSlice : public CL_CSSBoxProperty
{
public:
	CL_CSSBoxBorderImageSlice();
	void compute(const CL_CSSBoxBorderImageSlice *parent, CL_CSSResourceCache *layout, float em_size, float ex_size);
	CL_String to_string() const;

	enum Type
	{
		type_values,
		type_inherit
	};
	enum ValueType
	{
		value_type_number,
		value_type_percentage
	};
	Type type;
	ValueType value_top;
	ValueType value_right;
	ValueType value_bottom;
	ValueType value_left;
	float number_top;
	float number_right;
	float number_bottom;
	float number_left;
	float percentage_top;
	float percentage_right;
	float percentage_bottom;
	float percentage_left;
	bool fill_center;
};

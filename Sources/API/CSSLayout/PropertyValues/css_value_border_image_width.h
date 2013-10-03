/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "../CSSDocument/css_property_value.h"
#include "../CSSDocument/css_length.h"

namespace clan
{
/// \addtogroup clanCSSLayout_PropertyValues clanCSSLayout Property Values
/// \{

class CSSValueBorderImageWidth : public CSSPropertyValue
{
public:
	CSSValueBorderImageWidth();
	void compute(const CSSValueBorderImageWidth *parent, CSSResourceCache *layout, float em_size, float ex_size);
	std::string to_string() const;
	void apply(CSSComputedValuesUpdater *updater);
	std::string get_name() const;
	std::unique_ptr<CSSPropertyValue> clone() const;

	enum Type
	{
		type_values,
		type_inherit
	};
	enum ValueType
	{
		value_type_length,
		value_type_number,
		value_type_percentage,
		value_type_auto
	};
	Type type;
	ValueType value_top;
	ValueType value_right;
	ValueType value_bottom;
	ValueType value_left;
	CSSLength length_top;
	CSSLength length_right;
	CSSLength length_bottom;
	CSSLength length_left;
	float number_top;
	float number_right;
	float number_bottom;
	float number_left;
	float percentage_top;
	float percentage_right;
	float percentage_bottom;
	float percentage_left;
};

/// \}
}

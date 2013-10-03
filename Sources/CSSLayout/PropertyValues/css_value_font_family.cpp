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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/PropertyValues/css_value_font_family.h"
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueFontFamily::CSSValueFontFamily()
: type(type_inherit)
{
}
std::string CSSValueFontFamily::get_name() const
{
	return "font-family";
}
std::unique_ptr<CSSPropertyValue> CSSValueFontFamily::clone() const
{
	return std::unique_ptr<CSSPropertyValue>(new CSSValueFontFamily (*this));
}


void CSSValueFontFamily::apply(CSSComputedValuesUpdater *updater)
{
	updater->get_font().font_family = *this;
}

void CSSValueFontFamily::compute(const CSSValueFontFamily *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			names = parent->names;
		}
		else
		{
			type = type_names;
			names.clear();
			names.push_back(CSSValueFontFamilyName());
		}
	}
}

std::string CSSValueFontFamily::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	std::string s;
	for (size_t i = 0; i < names.size(); i++)
	{
		if (i > 0)
			s += ", ";
		switch (names[i].type)
		{
		default:
		case CSSValueFontFamilyName::type_family_name:
			s += string_format("\"%1\"", names[i].name);
			break;
		case CSSValueFontFamilyName::type_serif:
			s += "serif";
			break;
		case CSSValueFontFamilyName::type_sans_serif:
			s += "sans-serif";
			break;
		case CSSValueFontFamilyName::type_cursive:
			s += "cursive";
			break;
		case CSSValueFontFamilyName::type_fantasy:
			s += "fantasy";
			break;
		case CSSValueFontFamilyName::type_monospace:
			s += "monospace";
			break;
		}
	}
	return s;
}

}

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
#include "css_parser_border_image_width.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserBorderImageWidth::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-image-width");
	return names;
}

void CSSParserBorderImageWidth::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueBorderImageWidth> border_image_width(new CSSValueBorderImageWidth());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);

	if (token.type == CSSToken::type_ident && pos == tokens.size() && equals(token.value, "inherit"))
	{
		border_image_width->type = CSSValueBorderImageWidth::type_inherit;
		inout_values.push_back(std::move(border_image_width));
	}
	else
	{
		int num_lengths;
		CSSValueBorderImageWidth::ValueType value_types[4];
		CSSLength lengths[4];
		float percentages[4] = { 0, 0, 0, 0 };
		float numbers[4] = { 0, 0, 0, 0 };
		for (num_lengths = 0; num_lengths < 4; num_lengths++)
		{
			if (is_length(token))
			{
				value_types[num_lengths] = CSSValueBorderImageWidth::value_type_length;
				if (!parse_length(token, lengths[num_lengths]))
					return;
			}
			else if (token.type == CSSToken::type_number)
			{
				value_types[num_lengths] = CSSValueBorderImageWidth::value_type_number;
				numbers[num_lengths] = StringHelp::text_to_float(token.value);
			}
			else if (token.type == CSSToken::type_percentage)
			{
				value_types[num_lengths] = CSSValueBorderImageWidth::value_type_percentage;
				percentages[num_lengths] = StringHelp::text_to_float(token.value);
			}
			else if (token.type == CSSToken::type_ident && equals(token.value, "auto"))
			{
				value_types[num_lengths] = CSSValueBorderImageWidth::value_type_auto;
			}
			else
			{
				break;
			}

			token = next_token(pos, tokens);
		}
		if (num_lengths < 1 || pos != tokens.size())
			return;

		border_image_width->type = CSSValueBorderImageWidth::type_values;

		if (num_lengths == 1)
		{
			for (int i = 1; i < 4; i++)
			{
				value_types[i] = value_types[0];
				lengths[i] = lengths[0];
				numbers[i] = numbers[0];
				percentages[i] = percentages[0];
			}
		}
		else if (num_lengths == 2)
		{
			value_types[2] = value_types[0];
			lengths[2] = lengths[0];
			numbers[2] = numbers[0];
			percentages[2] = percentages[0];
			value_types[3] = value_types[1];
			lengths[3] = lengths[1];
			numbers[3] = numbers[1];
			percentages[3] = percentages[1];
		}
		else if (num_lengths == 3)
		{
			value_types[3] = value_types[1];
			lengths[3] = lengths[1];
			numbers[3] = numbers[1];
			percentages[3] = percentages[1];
		}

		border_image_width->value_top = value_types[0];
		border_image_width->value_right = value_types[1];
		border_image_width->value_bottom = value_types[2];
		border_image_width->value_left = value_types[3];
		border_image_width->length_top = lengths[0];
		border_image_width->length_right = lengths[1];
		border_image_width->length_bottom = lengths[2];
		border_image_width->length_left = lengths[3];
		border_image_width->number_top = numbers[0];
		border_image_width->number_right = numbers[1];
		border_image_width->number_bottom = numbers[2];
		border_image_width->number_left = numbers[3];
		border_image_width->percentage_top = percentages[0];
		border_image_width->percentage_right = percentages[1];
		border_image_width->percentage_bottom = percentages[2];
		border_image_width->percentage_left = percentages[3];

		inout_values.push_back(std::move(border_image_width));
	}
}

}

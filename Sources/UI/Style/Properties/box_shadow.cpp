/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "UI/precomp.h"
#include "box_shadow.h"

namespace clan
{
	void force_link_style_parser_box_shadow()
	{
	}

	StylePropertyDefault style_default_box_shadow("box-shadow", StyleGetValue::from_keyword("none"));

	BoxShadowPropertyParser style_parser_box_shadow;

	void BoxShadowPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue box_shadow;
		std::vector<StyleValue> box_shadow_styles;
		std::vector<StyleValue> box_shadow_colors;
		std::vector<StyleValue> box_shadow_horizontal_offsets;
		std::vector<StyleValue> box_shadow_vertical_offsets;
		std::vector<StyleValue> box_shadow_blur_radius;
		std::vector<StyleValue> box_shadow_spread_distances;

		size_t pos = 0;
		size_t last_pos = pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			box_shadow = StyleValue::from_keyword("inherit");
		}
		else if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "none"))
		{
			box_shadow = StyleValue::from_keyword("none");
		}
		else
		{
			box_shadow = StyleValue::from_keyword("array");

			while (true)
			{
				StyleValue style;
				StyleValue color;
				StyleValue horizontal_offset;
				StyleValue vertical_offset;
				StyleValue blur_radius;
				StyleValue spread_distance;

				if (token.type == StyleTokenType::ident && equals(token.value, "inset"))
				{
					style = StyleValue::from_keyword("inset");
					last_pos = pos;
					token = next_token(pos, tokens);
				}
				else
				{
					style = StyleValue::from_keyword("outset");
				}

				int num_lengths;
				StyleValue lengths[4];
				for (num_lengths = 0; num_lengths < 4 && is_length(token); num_lengths++)
				{
					if (!parse_length(token, lengths[num_lengths]))
						return;
					last_pos = pos;
					token = next_token(pos, tokens);
				}
				if (num_lengths < 2)
					return;

				Colorf c = Colorf::black;
				if (parse_color(tokens, last_pos, c))
				{
					pos = last_pos;
					token = next_token(pos, tokens);
				}

				color = StyleValue::from_color(c);

				if (num_lengths == 2)
				{
					horizontal_offset = lengths[0];
					vertical_offset = lengths[1];
				}
				else if (num_lengths == 3)
				{
					horizontal_offset = lengths[0];
					vertical_offset = lengths[1];
					blur_radius = lengths[2];
				}
				else if (num_lengths == 4)
				{
					horizontal_offset = lengths[0];
					vertical_offset = lengths[1];
					blur_radius = lengths[2];
					spread_distance = lengths[3];
				}

				box_shadow_styles.push_back(style);
				box_shadow_colors.push_back(color);
				box_shadow_horizontal_offsets.push_back(horizontal_offset);
				box_shadow_vertical_offsets.push_back(vertical_offset);
				box_shadow_blur_radius.push_back(blur_radius);
				box_shadow_spread_distances.push_back(spread_distance);

				if (pos != tokens.size())
				{
					if (token.type != StyleTokenType::delim || token.value != ",")
						return;
					token = next_token(pos, tokens);
				}
				else
				{
					break;
				}
			}
		}

		setter->set_value("box-shadow", box_shadow);
		setter->set_value_array("box-shadow-style", box_shadow_styles);
		setter->set_value_array("box-shadow-color", box_shadow_colors);
		setter->set_value_array("box-shadow-horizontal-offset", box_shadow_horizontal_offsets);
		setter->set_value_array("box-shadow-vertical-offset", box_shadow_vertical_offsets);
		setter->set_value_array("box-shadow-blur-radius", box_shadow_blur_radius);
		setter->set_value_array("box-shadow-spread_distance", box_shadow_spread_distances);
	}
}

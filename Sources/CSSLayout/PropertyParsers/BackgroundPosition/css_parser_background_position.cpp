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
#include "css_parser_background_position.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserBackgroundPosition::get_names()
{
	std::vector<std::string> names;
	names.push_back("background-position");
	return names;
}

void CSSParserBackgroundPosition::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueBackgroundPosition> position(new CSSValueBackgroundPosition());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);

	if (token.type == CSSToken::type_ident && equals(token.value, "inherit") && tokens.size() == 1)
	{
		position->type = CSSValueBackgroundPosition::type_inherit;
		inout_values.push_back(std::move(position));
		return;
	}

	position->type = CSSValueBackgroundPosition::type_value;
	position->positions.clear();
	bool done = false;
	while (!done)
	{
		CSSValueBackgroundPosition::Position bg_pos;
		bool x_specified = false;
		bool y_specified = false;
		bool center_specified = false;

		while (true)
		{
			if (token.type == CSSToken::type_ident)
			{
				if (!y_specified && equals(token.value, "top"))
				{
					bg_pos.type_y = CSSValueBackgroundPosition::type2_top;
					y_specified = true;

					if (center_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}
				}
				else if (!y_specified && equals(token.value, "bottom"))
				{
					bg_pos.type_y = CSSValueBackgroundPosition::type2_bottom;
					y_specified = true;

					if (center_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}
				}
				else if (!x_specified && equals(token.value, "left"))
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_left;
					x_specified = true;

					if (center_specified)
					{
						bg_pos.type_y = CSSValueBackgroundPosition::type2_center;
						y_specified = true;
						center_specified = false;
					}
				}
				else if (!x_specified && equals(token.value, "right"))
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_right;
					x_specified = true;

					if (center_specified)
					{
						bg_pos.type_y = CSSValueBackgroundPosition::type2_center;
						y_specified = true;
						center_specified = false;
					}
				}
				else if (equals(token.value, "center"))
				{
					if (center_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}

					if (x_specified && !y_specified)
					{
						bg_pos.type_y = CSSValueBackgroundPosition::type2_center;
						y_specified = true;
					}
					else if (y_specified && !x_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
						x_specified = true;
					}
					else if (!x_specified && !y_specified)
					{
						center_specified = true;
					}
					else
					{
						debug_parse_error(name, tokens);
						return;
					}
				}
			}
			else if (is_length(token))
			{
				CSSLength length;
				if (parse_length(token, length))
				{
					if (center_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}

					if (!x_specified && !y_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_length;
						bg_pos.length_x = length;
						x_specified = true;
					}
					else if (x_specified && !y_specified)
					{
						bg_pos.type_y = CSSValueBackgroundPosition::type2_length;
						bg_pos.length_y = length;
						y_specified = true;
					}
					else
					{
						debug_parse_error(name, tokens);
						return;
					}
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (token.type == CSSToken::type_percentage)
			{
				if (center_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
					x_specified = true;
					center_specified = false;
				}

				if (!x_specified && !y_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_percentage;
					bg_pos.percentage_x = StringHelp::text_to_float(token.value);
					x_specified = true;
				}
				else if (x_specified && !y_specified)
				{
					bg_pos.type_y = CSSValueBackgroundPosition::type2_percentage;
					bg_pos.percentage_y = StringHelp::text_to_float(token.value);
					y_specified = true;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (token.type == CSSToken::type_delim && token.value == "-")
			{
				token = next_token(pos, tokens);
				if (is_length(token))
				{
					CSSLength length;
					if (parse_length(token, length))
					{
						length.value = -length.value;
						if (center_specified)
						{
							bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
							x_specified = true;
							center_specified = false;
						}

						if (!x_specified && !y_specified)
						{
							bg_pos.type_x = CSSValueBackgroundPosition::type1_length;
							bg_pos.length_x = length;
							x_specified = true;
						}
						else if (x_specified && !y_specified)
						{
							bg_pos.type_y = CSSValueBackgroundPosition::type2_length;
							bg_pos.length_y = length;
							y_specified = true;
						}
						else
						{
							debug_parse_error(name, tokens);
							return;
						}
					}
					else
					{
						debug_parse_error(name, tokens);
						return;
					}
				}
				else if (token.type == CSSToken::type_percentage)
				{
					if (center_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}

					if (!x_specified && !y_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_percentage;
						bg_pos.percentage_x = -StringHelp::text_to_float(token.value);
						x_specified = true;
					}
					else if (x_specified && !y_specified)
					{
						bg_pos.type_y = CSSValueBackgroundPosition::type2_percentage;
						bg_pos.percentage_y = -StringHelp::text_to_float(token.value);
						y_specified = true;
					}
					else
					{
						debug_parse_error(name, tokens);
						return;
					}
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}

			if (pos == tokens.size())
			{
				done = true;
				break;
			}
			else
			{
				token = next_token(pos, tokens);
				if (token.type == CSSToken::type_delim && token.value == ",")
					break;
			}
		}

		if (!x_specified)
			bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
		else if (!y_specified)
			bg_pos.type_y = CSSValueBackgroundPosition::type2_center;

		position->positions.push_back(bg_pos);
	}

	inout_values.push_back(std::move(position));
}

}

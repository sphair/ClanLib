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
#include "css_parser_shadow.h"
#include "API/CSSLayout/PropertyValues/css_value_shadow.h"

namespace clan
{

std::vector<std::string> CSSParserShadow::get_names()
{
	std::vector<std::string> names;
	names.push_back("box-shadow");
	return names;
}

void CSSParserShadow::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueShadow> box_shadow(new CSSValueShadow());

	size_t pos = 0;
	size_t last_pos = pos;
	CSSToken token = next_token(pos, tokens);

	if (token.type == CSSToken::type_ident && pos == tokens.size() && equals(token.value, "inherit"))
	{
		box_shadow->type = CSSValueShadow::type_inherit;
	}
	else if (token.type == CSSToken::type_ident && pos == tokens.size() && equals(token.value, "none"))
	{
		box_shadow->type = CSSValueShadow::type_none;
	}
	else
	{
		box_shadow->type = CSSValueShadow::type_shadows;
		box_shadow->shadows.clear();
		while (true)
		{
			bool inset = false;
			if (token.type == CSSToken::type_ident && equals(token.value, "inset"))
			{
				inset = true;
				last_pos = pos;
				token = next_token(pos, tokens);
			}

			int num_lengths;
			CSSLength lengths[4];
			for (num_lengths = 0; num_lengths < 4 && is_length(token); num_lengths++)
			{
				if (!parse_length(token, lengths[num_lengths]))
					return;
				last_pos = pos;
				token = next_token(pos, tokens);
			}
			if (num_lengths < 2)
				return;

			Colorf color = Colorf::black;
			if (parse_color(tokens, last_pos, color))
			{
				pos = last_pos;
				token = next_token(pos, tokens);
			}

			CSSValueShadow::Shadow shadow;
			shadow.inset = inset;
			shadow.color = color;
			if (num_lengths == 2)
			{
				shadow.horizontal_offset = lengths[0];
				shadow.vertical_offset = lengths[1];
			}
			else if (num_lengths == 3)
			{
				shadow.horizontal_offset = lengths[0];
				shadow.vertical_offset = lengths[1];
				shadow.blur_radius = lengths[2];
			}
			else if (num_lengths == 4)
			{
				shadow.horizontal_offset = lengths[0];
				shadow.vertical_offset = lengths[1];
				shadow.blur_radius = lengths[2];
				shadow.spread_distance = lengths[3];
			}
			box_shadow->shadows.push_back(shadow);

			if (pos != tokens.size())
			{
				if (token.type != CSSToken::type_delim || token.value != ",")
					return;
				token = next_token(pos, tokens);
			}
			else
			{
				break;
			}
		}
	}

	inout_values.push_back(std::move(box_shadow));
}

}

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
#include "css_parser_text_decoration.h"
#include "API/CSSLayout/PropertyValues/css_value_text_decoration.h"

namespace clan
{

std::vector<std::string> CSSParserTextDecoration::get_names()
{
	std::vector<std::string> names;
	names.push_back("text-decoration");
	return names;
}

void CSSParserTextDecoration::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueTextDecoration> text_decoration(new CSSValueTextDecoration());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "none") && pos == tokens.size())
		{
			text_decoration->type = CSSValueTextDecoration::type_none;
		}
		else if (equals(token.value, "inherit") && pos == tokens.size())
		{
			text_decoration->type = CSSValueTextDecoration::type_inherit;
		}
		else
		{
			int underline = 0;
			int overline = 0;
			int line_through = 0;
			int blink = 0;
			do
			{
				if (token.type == CSSToken::type_ident)
				{
					if (equals(token.value, "underline"))
						underline++;
					else if (equals(token.value, "overline"))
						overline++;
					else if (equals(token.value, "line-through"))
						line_through++;
					else if (equals(token.value, "blink"))
						blink++;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}

				token = next_token(pos, tokens);
			} while (token.type != CSSToken::type_null);

			if (underline < 2 && overline < 2 && line_through < 2 && blink < 2)
			{
				text_decoration->type = CSSValueTextDecoration::type_values;
				text_decoration->underline = (underline == 1);
				text_decoration->overline = (overline == 1);
				text_decoration->line_through = (line_through == 1);
				text_decoration->blink = (blink == 1);
			}
			else
			{
				return;
			}
		}
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(text_decoration));
}

}

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
#include "css_parser_font_family.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserFontFamily::get_names()
{
	std::vector<std::string> names;
	names.push_back("font-family");
	return names;
}

void CSSParserFontFamily::parse(CSSBoxProperties &properties, const std::string &propname, const std::vector<CSSToken> &tokens)
{
	CSSValueFontFamily family;
	family.type = CSSValueFontFamily::type_names;

	size_t pos = 0;
	CSSToken token;
	token = next_token(pos, tokens);

	if (equals(token.value, "inherit") && tokens.size() == 1)
	{
		properties.font_family.type = CSSValueFontFamily::type_inherit;
		return;
	}

	while (true)
	{
		if (token.type != CSSToken::type_ident && token.type != CSSToken::type_string)
		{
			debug_parse_error(propname, tokens);
			return;
		}

		CSSValueFontFamilyName name;
		if (equals(token.value, "serif"))
		{
			name.type = CSSValueFontFamilyName::type_serif;
		}
		else if (equals(token.value, "sans-serif"))
		{
			name.type = CSSValueFontFamilyName::type_sans_serif;
		}
		else if (equals(token.value, "cursive"))
		{
			name.type = CSSValueFontFamilyName::type_cursive;
		}
		else if (equals(token.value, "fantasy"))
		{
			name.type = CSSValueFontFamilyName::type_fantasy;
		}
		else if (equals(token.value, "monospace"))
		{
			name.type = CSSValueFontFamilyName::type_monospace;
		}
		else if (equals(token.value, "default"))
		{
			// reserved for future use
			return;
		}
		else if (equals(token.value, "initial"))
		{
			// reserved for future use
			return;
		}
		else
		{
			name.type = CSSValueFontFamilyName::type_family_name;
		}

		if (name.type == CSSValueFontFamilyName::type_family_name)
		{
			name.name = token.value;
			while (pos != tokens.size())
			{
				token = tokens[pos++];
				if (token.type == CSSToken::type_whitespace)
				{
					name.name += " ";
				}
				else if (token.type == CSSToken::type_ident)
				{
					name.name += token.value;
				}
				else if (token.type == CSSToken::type_delim && token.value == ",")
				{
					break;
				}
			}
			family.names.push_back(name);
			if (pos == tokens.size())
				break;
			token = next_token(pos, tokens);
		}
		else
		{
			family.names.push_back(name);

			if (pos == tokens.size())
				break;
			token = next_token(pos, tokens);
			if (token.type != CSSToken::type_delim || token.value != ",")
			{
				debug_parse_error(propname, tokens);
				return;
			}
			token = next_token(pos, tokens);
		}
	}

	properties.font_family = family;
}

}

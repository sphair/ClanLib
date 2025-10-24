/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "../css_box_properties.h"

std::vector<CL_String> CL_CSSParserFontFamily::get_names()
{
	std::vector<CL_String> names;
	names.push_back("font-family");
	return names;
}

void CL_CSSParserFontFamily::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	CL_CSSBoxFontFamily family;
	family.type = CL_CSSBoxFontFamily::type_names;

	size_t pos = 0;
	CL_CSSToken token;
	token = next_token(pos, tokens);
	while (true)
	{
		if (token.type != CL_CSSToken::type_ident && token.type != CL_CSSToken::type_string)
			return;

		CL_CSSBoxFontFamilyName name;
		if (token.value == "serif")
		{
			name.type = CL_CSSBoxFontFamilyName::type_serif;
		}
		else if (token.value == "sans-serif")
		{
			name.type = CL_CSSBoxFontFamilyName::type_sans_serif;
		}
		else if (token.value == "cursive")
		{
			name.type = CL_CSSBoxFontFamilyName::type_cursive;
		}
		else if (token.value == "fantasy")
		{
			name.type = CL_CSSBoxFontFamilyName::type_fantasy;
		}
		else if (token.value == "monospace")
		{
			name.type = CL_CSSBoxFontFamilyName::type_monospace;
		}
		else if (token.value == "default")
		{
			// reserved for future use
			return;
		}
		else if (token.value == "initial")
		{
			// reserved for future use
			return;
		}
		else
		{
			name.type = CL_CSSBoxFontFamilyName::type_family_name;
		}

		if (name.type == CL_CSSBoxFontFamilyName::type_family_name)
		{
			name.name = token.value;
			while (pos != tokens.size())
			{
				token = tokens[pos++];
				if (token.type == CL_CSSToken::type_whitespace)
				{
					name.name += " ";
				}
				else if (token.type == CL_CSSToken::type_ident)
				{
					name.name += token.value;
				}
				else if (token.type == CL_CSSToken::type_delim && token.value == ",")
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
			if (token.type != CL_CSSToken::type_delim || token.value != ",")
				return;
			token = next_token(pos, tokens);
		}
	}

	properties.font_family = family;
}

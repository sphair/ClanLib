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
#include "css_parser_content.h"
#include "../css_box_properties.h"

std::vector<CL_String> CL_CSSParserContent::get_names()
{
	std::vector<CL_String> names;
	names.push_back("content");
	return names;
}

void CL_CSSParserContent::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);
	if (token.type == CL_CSSToken::type_ident && pos == tokens.size())
	{
		if (token.value == "none")
		{
			properties.content.type = CL_CSSBoxContent::type_none;
		}
		else if (token.value == "normal")
		{
			properties.content.type = CL_CSSBoxContent::type_normal;
		}
		else if (token.value == "open-quote")
		{
			properties.content.type = CL_CSSBoxContent::type_open_quote;
		}
		else if (token.value == "close-quote")
		{
			properties.content.type = CL_CSSBoxContent::type_close_quote;
		}
		else if (token.value == "inherit")
		{
			properties.content.type = CL_CSSBoxContent::type_inherit;
		}
	}
	else if (token.type == CL_CSSToken::type_string && pos == tokens.size())
	{
		properties.content.type = CL_CSSBoxContent::type_string;
		properties.content.str = token.value;
	}
	else if (token.type == CL_CSSToken::type_uri && pos == tokens.size())
	{
		properties.content.type = CL_CSSBoxContent::type_uri;
		properties.content.str = token.value;
	}
	else if (token.type == CL_CSSToken::type_function)
	{
		if (token.value == "counter")
		{
			//properties.content.type = CSSBoxContent::type_counter;
		}
		else if (token.value == "counters")
		{
			//properties.content.type = CSSBoxContent::type_counters;
		}
		else if (token.value == "attr")
		{
			//properties.content.type = CSSBoxContent::type_attr;
		}
	}
}

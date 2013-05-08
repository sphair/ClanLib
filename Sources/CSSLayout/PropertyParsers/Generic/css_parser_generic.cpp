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
**    Mark Page
*/

#include "CSSLayout/precomp.h"
#include "css_parser_generic.h"
#include "API/CSSLayout/PropertyValues/css_value_generic.h"

namespace clan
{

std::vector<std::string> CSSParserGeneric::get_names()
{
	std::vector<std::string> names;
	return names;
}

void CSSParserGeneric::parse(const std::string &propname, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueGeneric> generic_values(new CSSValueGeneric(propname));
	generic_values->type = CSSValueGeneric::type_tokens;

	size_t pos = 0;
	CSSToken token;
	token = next_token(pos, tokens);

	if (equals(token.value, "inherit") && tokens.size() == 1)
	{
		generic_values->type = CSSValueGeneric::type_inherit;
		inout_values.push_back(std::move(generic_values));
		return;
	}

	generic_values->tokens = tokens;

	inout_values.push_back(std::move(generic_values));
}

}

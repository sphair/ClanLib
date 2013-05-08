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
#include "css_parser_text_transform.h"
#include "API/CSSLayout/PropertyValues/css_value_text_transform.h"

namespace clan
{

std::vector<std::string> CSSParserTextTransform::get_names()
{
	std::vector<std::string> names;
	names.push_back("text-transform");
	return names;
}

void CSSParserTextTransform::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueTextTransform> text_transform(new CSSValueTextTransform());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "capitalize"))
			text_transform->type = CSSValueTextTransform::type_capitalize;
		else if (equals(token.value, "uppercase"))
			text_transform->type = CSSValueTextTransform::type_uppercase;
		else if (equals(token.value, "lowercase"))
			text_transform->type = CSSValueTextTransform::type_lowercase;
		else if (equals(token.value, "none"))
			text_transform->type = CSSValueTextTransform::type_none;
		else if (equals(token.value, "inherit"))
			text_transform->type = CSSValueTextTransform::type_inherit;
		else
			return;
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(text_transform));
}

}

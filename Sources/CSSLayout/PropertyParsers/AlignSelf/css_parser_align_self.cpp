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
#include "css_parser_align_self.h"
#include "API/CSSLayout/PropertyValues/css_value_align_self.h"

namespace clan
{

std::vector<std::string> CSSParserAlignSelf::get_names()
{
	std::vector<std::string> names;
	names.push_back("align-self");
	return names;
}

void CSSParserAlignSelf::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		std::unique_ptr<CSSValueAlignSelf> align_self(new CSSValueAlignSelf());

		if (equals(token.value, "auto"))
		{
			align_self->type = CSSValueAlignSelf::type_auto;
			inout_values.push_back(std::move(align_self));
		}
		else if (equals(token.value, "flex-start"))
		{
			align_self->type = CSSValueAlignSelf::type_flex_start;
			inout_values.push_back(std::move(align_self));
		}
		else if (equals(token.value, "flex-end"))
		{
			align_self->type = CSSValueAlignSelf::type_flex_end;
			inout_values.push_back(std::move(align_self));
		}
		else if (equals(token.value, "center"))
		{
			align_self->type = CSSValueAlignSelf::type_center;
			inout_values.push_back(std::move(align_self));
		}
		else if (equals(token.value, "baseline"))
		{
			align_self->type = CSSValueAlignSelf::type_baseline;
			inout_values.push_back(std::move(align_self));
		}
		else if (equals(token.value, "stretch"))
		{
			align_self->type = CSSValueAlignSelf::type_stretch;
			inout_values.push_back(std::move(align_self));
		}
		else if (equals(token.value, "inherit"))
		{
			align_self->type = CSSValueAlignSelf::type_inherit;
			inout_values.push_back(std::move(align_self));
		}
	}
}

}

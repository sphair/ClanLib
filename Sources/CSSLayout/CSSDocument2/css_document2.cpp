/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/CSSLayout/css_document2.h"
#include "css_document2_impl.h"

CL_CSSDocument2::CL_CSSDocument2()
: impl(new CL_CSSDocument2_Impl())
{
}

CL_CSSDocument2::~CL_CSSDocument2()
{
}

void CL_CSSDocument2::add_sheet(const CL_String &filename, const CL_String &base_uri)
{
	CL_File file(filename, CL_File::open_existing, CL_File::access_read);
	add_sheet(file, base_uri);
}

void CL_CSSDocument2::add_sheet(CL_IODevice &iodevice, const CL_String &base_uri)
{
	CL_CSSTokenizer tokenizer(iodevice);
	impl->read_stylesheet(tokenizer, base_uri);
}

CL_CSSPropertyList2 CL_CSSDocument2::select(const CL_DomElement &node, const CL_String &pseudo_element)
{
	CL_DomSelectNode select_node(node);
	return select(&select_node, pseudo_element);
}

CL_CSSPropertyList2 CL_CSSDocument2::select(CL_CSSSelectNode2 *node, const CL_String &pseudo_element)
{
	std::vector<CL_CSSRulesetMatch2> rulesets = impl->select_rulesets(node, pseudo_element);
	CL_CSSPropertyList2 properties;
	for (size_t i = rulesets.size(); i > 0; i--)
	{
		for (size_t j = rulesets[i-1].ruleset->properties.size(); j > 0; j--)
		{
			if (rulesets[i-1].ruleset->properties[j-1].is_important())
				properties.push_back(rulesets[i-1].ruleset->properties[j-1]);
		}
	}
	for (size_t i = rulesets.size(); i > 0; i--)
	{
		for (size_t j = rulesets[i-1].ruleset->properties.size(); j > 0; j--)
		{
			if (!rulesets[i-1].ruleset->properties[j-1].is_important())
				properties.push_back(rulesets[i-1].ruleset->properties[j-1]);
		}
	}
	return properties;
}

CL_CSSPropertyList2 CL_CSSDocument2::get_style_properties(const CL_String &style_string, const CL_String &base_uri)
{
	CL_CSSTokenizer tokenizer(style_string);
	CL_CSSToken token;
	std::vector<CL_CSSProperty2> property_list;
	while (true)
	{
		tokenizer.read(token, true);
		if (token.type == CL_CSSToken::type_ident)
		{
			CL_String property_name = token.value;
			tokenizer.read(token, true);
			if (token.type == CL_CSSToken::type_colon)
			{
				tokenizer.read(token, true);

				CL_CSSProperty2 property;
				property.set_name(property_name);
				CL_CSSDocument2_Impl::read_property_value(tokenizer, token, property, base_uri);
				if (!property.get_value_tokens().empty())
					property_list.push_back(property);
			}
			else
			{
				bool end_of_scope = CL_CSSDocument2_Impl::read_end_of_statement(tokenizer, token);
				if (end_of_scope)
					break;
			}
		}
		else if (token.type == CL_CSSToken::type_null)
		{
			break;
		}
	}

	CL_CSSPropertyList2 properties;
	for (size_t i = property_list.size(); i > 0; i--)
		properties.push_back(property_list[i - 1]);

	return properties;
}

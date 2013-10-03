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
#include "API/CSSLayout/CSSDocument/css_style_properties.h"
#include "css_style_properties_impl.h"
#include "css_document_sheet.h"

namespace clan
{

CSSStyleProperties::CSSStyleProperties() : impl(new CSSStyleProperties_Impl())
{
}

CSSStyleProperties::CSSStyleProperties(const std::string &style_string, const std::string &base_uri)
: impl(new CSSStyleProperties_Impl())
{
	static CSSPropertyParsers parsers; // To do: this isn't a very nice way of doing it!

	CSSTokenizer tokenizer(style_string);
	CSSToken token;
	while (true)
	{
		tokenizer.read(token, true);
		if (token.type == CSSToken::type_ident)
		{
			std::string property_name = token.value;
			tokenizer.read(token, true);
			if (token.type == CSSToken::type_colon)
			{
				tokenizer.read(token, true);

				CSSProperty property;
				property.set_name(property_name);
				CSSDocumentSheet::read_property_value(tokenizer, token, property, base_uri);
				if (!property.get_value_tokens().empty())
				{
					parsers.parse(property, impl->values);
				}
			}
			else
			{
				bool end_of_scope = CSSDocumentSheet::read_end_of_statement(tokenizer, token);
				if (end_of_scope)
					break;
			}
		}
		else if (token.type == CSSToken::type_null)
		{
			break;
		}
	}

	std::reverse(impl->values.begin(), impl->values.end());
}

bool CSSStyleProperties::is_null() const
{
	//return !impl;
	return impl->values.empty();
}

const std::vector<std::unique_ptr<CSSPropertyValue> > &CSSStyleProperties::get_values() const
{
	return impl->values;
}

void CSSStyleProperties::set_value(const CSSPropertyValue &value, bool enable)
{
	impl->set_value(value, enable);
}

void CSSStyleProperties_Impl::set_value(const CSSPropertyValue &value, bool enable)
{
	std::string value_name = value.get_name();

	// Remove existing
	std::vector<std::unique_ptr<CSSPropertyValue> >::iterator it;
	for (it = values.begin(); it != values.end(); ++it)
	{
		if ((*it)->get_name() == value_name)
		{
			it = values.erase(it);
			break;
		}
	}

	if (enable)
		values.push_back( value.clone() );
}

}

/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/XML/xml_token_load.h"
#include "xml_token_load_generic.h"
#include "API/Core/XML/xml_token_string.h"

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenLoad construction:

CL_XMLTokenLoad::CL_XMLTokenLoad()
	: impl(new CL_XMLTokenLoad_Generic)
{
}
	
CL_XMLTokenLoad::CL_XMLTokenLoad(const CL_XMLTokenLoad &copy)
	: impl(copy.impl)
{
}
	
CL_XMLTokenLoad::~CL_XMLTokenLoad()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenLoad attributes:

CL_XMLToken::TokenType CL_XMLTokenLoad::get_type() const
{
	return impl->type;
}

CL_XMLToken::TokenVariant CL_XMLTokenLoad::get_variant() const
{
	return impl->variant;
}

std::string CL_XMLTokenLoad::get_name() const
{
	return impl->name.to_string();
}

std::string CL_XMLTokenLoad::get_value() const
{
	return impl->value.to_string();
}

int CL_XMLTokenLoad::get_attributes_number() const
{
	return impl->attributes.size();
}

std::pair<std::string, std::string> CL_XMLTokenLoad::get_attribute(int attribute_num) const
{
	if (attribute_num < 0 || attribute_num >= (int)impl->attributes.size())
		return std::make_pair(std::string(), std::string());

	std::pair<CL_XMLTokenString, CL_XMLTokenString> const & attribute = impl->attributes[attribute_num];
	return std::make_pair(attribute.first.to_string(), attribute.second.to_string());
}

const std::pair<CL_XMLTokenString, CL_XMLTokenString> & CL_XMLTokenLoad::get_attribute_fast(int attribute_num) const
{
	return impl->attributes[attribute_num];
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenLoad operations:

void CL_XMLTokenLoad::set_type(TokenType type)
{
	impl->type = type;
}

void CL_XMLTokenLoad::set_variant(TokenVariant variant)
{
	impl->variant = variant;
}

void CL_XMLTokenLoad::set_name(const CL_XMLTokenString & name)
{
	impl->name = name;
}

void CL_XMLTokenLoad::set_value(const CL_XMLTokenString & value)
{
	impl->value = value;
}

void CL_XMLTokenLoad::set_attribute(const CL_XMLTokenString & name, const CL_XMLTokenString & value)
{
	int size = impl->attributes.size();
	for (int i=0; i<size; i++)
	{
		if (impl->attributes[i].first == name)
		{
			impl->attributes[i].second = value;
			return;
		}
	}
	impl->attributes.push_back(std::make_pair(name, value));
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenLoad implementation:

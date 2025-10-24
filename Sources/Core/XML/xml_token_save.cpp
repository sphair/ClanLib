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
#include "API/Core/XML/xml_token_save.h"
#include "xml_token_save_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenSave construction:

CL_XMLTokenSave::CL_XMLTokenSave()
	: impl(new CL_XMLTokenSave_Generic)
{
}
	
CL_XMLTokenSave::CL_XMLTokenSave(const CL_XMLTokenSave &copy)
	: impl(copy.impl)
{
}
	
CL_XMLTokenSave::~CL_XMLTokenSave()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenSave attributes:

CL_XMLTokenSave::TokenType CL_XMLTokenSave::get_type() const
{
	return impl->type;
}

CL_XMLTokenSave::TokenVariant CL_XMLTokenSave::get_variant() const
{
	return impl->variant;
}

std::string CL_XMLTokenSave::get_name() const
{
	return impl->name;
}

std::string CL_XMLTokenSave::get_value() const
{
	return impl->value;
}

int CL_XMLTokenSave::get_attributes_number() const
{
	return impl->attributes.size();
}

std::pair<std::string, std::string> CL_XMLTokenSave::get_attribute(int attribute_num) const
{
	if (attribute_num < 0 || attribute_num >= (int)impl->attributes.size())
		return std::make_pair(std::string(), std::string());

	return impl->attributes[attribute_num];
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenSave operations:

void CL_XMLTokenSave::set_type(TokenType type)
{
	impl->type = type;
}

void CL_XMLTokenSave::set_variant(TokenVariant variant)
{
	impl->variant = variant;
}

void CL_XMLTokenSave::set_name(const std::string &name)
{
	impl->name = name;
}

void CL_XMLTokenSave::set_value(const std::string &value)
{
	impl->value = value;
}

void CL_XMLTokenSave::set_attribute(const std::string &name, const std::string &value)
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
	impl->attributes.push_back(std::pair<std::string, std::string>(name, value));
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenSave implementation:

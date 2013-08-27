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

#pragma once

#include "../api_csslayout.h"
#include "../../Core/Text/string_help.h"
#include <string>
#include <vector>

namespace clan
{
/// \addtogroup clanCSSLayout_HTML clanCSSLayout HTML
/// \{

class CL_API_CSSLAYOUT HTMLAttribute
{
public:
	HTMLAttribute() { }
	HTMLAttribute(const std::string &name, const std::string &value) : name(name), value(value) { }

	std::string name;
	std::string value;
};

class CL_API_CSSLAYOUT HTMLToken
{
public:
	enum Type
	{
		type_null,
		type_dtd,
		type_tag_begin,
		type_tag_end,
		type_tag_single,
		type_text,
		type_script_tag,
		type_style_tag,
		type_comment,
		type_invalid
	};
	HTMLToken() : type(type_null) { }
	HTMLToken(Type type) : type(type) { }
	HTMLToken(Type type, const std::string &name) : type(type), name(name) { }
	HTMLToken(Type type, const std::string &name, const std::string &value) : type(type), name(name), value(value) { }
	HTMLToken(Type type, const std::string &name, const std::string &value, const std::vector<HTMLAttribute> &attributes) : type(type), name(name), value(value), attributes(attributes) { }
	
	Type type;
	std::string name;
	std::string value;
	std::vector<HTMLAttribute> attributes;

	bool has_attribute(const std::string &name) const
	{
		for (size_t i = 0; i < attributes.size(); i++)
			if (StringHelp::compare(attributes[i].name, name, true) == 0)
				return true;
		return false;
	}

	std::string get_attribute(const std::string &name) const
	{
		for (size_t i = 0; i < attributes.size(); i++)
			if (StringHelp::compare(attributes[i].name, name, true) == 0)
				return attributes[i].value;
		return std::string();
	}
};

/// \}
}

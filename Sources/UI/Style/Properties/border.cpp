/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "UI/precomp.h"
#include "border.h"

namespace clan
{
	BorderPropertyParser style_parser_border;
	BorderColorPropertyParser style_parser_border_color;
	BorderStylePropertyParser style_parser_border_style;
	BorderWidthPropertyParser style_parser_border_width;
	BorderLTRBPropertyParser style_parser_border_ltrb;
	BorderLTRBColorPropertyParser style_parser_border_ltrb_color;
	BorderLTRBStylePropertyParser style_parser_border_ltrb_style;
	BorderLTRBWidthPropertyParser style_parser_border_ltrb_width;
	BorderRadiusPropertyParser style_parser_border_radius;
	BorderRadiusCornerPropertyParser style_parser_border_radius_corner;

	void BorderPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderStylePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderWidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderLTRBPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderLTRBColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderLTRBStylePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderLTRBWidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderRadiusPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BorderRadiusCornerPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}
}

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
#include "layout.h"

namespace clan
{
	LayoutPropertyParser style_parser_layout;
	PositionPropertyParser style_parser_position;
	LeftPropertyParser style_parser_left;
	TopPropertyParser style_parser_top;
	RightPropertyParser style_parser_right;
	BottomPropertyParser style_parser_bottom;
	ZIndexPropertyParser style_parser_zindex;

	void LayoutPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void PositionPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void LeftPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void TopPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void RightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BottomPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void ZIndexPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}
}

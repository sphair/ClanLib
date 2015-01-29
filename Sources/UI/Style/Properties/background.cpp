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
#include "background.h"

namespace clan
{
	BackgroundPropertyParser style_parser_background;
	BackgroundAttachmentPropertyParser style_parser_background_attachment;
	BackgroundClipPropertyParser style_parser_background_clip;
	BackgroundColorPropertyParser style_parser_background_color;
	BackgroundImagePropertyParser style_parser_background_image;
	BackgroundOriginPropertyParser style_parser_background_origin;
	BackgroundPositionPropertyParser style_parser_background_position;
	BackgroundRepeatPropertyParser style_parser_background_repeat;
	BackgroundSizePropertyParser style_parser_background_size;

	void BackgroundPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundAttachmentPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundClipPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundImagePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundOriginPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundPositionPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundRepeatPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundSizePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}
}

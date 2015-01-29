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
#include "text.h"

namespace clan
{
	ColorPropertyParser style_parser_color;
	TextAlignPropertyParser style_parser_text_align;
	TextDecorationPropertyParser style_parser_text_decoration;
	TextIndentPropertyParser style_parser_text_indent;
	TextTransformPropertyParser style_parser_text_transform;
	TextShadowPropertyParser style_parser_text_shadow;
	WordSpacingPropertyParser style_parser_word_spacing;
	LetterSpacingPropertyParser style_parser_letter_spacing;
	FontPropertyParser style_parser_font;
	FontFamilyPropertyParser style_parser_font_family;
	FontSizePropertyParser style_parser_font_size;
	LineHeightPropertyParser style_parser_line_height;
	FontStylePropertyParser style_parser_font_style;
	FontVariantPropertyParser style_parser_font_variant;
	FontWeightPropertyParser style_parser_font_weight;

	void ColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void TextAlignPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void TextDecorationPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void TextIndentPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void TextTransformPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void TextShadowPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void WordSpacingPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void LetterSpacingPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void FontPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void FontFamilyPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void FontSizePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void LineHeightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void FontStylePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void FontVariantPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void FontWeightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}
}

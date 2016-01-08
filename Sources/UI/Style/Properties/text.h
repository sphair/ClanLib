/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "API/UI/Style/style_property_parser.h"

namespace clan
{
	void force_link_style_parser_text();

	class ColorPropertyParser : public StylePropertyParser
	{
	public:
		ColorPropertyParser() : StylePropertyParser({ "color" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class TextAlignPropertyParser : public StylePropertyParser
	{
	public:
		TextAlignPropertyParser() : StylePropertyParser({ "text-align" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class TextDecorationPropertyParser : public StylePropertyParser
	{
	public:
		TextDecorationPropertyParser() : StylePropertyParser({ "text-decoration" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class TextIndentPropertyParser : public StylePropertyParser
	{
	public:
		TextIndentPropertyParser() : StylePropertyParser({ "text-indent" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class TextTransformPropertyParser : public StylePropertyParser
	{
	public:
		TextTransformPropertyParser() : StylePropertyParser({ "text-transform" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class TextShadowPropertyParser : public StylePropertyParser
	{
	public:
		TextShadowPropertyParser() : StylePropertyParser({ "text-shadow" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class WordSpacingPropertyParser : public StylePropertyParser
	{
	public:
		WordSpacingPropertyParser() : StylePropertyParser({ "word-spacing" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class LetterSpacingPropertyParser : public StylePropertyParser
	{
	public:
		LetterSpacingPropertyParser() : StylePropertyParser({ "letter-spacing" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class FontPropertyParser : public StylePropertyParser
	{
	public:
		FontPropertyParser() : StylePropertyParser({ "font" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class FontFamilyPropertyParser : public StylePropertyParser
	{
	public:
		FontFamilyPropertyParser() : StylePropertyParser({ "font-family" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class FontSizePropertyParser : public StylePropertyParser
	{
	public:
		FontSizePropertyParser() : StylePropertyParser({ "font-size" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class LineHeightPropertyParser : public StylePropertyParser
	{
	public:
		LineHeightPropertyParser() : StylePropertyParser({ "line-height" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class FontStylePropertyParser : public StylePropertyParser
	{
	public:
		FontStylePropertyParser() : StylePropertyParser({ "font-style" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class FontVariantPropertyParser : public StylePropertyParser
	{
	public:
		FontVariantPropertyParser() : StylePropertyParser({ "font-variant" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class FontWeightPropertyParser : public StylePropertyParser
	{
	public:
		FontWeightPropertyParser() : StylePropertyParser({ "font-weight" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class ClanFontRenderingPropertyParser : public StylePropertyParser
	{
	public:
		ClanFontRenderingPropertyParser() : StylePropertyParser({ "-clan-font-rendering" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};
}

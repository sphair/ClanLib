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

#pragma once

#include "../css_property_parser.h"
#include "API/CSSLayout/css_box_properties.h"

class CL_CSSParserBorderImage : public CL_CSSPropertyParser
{
public:
	std::vector<CL_String> get_names();
	void parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set);
	
private:
	bool parse_source(CL_CSSBoxBorderImageSource &border_image_source, size_t &parse_pos, const std::vector<CL_CSSToken> &tokens);
	bool parse_slice(CL_CSSBoxBorderImageSlice &border_image_slice, size_t &parse_pos, const std::vector<CL_CSSToken> &tokens);
	bool parse_width(CL_CSSBoxBorderImageWidth &border_image_width, size_t &parse_pos, const std::vector<CL_CSSToken> &tokens);
	bool parse_outset(CL_CSSBoxBorderImageOutset &border_image_outset, size_t &parse_pos, const std::vector<CL_CSSToken> &tokens);
	bool parse_repeat(CL_CSSBoxBorderImageRepeat &border_image_repeat, size_t &parse_pos, const std::vector<CL_CSSToken> &tokens);
};

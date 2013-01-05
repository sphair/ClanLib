/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

namespace clan
{

class CSSParserBackground : public CSSPropertyParser
{
public:
	std::vector<std::string> get_names();
	void parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens);

private:
	bool parse_bgcolor(CSSValueBackgroundColor &bgcolor, size_t &parse_pos, const std::vector<CSSToken> &tokens);
	bool parse_image(CSSValueBackgroundImage::Image &layer_image, size_t &parse_pos, const std::vector<CSSToken> &tokens);
	bool parse_repeat(CSSValueBackgroundRepeat::RepeatStyle &layer_repeat_x, CSSValueBackgroundRepeat::RepeatStyle &layer_repeat_y, size_t &parse_pos, const std::vector<CSSToken> &tokens);
	bool parse_attachment(CSSValueBackgroundAttachment::Attachment &layer_attachment, size_t &parse_pos, const std::vector<CSSToken> &tokens);
	bool parse_position(CSSValueBackgroundPosition::Position &layer_position, size_t &parse_pos, const std::vector<CSSToken> &tokens);
	bool parse_size(CSSValueBackgroundSize::Size &layer_size, size_t &parse_pos, const std::vector<CSSToken> &tokens);
	bool parse_boxes(CSSValueBackgroundOrigin::OriginType &layer_origin, CSSValueBackgroundClip::ClipType &layer_clip, size_t &parse_pos, const std::vector<CSSToken> &tokens);
};

}

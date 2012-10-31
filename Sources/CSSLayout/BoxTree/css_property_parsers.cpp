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

#include "CSSLayout/precomp.h"
#include "css_property_parsers.h"
#include "css_property_parser.h"
#include "API/CSSLayout/css_property.h"
#include "PropertyParsers/css_parser_background.h"
#include "PropertyParsers/css_parser_background_attachment.h"
#include "PropertyParsers/css_parser_background_color.h"
#include "PropertyParsers/css_parser_background_image.h"
#include "PropertyParsers/css_parser_background_position.h"
#include "PropertyParsers/css_parser_background_repeat.h"
#include "PropertyParsers/css_parser_background_clip.h"
#include "PropertyParsers/css_parser_background_origin.h"
#include "PropertyParsers/css_parser_background_size.h"
#include "PropertyParsers/css_parser_border.h"
#include "PropertyParsers/css_parser_border_collapse.h"
#include "PropertyParsers/css_parser_border_color.h"
#include "PropertyParsers/css_parser_border_ltrb.h"
#include "PropertyParsers/css_parser_border_ltrb_color.h"
#include "PropertyParsers/css_parser_border_ltrb_style.h"
#include "PropertyParsers/css_parser_border_ltrb_width.h"
#include "PropertyParsers/css_parser_border_spacing.h"
#include "PropertyParsers/css_parser_border_style.h"
#include "PropertyParsers/css_parser_border_width.h"
#include "PropertyParsers/css_parser_border_radius.h"
#include "PropertyParsers/css_parser_border_radius_corner.h"
#include "PropertyParsers/css_parser_border_image.h"
#include "PropertyParsers/css_parser_border_image_outset.h"
#include "PropertyParsers/css_parser_border_image_repeat.h"
#include "PropertyParsers/css_parser_border_image_slice.h"
#include "PropertyParsers/css_parser_border_image_source.h"
#include "PropertyParsers/css_parser_border_image_width.h"
#include "PropertyParsers/css_parser_decoration_break.h"
#include "PropertyParsers/css_parser_shadow.h"
#include "PropertyParsers/css_parser_bottom.h"
#include "PropertyParsers/css_parser_caption_side.h"
#include "PropertyParsers/css_parser_clan_box_direction.h"
#include "PropertyParsers/css_parser_clan_box_sizing_factor.h"
#include "PropertyParsers/css_parser_clear.h"
#include "PropertyParsers/css_parser_clip.h"
#include "PropertyParsers/css_parser_color.h"
#include "PropertyParsers/css_parser_content.h"
#include "PropertyParsers/css_parser_counter_increment.h"
#include "PropertyParsers/css_parser_counter_reset.h"
#include "PropertyParsers/css_parser_cursor.h"
#include "PropertyParsers/css_parser_direction.h"
#include "PropertyParsers/css_parser_display.h"
#include "PropertyParsers/css_parser_empty_cells.h"
#include "PropertyParsers/css_parser_float.h"
#include "PropertyParsers/css_parser_font.h"
#include "PropertyParsers/css_parser_font_family.h"
#include "PropertyParsers/css_parser_font_size.h"
#include "PropertyParsers/css_parser_font_style.h"
#include "PropertyParsers/css_parser_font_variant.h"
#include "PropertyParsers/css_parser_font_weight.h"
#include "PropertyParsers/css_parser_height.h"
#include "PropertyParsers/css_parser_left.h"
#include "PropertyParsers/css_parser_letter_spacing.h"
#include "PropertyParsers/css_parser_line_height.h"
#include "PropertyParsers/css_parser_list_style.h"
#include "PropertyParsers/css_parser_list_style_image.h"
#include "PropertyParsers/css_parser_list_style_position.h"
#include "PropertyParsers/css_parser_list_style_type.h"
#include "PropertyParsers/css_parser_margin.h"
#include "PropertyParsers/css_parser_margin_ltrb.h"
#include "PropertyParsers/css_parser_max_height.h"
#include "PropertyParsers/css_parser_max_width.h"
#include "PropertyParsers/css_parser_min_height.h"
#include "PropertyParsers/css_parser_min_width.h"
#include "PropertyParsers/css_parser_orphans.h"
#include "PropertyParsers/css_parser_outline.h"
#include "PropertyParsers/css_parser_outline_color.h"
#include "PropertyParsers/css_parser_outline_style.h"
#include "PropertyParsers/css_parser_outline_width.h"
#include "PropertyParsers/css_parser_overflow.h"
#include "PropertyParsers/css_parser_padding.h"
#include "PropertyParsers/css_parser_padding_ltrb.h"
#include "PropertyParsers/css_parser_page_break_after.h"
#include "PropertyParsers/css_parser_page_break_before.h"
#include "PropertyParsers/css_parser_page_break_inside.h"
#include "PropertyParsers/css_parser_position.h"
#include "PropertyParsers/css_parser_quotes.h"
#include "PropertyParsers/css_parser_right.h"
#include "PropertyParsers/css_parser_table_layout.h"
#include "PropertyParsers/css_parser_text_align.h"
#include "PropertyParsers/css_parser_text_decoration.h"
#include "PropertyParsers/css_parser_text_indent.h"
#include "PropertyParsers/css_parser_text_transform.h"
#include "PropertyParsers/css_parser_top.h"
#include "PropertyParsers/css_parser_unicode_bidi.h"
#include "PropertyParsers/css_parser_vertical_align.h"
#include "PropertyParsers/css_parser_visibility.h"
#include "PropertyParsers/css_parser_white_space.h"
#include "PropertyParsers/css_parser_widows.h"
#include "PropertyParsers/css_parser_width.h"
#include "PropertyParsers/css_parser_word_spacing.h"
#include "PropertyParsers/css_parser_z_index.h"

namespace clan
{

CSSPropertyParsers::CSSPropertyParsers()
{
	add(new CSSParserBackground());
	add(new CSSParserBackgroundAttachment());
	add(new CSSParserBackgroundColor());
	add(new CSSParserBackgroundImage());
	add(new CSSParserBackgroundPosition());
	add(new CSSParserBackgroundRepeat());
	add(new CSSParserBackgroundClip());
	add(new CSSParserBackgroundOrigin());
	add(new CSSParserBackgroundSize());
	add(new CSSParserBorder());
	add(new CSSParserBorderCollapse());
	add(new CSSParserBorderColor());
	add(new CSSParserBorderLTRB());
	add(new CSSParserBorderLTRBColor());
	add(new CSSParserBorderLTRBStyle());
	add(new CSSParserBorderLTRBWidth());
	add(new CSSParserBorderSpacing());
	add(new CSSParserBorderStyle());
	add(new CSSParserBorderWidth());
	add(new CSSParserBorderRadius());
	add(new CSSParserBorderRadiusCorner());
	add(new CSSParserBorderImage());
	add(new CSSParserBorderImageOutset());
	add(new CSSParserBorderImageRepeat());
	add(new CSSParserBorderImageSlice());
	add(new CSSParserBorderImageSource());
	add(new CSSParserBorderImageWidth());
	add(new CSSParserDecorationBreak());
	add(new CSSParserShadow());
	add(new CSSParserBottom());
	add(new CSSParserCaptionSide());
	add(new CSSParserClear());
	add(new CSSParserClip());
	add(new CSSParserColor());
	add(new CSSParserContent());
	add(new CSSParserCounterIncrement());
	add(new CSSParserCounterReset());
	add(new CSSParserCursor());
	add(new CSSParserDirection());
	add(new CSSParserDisplay());
	add(new CSSParserEmptyCells());
	add(new CSSParserFloat());
	add(new CSSParserFont());
	add(new CSSParserFontFamily());
	add(new CSSParserFontSize());
	add(new CSSParserFontStyle());
	add(new CSSParserFontVariant());
	add(new CSSParserFontWeight());
	add(new CSSParserHeight());
	add(new CSSParserLeft());
	add(new CSSParserLetterSpacing());
	add(new CSSParserLineHeight());
	add(new CSSParserListStyle());
	add(new CSSParserListStyleImage());
	add(new CSSParserListStylePosition());
	add(new CSSParserListStyleType());
	add(new CSSParserMargin());
	add(new CSSParserMarginLTRB());
	add(new CSSParserMaxHeight());
	add(new CSSParserMaxWidth());
	add(new CSSParserMinHeight());
	add(new CSSParserMinWidth());
	add(new CSSParserOrphans());
	add(new CSSParserOutline());
	add(new CSSParserOutlineColor());
	add(new CSSParserOutlineStyle());
	add(new CSSParserOutlineWidth());
	add(new CSSParserOverflow());
	add(new CSSParserPadding());
	add(new CSSParserPaddingLTRB());
	add(new CSSParserPageBreakAfter());
	add(new CSSParserPageBreakBefore());
	add(new CSSParserPageBreakInside());
	add(new CSSParserPosition());
	add(new CSSParserQuotes());
	add(new CSSParserRight());
	add(new CSSParserTableLayout());
	add(new CSSParserTextAlign());
	add(new CSSParserTextDecoration());
	add(new CSSParserTextIndent());
	add(new CSSParserTextTransform());
	add(new CSSParserTop());
	add(new CSSParserUnicodeBidi());
	add(new CSSParserVerticalAlign());
	add(new CSSParserVisibility());
	add(new CSSParserWhiteSpace());
	add(new CSSParserWidows());
	add(new CSSParserWidth());
	add(new CSSParserWordSpacing());
	add(new CSSParserZIndex());
	add(new CSSParserClanBoxDirection());
	add(new CSSParserClanBoxSizingFactor());
}

CSSPropertyParsers::~CSSPropertyParsers()
{
	for (size_t i = 0; i < parsers.size(); i++)
		delete parsers[i];
}

void CSSPropertyParsers::parse(CSSBoxProperties &properties, const CSSProperty &property, std::map<std::string, CSSBoxProperty *> *out_change_set)
{
	std::string name = StringHelp::text_to_lower(property.get_name());
	std::map<std::string, CSSPropertyParser *>::iterator it = name_to_parser.find(name);
	if (it != name_to_parser.end())
		it->second->parse(properties, property.get_name(), property.get_value_tokens(), out_change_set);
}

void CSSPropertyParsers::add(CSSPropertyParser *parser)
{
	parsers.push_back(parser);
	std::vector<std::string> names = parser->get_names();
	for (size_t i = 0; i < names.size(); i++)
		name_to_parser[names[i]] = parser;
}

}

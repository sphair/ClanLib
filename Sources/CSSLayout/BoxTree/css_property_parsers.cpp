/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/CSSLayout/css_property2.h"
#include "PropertyParsers/css_parser_background.h"
#include "PropertyParsers/css_parser_background_attachment.h"
#include "PropertyParsers/css_parser_background_color.h"
#include "PropertyParsers/css_parser_background_image.h"
#include "PropertyParsers/css_parser_background_position.h"
#include "PropertyParsers/css_parser_background_repeat.h"
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
#include "PropertyParsers/css_parser_bottom.h"
#include "PropertyParsers/css_parser_caption_side.h"
#include "PropertyParsers/css_parser_clan_background_border_ltrb.h"
#include "PropertyParsers/css_parser_clan_total_width.h"
#include "PropertyParsers/css_parser_clan_total_height.h"
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

CL_CSSPropertyParsers::CL_CSSPropertyParsers()
{
	add(new CL_CSSParserBackground());
	add(new CL_CSSParserBackgroundAttachment());
	add(new CL_CSSParserBackgroundColor());
	add(new CL_CSSParserBackgroundImage());
	add(new CL_CSSParserBackgroundPosition());
	add(new CL_CSSParserBackgroundRepeat());
	add(new CL_CSSParserBorder());
	add(new CL_CSSParserBorderCollapse());
	add(new CL_CSSParserBorderColor());
	add(new CL_CSSParserBorderLTRB());
	add(new CL_CSSParserBorderLTRBColor());
	add(new CL_CSSParserBorderLTRBStyle());
	add(new CL_CSSParserBorderLTRBWidth());
	add(new CL_CSSParserBorderSpacing());
	add(new CL_CSSParserBorderStyle());
	add(new CL_CSSParserBorderWidth());
	add(new CL_CSSParserBottom());
	add(new CL_CSSParserCaptionSide());
	add(new CL_CSSParserClanBackgroundBorderLTRB());
	add(new CL_CSSParserClanTotalWidth());
	add(new CL_CSSParserClanTotalHeight());
	add(new CL_CSSParserClear());
	add(new CL_CSSParserClip());
	add(new CL_CSSParserColor());
	add(new CL_CSSParserContent());
	add(new CL_CSSParserCounterIncrement());
	add(new CL_CSSParserCounterReset());
	add(new CL_CSSParserCursor());
	add(new CL_CSSParserDirection());
	add(new CL_CSSParserDisplay());
	add(new CL_CSSParserEmptyCells());
	add(new CL_CSSParserFloat());
	add(new CL_CSSParserFont());
	add(new CL_CSSParserFontFamily());
	add(new CL_CSSParserFontSize());
	add(new CL_CSSParserFontStyle());
	add(new CL_CSSParserFontVariant());
	add(new CL_CSSParserFontWeight());
	add(new CL_CSSParserHeight());
	add(new CL_CSSParserLeft());
	add(new CL_CSSParserLetterSpacing());
	add(new CL_CSSParserLineHeight());
	add(new CL_CSSParserListStyle());
	add(new CL_CSSParserListStyleImage());
	add(new CL_CSSParserListStylePosition());
	add(new CL_CSSParserListStyleType());
	add(new CL_CSSParserMargin());
	add(new CL_CSSParserMarginLTRB());
	add(new CL_CSSParserMaxHeight());
	add(new CL_CSSParserMaxWidth());
	add(new CL_CSSParserMinHeight());
	add(new CL_CSSParserMinWidth());
	add(new CL_CSSParserOrphans());
	add(new CL_CSSParserOutline());
	add(new CL_CSSParserOutlineColor());
	add(new CL_CSSParserOutlineStyle());
	add(new CL_CSSParserOutlineWidth());
	add(new CL_CSSParserOverflow());
	add(new CL_CSSParserPadding());
	add(new CL_CSSParserPaddingLTRB());
	add(new CL_CSSParserPageBreakAfter());
	add(new CL_CSSParserPageBreakBefore());
	add(new CL_CSSParserPageBreakInside());
	add(new CL_CSSParserPosition());
	add(new CL_CSSParserQuotes());
	add(new CL_CSSParserRight());
	add(new CL_CSSParserTableLayout());
	add(new CL_CSSParserTextAlign());
	add(new CL_CSSParserTextDecoration());
	add(new CL_CSSParserTextIndent());
	add(new CL_CSSParserTextTransform());
	add(new CL_CSSParserTop());
	add(new CL_CSSParserUnicodeBidi());
	add(new CL_CSSParserVerticalAlign());
	add(new CL_CSSParserVisibility());
	add(new CL_CSSParserWhiteSpace());
	add(new CL_CSSParserWidows());
	add(new CL_CSSParserWidth());
	add(new CL_CSSParserWordSpacing());
	add(new CL_CSSParserZIndex());
}

CL_CSSPropertyParsers::~CL_CSSPropertyParsers()
{
	for (size_t i = 0; i < parsers.size(); i++)
		delete parsers[i];
}

void CL_CSSPropertyParsers::parse(CL_CSSBoxProperties &properties, const CL_CSSProperty2 &property)
{
	std::map<CL_String, CL_CSSPropertyParser *>::iterator it = name_to_parser.find(property.get_name());
	if (it != name_to_parser.end())
		it->second->parse(properties, property.get_name(), property.get_value_tokens());
}

void CL_CSSPropertyParsers::add(CL_CSSPropertyParser *parser)
{
	parsers.push_back(parser);
	std::vector<CL_String> names = parser->get_names();
	for (size_t i = 0; i < names.size(); i++)
		name_to_parser[names[i]] = parser;
}

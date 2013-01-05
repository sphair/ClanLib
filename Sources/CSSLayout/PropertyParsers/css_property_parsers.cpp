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
#include "API/CSSLayout/CSSDocument/css_property.h"
#include "AlignContent/css_parser_align_content.h"
#include "AlignItems/css_parser_align_items.h"
#include "AlignSelf/css_parser_align_self.h"
#include "Background/css_parser_background.h"
#include "BackgroundAttachment/css_parser_background_attachment.h"
#include "BackgroundColor/css_parser_background_color.h"
#include "BackgroundImage/css_parser_background_image.h"
#include "BackgroundPosition/css_parser_background_position.h"
#include "BackgroundRepeat/css_parser_background_repeat.h"
#include "BackgroundClip/css_parser_background_clip.h"
#include "BackgroundOrigin/css_parser_background_origin.h"
#include "BackgroundSize/css_parser_background_size.h"
#include "Border/css_parser_border.h"
#include "BorderCollapse/css_parser_border_collapse.h"
#include "BorderColor/css_parser_border_color.h"
#include "BorderLTRB/css_parser_border_ltrb.h"
#include "BorderLTRBColor/css_parser_border_ltrb_color.h"
#include "BorderLTRBStyle/css_parser_border_ltrb_style.h"
#include "BorderLTRBWidth/css_parser_border_ltrb_width.h"
#include "BorderSpacing/css_parser_border_spacing.h"
#include "BorderStyle/css_parser_border_style.h"
#include "BorderWidth/css_parser_border_width.h"
#include "BorderRadius/css_parser_border_radius.h"
#include "BorderRadiusCorner/css_parser_border_radius_corner.h"
#include "BorderImage/css_parser_border_image.h"
#include "BorderImageOutset/css_parser_border_image_outset.h"
#include "BorderImageRepeat/css_parser_border_image_repeat.h"
#include "BorderImageSlice/css_parser_border_image_slice.h"
#include "BorderImageSource/css_parser_border_image_source.h"
#include "BorderImageWidth/css_parser_border_image_width.h"
#include "DecorationBreak/css_parser_decoration_break.h"
#include "Shadow/css_parser_shadow.h"
#include "Bottom/css_parser_bottom.h"
#include "CaptionSide/css_parser_caption_side.h"
#include "Flex/css_parser_flex.h"
#include "FlexBasis/css_parser_flex_basis.h"
#include "FlexDirection/css_parser_flex_direction.h"
#include "FlexFlow/css_parser_flex_flow.h"
#include "FlexShrink/css_parser_flex_shrink.h"
#include "FlexGrow/css_parser_flex_grow.h"
#include "FlexWrap/css_parser_flex_wrap.h"
#include "Clear/css_parser_clear.h"
#include "Clip/css_parser_clip.h"
#include "Color/css_parser_color.h"
#include "Content/css_parser_content.h"
#include "CounterIncrement/css_parser_counter_increment.h"
#include "CounterReset/css_parser_counter_reset.h"
#include "Cursor/css_parser_cursor.h"
#include "Direction/css_parser_direction.h"
#include "Display/css_parser_display.h"
#include "EmptyCells/css_parser_empty_cells.h"
#include "Float/css_parser_float.h"
#include "Font/css_parser_font.h"
#include "FontFamily/css_parser_font_family.h"
#include "FontSize/css_parser_font_size.h"
#include "FontStyle/css_parser_font_style.h"
#include "FontVariant/css_parser_font_variant.h"
#include "FontWeight/css_parser_font_weight.h"
#include "Height/css_parser_height.h"
#include "JustifyContent/css_parser_justify_content.h"
#include "Left/css_parser_left.h"
#include "LetterSpacing/css_parser_letter_spacing.h"
#include "LineHeight/css_parser_line_height.h"
#include "ListStyle/css_parser_list_style.h"
#include "ListStyleImage/css_parser_list_style_image.h"
#include "ListStylePosition/css_parser_list_style_position.h"
#include "ListStyleType/css_parser_list_style_type.h"
#include "Margin/css_parser_margin.h"
#include "MarginLTRB/css_parser_margin_ltrb.h"
#include "MaxHeight/css_parser_max_height.h"
#include "MaxWidth/css_parser_max_width.h"
#include "MinHeight/css_parser_min_height.h"
#include "MinWidth/css_parser_min_width.h"
#include "Order/css_parser_order.h"
#include "Orphans/css_parser_orphans.h"
#include "Outline/css_parser_outline.h"
#include "OutlineColor/css_parser_outline_color.h"
#include "OutlineStyle/css_parser_outline_style.h"
#include "OutlineWidth/css_parser_outline_width.h"
#include "Overflow/css_parser_overflow.h"
#include "Padding/css_parser_padding.h"
#include "PaddingLTRB/css_parser_padding_ltrb.h"
#include "PageBreakAfter/css_parser_page_break_after.h"
#include "PageBreakBefore/css_parser_page_break_before.h"
#include "PageBreakInside/css_parser_page_break_inside.h"
#include "Position/css_parser_position.h"
#include "Quotes/css_parser_quotes.h"
#include "Right/css_parser_right.h"
#include "TableLayout/css_parser_table_layout.h"
#include "TextAlign/css_parser_text_align.h"
#include "TextDecoration/css_parser_text_decoration.h"
#include "TextIndent/css_parser_text_indent.h"
#include "TextTransform/css_parser_text_transform.h"
#include "Top/css_parser_top.h"
#include "UnicodeBidi/css_parser_unicode_bidi.h"
#include "VerticalAlign/css_parser_vertical_align.h"
#include "Visibility/css_parser_visibility.h"
#include "WhiteSpace/css_parser_white_space.h"
#include "Widows/css_parser_widows.h"
#include "Width/css_parser_width.h"
#include "WordSpacing/css_parser_word_spacing.h"
#include "ZIndex/css_parser_z_index.h"

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
	add(new CSSParserFlexDirection());
	add(new CSSParserFlexWrap());
	add(new CSSParserFlexFlow());
	add(new CSSParserOrder());
	add(new CSSParserFlex());
	add(new CSSParserFlexGrow());
	add(new CSSParserFlexShrink());
	add(new CSSParserFlexBasis());
	add(new CSSParserJustifyContent());
	add(new CSSParserAlignItems());
	add(new CSSParserAlignSelf());
	add(new CSSParserAlignContent());
}

CSSPropertyParsers::~CSSPropertyParsers()
{
	for (size_t i = 0; i < parsers.size(); i++)
		delete parsers[i];
}

void CSSPropertyParsers::parse(CSSBoxProperties &properties, const CSSProperty &property, std::map<std::string, CSSPropertyValue *> *out_change_set)
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

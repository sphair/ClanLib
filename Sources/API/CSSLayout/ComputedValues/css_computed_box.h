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

#include "../api_csslayout.h"
#include "../PropertyValues/css_value_align_content.h"
#include "../PropertyValues/css_value_align_items.h"
#include "../PropertyValues/css_value_align_self.h"
#include "../PropertyValues/css_value_background_attachment.h"
#include "../PropertyValues/css_value_background_color.h"
#include "../PropertyValues/css_value_background_image.h"
#include "../PropertyValues/css_value_background_position.h"
#include "../PropertyValues/css_value_background_repeat.h"
#include "../PropertyValues/css_value_background_origin.h"
#include "../PropertyValues/css_value_background_clip.h"
#include "../PropertyValues/css_value_background_size.h"
#include "../PropertyValues/css_value_border_collapse.h"
#include "../PropertyValues/css_value_border_color.h"
#include "../PropertyValues/css_value_border_spacing.h"
#include "../PropertyValues/css_value_border_style.h"
#include "../PropertyValues/css_value_border_width.h"
#include "../PropertyValues/css_value_border_radius.h"
#include "../PropertyValues/css_value_border_image_source.h"
#include "../PropertyValues/css_value_border_image_slice.h"
#include "../PropertyValues/css_value_border_image_width.h"
#include "../PropertyValues/css_value_border_image_outset.h"
#include "../PropertyValues/css_value_border_image_repeat.h"
#include "../PropertyValues/css_value_decoration_break.h"
#include "../PropertyValues/css_value_shadow.h"
#include "../PropertyValues/css_value_bottom.h"
#include "../PropertyValues/css_value_caption_side.h"
#include "../PropertyValues/css_value_flex_basis.h"
#include "../PropertyValues/css_value_flex_direction.h"
#include "../PropertyValues/css_value_flex_wrap.h"
#include "../PropertyValues/css_value_flex_grow.h"
#include "../PropertyValues/css_value_flex_shrink.h"
#include "../PropertyValues/css_value_clear.h"
#include "../PropertyValues/css_value_clip.h"
#include "../PropertyValues/css_value_color.h"
#include "../PropertyValues/css_value_counter_increment.h"
#include "../PropertyValues/css_value_counter_reset.h"
#include "../PropertyValues/css_value_content.h"
#include "../PropertyValues/css_value_cursor.h"
#include "../PropertyValues/css_value_order.h"
#include "../PropertyValues/css_value_direction.h"
#include "../PropertyValues/css_value_display.h"
#include "../PropertyValues/css_value_empty_cells.h"
#include "../PropertyValues/css_value_float.h"
#include "../PropertyValues/css_value_font_family.h"
#include "../PropertyValues/css_value_font_size.h"
#include "../PropertyValues/css_value_font_style.h"
#include "../PropertyValues/css_value_font_variant.h"
#include "../PropertyValues/css_value_font_weight.h"
#include "../PropertyValues/css_value_height.h"
#include "../PropertyValues/css_value_justify_content.h"
#include "../PropertyValues/css_value_left.h"
#include "../PropertyValues/css_value_letter_spacing.h"
#include "../PropertyValues/css_value_line_height.h"
#include "../PropertyValues/css_value_list_style_image.h"
#include "../PropertyValues/css_value_list_style_position.h"
#include "../PropertyValues/css_value_list_style_type.h"
#include "../PropertyValues/css_value_margin_width.h"
#include "../PropertyValues/css_value_max_height.h"
#include "../PropertyValues/css_value_max_width.h"
#include "../PropertyValues/css_value_min_height.h"
#include "../PropertyValues/css_value_min_width.h"
#include "../PropertyValues/css_value_orphans.h"
#include "../PropertyValues/css_value_outline_color.h"
#include "../PropertyValues/css_value_outline_style.h"
#include "../PropertyValues/css_value_outline_width.h"
#include "../PropertyValues/css_value_overflow.h"
#include "../PropertyValues/css_value_padding_width.h"
#include "../PropertyValues/css_value_page_break_after.h"
#include "../PropertyValues/css_value_page_break_before.h"
#include "../PropertyValues/css_value_page_break_inside.h"
#include "../PropertyValues/css_value_position.h"
#include "../PropertyValues/css_value_quotes.h"
#include "../PropertyValues/css_value_right.h"
#include "../PropertyValues/css_value_table_layout.h"
#include "../PropertyValues/css_value_text_align.h"
#include "../PropertyValues/css_value_text_decoration.h"
#include "../PropertyValues/css_value_text_indent.h"
#include "../PropertyValues/css_value_text_transform.h"
#include "../PropertyValues/css_value_top.h"
#include "../PropertyValues/css_value_unicode_bidi.h"
#include "../PropertyValues/css_value_vertical_align.h"
#include "../PropertyValues/css_value_visibility.h"
#include "../PropertyValues/css_value_white_space.h"
#include "../PropertyValues/css_value_widows.h"
#include "../PropertyValues/css_value_width.h"
#include "../PropertyValues/css_value_word_spacing.h"
#include "../PropertyValues/css_value_z_index.h"
#include <map>

namespace clan
{

class CSSPropertyValue;

class CL_API_CSSLAYOUT CSSComputedBox
{
public:
	CSSValueDisplay display;
	CSSValueWhiteSpace white_space;

	CSSValueFlexDirection flex_direction;
	CSSValueFlexWrap flex_wrap;
	CSSValueOrder order;

	CSSValueFlexGrow flex_grow;
	CSSValueFlexShrink flex_shrink;
	CSSValueFlexBasis flex_basis;

	CSSValueJustifyContent justify_content;
	CSSValueAlignItems align_items;
	CSSValueAlignSelf align_self;
	CSSValueAlignContent align_content;

	CSSValueMarginWidth margin_width_left;
	CSSValueMarginWidth margin_width_top;
	CSSValueMarginWidth margin_width_right;
	CSSValueMarginWidth margin_width_bottom;

	CSSValueBorderColor border_color_left;
	CSSValueBorderColor border_color_top;
	CSSValueBorderColor border_color_right;
	CSSValueBorderColor border_color_bottom;

	CSSValueBorderStyle border_style_left;
	CSSValueBorderStyle border_style_top;
	CSSValueBorderStyle border_style_right;
	CSSValueBorderStyle border_style_bottom;

	CSSValueBorderWidth border_width_left;
	CSSValueBorderWidth border_width_top;
	CSSValueBorderWidth border_width_right;
	CSSValueBorderWidth border_width_bottom;

	CSSValueBorderRadius border_radius_top_right;
	CSSValueBorderRadius border_radius_bottom_right;
	CSSValueBorderRadius border_radius_bottom_left;
	CSSValueBorderRadius border_radius_top_left;

	CSSValueBorderImageSource border_image_source;
	CSSValueBorderImageSlice border_image_slice;
	CSSValueBorderImageWidth border_image_width;
	CSSValueBorderImageOutset border_image_outset;
	CSSValueBorderImageRepeat border_image_repeat;

	CSSValueDecorationBreak decoration_break;
	CSSValueShadow shadow;

	CSSValueBackgroundAttachment background_attachment;
	CSSValueBackgroundColor background_color;
	CSSValueBackgroundImage background_image;
	CSSValueBackgroundPosition background_position;
	CSSValueBackgroundRepeat background_repeat;
	CSSValueBackgroundOrigin background_origin;
	CSSValueBackgroundClip background_clip;
	CSSValueBackgroundSize background_size;

	CSSValuePaddingWidth padding_width_left;
	CSSValuePaddingWidth padding_width_top;
	CSSValuePaddingWidth padding_width_right;
	CSSValuePaddingWidth padding_width_bottom;

	CSSValueWidth width;
	CSSValueHeight height;
	CSSValueMaxWidth max_width;
	CSSValueMaxHeight max_height;
	CSSValueMinWidth min_width;
	CSSValueMinHeight min_height;

	CSSValuePosition position;
	CSSValueZIndex z_index;
	CSSValueLeft left;
	CSSValueTop top;
	CSSValueRight right;
	CSSValueBottom bottom;
	CSSValueFloat float_box;
	CSSValueClear clear;
	CSSValueClip clip;
	CSSValueOverflow overflow;
	CSSValueVisibility visibility;

	CSSValueCounterIncrement counter_increment;
	CSSValueCounterReset counter_reset;

	CSSValueCursor cursor;
	CSSValueOutlineColor outline_color;
	CSSValueOutlineStyle outline_style;
	CSSValueOutlineWidth outline_width;

	CSSValueContent content;

	CSSValueColor color;
	CSSValueTextDecoration text_decoration;
	CSSValueTextIndent text_indent;
	CSSValueTextTransform text_transform;
	CSSValueTextAlign text_align;
	CSSValueFontFamily font_family;
	CSSValueFontSize font_size;
	CSSValueFontStyle font_style;
	CSSValueFontVariant font_variant;
	CSSValueFontWeight font_weight;
	CSSValueLetterSpacing letter_spacing;
	CSSValueWordSpacing word_spacing;
	CSSValueLineHeight line_height;
	CSSValueQuotes quotes;
	CSSValueDirection direction;
	CSSValueUnicodeBidi unicode_bidi;

	CSSValueOrphans orphans;
	CSSValueWidows widows;
	CSSValuePageBreakBefore page_break_before;
	CSSValuePageBreakAfter page_break_after;
	CSSValuePageBreakInside page_break_inside;

	CSSValueListStyleType list_style_type;
	CSSValueListStylePosition list_style_position;
	CSSValueListStyleImage list_style_image;

	CSSValueTableLayout table_layout;
	CSSValueCaptionSide caption_side;
	CSSValueBorderCollapse border_collapse;
	CSSValueBorderSpacing border_spacing;
	CSSValueVerticalAlign vertical_align;
	CSSValueEmptyCells empty_cells;

	void compute(const CSSComputedBox *parent, CSSResourceCache *layout);
};

}

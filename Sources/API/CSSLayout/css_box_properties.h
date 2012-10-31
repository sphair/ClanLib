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

#include "PropertyTypes/css_box_background_attachment.h"
#include "PropertyTypes/css_box_background_color.h"
#include "PropertyTypes/css_box_background_image.h"
#include "PropertyTypes/css_box_background_position.h"
#include "PropertyTypes/css_box_background_repeat.h"
#include "PropertyTypes/css_box_background_origin.h"
#include "PropertyTypes/css_box_background_clip.h"
#include "PropertyTypes/css_box_background_size.h"
#include "PropertyTypes/css_box_border_collapse.h"
#include "PropertyTypes/css_box_border_color.h"
#include "PropertyTypes/css_box_border_spacing.h"
#include "PropertyTypes/css_box_border_style.h"
#include "PropertyTypes/css_box_border_width.h"
#include "PropertyTypes/css_box_border_radius.h"
#include "PropertyTypes/css_box_border_image_source.h"
#include "PropertyTypes/css_box_border_image_slice.h"
#include "PropertyTypes/css_box_border_image_width.h"
#include "PropertyTypes/css_box_border_image_outset.h"
#include "PropertyTypes/css_box_border_image_repeat.h"
#include "PropertyTypes/css_box_decoration_break.h"
#include "PropertyTypes/css_box_shadow.h"
#include "PropertyTypes/css_box_bottom.h"
#include "PropertyTypes/css_box_caption_side.h"
#include "PropertyTypes/css_box_clan_box_direction.h"
#include "PropertyTypes/css_box_clan_box_sizing_factor.h"
#include "PropertyTypes/css_box_clear.h"
#include "PropertyTypes/css_box_clip.h"
#include "PropertyTypes/css_box_color.h"
#include "PropertyTypes/css_box_counter_increment.h"
#include "PropertyTypes/css_box_counter_reset.h"
#include "PropertyTypes/css_box_content.h"
#include "PropertyTypes/css_box_cursor.h"
#include "PropertyTypes/css_box_direction.h"
#include "PropertyTypes/css_box_display.h"
#include "PropertyTypes/css_box_empty_cells.h"
#include "PropertyTypes/css_box_float.h"
#include "PropertyTypes/css_box_font_family.h"
#include "PropertyTypes/css_box_font_size.h"
#include "PropertyTypes/css_box_font_style.h"
#include "PropertyTypes/css_box_font_variant.h"
#include "PropertyTypes/css_box_font_weight.h"
#include "PropertyTypes/css_box_height.h"
#include "PropertyTypes/css_box_left.h"
#include "PropertyTypes/css_box_letter_spacing.h"
#include "PropertyTypes/css_box_line_height.h"
#include "PropertyTypes/css_box_list_style_image.h"
#include "PropertyTypes/css_box_list_style_position.h"
#include "PropertyTypes/css_box_list_style_type.h"
#include "PropertyTypes/css_box_margin_width.h"
#include "PropertyTypes/css_box_max_height.h"
#include "PropertyTypes/css_box_max_width.h"
#include "PropertyTypes/css_box_min_height.h"
#include "PropertyTypes/css_box_min_width.h"
#include "PropertyTypes/css_box_orphans.h"
#include "PropertyTypes/css_box_outline_color.h"
#include "PropertyTypes/css_box_outline_style.h"
#include "PropertyTypes/css_box_outline_width.h"
#include "PropertyTypes/css_box_overflow.h"
#include "PropertyTypes/css_box_padding_width.h"
#include "PropertyTypes/css_box_page_break_after.h"
#include "PropertyTypes/css_box_page_break_before.h"
#include "PropertyTypes/css_box_page_break_inside.h"
#include "PropertyTypes/css_box_position.h"
#include "PropertyTypes/css_box_quotes.h"
#include "PropertyTypes/css_box_right.h"
#include "PropertyTypes/css_box_table_layout.h"
#include "PropertyTypes/css_box_text_align.h"
#include "PropertyTypes/css_box_text_decoration.h"
#include "PropertyTypes/css_box_text_indent.h"
#include "PropertyTypes/css_box_text_transform.h"
#include "PropertyTypes/css_box_top.h"
#include "PropertyTypes/css_box_unicode_bidi.h"
#include "PropertyTypes/css_box_vertical_align.h"
#include "PropertyTypes/css_box_visibility.h"
#include "PropertyTypes/css_box_white_space.h"
#include "PropertyTypes/css_box_widows.h"
#include "PropertyTypes/css_box_width.h"
#include "PropertyTypes/css_box_word_spacing.h"
#include "PropertyTypes/css_box_z_index.h"
#include <map>

namespace clan
{

class CSSPropertyList;

class CSSBoxProperties
{
public:
	CSSBoxDisplay display;
	CSSBoxWhiteSpace white_space;

	CSSBoxClanBoxDirection clan_box_direction;
	CSSBoxClanBoxSizingFactor clan_box_width_expand_factor;
	CSSBoxClanBoxSizingFactor clan_box_height_expand_factor;
	CSSBoxClanBoxSizingFactor clan_box_width_shrink_factor;
	CSSBoxClanBoxSizingFactor clan_box_height_shrink_factor;

	CSSBoxMarginWidth margin_width_left;
	CSSBoxMarginWidth margin_width_top;
	CSSBoxMarginWidth margin_width_right;
	CSSBoxMarginWidth margin_width_bottom;

	CSSBoxBorderColor border_color_left;
	CSSBoxBorderColor border_color_top;
	CSSBoxBorderColor border_color_right;
	CSSBoxBorderColor border_color_bottom;

	CSSBoxBorderStyle border_style_left;
	CSSBoxBorderStyle border_style_top;
	CSSBoxBorderStyle border_style_right;
	CSSBoxBorderStyle border_style_bottom;

	CSSBoxBorderWidth border_width_left;
	CSSBoxBorderWidth border_width_top;
	CSSBoxBorderWidth border_width_right;
	CSSBoxBorderWidth border_width_bottom;

	CSSBoxBorderRadius border_radius_top_right;
	CSSBoxBorderRadius border_radius_bottom_right;
	CSSBoxBorderRadius border_radius_bottom_left;
	CSSBoxBorderRadius border_radius_top_left;

	CSSBoxBorderImageSource border_image_source;
	CSSBoxBorderImageSlice border_image_slice;
	CSSBoxBorderImageWidth border_image_width;
	CSSBoxBorderImageOutset border_image_outset;
	CSSBoxBorderImageRepeat border_image_repeat;

	CSSBoxDecorationBreak decoration_break;
	CSSBoxShadow shadow;

	CSSBoxBackgroundAttachment background_attachment;
	CSSBoxBackgroundColor background_color;
	CSSBoxBackgroundImage background_image;
	CSSBoxBackgroundPosition background_position;
	CSSBoxBackgroundRepeat background_repeat;
	CSSBoxBackgroundOrigin background_origin;
	CSSBoxBackgroundClip background_clip;
	CSSBoxBackgroundSize background_size;

	CSSBoxPaddingWidth padding_width_left;
	CSSBoxPaddingWidth padding_width_top;
	CSSBoxPaddingWidth padding_width_right;
	CSSBoxPaddingWidth padding_width_bottom;

	CSSBoxWidth width;
	CSSBoxHeight height;
	CSSBoxMaxWidth max_width;
	CSSBoxMaxHeight max_height;
	CSSBoxMinWidth min_width;
	CSSBoxMinHeight min_height;

	CSSBoxPosition position;
	CSSBoxZIndex z_index;
	CSSBoxLeft left;
	CSSBoxTop top;
	CSSBoxRight right;
	CSSBoxBottom bottom;
	CSSBoxFloat float_box;
	CSSBoxClear clear;
	CSSBoxClip clip;
	CSSBoxOverflow overflow;
	CSSBoxVisibility visibility;

	CSSBoxCounterIncrement counter_increment;
	CSSBoxCounterReset counter_reset;

	CSSBoxCursor cursor;
	CSSBoxOutlineColor outline_color;
	CSSBoxOutlineStyle outline_style;
	CSSBoxOutlineWidth outline_width;

	CSSBoxContent content;

	CSSBoxColor color;
	CSSBoxTextDecoration text_decoration;
	CSSBoxTextIndent text_indent;
	CSSBoxTextTransform text_transform;
	CSSBoxTextAlign text_align;
	CSSBoxFontFamily font_family;
	CSSBoxFontSize font_size;
	CSSBoxFontStyle font_style;
	CSSBoxFontVariant font_variant;
	CSSBoxFontWeight font_weight;
	CSSBoxLetterSpacing letter_spacing;
	CSSBoxWordSpacing word_spacing;
	CSSBoxLineHeight line_height;
	CSSBoxQuotes quotes;
	CSSBoxDirection direction;
	CSSBoxUnicodeBidi unicode_bidi;

	CSSBoxOrphans orphans;
	CSSBoxWidows widows;
	CSSBoxPageBreakBefore page_break_before;
	CSSBoxPageBreakAfter page_break_after;
	CSSBoxPageBreakInside page_break_inside;

	CSSBoxListStyleType list_style_type;
	CSSBoxListStylePosition list_style_position;
	CSSBoxListStyleImage list_style_image;

	CSSBoxTableLayout table_layout;
	CSSBoxCaptionSide caption_side;
	CSSBoxBorderCollapse border_collapse;
	CSSBoxBorderSpacing border_spacing;
	CSSBoxVerticalAlign vertical_align;
	CSSBoxEmptyCells empty_cells;

	void apply_properties(const std::string &style_string, std::map<std::string, CSSBoxProperty *> *out_change_set = 0);
	void apply_properties(const CSSPropertyList &css_properties, std::map<std::string, CSSBoxProperty *> *out_change_set = 0);

	void compute(const CSSBoxProperties *parent, CSSResourceCache *layout);
};

}

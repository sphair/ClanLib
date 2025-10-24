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

class CL_CSSPropertyList2;

class CL_CSSBoxProperties
{
public:
	CL_CSSBoxDisplay display;
	CL_CSSBoxWhiteSpace white_space;

	CL_CSSBoxMarginWidth margin_width_left;
	CL_CSSBoxMarginWidth margin_width_top;
	CL_CSSBoxMarginWidth margin_width_right;
	CL_CSSBoxMarginWidth margin_width_bottom;

	CL_CSSBoxBorderColor border_color_left;
	CL_CSSBoxBorderColor border_color_top;
	CL_CSSBoxBorderColor border_color_right;
	CL_CSSBoxBorderColor border_color_bottom;

	CL_CSSBoxBorderStyle border_style_left;
	CL_CSSBoxBorderStyle border_style_top;
	CL_CSSBoxBorderStyle border_style_right;
	CL_CSSBoxBorderStyle border_style_bottom;

	CL_CSSBoxBorderWidth border_width_left;
	CL_CSSBoxBorderWidth border_width_top;
	CL_CSSBoxBorderWidth border_width_right;
	CL_CSSBoxBorderWidth border_width_bottom;

	CL_CSSBoxBorderRadius border_radius_top_right;
	CL_CSSBoxBorderRadius border_radius_bottom_right;
	CL_CSSBoxBorderRadius border_radius_bottom_left;
	CL_CSSBoxBorderRadius border_radius_top_left;

	CL_CSSBoxBorderImageSource border_image_source;
	CL_CSSBoxBorderImageSlice border_image_slice;
	CL_CSSBoxBorderImageWidth border_image_width;
	CL_CSSBoxBorderImageOutset border_image_outset;
	CL_CSSBoxBorderImageRepeat border_image_repeat;

	CL_CSSBoxDecorationBreak decoration_break;
	CL_CSSBoxShadow shadow;

	CL_CSSBoxBackgroundAttachment background_attachment;
	CL_CSSBoxBackgroundColor background_color;
	CL_CSSBoxBackgroundImage background_image;
	CL_CSSBoxBackgroundPosition background_position;
	CL_CSSBoxBackgroundRepeat background_repeat;
	CL_CSSBoxBackgroundOrigin background_origin;
	CL_CSSBoxBackgroundClip background_clip;
	CL_CSSBoxBackgroundSize background_size;

	CL_CSSBoxPaddingWidth padding_width_left;
	CL_CSSBoxPaddingWidth padding_width_top;
	CL_CSSBoxPaddingWidth padding_width_right;
	CL_CSSBoxPaddingWidth padding_width_bottom;

	CL_CSSBoxWidth width;
	CL_CSSBoxHeight height;
	CL_CSSBoxMaxWidth max_width;
	CL_CSSBoxMaxHeight max_height;
	CL_CSSBoxMinWidth min_width;
	CL_CSSBoxMinHeight min_height;

	CL_CSSBoxPosition position;
	CL_CSSBoxZIndex z_index;
	CL_CSSBoxLeft left;
	CL_CSSBoxTop top;
	CL_CSSBoxRight right;
	CL_CSSBoxBottom bottom;
	CL_CSSBoxFloat float_box;
	CL_CSSBoxClear clear;
	CL_CSSBoxClip clip;
	CL_CSSBoxOverflow overflow;
	CL_CSSBoxVisibility visibility;

	CL_CSSBoxCounterIncrement counter_increment;
	CL_CSSBoxCounterReset counter_reset;

	CL_CSSBoxCursor cursor;
	CL_CSSBoxOutlineColor outline_color;
	CL_CSSBoxOutlineStyle outline_style;
	CL_CSSBoxOutlineWidth outline_width;

	CL_CSSBoxContent content;

	CL_CSSBoxColor color;
	CL_CSSBoxTextDecoration text_decoration;
	CL_CSSBoxTextIndent text_indent;
	CL_CSSBoxTextTransform text_transform;
	CL_CSSBoxTextAlign text_align;
	CL_CSSBoxFontFamily font_family;
	CL_CSSBoxFontSize font_size;
	CL_CSSBoxFontStyle font_style;
	CL_CSSBoxFontVariant font_variant;
	CL_CSSBoxFontWeight font_weight;
	CL_CSSBoxLetterSpacing letter_spacing;
	CL_CSSBoxWordSpacing word_spacing;
	CL_CSSBoxLineHeight line_height;
	CL_CSSBoxQuotes quotes;
	CL_CSSBoxDirection direction;
	CL_CSSBoxUnicodeBidi unicode_bidi;

	CL_CSSBoxOrphans orphans;
	CL_CSSBoxWidows widows;
	CL_CSSBoxPageBreakBefore page_break_before;
	CL_CSSBoxPageBreakAfter page_break_after;
	CL_CSSBoxPageBreakInside page_break_inside;

	CL_CSSBoxListStyleType list_style_type;
	CL_CSSBoxListStylePosition list_style_position;
	CL_CSSBoxListStyleImage list_style_image;

	CL_CSSBoxTableLayout table_layout;
	CL_CSSBoxCaptionSide caption_side;
	CL_CSSBoxBorderCollapse border_collapse;
	CL_CSSBoxBorderSpacing border_spacing;
	CL_CSSBoxVerticalAlign vertical_align;
	CL_CSSBoxEmptyCells empty_cells;

	void apply_properties(const CL_String &style_string, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set = 0);
	void apply_properties(const CL_CSSPropertyList2 &css_properties, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set = 0);

	void compute(const CL_CSSBoxProperties *parent, CL_CSSResourceCache *layout);
};

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
#include "API/CSSLayout/css_box_properties.h"
#include "API/CSSLayout/css_document2.h"
#include "API/CSSLayout/css_property_list2.h"
#include "css_property_parsers.h"

namespace clan
{

void CSSBoxProperties::apply_properties(const std::string &style_string, std::map<std::string, CSSBoxProperty *> *out_change_set)
{
	apply_properties(CSSDocument::get_style_properties(style_string), out_change_set);
}

void CSSBoxProperties::apply_properties(const CSSPropertyList &css_properties, std::map<std::string, CSSBoxProperty *> *out_change_set)
{
	CSSPropertyParsers property_parsers;
	for (size_t i = css_properties.size(); i > 0; i--)
		property_parsers.parse(*this, css_properties[i-1], out_change_set);
}

void CSSBoxProperties::compute(const CSSBoxProperties *parent, CSSResourceCache *layout)
{
	bool is_containing_block_width_auto = false;
	bool is_containing_block_height_auto = false;
	bool is_containing_block_ltr = true;

	// The order of the computations here is important since some of them rely on other properties having been computed first.

	if (parent)
	{
		float em_size = parent->font_size.length.value;
		float ex_size = em_size * 0.5f; // Allowed by the standard, but should ideally be queried from the font somehow
		font_size.compute(&parent->font_size, layout, em_size, ex_size);
		em_size = font_size.length.value;
		ex_size = em_size * 0.5f;

		width.compute(&parent->width, layout, em_size, ex_size, is_containing_block_width_auto);
		height.compute(&parent->height, layout, em_size, ex_size, is_containing_block_height_auto);
		color.compute(&parent->color, layout, em_size, ex_size);
		position.compute(&parent->position, layout, em_size, ex_size);
		float_box.compute(&parent->float_box, layout, em_size, ex_size);
		display.compute(&parent->display, layout, em_size, ex_size, position, float_box);
		CSSBoxRight::compute(left, right, parent, layout, em_size, ex_size, position, is_containing_block_ltr);
		CSSBoxBottom::compute(top, bottom, parent, layout, em_size, ex_size, position);

		white_space.compute(&parent->white_space, layout, em_size, ex_size);
		margin_width_left.compute(&parent->margin_width_left, layout, em_size, ex_size);
		margin_width_top.compute(&parent->margin_width_top, layout, em_size, ex_size);
		margin_width_right.compute(&parent->margin_width_right, layout, em_size, ex_size);
		margin_width_bottom.compute(&parent->margin_width_bottom, layout, em_size, ex_size);
		border_color_left.compute(&parent->border_color_left, layout, em_size, ex_size, color.color);
		border_color_top.compute(&parent->border_color_top, layout, em_size, ex_size, color.color);
		border_color_right.compute(&parent->border_color_right, layout, em_size, ex_size, color.color);
		border_color_bottom.compute(&parent->border_color_bottom, layout, em_size, ex_size, color.color);
		border_style_left.compute(&parent->border_style_left, layout, em_size, ex_size);
		border_style_top.compute(&parent->border_style_top, layout, em_size, ex_size);
		border_style_right.compute(&parent->border_style_right, layout, em_size, ex_size);
		border_style_bottom.compute(&parent->border_style_bottom, layout, em_size, ex_size);
		border_width_left.compute(&parent->border_width_left, layout, em_size, ex_size, border_style_left);
		border_width_top.compute(&parent->border_width_top, layout, em_size, ex_size, border_style_top);
		border_width_right.compute(&parent->border_width_right, layout, em_size, ex_size, border_style_right);
		border_width_bottom.compute(&parent->border_width_bottom, layout, em_size, ex_size, border_style_bottom);
		border_radius_top_left.compute(&parent->border_radius_top_left, layout, em_size, ex_size);
		border_radius_top_right.compute(&parent->border_radius_top_right, layout, em_size, ex_size);
		border_radius_bottom_left.compute(&parent->border_radius_bottom_left, layout, em_size, ex_size);
		border_radius_bottom_right.compute(&parent->border_radius_bottom_right, layout, em_size, ex_size);
		border_image_source.compute(&parent->border_image_source, layout, em_size, ex_size);
		border_image_slice.compute(&parent->border_image_slice, layout, em_size, ex_size);
		border_image_width.compute(&parent->border_image_width, layout, em_size, ex_size);
		border_image_outset.compute(&parent->border_image_outset, layout, em_size, ex_size);
		border_image_repeat.compute(&parent->border_image_repeat, layout, em_size, ex_size);
		decoration_break.compute(&parent->decoration_break, layout, em_size, ex_size);
		shadow.compute(&parent->shadow, layout, em_size, ex_size);
		background_attachment.compute(&parent->background_attachment, layout, em_size, ex_size);
		background_color.compute(&parent->background_color, layout, em_size, ex_size);
		background_image.compute(&parent->background_image, layout, em_size, ex_size);
		background_position.compute(&parent->background_position, layout, em_size, ex_size);
		background_repeat.compute(&parent->background_repeat, layout, em_size, ex_size);
		background_origin.compute(&parent->background_origin, layout, em_size, ex_size);
		background_clip.compute(&parent->background_clip, layout, em_size, ex_size);
		background_size.compute(&parent->background_size, layout, em_size, ex_size);
		padding_width_left.compute(&parent->padding_width_left, layout, em_size, ex_size);
		padding_width_top.compute(&parent->padding_width_top, layout, em_size, ex_size);
		padding_width_right.compute(&parent->padding_width_right, layout, em_size, ex_size);
		padding_width_bottom.compute(&parent->padding_width_bottom, layout, em_size, ex_size);
		max_width.compute(&parent->max_width, layout, em_size, ex_size);
		max_height.compute(&parent->max_height, layout, em_size, ex_size);
		min_width.compute(&parent->min_width, layout, em_size, ex_size);
		min_height.compute(&parent->min_height, layout, em_size, ex_size);
		z_index.compute(&parent->z_index, layout, em_size, ex_size);
		clear.compute(&parent->clear, layout, em_size, ex_size);
		clip.compute(&parent->clip, layout, em_size, ex_size);
		overflow.compute(&parent->overflow, layout, em_size, ex_size);
		counter_increment.compute(&parent->counter_increment, layout, em_size, ex_size);
		counter_reset.compute(&parent->counter_reset, layout, em_size, ex_size);
		cursor.compute(&parent->cursor, layout, em_size, ex_size);
		outline_color.compute(&parent->outline_color, layout, em_size, ex_size);
		outline_style.compute(&parent->outline_style, layout, em_size, ex_size);
		outline_width.compute(&parent->outline_width, layout, em_size, ex_size, outline_style);
		content.compute(&parent->content, layout, em_size, ex_size, false);
		text_decoration.compute(&parent->text_decoration, layout, em_size, ex_size);
		text_indent.compute(&parent->text_indent, layout, em_size, ex_size);
		text_transform.compute(&parent->text_transform, layout, em_size, ex_size);
		text_align.compute(&parent->text_align, layout, em_size, ex_size);
		font_family.compute(&parent->font_family, layout, em_size, ex_size);
		font_style.compute(&parent->font_style, layout, em_size, ex_size);
		font_variant.compute(&parent->font_variant, layout, em_size, ex_size);
		font_weight.compute(&parent->font_weight, layout, em_size, ex_size);
		letter_spacing.compute(&parent->letter_spacing, layout, em_size, ex_size);
		word_spacing.compute(&parent->word_spacing, layout, em_size, ex_size);
		line_height.compute(&parent->line_height, layout, em_size, ex_size);
		quotes.compute(&parent->quotes, layout, em_size, ex_size);
		direction.compute(&parent->direction, layout, em_size, ex_size);
		unicode_bidi.compute(&parent->unicode_bidi, layout, em_size, ex_size);
		orphans.compute(&parent->orphans, layout, em_size, ex_size);
		widows.compute(&parent->widows, layout, em_size, ex_size);
		page_break_before.compute(&parent->page_break_before, layout, em_size, ex_size);
		page_break_after.compute(&parent->page_break_after, layout, em_size, ex_size);
		page_break_inside.compute(&parent->page_break_inside, layout, em_size, ex_size);
		list_style_type.compute(&parent->list_style_type, layout, em_size, ex_size);
		list_style_position.compute(&parent->list_style_position, layout, em_size, ex_size);
		list_style_image.compute(&parent->list_style_image, layout, em_size, ex_size);
		table_layout.compute(&parent->table_layout, layout, em_size, ex_size);
		caption_side.compute(&parent->caption_side, layout, em_size, ex_size);
		border_collapse.compute(&parent->border_collapse, layout, em_size, ex_size);
		border_spacing.compute(&parent->border_spacing, layout, em_size, ex_size);
		vertical_align.compute(&parent->vertical_align, layout, em_size, ex_size, line_height);
		visibility.compute(&parent->visibility, layout, em_size, ex_size);
	}
	else
	{
		float em_size = 16.0f;
		float ex_size = em_size * 0.5f;

		font_size.compute(0, layout, em_size, ex_size);
		em_size = font_size.length.value;
		ex_size = em_size * 0.5f;

		width.compute(0, layout, em_size, ex_size, is_containing_block_width_auto);
		height.compute(0, layout, em_size, ex_size, is_containing_block_height_auto);
		color.compute(0, layout, em_size, ex_size);
		position.compute(0, layout, em_size, ex_size);
		float_box.compute(0, layout, em_size, ex_size);
		display.compute(0, layout, em_size, ex_size, position, float_box);
		CSSBoxRight::compute(left, right, 0, layout, em_size, ex_size, position, is_containing_block_ltr);
		CSSBoxBottom::compute(top, bottom, 0, layout, em_size, ex_size, position);

		white_space.compute(0, layout, em_size, ex_size);
		margin_width_left.compute(0, layout, em_size, ex_size);
		margin_width_top.compute(0, layout, em_size, ex_size);
		margin_width_right.compute(0, layout, em_size, ex_size);
		margin_width_bottom.compute(0, layout, em_size, ex_size);
		border_color_left.compute(0, layout, em_size, ex_size, color.color);
		border_color_top.compute(0, layout, em_size, ex_size, color.color);
		border_color_right.compute(0, layout, em_size, ex_size, color.color);
		border_color_bottom.compute(0, layout, em_size, ex_size, color.color);
		border_style_left.compute(0, layout, em_size, ex_size);
		border_style_top.compute(0, layout, em_size, ex_size);
		border_style_right.compute(0, layout, em_size, ex_size);
		border_style_bottom.compute(0, layout, em_size, ex_size);
		border_width_left.compute(0, layout, em_size, ex_size, border_style_left);
		border_width_top.compute(0, layout, em_size, ex_size, border_style_top);
		border_width_right.compute(0, layout, em_size, ex_size, border_style_right);
		border_width_bottom.compute(0, layout, em_size, ex_size, border_style_bottom);
		border_radius_top_left.compute(0, layout, em_size, ex_size);
		border_radius_top_right.compute(0, layout, em_size, ex_size);
		border_radius_bottom_left.compute(0, layout, em_size, ex_size);
		border_radius_bottom_right.compute(0, layout, em_size, ex_size);
		border_image_source.compute(0, layout, em_size, ex_size);
		border_image_slice.compute(0, layout, em_size, ex_size);
		border_image_width.compute(0, layout, em_size, ex_size);
		border_image_outset.compute(0, layout, em_size, ex_size);
		border_image_repeat.compute(0, layout, em_size, ex_size);
		decoration_break.compute(0, layout, em_size, ex_size);
		shadow.compute(0, layout, em_size, ex_size);
		background_attachment.compute(0, layout, em_size, ex_size);
		background_color.compute(0, layout, em_size, ex_size);
		background_image.compute(0, layout, em_size, ex_size);
		background_position.compute(0, layout, em_size, ex_size);
		background_repeat.compute(0, layout, em_size, ex_size);
		background_origin.compute(0, layout, em_size, ex_size);
		background_clip.compute(0, layout, em_size, ex_size);
		background_size.compute(0, layout, em_size, ex_size);
		padding_width_left.compute(0, layout, em_size, ex_size);
		padding_width_top.compute(0, layout, em_size, ex_size);
		padding_width_right.compute(0, layout, em_size, ex_size);
		padding_width_bottom.compute(0, layout, em_size, ex_size);
		max_width.compute(0, layout, em_size, ex_size);
		max_height.compute(0, layout, em_size, ex_size);
		min_width.compute(0, layout, em_size, ex_size);
		min_height.compute(0, layout, em_size, ex_size);
		z_index.compute(0, layout, em_size, ex_size);
		clear.compute(0, layout, em_size, ex_size);
		clip.compute(0, layout, em_size, ex_size);
		overflow.compute(0, layout, em_size, ex_size);
		counter_increment.compute(0, layout, em_size, ex_size);
		counter_reset.compute(0, layout, em_size, ex_size);
		cursor.compute(0, layout, em_size, ex_size);
		outline_color.compute(0, layout, em_size, ex_size);
		outline_style.compute(0, layout, em_size, ex_size);
		outline_width.compute(0, layout, em_size, ex_size, outline_style);
		content.compute(0, layout, em_size, ex_size, false);
		text_decoration.compute(0, layout, em_size, ex_size);
		text_indent.compute(0, layout, em_size, ex_size);
		text_transform.compute(0, layout, em_size, ex_size);
		text_align.compute(0, layout, em_size, ex_size);
		font_family.compute(0, layout, em_size, ex_size);
		font_style.compute(0, layout, em_size, ex_size);
		font_variant.compute(0, layout, em_size, ex_size);
		font_weight.compute(0, layout, em_size, ex_size);
		letter_spacing.compute(0, layout, em_size, ex_size);
		word_spacing.compute(0, layout, em_size, ex_size);
		line_height.compute(0, layout, em_size, ex_size);
		quotes.compute(0, layout, em_size, ex_size);
		direction.compute(0, layout, em_size, ex_size);
		unicode_bidi.compute(0, layout, em_size, ex_size);
		orphans.compute(0, layout, em_size, ex_size);
		widows.compute(0, layout, em_size, ex_size);
		page_break_before.compute(0, layout, em_size, ex_size);
		page_break_after.compute(0, layout, em_size, ex_size);
		page_break_inside.compute(0, layout, em_size, ex_size);
		list_style_type.compute(0, layout, em_size, ex_size);
		list_style_position.compute(0, layout, em_size, ex_size);
		list_style_image.compute(0, layout, em_size, ex_size);
		table_layout.compute(0, layout, em_size, ex_size);
		caption_side.compute(0, layout, em_size, ex_size);
		border_collapse.compute(0, layout, em_size, ex_size);
		border_spacing.compute(0, layout, em_size, ex_size);
		vertical_align.compute(0, layout, em_size, ex_size, line_height);
		visibility.compute(0, layout, em_size, ex_size);
	}
}

}

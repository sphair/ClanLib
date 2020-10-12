/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "UI/precomp.h"
#include "API/UI/Style/property_hash.h"

namespace clan
{

	PropertyHash::PropertyHash(const char *property_name)
	{
		hash = 2166136261U;
		while(true)
		{
			char value = *(property_name++);
			if (!value)
				break;
			hash = (hash * scramble_value) ^ static_cast<std::size_t>(value);
		}
	}

	PropertyHash &PropertyHash::append_index(int value)
	{
		hash = (hash * scramble_value) ^ static_cast<std::size_t>('[');
		if (value >= 10)
			throw clan::Exception("Fixme if required");
		hash = (hash * scramble_value) ^ static_cast<std::size_t>(value + '0');
		hash = (hash * scramble_value) ^ static_cast<std::size_t>(']');
		return *this;
	}

	PropertyHash &PropertyHash::append(const char *text)
	{
		while(true)
		{
			char value = *(text++);
			if (!value)
				break;
			hash = (hash * scramble_value) ^ static_cast<std::size_t>(value);
		}
		return *this;
	}


	PropertyHash PropertyHash::hash_align_content("align-content");
	PropertyHash PropertyHash::hash_align_items("align-items");
	PropertyHash PropertyHash::hash_align_self("align-self");
	PropertyHash PropertyHash::hash_background_attachment("background-attachment");
	PropertyHash PropertyHash::hash_background_clip("background-clip");
	PropertyHash PropertyHash::hash_background_color("background-color");
	PropertyHash PropertyHash::hash_background_image("background-image");
	PropertyHash PropertyHash::hash_background_origin("background-origin");
	PropertyHash PropertyHash::hash_background_position_x("background-position-x");
	PropertyHash PropertyHash::hash_background_position_y("background-position-y");
	PropertyHash PropertyHash::hash_background_repeat_x("background-repeat-x");
	PropertyHash PropertyHash::hash_background_repeat_y("background-repeat-y");
	PropertyHash PropertyHash::hash_background_size_x("background-size-x");
	PropertyHash PropertyHash::hash_background_size_y("background-size-y");
	PropertyHash PropertyHash::hash_border_bottom_left_radius_x("border-bottom-left-radius-x");
	PropertyHash PropertyHash::hash_border_bottom_left_radius_y("border-bottom-left-radius-y");
	PropertyHash PropertyHash::hash_border_bottom_right_radius_x("border-bottom-right-radius-x");
	PropertyHash PropertyHash::hash_border_bottom_right_radius_y("border-bottom-right-radius-y");
	PropertyHash PropertyHash::hash_border_bottom_width("border-bottom-width");
	PropertyHash PropertyHash::hash_border_image_outset_bottom("border-image-outset-bottom");
	PropertyHash PropertyHash::hash_border_image_outset_left("border-image-outset-left");
	PropertyHash PropertyHash::hash_border_image_outset_right("border-image-outset-right");
	PropertyHash PropertyHash::hash_border_image_outset_top("border-image-outset-top");
	PropertyHash PropertyHash::hash_border_image_repeat_x("border-image-repeat-x");
	PropertyHash PropertyHash::hash_border_image_repeat_y("border-image-repeat-y");
	PropertyHash PropertyHash::hash_border_image_slice_bottom("border-image-slice-bottom");
	PropertyHash PropertyHash::hash_border_image_slice_center("border-image-slice-center");
	PropertyHash PropertyHash::hash_border_image_slice_left("border-image-slice-left");
	PropertyHash PropertyHash::hash_border_image_slice_right("border-image-slice-right");
	PropertyHash PropertyHash::hash_border_image_slice_top("border-image-slice-top");
	PropertyHash PropertyHash::hash_border_image_source("border-image-source");
	PropertyHash PropertyHash::hash_border_image_width_bottom("border-image-width-bottom");
	PropertyHash PropertyHash::hash_border_image_width_left("border-image-width-left");
	PropertyHash PropertyHash::hash_border_image_width_right("border-image-width-right");
	PropertyHash PropertyHash::hash_border_image_width_top("border-image-width-top");
	PropertyHash PropertyHash::hash_border_left_width("border-left-width");
	PropertyHash PropertyHash::hash_border_right_width("border-right-width");
	PropertyHash PropertyHash::hash_border_top_color("border-top-color");
	PropertyHash PropertyHash::hash_border_top_left_radius_x("border-top-left-radius-x");
	PropertyHash PropertyHash::hash_border_top_left_radius_y("border-top-left-radius-y");
	PropertyHash PropertyHash::hash_border_top_right_radius_x("border-top-right-radius-x");
	PropertyHash PropertyHash::hash_border_top_right_radius_y("border-top-right-radius-y");
	PropertyHash PropertyHash::hash_border_top_style("border-top-style");
	PropertyHash PropertyHash::hash_border_top_width("border-top-width");
	PropertyHash PropertyHash::hash_bottom("bottom");
	PropertyHash PropertyHash::hash_box_shadow_blur_radius("box-shadow-blur-radius");
	PropertyHash PropertyHash::hash_box_shadow_color("box-shadow-color");
	PropertyHash PropertyHash::hash_box_shadow_horizontal_offset("box-shadow-horizontal-offset");
	PropertyHash PropertyHash::hash_box_shadow_spread_distance("box-shadow-spread-distance");
	PropertyHash PropertyHash::hash_box_shadow_style("box-shadow-style");
	PropertyHash PropertyHash::hash_box_shadow_vertical_offset("box-shadow-vertical-offset");
	PropertyHash PropertyHash::hash_color("color");
	PropertyHash PropertyHash::hash_flex_basis("flex-basis");
	PropertyHash PropertyHash::hash_flex_direction("flex-direction");
	PropertyHash PropertyHash::hash_flex_grow("flex-grow");
	PropertyHash PropertyHash::hash_flex_shrink("flex-shrink");
	PropertyHash PropertyHash::hash_flex_wrap("flex-wrap");
	PropertyHash PropertyHash::hash_font_family_names("font-family-names");
	PropertyHash PropertyHash::hash_font_family_names_X0("font-family-names[0]");
	PropertyHash PropertyHash::hash__clan_font_rendering("-clan-font-rendering");
	PropertyHash PropertyHash::hash_font_size("font-size");
	PropertyHash PropertyHash::hash_font_style("font-style");
	PropertyHash PropertyHash::hash_font_variant("font-variant");
	PropertyHash PropertyHash::hash_font_weight("font-weight");
	PropertyHash PropertyHash::hash_height("height");
	PropertyHash PropertyHash::hash_justify_content("justify-content");
	PropertyHash PropertyHash::hash_layout("layout");
	PropertyHash PropertyHash::hash_left("left");
	PropertyHash PropertyHash::hash_line_height("line-height");
	PropertyHash PropertyHash::hash_margin_bottom("margin-bottom");
	PropertyHash PropertyHash::hash_margin_left("margin-left");
	PropertyHash PropertyHash::hash_margin_right("margin-right");
	PropertyHash PropertyHash::hash_margin_top("margin-top");
	PropertyHash PropertyHash::hash_max_height("max-height");
	PropertyHash PropertyHash::hash_max_width("max-width");
	PropertyHash PropertyHash::hash_min_height("min-height");
	PropertyHash PropertyHash::hash_min_width("min-width");
	PropertyHash PropertyHash::hash_padding_bottom("padding-bottom");
	PropertyHash PropertyHash::hash_padding_left("padding-left");
	PropertyHash PropertyHash::hash_padding_right("padding-right");
	PropertyHash PropertyHash::hash_padding_top("padding-top");
	PropertyHash PropertyHash::hash_position("position");
	PropertyHash PropertyHash::hash_right("right");
	PropertyHash PropertyHash::hash_top("top");
	PropertyHash PropertyHash::hash_visibility("visibility");
	PropertyHash PropertyHash::hash_width("width");
	PropertyHash PropertyHash::hash_background_position("background-position");
	PropertyHash PropertyHash::hash_background_repeat("background-repeat");
	PropertyHash PropertyHash::hash_background_size("background-size");
	PropertyHash PropertyHash::hash_border_bottom_color("border-bottom-color");
	PropertyHash PropertyHash::hash_border_bottom_style("border-bottom-style");
	PropertyHash PropertyHash::hash_border_left_color("border-left-color");
	PropertyHash PropertyHash::hash_border_left_style("border-left-style");
	PropertyHash PropertyHash::hash_border_right_color("border-right-color");
	PropertyHash PropertyHash::hash_border_right_style("border-right-style");
	PropertyHash PropertyHash::hash_box_shadow("box-shadow");
	PropertyHash PropertyHash::hash_font_family("font-family");
	PropertyHash PropertyHash::hash_letter_spacing("letter-spacing");
	PropertyHash PropertyHash::hash_order("order");
	PropertyHash PropertyHash::hash_outline_color("outline-color");
	PropertyHash PropertyHash::hash_outline_style("outline-style");
	PropertyHash PropertyHash::hash_outline_width("outline-width");
	PropertyHash PropertyHash::hash_text_align("text-align");
	PropertyHash PropertyHash::hash_text_decoration_blink("text-decoration-blink");
	PropertyHash PropertyHash::hash_text_decoration_line_through("text-decoration-line-through");
	PropertyHash PropertyHash::hash_text_decoration_overline("text-decoration-overline");
	PropertyHash PropertyHash::hash_text_decoration_underline("text-decoration-underline");
	PropertyHash PropertyHash::hash_text_indent("text-indent");
	PropertyHash PropertyHash::hash_text_transform("text-transform");
	PropertyHash PropertyHash::hash_word_spacing("word-spacing");
	PropertyHash PropertyHash::hash_z_index("z-index");
}

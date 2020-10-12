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
**    Mark Page
*/

#pragma once

namespace clan
{
	class PropertyHash
	{
	public:
		PropertyHash(const char *property_name);
		PropertyHash(const std::string &property_name) : PropertyHash(property_name.c_str()) {}

		PropertyHash &append_index(int value);
		PropertyHash &append(const char *text);

		unsigned int hash;

		bool operator<(const PropertyHash &that) const { return hash < that.hash; }
		bool operator<=(const PropertyHash &that) const { return hash <= that.hash; }
		bool operator==(const PropertyHash &that) const { return hash == that.hash; }
		bool operator!=(const PropertyHash &that) const { return hash != that.hash; }

		static const unsigned int scramble_value = 16777619U;

		static PropertyHash hash_align_content;
		static PropertyHash hash_align_items;
		static PropertyHash hash_align_self;
		static PropertyHash hash_background_attachment;
		static PropertyHash hash_background_clip;
		static PropertyHash hash_background_color;
		static PropertyHash hash_background_image;
		static PropertyHash hash_background_origin;
		static PropertyHash hash_background_position_x;
		static PropertyHash hash_background_position_y;
		static PropertyHash hash_background_repeat_x;
		static PropertyHash hash_background_repeat_y;
		static PropertyHash hash_background_size_x;
		static PropertyHash hash_background_size_y;
		static PropertyHash hash_border_bottom_left_radius_x;
		static PropertyHash hash_border_bottom_left_radius_y;
		static PropertyHash hash_border_bottom_right_radius_x;
		static PropertyHash hash_border_bottom_right_radius_y;
		static PropertyHash hash_border_bottom_width;
		static PropertyHash hash_border_image_outset_bottom;
		static PropertyHash hash_border_image_outset_left;
		static PropertyHash hash_border_image_outset_right;
		static PropertyHash hash_border_image_outset_top;
		static PropertyHash hash_border_image_repeat_x;
		static PropertyHash hash_border_image_repeat_y;
		static PropertyHash hash_border_image_slice_bottom;
		static PropertyHash hash_border_image_slice_center;
		static PropertyHash hash_border_image_slice_left;
		static PropertyHash hash_border_image_slice_right;
		static PropertyHash hash_border_image_slice_top;
		static PropertyHash hash_border_image_source;
		static PropertyHash hash_border_image_width_bottom;
		static PropertyHash hash_border_image_width_left;
		static PropertyHash hash_border_image_width_right;
		static PropertyHash hash_border_image_width_top;
		static PropertyHash hash_border_left_width;
		static PropertyHash hash_border_right_width;
		static PropertyHash hash_border_top_color;
		static PropertyHash hash_border_top_left_radius_x;
		static PropertyHash hash_border_top_left_radius_y;
		static PropertyHash hash_border_top_right_radius_x;
		static PropertyHash hash_border_top_right_radius_y;
		static PropertyHash hash_border_top_style;
		static PropertyHash hash_border_top_width;
		static PropertyHash hash_bottom;
		static PropertyHash hash_box_shadow_blur_radius;
		static PropertyHash hash_box_shadow_color;
		static PropertyHash hash_box_shadow_horizontal_offset;
		static PropertyHash hash_box_shadow_spread_distance;
		static PropertyHash hash_box_shadow_style;
		static PropertyHash hash_box_shadow_vertical_offset;
		static PropertyHash hash_color;
		static PropertyHash hash_flex_basis;
		static PropertyHash hash_flex_direction;
		static PropertyHash hash_flex_grow;
		static PropertyHash hash_flex_shrink;
		static PropertyHash hash_flex_wrap;
		static PropertyHash hash_font_family_names;
		static PropertyHash hash_font_family_names_X0;
		static PropertyHash hash__clan_font_rendering;
		static PropertyHash hash_font_size;
		static PropertyHash hash_font_style;
		static PropertyHash hash_font_variant;
		static PropertyHash hash_font_weight;
		static PropertyHash hash_height;
		static PropertyHash hash_justify_content;
		static PropertyHash hash_layout;
		static PropertyHash hash_left;
		static PropertyHash hash_line_height;
		static PropertyHash hash_margin_bottom;
		static PropertyHash hash_margin_left;
		static PropertyHash hash_margin_right;
		static PropertyHash hash_margin_top;
		static PropertyHash hash_max_height;
		static PropertyHash hash_max_width;
		static PropertyHash hash_min_height;
		static PropertyHash hash_min_width;
		static PropertyHash hash_padding_bottom;
		static PropertyHash hash_padding_left;
		static PropertyHash hash_padding_right;
		static PropertyHash hash_padding_top;
		static PropertyHash hash_position;
		static PropertyHash hash_right;
		static PropertyHash hash_top;
		static PropertyHash hash_visibility;
		static PropertyHash hash_width;
		static PropertyHash hash_background_position;
		static PropertyHash hash_background_repeat;
		static PropertyHash hash_background_size;
		static PropertyHash hash_border_bottom_color;
		static PropertyHash hash_border_bottom_style;
		static PropertyHash hash_border_left_color;
		static PropertyHash hash_border_left_style;
		static PropertyHash hash_border_right_color;
		static PropertyHash hash_border_right_style;
		static PropertyHash hash_box_shadow;
		static PropertyHash hash_font_family;
		static PropertyHash hash_letter_spacing;
		static PropertyHash hash_order;
		static PropertyHash hash_outline_color;
		static PropertyHash hash_outline_style;
		static PropertyHash hash_outline_width;
		static PropertyHash hash_text_align;
		static PropertyHash hash_text_decoration_blink;
		static PropertyHash hash_text_decoration_line_through;
		static PropertyHash hash_text_decoration_overline;
		static PropertyHash hash_text_decoration_underline;
		static PropertyHash hash_text_indent;
		static PropertyHash hash_text_transform;
		static PropertyHash hash_word_spacing;
		static PropertyHash hash_z_index;
	};
}

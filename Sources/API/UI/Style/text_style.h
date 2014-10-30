/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include <string>
#include <memory>

namespace clan
{
	class Colorf;
	class TextStyleImpl;

	class TextStyle
	{
	public:
		TextStyle();
		~TextStyle();

		void set_font(const std::string &family, float size);
		void set_font(const std::string &family, float size, float line_height);
		void set_font_family(const std::string &family);
		void set_size(float size);
		void set_line_height_normal();
		void set_line_height(float height);
		void set_weight_normal();
		void set_weight_bold();
		void set_weight(int weight);
		void set_style_italic();
		void set_style_oblique();
		void set_variant_normal();
		void set_variant_small_caps();
		void set_color(const Colorf &color);
		void set_shadow_none();
		void set_shadow(float horz_offset, float vert_offset, float blur_radius, const Colorf &color);
		void set_unicode_bidi_normal();
		void set_unicode_bidi_embed();
		void set_unicode_bidi_override();
		void set_direction_ltr();
		void set_direction_rtl();
		void set_align_start();
		void set_align_end();
		void set_align_left();
		void set_align_right();
		void set_align_center();
		void set_align_justify();
		void set_align_last_start();
		void set_align_last_end();
		void set_align_last_left();
		void set_align_last_right();
		void set_align_last_center();
		void set_align_last_justify();
		void set_justify_auto();
		void set_justify_none();
		void set_justify_inter_word();
		void set_justify_distribute();
		void set_decoration_none();
		void set_decoration_underline();
		void set_decoration_overline();
		void set_decoration_line_through();
		void set_decoration_color_from_color();
		void set_decoration_color(const Colorf &color);
		void set_decoration_style_solid();
		void set_decoration_style_double();
		void set_decoration_style_dotted();
		void set_decoration_style_dashed();
		void set_decoration_style_wavy();
		void set_decoration_skip_none();
		void set_decoration_skip(bool objects, bool spaces, bool ink, bool edges);
		void set_underline_position_auto();
		void set_underline_position_under();
		void set_underline_position_left();
		void set_underline_position_right();
		void set_emphasis_none();
		void set_emphasis_filled_dot();
		void set_emphasis_filled_circle();
		void set_emphasis_filled_double_circle();
		void set_emphasis_filled_triangle();
		void set_emphasis_filled_sesame();
		void set_emphasis_open_dot();
		void set_emphasis_open_circle();
		void set_emphasis_open_double_circle();
		void set_emphasis_open_triangle();
		void set_emphasis_open_sesame();
		void set_emphasis_color_from_color();
		void set_emphasis_color(const Colorf &color);
		void set_emphasis_position_over_right();
		void set_emphasis_position_under_right();
		void set_emphasis_position_over_left();
		void set_emphasis_position_under_left();
		void set_indent(float indent, bool each_line, bool hanging);
		void set_hanging_none();
		void set_hanging(bool first, bool force_end, bool allow_end, bool last);
		void set_transform_none();
		void set_transform_capitalize();
		void set_transform_uppercase();
		void set_transform_lowercase();
		void set_transform_full_width();
		void set_vertical_align_baseline();
		void set_vertical_align_sub();
		void set_vertical_align_super();
		void set_vertical_align_top();
		void set_vertical_align_text_top();
		void set_vertical_align_middle();
		void set_vertical_align_bottom();
		void set_vertical_align_text_bottom();
		void set_vertical_align(float offset);
		void set_whitespace_collapse();
		void set_whitespace_pre();
		void set_whitespace_nowrap();
		void set_whitespace_pre_wrap();
		void set_whitespace_pre_line();
		void set_character_spacing_normal();
		void set_character_spacing(float length);
		void set_word_spacing_normal();
		void set_word_spacing(float length);
		void set_word_spacing_percentage(float percentage);
		void set_tab_size_spaces(int count);
		void set_tab_size_length(float length);
		void set_line_break_auto();
		void set_line_break_loose();
		void set_line_break_normal();
		void set_line_break_strict();
		void set_word_break_normal();
		void set_word_break_keep_all();
		void set_word_break_break_all();
		void set_hyphens_none();
		void set_hyphens_manual();
		void set_hyphens_auto();
		void set_overflow_wrap_normal();
		void set_overflow_wrap_break_word();

	private:
		std::shared_ptr<TextStyleImpl> impl;
	};
}

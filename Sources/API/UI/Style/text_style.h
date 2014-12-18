/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Display/Font/font_description.h"

namespace clan
{
	class Font;
	class Canvas;
	class Colorf;
	class TextStyleImpl;
	class TextBlockStyleImpl;

	class TextStyle
	{
	public:
		TextStyle();

		void set_font(const std::string &family, float size);
		void set_font(const std::string &family, float size, float line_height);

		void set_font_family(const std::string &family);
		std::string family() const;

		void set_size(float size);
		float size() const;

		void set_line_height_normal();
		void set_line_height(float height);
		float line_height() const;

		void set_weight_normal();
		void set_weight_bold();
		void set_weight(FontWeight weight);
		FontWeight weight() const;

		void set_style_normal();
		void set_style_italic();
		void set_style_oblique();
		bool is_style_normal() const;
		bool is_style_italic() const;
		bool is_style_oblique() const;

		//void set_variant_normal();
		//void set_variant_small_caps();

		void set_color(const Colorf &color);
		Colorf color() const;

		void set_shadow_none();
		void set_shadow(float horz_offset, float vert_offset, float blur_radius, const Colorf &color);
		bool has_shadow() const;
		float shadow_horz_offset() const;
		float shadow_vert_offset() const;
		float shadow_blur_radius() const;
		Colorf shadow_color() const;

		//void set_justify_auto();
		//void set_justify_none();
		//void set_justify_inter_word();
		//void set_justify_distribute();

		//void set_decoration_none();
		//void set_decoration_underline();
		//void set_decoration_overline();
		//void set_decoration_line_through();
		//void set_decoration_color_from_color();
		//void set_decoration_color(const Colorf &color);
		//void set_decoration_style_solid();
		//void set_decoration_style_double();
		//void set_decoration_style_dotted();
		//void set_decoration_style_dashed();
		//void set_decoration_style_wavy();
		//void set_decoration_skip_none();
		//void set_decoration_skip(bool objects, bool spaces, bool ink, bool edges);

		//void set_underline_position_auto();
		//void set_underline_position_under();
		//void set_underline_position_left();
		//void set_underline_position_right();

		//void set_emphasis_none();
		//void set_emphasis_filled_dot();
		//void set_emphasis_filled_circle();
		//void set_emphasis_filled_double_circle();
		//void set_emphasis_filled_triangle();
		//void set_emphasis_filled_sesame();
		//void set_emphasis_open_dot();
		//void set_emphasis_open_circle();
		//void set_emphasis_open_double_circle();
		//void set_emphasis_open_triangle();
		//void set_emphasis_open_sesame();
		//void set_emphasis_color_from_color();
		//void set_emphasis_color(const Colorf &color);
		//void set_emphasis_position_over_right();
		//void set_emphasis_position_under_right();
		//void set_emphasis_position_over_left();
		//void set_emphasis_position_under_left();

		//void set_hanging_none();
		//void set_hanging(bool first, bool force_end, bool allow_end, bool last);

		void set_transform_none();
		//void set_transform_capitalize();
		void set_transform_uppercase();
		void set_transform_lowercase();
		//void set_transform_full_width();
		bool is_transform_none() const;
		bool is_transform_uppercase() const;
		bool is_transform_lowercase() const;

		//void set_vertical_align_baseline();
		//void set_vertical_align_sub();
		//void set_vertical_align_super();
		//void set_vertical_align_top();
		//void set_vertical_align_text_top();
		//void set_vertical_align_middle();
		//void set_vertical_align_bottom();
		//void set_vertical_align_text_bottom();
		//void set_vertical_align(float offset);

		//void set_whitespace_collapse();
		//void set_whitespace_pre();
		//void set_whitespace_nowrap();
		//void set_whitespace_pre_wrap();
		//void set_whitespace_pre_line();

		//void set_character_spacing_normal();
		//void set_character_spacing(float length);

		//void set_word_spacing_normal();
		//void set_word_spacing(float length);
		//void set_word_spacing_percentage(float percentage);

		//void set_tab_size_spaces(int count);
		//void set_tab_size_length(float length);

		//void set_line_break_auto();
		//void set_line_break_loose();
		//void set_line_break_normal();
		//void set_line_break_strict();

		//void set_word_break_normal();
		//void set_word_break_keep_all();
		//void set_word_break_break_all();

		//void set_hyphens_none();
		//void set_hyphens_manual();
		//void set_hyphens_auto();

		//void set_overflow_wrap_normal();
		//void set_overflow_wrap_break_word();

		void set_target_opaque();
		void set_target_transparent();
		bool is_target_opaque() const;
		bool is_target_transparent() const;

		Font get_font(Canvas &canvas);

	private:
		std::shared_ptr<TextStyleImpl> impl;
	};
}

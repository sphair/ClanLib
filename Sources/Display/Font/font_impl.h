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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/glyph_metrics.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_family.h"
#include "API/Display/Render/texture_2d.h"
#include <list>
#include <map>
#include "glyph_cache.h"
#include "path_cache.h"
#include "font_family_impl.h"

#include "FontDraw/font_draw_subpixel.h"
#include "FontDraw/font_draw_flat.h"
#include "FontDraw/font_draw_path.h"
#include "FontDraw/font_draw_scaled.h"

namespace clan
{
	class FontEngine;
	class XMLResourceNode;
	class DomElement;

	class Font_Impl
	{
	public:
		Font_Impl(FontFamily &new_font_family, const FontDescription &description);
		~Font_Impl();

		const FontMetrics &get_font_metrics(Canvas &canvas);

		FontDescription get_description() const;

		int get_character_index(Canvas &canvas, const std::string &text, const Pointf &point);
		std::vector<Rectf> get_character_indices(Canvas &canvas, const std::string &text);

		GlyphMetrics get_metrics(Canvas &canvas, unsigned int glyph);

		GlyphMetrics measure_text(Canvas &canvas, const std::string &string);

		void draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color);

		void get_glyph_path(Canvas &canvas, unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics);

		void set_height(float value);
		void set_weight(FontWeight value);
		void set_line_height(float height);
		void set_style(FontStyle setting);
		void set_scalable(float height_threshold);
		FontHandle *get_handle(Canvas &canvas);

	private:
		void select_font_family(Canvas &canvas);

		FontDescription selected_description;
		float selected_line_height = 0.0f;
		float selected_pixel_ratio = 1.0f;
		float scaled_height = 1.0f;
		float selected_height_threshold = 64.0f;		// Values greater or equal to this value can be drawn scaled
		bool selected_pathfont = false;

		FontMetrics selected_metrics;

		FontEngine *font_engine = nullptr;	// If null, use select_font_family() to update
		FontFamily font_family;

		Font_Draw *font_draw = nullptr;

		Font_DrawSubPixel font_draw_subpixel;
		Font_DrawFlat font_draw_flat;
		Font_DrawScaled font_draw_scaled;
		Font_DrawPath font_draw_path;
	};
}

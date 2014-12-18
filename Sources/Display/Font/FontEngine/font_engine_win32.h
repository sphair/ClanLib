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
**    Mark Page
*/

#pragma once

#include "font_engine.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Font/font_metrics.h"

namespace clan
{

class DataBuffer;

class FontEngine_Win32 : public FontEngine
{
public:
	FontEngine_Win32(const FontDescription &description, DataBuffer &font_databuffer);
	~FontEngine_Win32();

	const FontMetrics &get_metrics() const override { return font_metrics; }

	FontPixelBuffer get_font_glyph(int glyph) override;

	FontPixelBuffer get_font_glyph_standard(int glyph, bool anti_alias);

	FontPixelBuffer get_font_glyph_subpixel(int glyph);
	const FontDescription &get_desc() const override { return font_description; }

	void load_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics) override;
	DataBuffer get_databuffer() override;

private:
	void load_font(DataBuffer &font_databuffer);

	inline Pointf to_point(POINTFX &fx)
	{
		return Pointf(to_float(fx.x), -to_float(fx.y));
	}

	inline float to_float(const FIXED &fixed)
	{
		double v = static_cast<int>(fixed.value) << 16 + static_cast<int>(fixed.fract);
		return static_cast<float>(v / 65536.0);
	}

	FontPixelBuffer get_font_glyph_lcd(int glyph);
	FontPixelBuffer get_font_glyph_gray8(int glyph);
	FontPixelBuffer get_font_glyph_mono(int glyph);
	bool try_load_glyph_bitmap(int glyph, UINT format, MAT2 &matrix, DataBuffer &glyph_bitmap, GLYPHMETRICS &glyph_metrics);
	FontPixelBuffer get_empty_font_glyph(int glyph);
	int decode_charset(FontDescription::Charset selected_charset);
	HFONT handle = 0;
	TEXTMETRIC metrics;

	FontDescription font_description;
	FontMetrics font_metrics;

};

}

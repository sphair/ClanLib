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
#include "API/Display/2D/path.h"
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#include "API/Core/System/databuffer.h"

namespace clan
{

class DataBuffer;

class FontEngine_Cocoa : public FontEngine
{
public:
	FontEngine_Cocoa(const FontDescription &description, DataBuffer &font_databuffer);
	~FontEngine_Cocoa();

	const FontMetrics &get_metrics() const override { return font_metrics; }
	FontPixelBuffer get_font_glyph_standard(int glyph, bool anti_alias);
	FontPixelBuffer get_font_glyph_subpixel(int glyph);
	FontPixelBuffer get_font_glyph(int glyph) override;
	const FontDescription &get_desc() const override { return font_description; }

	void load_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics);

private:
	void load_font(const FontDescription& desc, DataBuffer &font_databuffer);
	FontPixelBuffer get_font_glyph_lcd(int glyph);
	FontPixelBuffer get_empty_font_glyph(int glyph);

	CTFontRef handle;
    float avg_glyph_width;
    float max_glyph_height;
	FontDescription font_description;
	FontMetrics font_metrics;
	DataBuffer data_buffer;

};

}

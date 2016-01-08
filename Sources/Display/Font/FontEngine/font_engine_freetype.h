/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Harry Storbacka
*/

#pragma once

#include "../FontEngine/font_engine.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/System/databuffer.h"

extern "C"
{
	#include <ft2build.h>
	#include FT_FREETYPE_H
	#include FT_GLYPH_H
	#include FT_LCD_FILTER_H
}

namespace clan
{

struct TagStruct
{
	FT_Tag previous;
	FT_Tag current;
	FT_Tag next;
};

struct TaggedPoint
{
	Pointf pos;
	unsigned char tag;
};

class FontEngine_Freetype : public FontEngine
{
/// \name Construction
/// \{
public:
	FontEngine_Freetype(const FontDescription &description, DataBuffer &font_databuffer, float pixel_ratio);
	~FontEngine_Freetype();

/// \}
/// \name Attributes
/// \{

public:
	bool is_automatic_recreation_allowed() const override { return true; }
	const FontMetrics &get_metrics() const override { return font_metrics; }

	FontPixelBuffer get_font_glyph(int glyph) override;

	FontPixelBuffer get_font_glyph_standard(int glyph, bool anti_alias);

	FontPixelBuffer get_font_glyph_subpixel(int glyph);
	const FontDescription &get_desc() const override { return font_description; }
	
/// \}
/// \name Operations
/// \{

public:
	void load_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics) override;

/// \}
/// \name Implementation
/// \{

private:
	void calculate_font_metrics();
	TagStruct get_tag_struct(int cont, int index, FT_Outline *outline);
	int get_index_of_next_contour_point(int cont, int index, FT_Outline *outline);
	int get_index_of_prev_contour_point(int cont, int index, FT_Outline *outline);
	Pointf FT_Vector_to_Pointf(const FT_Vector &);

	FT_Face face;

	std::vector<TaggedPoint> get_contour_points(int cont, FT_Outline *outline);

	DataBuffer data_buffer;
	FontDescription font_description;
	FontMetrics font_metrics;
	float pixel_ratio;

/// \}

};

}

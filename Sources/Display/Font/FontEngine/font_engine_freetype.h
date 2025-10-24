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
**    Mark Page
**    Harry Storbacka
*/

#pragma once

#include "font_engine.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/Core/System/databuffer.h"

extern "C"
{
	#include <ft2build.h>
	#include FT_FREETYPE_H
	#include FT_GLYPH_H
	#include FT_LCD_FILTER_H
}

class CL_GlyphOutline;

struct CL_TagStruct
{
	FT_Tag previous;
	FT_Tag current;
	FT_Tag next;
};

struct CL_TaggedPoint
{
	CL_Pointf pos;
	unsigned char tag;
};

class CL_FontEngine_Freetype : public CL_FontEngine
{
/// \name Construction
/// \{
public:
	CL_FontEngine_Freetype(CL_IODevice &io_dev, float height, float average_width);
	~CL_FontEngine_Freetype();

/// \}
/// \name Attributes
/// \{

public:
	CL_FontMetrics get_metrics();
	float get_kerning(const CL_StringRef::char_type &ch1, const CL_StringRef::char_type &ch2);
	float get_advance_x(const CL_StringRef::char_type &ch);

	CL_Size get_size(const CL_StringRef &text, int pos);

	/// \brief Constructs a pixel buffer from a Freetype glyph.
	///
	/// \param glyph The glyph
	/// \param anti_alias If anti_aliasing should be used
	CL_FontPixelBuffer get_font_glyph_standard(int glyph, bool anti_alias);

	/// \brief Constructs a pixel buffer using subpixel rendering from a Freetype glyph.
	///
	/// \param glyph The glyph
	CL_FontPixelBuffer get_font_glyph_subpixel(int glyph);

/// \}
/// \name Operations
/// \{

public:
	CL_GlyphOutline *load_glyph_outline(int glyph_index);

/// \}
/// \name Implementation
/// \{

private:
	CL_TagStruct get_tag_struct(int cont, int index, FT_Outline *outline);
	int get_index_of_next_contour_point(int cont, int index, FT_Outline *outline);
	int get_index_of_prev_contour_point(int cont, int index, FT_Outline *outline);
	CL_Pointf FT_Vector_to_CL_Pointf(const FT_Vector &);

	FT_Face face;

	std::vector<CL_TaggedPoint> get_contour_points(int cont, FT_Outline *outline);

	CL_DataBuffer data_buffer;
/// \}

};

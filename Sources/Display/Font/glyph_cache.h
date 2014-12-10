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
**    Mark Page
*/

#pragma once

#include "API/Display/Font/font.h"
#include "API/Display/Font/glyph_metrics.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/texture_2d.h"
#include <list>
#include <map>

namespace clan
{

class Colorf;
class TextureGroup;
class FontEngine;
class Font_TextureGlyph;
class Subtexture;
class FontPixelBuffer;
class Path;

/// \brief Font texture format (holds a pixel buffer containing a glyph)
class Font_TextureGlyph
{
public:
	Font_TextureGlyph() : glyph(0) { };

	/// \brief Glyph this pixel buffer refers to.
	unsigned int glyph;

	/// \brief The pixel buffer containing the glyph.
	///
	/// This maybe a null texture
	Texture2D texture;

	/// \brief Geometry of the glyph inside the subtexture (excluding the border)
	Rect geometry;

	/// \brief Offset to draw the font to buffer
	/** For example:
	    x = pos_x + pixelbuffer.offset.x
	    y = pos_y + pixelbuffer.offset.y*/
	Point offset;

	GlyphMetrics metrics;

};

class GlyphCache
{
/// \name Construction
/// \{
public:
	GlyphCache();
	virtual ~GlyphCache();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns information about the current font.
	FontMetrics get_font_metrics();

	/// \brief Get a glyph. Returns NULL if the glyph was not found
	Font_TextureGlyph *get_glyph(Canvas &canvas, FontEngine *font_engine, unsigned int glyph);

	void get_glyph_path(FontEngine *font_engine, unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics);

/// \}
/// \name Operations
/// \{
public:
	GlyphMetrics get_metrics(FontEngine *font_engine, Canvas &canvas, unsigned int glyph);

	void draw(FontEngine *font_engine, Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color);

	int get_character_index(Canvas &canvas, FontEngine *font_engine, const std::string &text, const Point &point);

	void insert_glyph(Canvas &canvas, unsigned int glyph, Subtexture &sub_texture, const Point &offset, const GlyphMetrics &glyph_metrics);
	void insert_glyph(Canvas &canvas, FontEngine *font_engine, int glyph);
	void insert_glyph(Canvas &canvas, FontPixelBuffer &pb);

/// \}
/// \name Implementation
/// \{
private:

	std::vector<Font_TextureGlyph* > glyph_list;

	TextureGroup texture_group;

	static const int glyph_border_size = 1;

public:
	// Contains the anti alias setting
	bool anti_alias;

	// true to enable subpixel rendering setting (implies anti_alias is true)
	bool enable_subpixel;

	FontMetrics font_metrics;
/// \}
};

}

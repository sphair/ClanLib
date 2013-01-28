/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Display/Font/font_system.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
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
	Font_TextureGlyph *get_glyph(FontEngine *font_engine, GraphicContext &gc, unsigned int glyph);

/// \}
/// \name Operations
/// \{
public:

	/// \brief Print text on gc.
	void draw_text(FontEngine *font_engine,Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color);

	/// \brief Calculate size of text string.
	Size get_text_size(FontEngine *font_engine, GraphicContext &gc, const std::string &text);

	/// \brief Set the font metrics for the bitmap font. This is done automatically if the font is loaded from the system font
	void set_font_metrics(const FontMetrics &metrics);

	void set_texture_group(TextureGroup &new_texture_group);

	int get_character_index(FontEngine *font_engine, GraphicContext &gc, const std::string &text, const Point &point);

	void insert_glyph(GraphicContext &gc, unsigned int glyph, Subtexture &sub_texture, const Point &offset, const Point &increment);
	void insert_glyph(FontEngine *font_engine, GraphicContext &gc, int glyph);
	void insert_glyph(GraphicContext &gc, FontPixelBuffer &pb);
	void insert_glyph(FontEngine *font_engine, GraphicContext &gc, const std::string &text);

/// \}
/// \name Implementation
/// \{
private:
	/// \brief Set the font metrics from the OS font
	void write_font_metrics(GraphicContext &gc);

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

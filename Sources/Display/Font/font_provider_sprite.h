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

#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/Font/font_sprite.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/sprite.h"
#include <list>
#include <map>
#include "FontEngine/font_engine.h"

namespace clan
{

class Colorf;

class Font_Sprite_Glyph
{
public:
	Font_Sprite_Glyph() { };

	/// \brief Glyph this pixel buffer refers to.
	unsigned int glyph;

	/// \brief The sprite index of the glyph
	int sprite_index;
};


class FontProvider_Sprite : public FontProvider, private FontEngine
{
/// \name Construction
/// \{

public:

	FontProvider_Sprite();
	virtual ~FontProvider_Sprite();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns information about the current font.
	virtual FontMetrics get_font_metrics();
	FontMetrics get_metrics();

	/// \brief Constructs a pixel buffer from a Freetype glyph.
	///
	/// \param glyph The glyph
	/// \param anti_alias If anti_aliasing should be used
	FontPixelBuffer get_font_glyph_standard(int glyph, bool anti_alias);

	/// \brief Constructs a pixel buffer using subpixel rendering from a Freetype glyph.
	///
	/// \param glyph The glyph
	FontPixelBuffer get_font_glyph_subpixel(int glyph);

/// \}
/// \name Operations
/// \{

public:
	void load_font(GraphicContext &gc, const std::string &resource_id, const XMLResourceDocument &resources);

	/// \brief Print text on gc.
	virtual void draw_text(Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color);

	/// \brief Draw scaled text
	void draw_text(Canvas &canvas, float xpos, float ypos, float scale_x, float scale_y, const std::string &text, const Colorf &color);

	/// \brief Calculate size of text string.
	virtual Size get_text_size(GraphicContext &gc, const std::string &text);

	/// \brief Set the font metrics for the bitmap font. This is done automatically if the font is loaded from the system font
	void set_font_metrics(const FontMetrics &metrics);

	int get_character_index(GraphicContext &gc, const std::string &text, const Point &point);

	Shape2D load_glyph_outline(int c, int &out_advance_x);

/// \}
/// \name Implementation
/// \{

private:

	void setup_glyphs( GraphicContext &gc, const std::string &letters, int spacelen, bool monospace);
	Font_Sprite_Glyph *get_glyph(unsigned int glyph);

	Sprite spr_glyphs;

	FontMetrics font_metrics;

	// Character to glyph map.
	std::vector<Font_Sprite_Glyph> glyph_list;

	// Height of font (i.e. height of the tallest character, before scaling/offsets)
	int height;
	
	// If 0, variable width font. If another number, all characters are this width
	int fixed_width;

	// Distance to go at any unrecognized character, before scaling/offsets
	int spacelen;

/// \}
};

}

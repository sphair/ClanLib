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

#include "API/Display/2D/texture_group.h"
#include "../glyph_cache.h"

namespace clan
{

class Colorf;

class FontEngine_Freetype;

class FontProvider_Freetype
{
/// \name Construction
/// \{
public:
	FontProvider_Freetype();
	virtual ~FontProvider_Freetype();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns information about the current font.
	FontMetrics get_font_metrics();

	/// \brief Get a glyph. Returns NULL if the glyph was not found
	Font_TextureGlyph *get_glyph(GraphicContext &gc, unsigned int glyph);

/// \}
/// \name Operations
/// \{
public:
	/// \brief Set the font metrics for the bitmap font. This is done automatically if the font is loaded from the system font
	void set_font_metrics(const FontMetrics &metrics);

	void set_texture_group(TextureGroup &new_texture_group);

	int get_character_index(GraphicContext &gc, const std::string &text, const Point &point);

	void load_font(const FontDescription &desc);
	void load_font(const FontDescription &desc, IODevice &file);
	void load_font(const FontDescription &desc, const FileSystem &fs);

/// \}
/// \name Implementation
/// \{
private:
	void free_font();

	FontEngine_Freetype *font_engine;

	GlyphCache glyph_cache;
/// \}
};

}

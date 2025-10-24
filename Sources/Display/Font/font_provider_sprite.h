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

class CL_Colorf;

class CL_Font_Sprite_Glyph
{
public:
	CL_Font_Sprite_Glyph() { };

	/// \brief Glyph this pixel buffer refers to.
	unsigned int glyph;

	/// \brief The sprite index of the glyph
	int sprite_index;
};


class CL_FontProvider_Sprite : public CL_FontProvider, private CL_FontEngine
{
/// \name Construction
/// \{

public:

	CL_FontProvider_Sprite(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources);
	virtual ~CL_FontProvider_Sprite();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns information about the current font.
	virtual CL_FontMetrics get_font_metrics(CL_GraphicContext &gc);
	CL_FontMetrics get_metrics();

	CL_FontPixelBuffer get_font_glyph(int glyph, bool anti_alias, const CL_Colorf &color);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the font provider.
	virtual void destroy();

	/// \brief Print text on gc.
	virtual void draw_text(CL_GraphicContext &gc, float xpos, float ypos, const CL_StringRef &text, const CL_Colorf &color);

	/// \brief Draw scaled text
	void draw_text(CL_GraphicContext &gc, float xpos, float ypos, float scale_x, float scale_y, const CL_StringRef &text, const CL_Colorf &color);

	/// \brief Calculate size of text string.
	virtual CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);

	/// \brief Set the font metrics for the bitmap font. This is done automatically if the font is loaded from the system font
	void set_font_metrics(const CL_FontMetrics &metrics);

	int get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point);

/// \}
/// \name Implementation
/// \{

private:

	void setup_glyphs( CL_GraphicContext &gc, const CL_StringRef &letters, int spacelen, bool monospace);
	CL_Font_Sprite_Glyph *get_glyph(unsigned int glyph);

	CL_Sprite spr_glyphs;

	CL_FontMetrics font_metrics;

	// Character to glyph map.
	std::vector<CL_Font_Sprite_Glyph> glyph_list;

	// Height of font (i.e. height of the tallest character, before scaling/offsets)
	int height;
	
	// If 0, variable width font. If another number, all characters are this width
	int fixed_width;

	// Distance to go at any unrecognized character, before scaling/offsets
	int spacelen;

/// \}
};





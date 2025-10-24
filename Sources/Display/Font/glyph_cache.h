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

#include "API/Display/Font/font.h"
#include "API/Display/Font/font_system.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include <list>
#include <map>

class CL_Colorf;
class CL_TextureGroup;
class CL_FontEngine;
class CL_Font_TextureGlyph;

class CL_GlyphCache
{
/// \name Construction
/// \{
public:
	CL_GlyphCache();
	virtual ~CL_GlyphCache();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns information about the current font.
	CL_FontMetrics get_font_metrics();

	/// \brief Get a glyph. Returns NULL if the glyph was not found
	CL_Font_TextureGlyph *get_glyph(CL_FontEngine *font_engine, CL_GraphicContext &gc, unsigned int glyph);

/// \}
/// \name Operations
/// \{
public:

	/// \brief Print text on gc.
	void draw_text(CL_FontEngine *font_engine,CL_GraphicContext &gc, float xpos, float ypos, const CL_StringRef &text, const CL_Colorf &color);

	/// \brief Calculate size of text string.
	CL_Size get_text_size(CL_FontEngine *font_engine, CL_GraphicContext &gc, const CL_StringRef &text);

	/// \brief Set the font metrics for the bitmap font. This is done automatically if the font is loaded from the system font
	void set_font_metrics(const CL_FontMetrics &metrics);

	void set_texture_group(CL_TextureGroup &new_texture_group);

	int get_character_index(CL_FontEngine *font_engine, CL_GraphicContext &gc, const CL_String &text, const CL_Point &point);

	void insert_glyph(CL_GraphicContext &gc, CL_Font_System_Position &position, CL_PixelBuffer &pixel_buffer);
	void insert_glyph(CL_FontEngine *font_engine, CL_GraphicContext &gc, int glyph);
	void insert_glyph(CL_GraphicContext &gc, CL_FontPixelBuffer &pb);
	void insert_glyph(CL_FontEngine *font_engine, CL_GraphicContext &gc, const CL_StringRef &text);

/// \}
/// \name Implementation
/// \{
private:
	/// \brief Set the font metrics from the OS font
	void write_font_metrics(CL_GraphicContext &gc);

	std::vector<CL_Font_TextureGlyph* > glyph_list;

	CL_TextureGroup texture_group;

	static const int glyph_border_size = 1;

public:
	// Contains the anti alias setting
	bool anti_alias;

	// true to enable subpixel rendering setting (implies anti_alias is true)
	bool enable_subpixel;

	CL_FontMetrics font_metrics;
/// \}
};

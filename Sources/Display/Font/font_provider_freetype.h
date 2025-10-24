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
#include "API/Display/2D/texture_group.h"
#include "glyph_cache.h"

class CL_Colorf;

class CL_FontEngine_Freetype;

class CL_FontProvider_Freetype : public CL_FontProvider
{
/// \name Construction
/// \{
public:
	CL_FontProvider_Freetype();
	virtual ~CL_FontProvider_Freetype();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns information about the current font.
	CL_FontMetrics get_font_metrics();

	/// \brief Get a glyph. Returns NULL if the glyph was not found
	CL_Font_TextureGlyph *get_glyph(CL_GraphicContext &gc, unsigned int glyph);

/// \}
/// \name Operations
/// \{
public:
	/// \brief Print text on gc.
	void draw_text(CL_GraphicContext &gc, float xpos, float ypos, const CL_StringRef &text, const CL_Colorf &color);

	/// \brief Calculate size of text string.
	CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);

	/// \brief Set the font metrics for the bitmap font. This is done automatically if the font is loaded from the system font
	void set_font_metrics(const CL_FontMetrics &metrics);

	void set_texture_group(CL_TextureGroup &new_texture_group);

	int get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point);

	void load_font(const CL_FontDescription &desc);
	void load_font(const CL_FontDescription &desc, CL_IODevice &file);
	void load_font(const CL_FontDescription &desc, const CL_VirtualDirectory &directory);
	void load_font(const CL_StringRef &resource_id, CL_ResourceManager *resources);

	/// \brief Destroys the font provider.
	virtual void destroy();

/// \}
/// \name Implementation
/// \{
private:
	void free_font();

	CL_FontEngine_Freetype *font_engine;

	CL_GlyphCache glyph_cache;
/// \}
};

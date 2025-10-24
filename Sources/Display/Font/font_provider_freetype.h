/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "API/Display/Font/font_freetype.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/texture_group.h"
#include <list>
#include <map>

class CL_FreetypeFont;
class CL_Colorf;

class CL_Font_Freetype_Glyph
{
public:
	CL_Font_Freetype_Glyph() : glyph(0), empty_buffer(true), offset(0,0), increment(0,0) { };

	/// \brief Glyph this pixel buffer refers to.
	unsigned int glyph;

	/// \brief True when the pixel buffer is empty
	bool empty_buffer;

	/// \brief The pixel buffer containing the glyph
	CL_Subtexture subtexture;
	CL_Texture texture;

	/// \brief Offset to draw the font to buffer
	/** For example:
	    x = pos_x + pixelbuffer.offset.x
	    y = pos_y + pixelbuffer.offset.y*/
	CL_Point offset;

	/// \brief Increment to draw the next glyph
	/** For example:
	    pos_x += pixelbuffer.increment.x;
	    pos_y += pixelbuffer.increment.y;*/
	CL_Point increment;
};


class CL_FontProvider_Freetype : public CL_FontProvider
{
/// \name Construction
/// \{

public:

	CL_FontProvider_Freetype(CL_GraphicContext &gc);
	virtual ~CL_FontProvider_Freetype();


/// \}
/// \name Attributes
/// \{

public:

	CL_FreetypeFont *get_handle();

	/// \brief Returns information about the current font.
	virtual CL_FontMetrics get_font_metrics(CL_GraphicContext &gc);


/// \}
/// \name Operations
/// \{

public:
	void load_font(const CL_FontDescription &desc);
	void load_font(const CL_FontDescription &desc, CL_IODevice &file);
	void load_font(const CL_FontDescription &desc, const CL_VirtualDirectory &directory);

	/// \brief Destroys the font provider.
	virtual void destroy();

	/// \brief Print text on gc.
	virtual void draw_text(CL_GraphicContext &gc, int x, int y, const CL_StringRef &text, const CL_Colorf &color);

	/// \brief Calculate size of text string.
	virtual CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);

	int get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point);

	void set_texture_group(CL_TextureGroup &new_texture_group);


/// \}
/// \name Implementation
/// \{

private:
	/// \brief Get a glyph. Returns NULL if the glyph was not found
	CL_Font_Freetype_Glyph *get_glyph(CL_GraphicContext &gc, unsigned int glyph);

	void free_font();
	void insert_glyph(CL_GraphicContext &gc, int glyph);
	void insert_glyph(CL_GraphicContext &gc, CL_FontPixelBuffer &pb);
	CL_FontPixelBuffer get_font_glyph(CL_GraphicContext &gc, int glyph, bool anti_alias, const CL_Colorf &color);

	std::vector<CL_Font_Freetype_Glyph* > glyph_list;

	CL_FreetypeFont *handle;

	// Contains the font height
	int size_height;

	// Contains the anti alias setting
	bool anti_alias;

	CL_TextureGroup texture_group;

	CL_FontMetrics font_metrics;

/// \}
};




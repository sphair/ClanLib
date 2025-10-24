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
#include "API/Display/Font/font_system.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/texture_group.h"
#include <list>
#include <map>

class CL_FreetypeFont;
class CL_Colorf;
class CL_TextureGroup;

class CL_FontProvider_Texture : public CL_FontProvider
{
/// \name Construction
/// \{

public:

	CL_FontProvider_Texture(CL_GraphicContext &gc);
	virtual ~CL_FontProvider_Texture();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns information about the current font.
	virtual CL_FontMetrics get_font_metrics(CL_GraphicContext &gc);

	/// \brief Get a glyph. Returns NULL if the glyph was not found
	CL_Font_System_Glyph *get_glyph(CL_GraphicContext &gc, unsigned int glyph);

#ifdef WIN32
	HFONT get_handle();
#else
	CL_FreetypeFont *get_handle();
#endif


/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the font provider.
	virtual void destroy();

	/// \brief Print text on gc.
	virtual void draw_text(CL_GraphicContext &gc, int xpos, int ypos, const CL_StringRef &text, const CL_Colorf &color);

	/// \brief Calculate size of text string.
	virtual CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);

	/// \brief Load a system font (for use by insert_glyph to load text from a system font)
	void load_font( CL_GraphicContext &context, const CL_FontDescription &desc);

	/// \brief Free the system font when it is no longer required
	void free_font();

	/// \brief Insert a glyph from a pixel buffer
	/** param: gc = The graphic context
	    param: position = The bitmap font position
	    param: pixel_buffer = Pixel buffer containing the glyph.*/
	void insert_glyph(CL_GraphicContext &gc, CL_Font_System_Position &position, CL_PixelBuffer &pixel_buffer);

	/// \brief Insert glyphs from a text string (using to system font)
	/** param: gc = The graphic context
	    param: text = The text to use*/
	void insert_glyph(CL_GraphicContext &gc, const CL_StringRef &text);

	/// \brief Set the font metrics for the bitmap font. This is done automatically if the font is loaded from the system font
	void set_font_metrics(const CL_FontMetrics &metrics);

	void set_texture_group(CL_TextureGroup &new_texture_group);

	int get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point);

	static void register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface);

/// \}
/// \name Implementation
/// \{

private:
#ifndef WIN32
	/// \brief Get the registered font or the best font match if not found
	///
	/// Input font sizes are win32 font sizes, and are transformed to freetype font sizes
	//
	/// \param desc = Description
	///
	/// \return Updated description
	CL_FontDescription get_registered_font(const CL_FontDescription &desc);
#endif

	void insert_glyph(CL_GraphicContext &gc, int glyph);
	void insert_glyph(CL_GraphicContext &gc, CL_FontPixelBuffer &pb);
	CL_FontPixelBuffer get_font_glyph(CL_GraphicContext &gc, int glyph, bool anti_alias, const CL_Colorf &color);

	/// \brief Set the font metrics from the OS font
	void write_font_metrics(CL_GraphicContext &gc);

	std::vector<CL_Font_System_Glyph* > glyph_list;

#ifdef WIN32
	HFONT handle;
#else
	CL_FreetypeFont *handle;
#endif

	// Contains the font height
	int size_height;

	// Contains the anti alias setting
	bool anti_alias;

	CL_TextureGroup texture_group;

	CL_FontMetrics font_metrics;

#ifndef WIN32
	static std::map<CL_String /*font_typeface*/, CL_String /*font_filename*/ > font_register_cache;
#endif

/// \}
};





/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include "font.h"

#include "../2D/subtexture.h"
#include "../Render/texture.h"

class CL_FontProvider;
class CL_Font_System_Impl;
class CL_TextureGroup;

class CL_FontProvider_System;

/// \brief Bitmap Font Position class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
typedef struct _CL_Font_System_Position
{
	/// \brief The glyph (0 denotes end of list)
	unsigned int glyph;

	/// \brief Font X Position in the pixel buffer
	int x_pos;

	/// \brief Font Y Position in the pixel buffer
	int y_pos;

	/// \brief Font Width
	int width;

	/// \brief Font Height
	int height;

	/// \brief X Offset when drawing
	int x_offset;

	/// \brief Y Offset when drawing
	int y_offset;

	/// \brief X Increment when drawing
	int x_increment;

	/// \brief Y Increment when drawing
	int y_increment;
} CL_Font_System_Position;

/// \brief Font texture format (holds a pixel buffer containing a glyph)
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_Font_TextureGlyph
{
public:
	CL_Font_TextureGlyph() : glyph(0), empty_buffer(true), offset(0,0), increment(0,0) { };

	/// \brief Glyph this pixel buffer refers to.
	unsigned int glyph;

	/// \brief True when the pixel buffer is empty
	bool empty_buffer;

	/// \brief The pixel buffer containing the glyph
	CL_Subtexture subtexture;

	/// \brief Geometry of the glyph inside the subtexture (excluding the border)
	CL_Rect geometry;

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


/// \}

/// \brief Bitmap Font class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Font_System : public CL_Font
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a CL_Texture bitmap font.
	CL_Font_System();

	/// \brief Constructs a Font System
	///
	/// \param context = Graphic Context
	/// \param typeface_name = String Ref
	/// \param height = value
	CL_Font_System( CL_GraphicContext &context, const CL_StringRef &typeface_name, int height);

	/// \brief Constructs a Font System
	///
	/// \param context = Graphic Context
	/// \param desc = Font Description
	CL_Font_System( CL_GraphicContext &context, const CL_FontDescription &desc);

	~CL_Font_System();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the font provider.
	CL_FontProvider_System *get_provider() const;

	/// \brief Get a glyph. Returns NULL if the glyph was not found
	CL_Font_TextureGlyph *get_glyph(CL_GraphicContext &gc, int glyph);

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set the texture font to use a specified texture group
	void set_texture_group(CL_TextureGroup &new_texture_group);

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

	/// \brief Registers a font for lookup when creating fonts.
	static void register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}

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

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include "font.h"

#include "../2D/subtexture.h"
#include "../Render/texture_2d.h"

namespace clan
{

class FontProvider;
class Font_System_Impl;
class TextureGroup;
class Subtexture;

class FontProvider_System;

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

	/// \brief Increment to draw the next glyph
	/** For example:
	    pos_x += pixelbuffer.increment.x;
	    pos_y += pixelbuffer.increment.y;*/
	Point increment;
};


/// \}

/// \brief System Font class.
class CL_API_DISPLAY Font_System : public Font
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null System font.
	Font_System();

	/// \brief Constructs a Font System
	///
	/// \param canvas = Canvas
	/// \param typeface_name = String Ref
	/// \param height = value
	Font_System( Canvas &canvas, const std::string &typeface_name, int height);

	/// \brief Constructs a Font System
	///
	/// \param canvas = Canvas
	/// \param typeface_name = String Ref
	/// \param file_name = String Ref
	/// \param height = value
	Font_System( Canvas &canvas, const std::string &typeface_name, const std::string &file_name, int height);

	/// \brief Constructs a Font System
	///
	/// \param canvas = Canvas
	/// \param desc = Font Description
	Font_System( Canvas &canvas, const FontDescription &desc);

	/// \brief Constructs a Font System from a Font, ensuring the correct type
	explicit Font_System( const Font &font);

	~Font_System();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the font provider.
	FontProvider_System *get_provider() const;

	/// \brief Get a glyph. Returns NULL if the glyph was not found
	Font_TextureGlyph *get_glyph(GraphicContext &gc, int glyph);

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set the texture font to use a specified texture group
	void set_texture_group(TextureGroup &new_texture_group);

	/// \brief Load a system font (for use by insert_glyph to load text from a system font)
	void load_font( GraphicContext &context, const FontDescription &desc);

	/// \brief Free the system font when it is no longer required
	void free_font();

	/// \brief Insert a glyph from a pixel buffer
	/** param: gc = The graphic context
	    param: position = The bitmap font position
	    param: pixel_buffer = Pixel buffer containing the glyph.*/
	void insert_glyph(GraphicContext &gc, unsigned int glyph, Subtexture &sub_texture, const Point &offset, const Point &increment);

	/// \brief Insert glyphs from a text string (using to system font)
	/** param: gc = The graphic context
	    param: text = The text to use*/
	void insert_glyph(GraphicContext &gc, const std::string &text);

	/// \brief Set the font metrics for the bitmap font. This is done automatically if the font is loaded from the system font
	void set_font_metrics(const FontMetrics &metrics);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}

/// \}

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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include "font.h"

namespace clan
{

class Font_Vector_Impl;
class FontMetrics;
class FileSystem;
class FontProvider_Vector;

/// \brief Vector font drawing class.
class CL_API_DISPLAY Font_Vector : public Font
{
/// \name Construction
/// \{

public:
/// \brief Constructs vector font.
	Font_Vector();

	/// \brief Constructs a Font Vector
	///
	/// \param canvas = Canvas
	/// \param typeface_name = String Ref
	/// \param height = value
	Font_Vector( Canvas &canvas, const std::string &typeface_name, int height, const std::string &filename);

	/// \brief Constructs a Font Vector
	///
	/// \param canvas = Canvas
	/// \param desc = Font Description
	Font_Vector( Canvas &canvas, const FontDescription &desc, const std::string &filename);

	/// \brief Constructs a Font Vector from a Font, ensuring the correct type
	explicit Font_Vector( const Font &font);

	~Font_Vector();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the font provider.
	FontProvider_Vector *get_provider() const;

	/// \brief Get the largest bounding box for each glyph for this font in a given reference string
	Rectf get_bounding_box(const std::string &reference_string) const;

	const std::vector<Vec2f> &get_glyph_filled(unsigned int glyph);
	const std::vector< std::vector<Vec2f> > &get_glyph_outline(unsigned int glyph);


/// \}
/// \name Operations
/// \{

public:
	/// \brief Set to draw filled (default)
	void set_filled(bool enable = true);

	/// \brief Set the texture
	///
	/// \param src_texture = The texture to use.
	/// \param bounding_rect = The bounding rect of the glyph. Use get_bounding_box() to calculate this
	/// \param texture_rect = The texture rect
	void set_texture(const Texture2D &src_texture, const Rectf &bounding_rect, const Rectf &texture_rect = Rectf(0.0f, 0.0f, 1.0f, 1.0f) );
	void set_texture(const Texture2D &src_texture, const Rectf &bounding_rect, const Rect &texture_rect );

	/// \brief Reset the texture
	void reset_texture();

/// \}
/// \name Implementation
/// \{

/// \}
};

}

/// \}

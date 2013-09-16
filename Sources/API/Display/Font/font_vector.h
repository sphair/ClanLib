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


#pragma once

#include "../api_display.h"
#include "font.h"

namespace clan
{
/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

class Font_Vector_Impl;
class FontMetrics;
class FileSystem;
class Font_Vector_Impl;

/// \brief Vector font drawing class.
class CL_API_DISPLAY Font_Vector
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

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	/// \brief Get the largest bounding box for each glyph for this font in a given reference string
	Rectf get_bounding_box(const std::string &reference_string) const;

	const std::vector<Vec2f> &get_glyph_filled(unsigned int glyph);
	const std::vector< std::vector<Vec2f> > &get_glyph_outline(unsigned int glyph);

	/// \brief Calculate size of text string.
	///
	/// Multiline text (seperated by /n) is supported\n
	/// \n
	/// Note: The height also includes whitespace (to give the maximum font height), so "." and "X" returns the same height.\n
	/// The width is the pixel width\n
	/// \n
	/// The size is the increment value to the next glyph
	Size get_text_size(GraphicContext &gc, const std::string &text);

	/// \brief Gets the size of a specified glyph
	///
	/// The size is the increment value to the next glyph
	///
	/// \param glyph = The glyph to get
	/// \return The size
	Size get_glyph_size(GraphicContext &gc, unsigned int glyph);

	/// \brief Retrieves font metrics description for the selected font.
	FontMetrics get_font_metrics();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(Canvas &canvas, int x, int y, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(Canvas &canvas, float x, float y, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param position = Dest position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Print text on gc adding ellipses if it does not fit
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param content_box = Rectangle the text is allowed within
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text_ellipsis(Canvas &canvas, int x, int y, Rect content_box, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Print text on gc adding ellipses if it does not fit
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param content_box = Rectangle the text is allowed within
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text_ellipsis(Canvas &canvas, float x, float y, Rectf content_box, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Print text on gc adding ellipses if it does not fit
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param position = Dest position
	/// \param content_box = Rectangle the text is allowed within
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text_ellipsis(Canvas &canvas, const Pointf &position, Rectf content_box, const std::string &text, const Colorf &color = Colorf::white);

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
	std::shared_ptr<Font_Vector_Impl> impl;

/// \}
};

}

/// \}

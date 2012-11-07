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
**    Magnus Norddahl
*/

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include <memory>
#include "../Render/graphic_context.h"
#include "../Image/pixel_buffer.h"
#include "font_description.h"
#include "font_manager.h"

namespace clan
{

class FontProvider;
class Canvas;
class Font_Impl;

/// \brief Font class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY Font
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a font.
	Font();

	/// \brief Constructs standard font - Currently this defaults to Font_System
	///
	/// \param font_manager = The font manager
	Font( GraphicContext &context, const std::string &typeface_name, int height, FontManager font_manager = FontManager());

	// \brief Constructs standard font - Currently this defaults to Font_System
	Font( GraphicContext &context, const FontDescription &desc, FontManager font_manager = FontManager());

	/// \brief Constructs a Font
	///
	/// \param provider = Font Provider
	Font(FontProvider *provider);

	/// \brief Constructs a Font
	///
	/// \param copy = Font
	Font(const Font &copy);

	virtual ~Font();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	/// \brief Retrieves the font provider.
	FontProvider *get_provider() const;

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

	/// \brief Get the character index at a specified point
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = The Graphic Context
	/// \param text = The string
	/// \param point = The point
	/// \return The character index. -1 = Not at specified point
	int get_character_index(GraphicContext &gc, const std::string &text, const Point &point);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<Font_Impl> impl;
/// \}
};

/// \brief Font pixel buffer format (holds a pixel buffer containing a glyph)
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY FontPixelBuffer
{
public:
	FontPixelBuffer() : glyph(0), empty_buffer(true), offset(0,0), increment(0,0) { };

	/// \brief Glyph this pixel buffer refers to. 0 = Glyph if not valid
	unsigned int glyph;

	/// \brief True when the pixel buffer is empty
	bool empty_buffer;

	/// \brief The pixel buffer containing the glyph
	PixelBuffer buffer;

	/// \brief The rect containing the image inside "buffer"
	Rect buffer_rect;

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

}

/// \}


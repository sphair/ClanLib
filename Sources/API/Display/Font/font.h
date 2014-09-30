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
*/

#pragma once

#include <memory>
#include "../Render/graphic_context.h"
#include "../Image/pixel_buffer.h"
#include "../2D/sprite.h"
#include "font_description.h"

namespace clan
{
/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

class FontProvider;
class Canvas;
class Font_Impl;
class GlyphMetrics;

/// \brief Font class
///
/// A Font is a collection of images that can be used to represent text on a screen.
class Font
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a font.
	Font();

	/// \brief Constructs standard font
	///
	/// \param canvas = Canvas
	Font( Canvas &canvas, const std::string &typeface_name, int height);

	// \brief Constructs standard font
	Font( Canvas &canvas, const FontDescription &desc);

	// \brief Constructs standard font
	Font( Canvas &canvas, const FontDescription &desc, const std::string &ttf_filename);

	// \brief Constructs standard font
	Font( Canvas &canvas, const FontDescription &desc, const std::string &ttf_filename, FileSystem fs);

	/// \brief Constructs a Font based on a sprite
	///
	/// \param sprite = Sprite with glyphs
	/// \param glyph_list = Letter to glyph mapping
	/// \param spacelen = Width of space character
	/// \param monospace = Force monospaced font (using widest sprite character)
	/// \param metrics = Font metrics for the sprite font
	Font( Canvas &canvas, Sprite &sprite, const std::string &glyph_list, int spacelen, bool monospace, const FontMetrics &metrics);

/// \}

/// \name Resources
/// \{
public:
	/// \brief Retrieves a Font resource from the resource manager
	///
	/// \param canvas = Canvas
	/// \param resources = Resource manager
	/// \param id = id
	static Resource<Font> resource(Canvas &canvas, const FontDescription &desc, const ResourceManager &resources);

/// \}

/// \name Attributes
/// \{
public:

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param canvas = Canvas
	/// \param x = X position
	/// \param y = Y position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(Canvas &canvas, int x, int y, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param canvas = Canvas
	/// \param x = X position
	/// \param y = Y position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(Canvas &canvas, float x, float y, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param canvas = Canvas
	/// \param position = Dest position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Print text on gc adding ellipses if it does not fit
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param canvas = Canvas
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
	/// \param canvas = Canvas
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
	/// \param canvas = Canvas
	/// \param position = Dest position
	/// \param content_box = Rectangle the text is allowed within
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text_ellipsis(Canvas &canvas, const Pointf &position, Rectf content_box, const std::string &text, const Colorf &color = Colorf::white);

	/// \brief Draw the glyph
	///
	/// \param canvas = Canvas
	/// \param position = Dest position
	/// \param glyph = The glyph to draw
	/// \param color = The text color
	void draw_glyph(Canvas &canvas, const Pointf &position, unsigned int glyph, const Colorf &color = Colorf::white);
	void draw_glyph(Canvas &canvas, float x, float y, unsigned int glyph, const Colorf &color = Colorf::white) { draw_glyph(canvas, Pointf(x, y), glyph, color); }
	void draw_glyph(Canvas &canvas, int x, int y, unsigned int glyph, const Colorf &color = Colorf::white) { draw_glyph(canvas, Pointf(x, y), glyph, color); }

	/// \brief Calculate size of text string.
	///
	/// Multiline text (seperated by /n) is supported\n
	/// \n
	/// Note: The height also includes whitespace (to give the maximum font height), so "." and "X" returns the same height.\n
	/// The width is the pixel width\n
	/// \n
	/// The size is the increment value to the next glyph
	Size get_text_size(Canvas &canvas, const std::string &text);

	/// \brief Gets the size of a specified glyph
	///
	/// The size is the increment value to the next glyph
	///
	/// \param glyph = The glyph to get
	/// \return The size
	Size get_glyph_size(Canvas &canvas, unsigned int glyph);

	/// \brief Gets the glyph metrics
	///
	/// \param glyph = The glyph to get
	/// \return The glyph metrics
	GlyphMetrics get_glyph_metrics(Canvas &canvas, unsigned int glyph);

	/// \brief Retrieves font metrics description for the selected font.
	FontMetrics get_font_metrics();

	/// \brief Get the character index at a specified point
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param canvas = Canvas
	/// \param text = The string
	/// \param point = The point
	/// \return The character index. -1 = Not at specified point
	int get_character_index(Canvas &canvas, const std::string &text, const Point &point);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<Font_Impl> impl;
/// \}
};

}

/// \}


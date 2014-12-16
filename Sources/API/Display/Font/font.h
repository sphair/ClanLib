/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include <memory>
#include "../Render/graphic_context.h"
#include "../Image/pixel_buffer.h"
#include "../2D/sprite.h"
#include "font_description.h"
#include "glyph_metrics.h"

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
	/// \brief Constructs a null font.
	Font();

	/// \brief Constructs standard font
	///
	/// \param canvas = Canvas
	Font(Canvas &canvas, const std::string &typeface_name, int height);

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
	/// \brief Sets the typeface name
	void set_typeface_name(const std::string &name);

	/// \brief Sets the font height
	void set_height(int value);

	/// \brief Sets the font weight
	void set_weight(int value);

	/// \brief Sets the distance between each line
	void set_line_height(float height);

	/// \brief Sets the font italic setting
	void set_italic(bool setting);

	/// \brief Print text 
	///
	/// \param canvas = Canvas
	/// \param position = Dest position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color = Colorf::white);
	void draw_text(Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color = Colorf::white) { draw_text(canvas, Pointf(xpos, ypos), text, color); }

	/// \brief Gets the glyph metrics
	///
	/// \param glyph = The glyph to get
	/// \return The glyph metrics
	GlyphMetrics get_metrics(Canvas &canvas, unsigned int glyph);

	/// \brief Measure text size
	///
	/// \param string = The text to use
	/// \return The metrics
	GlyphMetrics measure_text(Canvas &canvas, const std::string &string);

	/// \brief Retrieves font metrics description for the selected font.
	FontMetrics get_font_metrics();

	/// \brief Retrieves clipped version of the text that will fit into a box
	///
	/// \return The string
	std::string get_clipped_text(Canvas &canvas, const Sizef &box_size, const std::string &text, const std::string &ellipsis_text = "...");

	/// \brief Get the character index at a specified point
	///
	/// \param canvas = Canvas
	/// \param text = The string
	/// \param point = The point
	/// \return The character index. -1 = Not at specified point
	int get_character_index(Canvas &canvas, const std::string &text, const Point &point);

	// Finds the offset for the last visible character when clipping the head
	size_t clip_from_left(Canvas &canvas, const std::string &text, float width);

	// Finds the offset for the first visible character when clipping the tail
	size_t clip_from_right(Canvas &canvas, const std::string &text, float width);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<Font_Impl> impl;

	friend class Path;

/// \}
};

}

/// \}


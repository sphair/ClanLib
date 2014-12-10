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

#include "font.h"
#include "../2D/brush.h"

namespace clan
{
/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

class PathFont_Impl;
class FontMetrics;
class FileSystem;
class PathFont_Impl;

/// \brief Vector font drawing class.
class PathFont
{
/// \name Construction
/// \{

public:
/// \brief Constructs vector font.
	PathFont();

	/// \brief Constructs a Font Vector
	///
	/// \param canvas = Canvas
	/// \param typeface_name = Name
	/// \param height = value
	PathFont( Canvas &canvas, const std::string &typeface_name, int height, const std::string &filename);

	/// \brief Constructs a Font Vector
	///
	/// \param canvas = Canvas
	/// \param desc = Font Description
	PathFont( Canvas &canvas, const FontDescription &desc, const std::string &filename);

	/// \brief Constructs a Font Vector from a Font, ensuring the correct type
	explicit PathFont( const Font &font);

	~PathFont();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	/// \brief Retrieves font metrics description for the selected font.
	FontMetrics get_font_metrics();

	/// \brief Gets the glyph metrics
	///
	/// \param glyph = The glyph to get
	/// \return The glyph metrics
	GlyphMetrics get_metrics(Canvas &canvas, unsigned int glyph);

	/// \brief Measure text size
	///
	/// \param string = The text
	/// \return The metrics
	GlyphMetrics measure_text(Canvas &canvas, const std::string &string);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Draw text
	///
	/// \param canvas = Canvas
	/// \param position = Dest position
	/// \param text = The text to draw
	/// \param brush = The brush
	void draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Brush &brush = Brush::solid(1.0f, 1.0f, 1.0f));

/// \}
/// \name Implementation
/// \{
	std::shared_ptr<PathFont_Impl> impl;

/// \}
};

}

/// \}

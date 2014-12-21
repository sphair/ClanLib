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
/// \addtogroup clanDisplay_Font clanDisplay FontFace
/// \{

class FontProvider;
class Canvas;
class FontFace_Impl;
class GlyphMetrics;

/// \brief FontFace class
///
/// A FontFace is a collection of font descriptions
class FontFace
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null font face
	FontFace();

	/// \brief Constructs a font face with the given family name
	FontFace(const std::string &family_name);

/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if the font face is a null object
	bool is_null() const;

	/// \brief Font family name used for this font face
	const std::string &get_family_name() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Add standard font
	void add(const std::string &typeface_name, int height);

	// \brief Add standard font
	void add(const FontDescription &desc);

	// \brief Add standard font
	void add(const FontDescription &desc, const std::string &ttf_filename);

	// \brief Add standard font
	void add(const FontDescription &desc, const std::string &ttf_filename, FileSystem fs);

	/// \brief Add a Font based on a sprite
	///
	/// \param sprite = Sprite with glyphs
	/// \param glyph_list = Letter to glyph mapping
	/// \param spacelen = Width of space character
	/// \param monospace = Force monospaced font (using widest sprite character)
	/// \param metrics = Font metrics for the sprite font
	void add(Canvas &canvas, Sprite &sprite, const std::string &glyph_list, int spacelen, bool monospace, const FontMetrics &metrics);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<FontFace_Impl> impl;

	friend class Font_Impl;

/// \}
};

}

/// \}


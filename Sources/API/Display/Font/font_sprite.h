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

namespace clan
{

class FontProvider;
class Font_Sprite_Impl;
class FontProvider_Sprite;

/// \brief Sprite Font class.
class CL_API_DISPLAY Font_Sprite : public Font
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a sprite font.
	Font_Sprite();

	/// \brief Constructs a Font Sprite
	///
	/// \param canvas = Canvas
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	Font_Sprite( Canvas &canvas, const std::string &resource_id, ResourceManager *resources );

	/// \brief Constructs a Font Sprite from a Font, ensuring the correct type
	explicit Font_Sprite( const Font &font);

	~Font_Sprite();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the font provider.
	FontProvider_Sprite *get_provider() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set the font metrics for the font.
	void set_font_metrics(const FontMetrics &metrics);

	using Font::draw_text;

	/// \brief Draw scaled text
	///
	/// Multiline text (seperated by /n) is supported\n
	//
	/// \param gc = Graphic Context
	/// \param xpos = X position
	/// \param ypos = Y position
	/// \param scale_x = X scale
	/// \param scale_y = Y scale
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(Canvas &canvas, float xpos, float ypos, float scale_x, float scale_y, const std::string &text, const Colorf &color = Colorf::white);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}

/// \}

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
class VirtualDirectory;
class FontProvider_Vector;

/// \brief Vector font drawing class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
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
	Font_Vector( Canvas &canvas, const std::string &typeface_name, int height);

	/// \brief Constructs a Font Vector
	///
	/// \param canvas = Canvas
	/// \param typeface_name = String Ref
	/// \param file_name = String Ref
	/// \param height = value
	Font_Vector( Canvas &canvas, const std::string &typeface_name, const std::string &file_name, int height);

	/// \brief Constructs a Font Vector
	///
	/// \param canvas = Canvas
	/// \param desc = Font Description
	Font_Vector( Canvas &canvas, const FontDescription &desc);

	/// \brief Constructs a Font Vector from a Font, ensuring the correct type
	explicit Font_Vector( const Font &font);

	~Font_Vector();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the font provider.
	FontProvider_Vector *get_provider() const;

	/// \brief Returns the amount of glyphs used by text.
	int get_glyph_count(const std::string &text);

	/// \brief Find glyph indexes and inter-spacing (x,y) values for text.
	void get_glyphs(
		const std::string &text,
		int *out_glyphs,
		float *out_interspacing_x,
		float *out_interspacing_y);

	/// \brief Get glyphs size
	///
	/// \param glyphs = value
	/// \param length = value
	///
	/// \return Sizef
	Sizef get_glyphs_size(int *glyphs, int length);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Set to draw filled (default)
	void set_filled(bool enable = true);

/// \}
/// \name Implementation
/// \{

/// \}
};

}

/// \}

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
class GlyphPrimitivesArrayOutline;
class GlyphPrimitivesArray;
class FontProvider_Vector;

/// \brief Glyph Primitives Array
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class GlyphPrimitivesArray
{
//! Construction:
public:

	/// \brief Constructs a GlyphPrimitivesArray
	///
	/// \param num_triangles = value
	GlyphPrimitivesArray(int num_triangles)
	{
		int size = num_triangles*3;
		vertex.resize(size);
	}

	~GlyphPrimitivesArray()
	{
	}

	std::vector<Vec2f> vertex;
};

/// \brief Glyph Primitives Array Outline
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class GlyphPrimitivesArrayOutline
{
//! Construction:
public:

	/// \brief Constructs a GlyphPrimitivesArrayOutline
	///
	/// \param num_lines = value
	GlyphPrimitivesArrayOutline(int num_lines)
	{
		vertex.resize(1);
		vertex[0].resize(num_lines+1);
	}
	
//! Operations:
public:

	/// \brief New subarray
	///
	/// \param num_lines = value
	void new_subarray(int num_lines)
	{
		std::vector<std::vector<Vec2f> >::size_type size = vertex.size();
		vertex.resize(size+1);

		vertex[size].resize(num_lines+1);
	}

	std::vector< std::vector<Vec2f> > vertex;
};

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
	/// \param typeface_name = String Ref
	/// \param height = value
	Font_Vector( const std::string &typeface_name, int height);

	/// \brief Constructs a Font Vector
	///
	/// \param typeface_name = String Ref
	/// \param height = value
	/// \param file = IODevice
	Font_Vector( const std::string &typeface_name, int height, IODevice &file);

	/// \brief Constructs a Font Vector
	///
	/// \param typeface_name = String Ref
	/// \param height = value
	/// \param directory = Virtual Directory
	Font_Vector( const std::string &typeface_name, int height, const VirtualDirectory &directory);

	/// \brief Constructs a Font Vector
	///
	/// \param desc = Font Description
	Font_Vector( const FontDescription &desc);

	/// \brief Constructs a Font Vector
	///
	/// \param desc = Font Description
	/// \param file = IODevice
	Font_Vector( const FontDescription &desc, IODevice &file);

	/// \brief Constructs a Font Vector
	///
	/// \param desc = Font Description
	/// \param directory = Virtual Directory
	Font_Vector( const FontDescription &desc, const VirtualDirectory &directory);

	~Font_Vector();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the font provider.
	FontProvider_Vector *get_provider() const;

	/// \brief Returns the amount of glyphs used by text.
	int get_glyph_count(const std::string &text);

	/// \brief Gets the glyph outline for a glyph.
	GlyphPrimitivesArrayOutline get_glyph_outline(int glyph);

	/// \brief Gets the glyph triangles for a glyph.
	GlyphPrimitivesArray get_glyph_triangles(int glyph);

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

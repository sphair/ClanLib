/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

class CL_Font_Vector_Impl;
class CL_FontMetrics;
class CL_VirtualDirectory;
class CL_GlyphPrimitivesArrayOutline;
class CL_GlyphPrimitivesArray;
class CL_FontProvider_Vector;

/// \brief Glyph Primitives Array
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_GlyphPrimitivesArray
{
//! Construction:
public:

	/// \brief Constructs a GlyphPrimitivesArray
	///
	/// \param num_triangles = value
	CL_GlyphPrimitivesArray(int num_triangles)
	{
		int size = num_triangles*3;
		vertex.resize(size);
	}

	~CL_GlyphPrimitivesArray()
	{
	}

	std::vector<CL_Vec2f> vertex;
};

/// \brief Glyph Primitives Array Outline
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_GlyphPrimitivesArrayOutline
{
//! Construction:
public:

	/// \brief Constructs a GlyphPrimitivesArrayOutline
	///
	/// \param num_lines = value
	CL_GlyphPrimitivesArrayOutline(int num_lines)
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
		std::vector<std::vector<CL_Vec2f> >::size_type size = vertex.size();
		vertex.resize(size+1);

		vertex[size].resize(num_lines+1);
	}

	std::vector< std::vector<CL_Vec2f> > vertex;
};

/// \brief Vector font drawing class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Font_Vector : public CL_Font
{
/// \name Construction
/// \{

public:
/// \brief Constructs vector font.
	CL_Font_Vector();

	/// \brief Constructs a Font Vector
	///
	/// \param typeface_name = String Ref
	/// \param height = value
	CL_Font_Vector( const CL_StringRef &typeface_name, int height);

	/// \brief Constructs a Font Vector
	///
	/// \param typeface_name = String Ref
	/// \param height = value
	/// \param file = IODevice
	CL_Font_Vector( const CL_StringRef &typeface_name, int height, CL_IODevice &file);

	/// \brief Constructs a Font Vector
	///
	/// \param typeface_name = String Ref
	/// \param height = value
	/// \param directory = Virtual Directory
	CL_Font_Vector( const CL_StringRef &typeface_name, int height, const CL_VirtualDirectory &directory);

	/// \brief Constructs a Font Vector
	///
	/// \param desc = Font Description
	CL_Font_Vector( const CL_FontDescription &desc);

	/// \brief Constructs a Font Vector
	///
	/// \param desc = Font Description
	/// \param file = IODevice
	CL_Font_Vector( const CL_FontDescription &desc, CL_IODevice &file);

	/// \brief Constructs a Font Vector
	///
	/// \param desc = Font Description
	/// \param directory = Virtual Directory
	CL_Font_Vector( const CL_FontDescription &desc, const CL_VirtualDirectory &directory);

	~CL_Font_Vector();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the font provider.
	CL_FontProvider_Vector *get_provider() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Returns the amount of glyphs used by text.
	int get_glyph_count(const CL_StringRef &text);

	/// \brief Gets the glyph outline for a glyph.
	CL_GlyphPrimitivesArrayOutline get_glyph_outline(int glyph);

	/// \brief Gets the glyph triangles for a glyph.
	CL_GlyphPrimitivesArray get_glyph_triangles(int glyph);

	/// \brief Find glyph indexes and inter-spacing (x,y) values for text.
	void get_glyphs(
		const CL_StringRef &text,
		int *out_glyphs,
		float *out_interspacing_x,
		float *out_interspacing_y);

	/// \brief Draw glyphs on device context.
	void draw_glyphs(
		CL_GraphicContext &gc,
		float x,
		float y,
		int *glyphs,
		int length,
		bool filled = true,
		float *interspacing_x = 0,
		float *interspacing_y = 0,
		const CL_Colorf &color = CL_Colorf::white);

	/// \brief Get glyphs size
	///
	/// \param glyphs = value
	/// \param length = value
	///
	/// \return Sizef
	CL_Sizef get_glyphs_size(int *glyphs, int length);

/// \}
/// \name Implementation
/// \{

/// \}
};

/// \}

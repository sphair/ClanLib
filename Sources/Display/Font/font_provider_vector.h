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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/


#pragma once


#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font.h"

#include <list>
#include <map>

class CL_FreetypeFont;
class CL_GlyphPrimitivesArray;
class CL_GlyphPrimitivesArrayOutline;
class CL_GlyphOutline;

class CL_FontEngine_Freetype;

class CL_FontProvider_Vector : public CL_FontProvider
{
/// \name Construction
/// \{

public:

	CL_FontProvider_Vector();

	virtual ~CL_FontProvider_Vector();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns information about the current font.
	virtual CL_FontMetrics get_font_metrics(CL_GraphicContext &gc);


/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the font provider.
	virtual void destroy();

	/// \brief Print text on gc.
	virtual void draw_text(CL_GraphicContext &gc, float x, float y, const CL_StringRef &text, const CL_Colorf &color);

	/// \brief Calculate size of text string.
	virtual CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);

	int get_glyph_count(const CL_StringRef &text);

	CL_GlyphPrimitivesArrayOutline get_glyph_outline(int glyph);

	CL_GlyphPrimitivesArray get_glyph_triangles(int glyph);

	void get_glyphs(
		const CL_StringRef &text,
		int *out_glyphs,
		float *out_interspacing_x,
		float *out_interspacing_y);

	void draw_glyphs(
		CL_GraphicContext &gc,
		float x,
		float y,
		int *glyphs,
		int length,
		bool filled,
		float *interspacing_x = 0,
		float *interspacing_y = 0,
		const CL_Colorf &color = CL_Colorf::white);

	CL_Sizef get_glyphs_size(int *glyphs, int length);

	int get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point);

	void load_font(const CL_FontDescription &desc);
	void load_font(const CL_FontDescription &desc, CL_IODevice &file);
	void load_font(const CL_FontDescription &desc, const CL_VirtualDirectory &directory);

/// \}
/// \name Implementation
/// \{

private:

	std::map<int, CL_GlyphOutline*> char_cache;

	CL_FontMetrics metrics;

	CL_FontEngine_Freetype *font_engine;

	int size_height;

/// \}
};





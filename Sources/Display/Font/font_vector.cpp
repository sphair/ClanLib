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
**    Mark Page
*/

#include "Display/precomp.h"

#include "API/Display/Font/font_vector.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "glyph_outline.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "font_provider_vector.h"

class CL_Font_Vector_Impl
{
public:
	CL_Font_Vector_Impl() : provider(0) { }

	~CL_Font_Vector_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_FontProvider_Vector *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Vector Construction:

CL_Font_Vector::CL_Font_Vector()
{
}

CL_Font_Vector::CL_Font_Vector( const CL_StringRef &typeface_name, int height) : CL_Font( new CL_FontProvider_Vector())
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc);
}

CL_Font_Vector::CL_Font_Vector( const CL_StringRef &typeface_name, int height, CL_IODevice &file) : CL_Font( new CL_FontProvider_Vector())
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc, file);
}

CL_Font_Vector::CL_Font_Vector( const CL_StringRef &typeface_name, int height, const CL_VirtualDirectory &directory) : CL_Font( new CL_FontProvider_Vector())
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc, directory);
}

CL_Font_Vector::CL_Font_Vector( const CL_FontDescription &desc) : CL_Font( new CL_FontProvider_Vector())
{
	get_provider()->load_font(desc);
}

CL_Font_Vector::CL_Font_Vector( const CL_FontDescription &desc, CL_IODevice &file) : CL_Font( new CL_FontProvider_Vector())
{
	get_provider()->load_font(desc, file);
}

CL_Font_Vector::CL_Font_Vector( const CL_FontDescription &desc, const CL_VirtualDirectory &directory) : CL_Font( new CL_FontProvider_Vector())
{
	get_provider()->load_font(desc, directory);
}

CL_Font_Vector::~CL_Font_Vector()
{

}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Vector Attributes:

CL_FontProvider_Vector *CL_Font_Vector::get_provider() const
{
	return static_cast <CL_FontProvider_Vector *> (CL_Font::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Vector Operations:

int CL_Font_Vector::get_glyph_count(const CL_StringRef &text)
{
	return get_provider()->get_glyph_count(text);
}

CL_GlyphPrimitivesArrayOutline CL_Font_Vector::get_glyph_outline(int glyph)
{
	return get_provider()->get_glyph_outline(glyph);
}

CL_GlyphPrimitivesArray CL_Font_Vector::get_glyph_triangles(int glyph)
{
	return get_provider()->get_glyph_triangles(glyph);
}

void CL_Font_Vector::get_glyphs(
	const CL_StringRef &text,
	int *out_glyphs,
	float *out_interspacing_x,
	float *out_interspacing_y)
{
	get_provider()->get_glyphs(text, out_glyphs, out_interspacing_x, out_interspacing_y);
}

void CL_Font_Vector::draw_glyphs(
	CL_GraphicContext &gc,
	float x,
	float y,
	int *glyphs,
	int length,
	bool filled,
	float *interspacing_x,
	float *interspacing_y,
	const CL_Colorf &color)
{
	get_provider()->draw_glyphs(gc, x,y, glyphs, length, filled, interspacing_x, interspacing_y, color);
}


CL_Sizef CL_Font_Vector::get_glyphs_size(int *glyphs, int length)
{
	return get_provider()->get_glyphs_size(glyphs,length);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Vector Implementation:



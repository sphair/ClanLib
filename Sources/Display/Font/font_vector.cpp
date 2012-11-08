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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font_Vector Construction:

Font_Vector::Font_Vector()
{
}

Font_Vector::Font_Vector( Canvas &canvas, const std::string &typeface_name, int height) : Font( new FontProvider_Vector())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc);
}

Font_Vector::Font_Vector( Canvas &canvas, const std::string &typeface_name, int height, IODevice &file) : Font( new FontProvider_Vector())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc, file);
}

Font_Vector::Font_Vector( Canvas &canvas, const std::string &typeface_name, int height, const VirtualDirectory &directory) : Font( new FontProvider_Vector())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc, directory);
}

Font_Vector::Font_Vector( Canvas &canvas, const FontDescription &desc) : Font( new FontProvider_Vector())
{
	get_provider()->load_font(desc);
}

Font_Vector::Font_Vector( Canvas &canvas, const FontDescription &desc, IODevice &file) : Font( new FontProvider_Vector())
{
	get_provider()->load_font(desc, file);
}

Font_Vector::Font_Vector( Canvas &canvas, const FontDescription &desc, const VirtualDirectory &directory) : Font( new FontProvider_Vector())
{
	get_provider()->load_font(desc, directory);
}

Font_Vector::~Font_Vector()
{

}

/////////////////////////////////////////////////////////////////////////////
// Font_Vector Attributes:

FontProvider_Vector *Font_Vector::get_provider() const
{
	return static_cast <FontProvider_Vector *> (Font::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// Font_Vector Operations:

int Font_Vector::get_glyph_count(const std::string &text)
{
	return get_provider()->get_glyph_count(text);
}

GlyphPrimitivesArrayOutline Font_Vector::get_glyph_outline(int glyph)
{
	return get_provider()->get_glyph_outline(glyph);
}

GlyphPrimitivesArray Font_Vector::get_glyph_triangles(int glyph)
{
	return get_provider()->get_glyph_triangles(glyph);
}

void Font_Vector::get_glyphs(
	const std::string &text,
	int *out_glyphs,
	float *out_interspacing_x,
	float *out_interspacing_y)
{
	get_provider()->get_glyphs(text, out_glyphs, out_interspacing_x, out_interspacing_y);
}

void Font_Vector::set_filled(bool enable)
{
	get_provider()->set_filled(enable);
}


Sizef Font_Vector::get_glyphs_size(int *glyphs, int length)
{
	return get_provider()->get_glyphs_size(glyphs,length);
}

/////////////////////////////////////////////////////////////////////////////
// Font_Vector Implementation:

}

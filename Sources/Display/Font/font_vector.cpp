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
**    Mark Page
*/

#include "Display/precomp.h"

#include "API/Display/Font/font_vector.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/IOData/file_system.h"
#include "API/Display/2D/shape2d.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "font_provider_vector.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font_Vector Construction:

Font_Vector::Font_Vector()
{
}

Font_Vector::Font_Vector( Canvas &canvas, const std::string &typeface_name, int height, const std::string &filename) : Font( new FontProvider_Vector())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	*this = Font_Vector(canvas, desc, filename);
}

Font_Vector::Font_Vector( Canvas &canvas, const FontDescription &desc, const std::string &filename) : Font( new FontProvider_Vector())
{
	get_provider()->load_font(desc, filename);
}

Font_Vector::Font_Vector( const Font &font) : Font(font)
{
	if (!get_provider())
	{
		throw Exception("Font is not of type Font_Vector");
	}
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

Rectf Font_Vector::get_bounding_box(const std::string &reference_string) const
{
	return get_provider()->get_bounding_box(reference_string);
}


/////////////////////////////////////////////////////////////////////////////
// Font_Vector Operations:

void Font_Vector::set_filled(bool enable)
{
	get_provider()->set_filled(enable);
}

void Font_Vector::set_texture(const Texture2D &src_texture, const Rectf &bounding_rect, const Rectf &texture_rect)
{
	get_provider()->set_texture(src_texture, bounding_rect, texture_rect);
}

void Font_Vector::set_texture(const Texture2D &src_texture, const Rectf &bounding_rect, const Rect &texture_rect)
{
	Sizef texture_size = src_texture.get_size();
	Rectf texture_rect_scaled( texture_rect.left / texture_size.width, texture_rect.top / texture_size.height, texture_rect.right / texture_size.width, texture_rect.bottom / texture_size.height);

	get_provider()->set_texture(src_texture, bounding_rect, texture_rect_scaled);
}


void Font_Vector::reset_texture()
{
	get_provider()->set_texture(Texture2D(), Rectf(), Rectf());
}

const std::vector<Vec2f> &Font_Vector::get_glyph_filled(unsigned int glyph)
{
	return get_provider()->get_glyph_filled(glyph);
}
const std::vector< std::vector<Vec2f> > &Font_Vector::get_glyph_outline(unsigned int glyph)
{
	return get_provider()->get_glyph_outline(glyph);

}



/////////////////////////////////////////////////////////////////////////////
// Font_Vector Implementation:

}

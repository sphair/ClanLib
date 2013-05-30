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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Font/font_system.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "font_provider_system.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font_manager.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font Construction:

Font_System::Font_System()
{
}

Font_System::Font_System(Canvas &canvas, const std::string &typeface_name, int height) : Font( new FontProvider_System())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	*this = Font_System(canvas, desc);
}

Font_System::Font_System(Canvas &canvas, const std::string &typeface_name, const std::string &file_name, int height) : Font( new FontProvider_System())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_filename(file_name);
	desc.set_height(height);
	*this = Font_System(canvas, desc);
}

Font_System::Font_System(Canvas &canvas, const FontDescription &desc) : Font( new FontProvider_System())
{
	load_font(canvas, desc);
}

Font_System::Font_System( const Font &font) : Font(font)
{
	if (!get_provider())
	{
		throw Exception("Font is not of type Font_System");
	}
}

Font_System::~Font_System()
{

}

/////////////////////////////////////////////////////////////////////////////
// Font_System Attributes:

FontProvider_System *Font_System::get_provider() const
{
	return static_cast <FontProvider_System *> (Font::get_provider());
}

Font_TextureGlyph *Font_System::get_glyph(GraphicContext &gc, int glyph)
{
	return (get_provider()->get_glyph(gc, glyph));
}

/////////////////////////////////////////////////////////////////////////////
// Font_System Operations:

void Font_System::set_texture_group(TextureGroup &new_texture_group)
{
	get_provider()->set_texture_group(new_texture_group);
}

void Font_System::load_font( GraphicContext &context, const FontDescription &desc)
{
	get_provider()->load_font(context, desc);
}

void Font_System::free_font()
{
	get_provider()->free_font();
}

void Font_System::insert_glyph(GraphicContext &gc, unsigned int glyph, Subtexture &sub_texture, const Point &offset, const Point &increment)
{
	get_provider()->insert_glyph(gc, glyph, sub_texture, offset, increment);
}

void Font_System::insert_glyph(GraphicContext &gc, const std::string &text)
{
	get_provider()->insert_glyph(gc, text);
}

void Font_System::set_font_metrics(const FontMetrics &metrics)
{
	get_provider()->set_font_metrics(metrics);
}

/////////////////////////////////////////////////////////////////////////////
// Font_System Implementation:

}

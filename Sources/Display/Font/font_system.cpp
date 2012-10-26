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

#include "Display/precomp.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Font/font_system.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "font_provider_system.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font Construction:

Font_System::Font_System()
{
}

Font_System::Font_System(
	GraphicContext &context, const std::string &typeface_name, int height) : Font( new FontProvider_System())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	load_font(context, desc);
}

Font_System::Font_System(
	GraphicContext &context, const FontDescription &desc) : Font( new FontProvider_System())
{
	load_font(context, desc);
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

void Font_System::insert_glyph(GraphicContext &gc, Font_System_Position &position, PixelBuffer &pixel_buffer)
{
	get_provider()->insert_glyph(gc, position, pixel_buffer);
}

void Font_System::insert_glyph(GraphicContext &gc, const std::string &text)
{
	get_provider()->insert_glyph(gc, text);
}

void Font_System::set_font_metrics(const FontMetrics &metrics)
{
	get_provider()->set_font_metrics(metrics);
}

void Font_System::register_font(const std::string &font_filename, const std::string &font_typeface)
{
	FontProvider_System::register_font(font_filename, font_typeface);
}

/////////////////////////////////////////////////////////////////////////////
// Font_System Implementation:

}

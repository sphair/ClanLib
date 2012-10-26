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
#include "API/Display/Font/font_freetype.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "font_provider_freetype.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font Construction:

Font_Freetype::Font_Freetype()
{
}

Font_Freetype::Font_Freetype(const std::string &typeface_name, int height) : Font(new FontProvider_Freetype())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc);
}

Font_Freetype::Font_Freetype(const std::string &typeface_name, int height, IODevice &file) : Font(new FontProvider_Freetype())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc, file);
}

Font_Freetype::Font_Freetype(const std::string &typeface_name, int height, const VirtualDirectory &directory) : Font(new FontProvider_Freetype())
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc, directory);
}

Font_Freetype::Font_Freetype(const FontDescription &desc) : Font(new FontProvider_Freetype())
{
	get_provider()->load_font(desc);
}

Font_Freetype::Font_Freetype(const FontDescription &desc, IODevice &file) : Font(new FontProvider_Freetype())
{
	get_provider()->load_font(desc, file);
}

Font_Freetype::Font_Freetype(const FontDescription &desc, const VirtualDirectory &directory) : Font(new FontProvider_Freetype())
{
	get_provider()->load_font(desc, directory);
}

Font_Freetype::Font_Freetype(const std::string &resource_id, ResourceManager *resources) : Font(new FontProvider_Freetype())
{
	get_provider()->load_font(resource_id, resources);
}

Font_Freetype::~Font_Freetype()
{
}

/////////////////////////////////////////////////////////////////////////////
// Font_Freetype Attributes:

FontProvider_Freetype *Font_Freetype::get_provider() const
{
	return static_cast <FontProvider_Freetype *> (Font::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// Font_Freetype Operations:

void Font_Freetype::set_texture_group(TextureGroup &new_texture_group)
{
	get_provider()->set_texture_group(new_texture_group);
}

/////////////////////////////////////////////////////////////////////////////
// Font_Freetype Implementation:

}

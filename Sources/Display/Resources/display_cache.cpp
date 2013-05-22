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
*/

#include "Display/precomp.h"
#include "API/Display/Resources/display_cache.h"
#include "display_cache_impl.h"
#include "xml_display_cache_provider.h"

namespace clan
{

DisplayCache::DisplayCache()
{
}

DisplayCache::DisplayCache(const XMLResourceDocument &doc)
	: impl(new DisplayCache_Impl(new XMLDisplayCacheProvider(doc)))
{
}

DisplayCache::DisplayCache(DisplayCacheProvider *provider)
	: impl(new DisplayCache_Impl(provider))
{
}

bool DisplayCache::is_null() const
{
	return !impl;
}

DisplayCacheProvider *DisplayCache::get_provider() const
{
	return impl->provider;
}

Resource<Sprite> DisplayCache::get_sprite(GraphicContext &gc, const std::string &id)
{
	return get_provider()->get_sprite(gc, id);
}

Resource<Image> DisplayCache::get_image(GraphicContext &gc, const std::string &id)
{
	return get_provider()->get_image(gc, id);
}

Resource<Texture> DisplayCache::get_texture(GraphicContext &gc, const std::string &id)
{
	return get_provider()->get_texture(gc, id);
}

Resource<Font> DisplayCache::get_font(Canvas &canvas, const FontDescription &desc)
{
	return get_provider()->get_font(canvas, desc);
}

}

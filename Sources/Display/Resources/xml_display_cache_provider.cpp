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
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/image.h"
#include "API/Display/Font/font.h"
#include "API/Display/Render/texture.h"
#include "xml_display_cache_provider.h"

namespace clan
{

XMLDisplayCacheProvider::XMLDisplayCacheProvider(const XMLResourceDocument &doc)
	: doc(doc)
{
}

XMLDisplayCacheProvider::~XMLDisplayCacheProvider()
{
}

Resource<Sprite> XMLDisplayCacheProvider::get_sprite(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Sprite> >::iterator it = sprites.find(id);
	if (it != sprites.end())
		return it->second;

	Resource<Sprite> sprite = load_sprite(gc, id);
	sprites[id] = sprite;
	return sprite;
}

Resource<Image> XMLDisplayCacheProvider::get_image(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Image> >::iterator it = images.find(id);
	if (it != images.end())
		return it->second;

	Resource<Image> image = load_image(gc, id);
	images[id] = image;
	return image;
}

Resource<Texture> XMLDisplayCacheProvider::get_texture(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Texture> >::iterator it = textures.find(id);
	if (it != textures.end())
		return it->second;

	Resource<Texture> texture = load_texture(gc, id);
	textures[id] = texture;
	return texture;
}

Resource<Font> XMLDisplayCacheProvider::get_font(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Font> >::iterator it = fonts.find(id);
	if (it != fonts.end())
		return it->second;

	Resource<Font> font = load_font(gc, id);
	fonts[id] = font;
	return font;
}

Resource<Sprite> XMLDisplayCacheProvider::load_sprite(GraphicContext &gc, const std::string &id)
{
	XMLResourceNode node = doc.get_resource(id);

	Sprite sprite;
	return Resource<Sprite>(sprite);
}

Resource<Image> XMLDisplayCacheProvider::load_image(GraphicContext &gc, const std::string &id)
{
	XMLResourceNode node = doc.get_resource(id);

	Image image;
	return Resource<Image>(image);
}

Resource<Texture> XMLDisplayCacheProvider::load_texture(GraphicContext &gc, const std::string &id)
{
	XMLResourceNode node = doc.get_resource(id);

	Texture texture;
	return Resource<Texture>(texture);
}

Resource<Font> XMLDisplayCacheProvider::load_font(GraphicContext &gc, const std::string &id)
{
	XMLResourceNode node = doc.get_resource(id);

	Font font;
	return Resource<Font>(font);
}

}

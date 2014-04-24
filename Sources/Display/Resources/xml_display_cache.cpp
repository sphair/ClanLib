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
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/image.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/IOData/path_help.h"
#include "xml_display_cache.h"
#include "../Font/font_impl.h"

namespace clan
{

XMLDisplayCache::XMLDisplayCache(const XMLResourceDocument &doc)
	: doc(doc)
{
}

XMLDisplayCache::~XMLDisplayCache()
{
}

Resource<Sprite> XMLDisplayCache::get_sprite(Canvas &canvas, const std::string &id)
{
	std::map<std::string, Resource<Sprite> >::iterator it = sprites.find(id);
	if (it != sprites.end())
	{
		Resource<Sprite> sprite = it->second;
		sprite.get() = sprite.get().clone();
		return sprite;
	}

	Resource<Sprite> sprite = Sprite::load(canvas, id, doc);
	sprites[id] = sprite;
	sprite.get() = sprite.get().clone();
	return sprite;
}

Resource<Image> XMLDisplayCache::get_image(Canvas &canvas, const std::string &id)
{
	std::map<std::string, Resource<Image> >::iterator it = images.find(id);
	if (it != images.end())
	{
		Resource<Image> image = it->second;
		image.get() = image.get().clone();
		return image;
	}
	
	Resource<Image> image = Image::load(canvas, id, doc);
	images[id] = image;
	image.get() = image.get().clone();
	return image;
}

Resource<CollisionOutline> XMLDisplayCache::get_collision(const std::string &id)
{
	std::map<std::string, Resource<CollisionOutline> >::iterator it = collisions.find(id);
	if (it != collisions.end())
	{
		Resource<CollisionOutline> collision = it->second;
		collision.get() = collision.get().clone();
		return collision;
	}

	Resource<CollisionOutline> collision = CollisionOutline::load(id, doc);
	collisions[id] = collision;
	collision.get() = collision.get().clone();
	return collision;
}

Resource<Texture> XMLDisplayCache::get_texture(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Texture> >::iterator it = textures.find(id);
	if (it != textures.end())
		return it->second;

	Resource<Texture> texture = Texture::load(gc, id, doc);
	textures[id] = texture;
	return texture;
}

Resource<Font> XMLDisplayCache::get_font(Canvas &canvas, const FontDescription &desc)
{
	std::string id = desc.get_unique_id();

	std::map<std::string, Resource<Font> >::iterator it = fonts.find(id);
	if (it != fonts.end())
		return it->second;

	Resource<Font> font = load_font(canvas, desc);
	fonts[id] = font;
	return font;
}

Resource<Font> XMLDisplayCache::load_font(Canvas &canvas, const FontDescription &desc)
{
	bool is_resource_font = false;

	if (doc.resource_exists(desc.get_typeface_name()))
	{
		DomElement font_element = doc.get_resource(desc.get_typeface_name()).get_element();
		std::string type = font_element.get_tag_name();
		if (type == "font")
		{
			is_resource_font = true;
		}
	}

	if (is_resource_font)
	{
		Callback<Resource<Sprite>(Canvas &, const std::string &)> cb(this, &XMLDisplayCache::get_sprite);
		return Resource<Font>(Font_Impl::load(canvas, desc, desc.get_typeface_name(), doc, cb));
	}
	else
	{
		return Resource<Font>(Font(canvas, desc));
	}
}

}

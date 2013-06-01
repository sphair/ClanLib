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
#include "API/Display/Font/font_sprite.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/IOData/path_help.h"
#include "xml_display_cache.h"

namespace clan
{

XMLDisplayCache::XMLDisplayCache(const XMLResourceDocument &doc)
	: doc(doc)
{
}

XMLDisplayCache::~XMLDisplayCache()
{
}

Resource<Sprite> XMLDisplayCache::get_sprite(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Sprite> >::iterator it = sprites.find(id);
	if (it != sprites.end())
		return it->second;

	Resource<Sprite> sprite = Sprite::load(gc, id, doc);
	sprites[id] = sprite;
	return sprite;
}

Resource<Image> XMLDisplayCache::get_image(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Image> >::iterator it = images.find(id);
	if (it != images.end())
		return it->second;

	Resource<Image> image = Image::load(gc, id, doc);
	images[id] = image;
	return image;
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
	// To do: Fix this retarded way of detecting sprite fonts..
	bool is_sprite_font = false;
	try
	{
		doc.get_resource(desc.get_typeface_name());
		is_sprite_font = true;
	}
	catch (Exception &)
	{
		is_sprite_font = false;
	}

	if (is_sprite_font)
	{
		Callback_2<Resource<Sprite>, GraphicContext &, const std::string &> cb(this, &XMLDisplayCache::get_sprite);
		return Resource<Font>(Font::load(canvas, desc.get_typeface_name(), doc, cb));
	}
	else
	{
		return Resource<Font>(Font(canvas, desc));
	}
}

}

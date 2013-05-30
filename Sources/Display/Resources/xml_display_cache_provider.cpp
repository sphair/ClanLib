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
#include "API/Display/2D/sprite_description.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/image_description.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/IOData/path_help.h"
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

Resource<Font> XMLDisplayCacheProvider::get_font(Canvas &canvas, const FontDescription &desc)
{
	std::string id = desc.get_unique_id();

	std::map<std::string, Resource<Font> >::iterator it = fonts.find(id);
	if (it != fonts.end())
		return it->second;

	Resource<Font> font = load_font(canvas, desc);
	fonts[id] = font;
	return font;
}

Resource<Sprite> XMLDisplayCacheProvider::load_sprite(GraphicContext &gc, const std::string &id)
{
	Sprite sprite(gc, id, doc);
	return Resource<Sprite>(sprite);
}

Resource<Image> XMLDisplayCacheProvider::load_image(GraphicContext &gc, const std::string &id)
{
	ImageImportDescription import_desc; // Why is this not part of ImageDescription?

	ImageDescription desc(gc, id, doc, import_desc);

	Image image(gc, desc);

	// More hacks that also belongs to ImageDescription:

	XMLResourceNode resource = doc.get_resource(id);

	DomNode cur_node = resource.get_element().get_first_child();
	while (!cur_node.is_null())
	{
		if (!cur_node.is_element())
			continue;

		DomElement cur_element = cur_node.to_element();

		std::string tag_name = cur_element.get_tag_name();

		// <color red="float" green="float" blue="float" alpha="float" />
		if (tag_name == "color")
		{
			Colorf color;
			color.r = (float)StringHelp::text_to_float(cur_element.get_attribute("red", "1.0"));
			color.g = (float)StringHelp::text_to_float(cur_element.get_attribute("green", "1.0"));
			color.b = (float)StringHelp::text_to_float(cur_element.get_attribute("blue", "1.0"));
			color.a = (float)StringHelp::text_to_float(cur_element.get_attribute("alpha", "1.0"));
			image.set_color(color);
		}
		// <scale x="float" y="float />
		else if (tag_name == "scale")
		{
			float scale_x = StringHelp::text_to_float(cur_element.get_attribute("x", "1.0"));
			float scale_y = StringHelp::text_to_float(cur_element.get_attribute("y", "1.0"));
			image.set_scale(scale_x, scale_y);
		}
		// <translation origin="string" x="integer" y="integer" />
		else if (tag_name == "translation")
		{
			std::string hotspot = cur_element.get_attribute("origin", "top_left");
			Origin origin;

			if(hotspot == "center")
				origin = origin_center;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_top_left;

			int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
			int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

			image.set_alignment(origin, xoffset, yoffset);
		}

		cur_node = cur_node.get_next_sibling();
	}

	return Resource<Image>(image);
}

Resource<Texture> XMLDisplayCacheProvider::load_texture(GraphicContext &gc, const std::string &id)
{
	XMLResourceNode resource = doc.get_resource(id);

	std::string type = resource.get_type();

	if (type != "texture")
		throw Exception(string_format("Resource '%1' is not of type 'texture'", id));

	ImageImportDescription import_desc; // The infamous ImageImportDescription strikes again!

	std::string filename = resource.get_element().get_attribute("file");
	FileSystem fs = resource.get_file_system();

	Texture2D texture(gc, PathHelp::combine(resource.get_base_path(), filename), fs, import_desc);

	return Resource<Texture>(texture);
}

Resource<Font> XMLDisplayCacheProvider::load_font(Canvas &canvas, const FontDescription &desc)
{
	Font font(canvas, desc);
	return Resource<Font>(font);
}

}

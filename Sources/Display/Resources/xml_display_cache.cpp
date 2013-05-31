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

	Resource<Texture> texture = load_texture(gc, id);
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

Resource<Texture> XMLDisplayCache::load_texture(GraphicContext &gc, const std::string &id)
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
		return load_sprite_font(canvas, desc.get_typeface_name());
	}
	else
	{
		return Resource<Font>(Font(canvas, desc));
	}
}

Resource<Font> XMLDisplayCache::load_sprite_font(Canvas &canvas, const std::string &id)
{
	XMLResourceNode resource = doc.get_resource(id);
	std::string type = resource.get_element().get_tag_name();
	
	if (type != "font")
		throw Exception(string_format("Resource '%1' is not of type 'font'", id));

	DomElement bitmap_element = resource.get_element().named_item("bitmap").to_element();

	FontMetrics font_metrics;

	if (!bitmap_element.is_null())
	{
		if (!bitmap_element.has_attribute("glyphs")) 
			throw Exception(string_format("Font resource %1 has no 'glyphs' attribute.", resource.get_name()));
		
		if (!bitmap_element.has_attribute("letters")) 
			throw Exception(string_format("Font resource %1 has no 'letters' attribute.", resource.get_name()));

		GraphicContext gc = canvas;
		Resource<Sprite> spr_glyphs = get_sprite(gc, bitmap_element.get_attribute("glyphs"));

		const std::string &letters = bitmap_element.get_attribute("letters");

		int spacelen = StringHelp::text_to_int(bitmap_element.get_attribute("spacelen", "-1"));
		bool monospace = StringHelp::text_to_bool(bitmap_element.get_attribute("monospace", "false"));

		// Modify the default font metrics, if specified

		if (bitmap_element.has_attribute("height")) 
			font_metrics.set_height(StringHelp::text_to_float(bitmap_element.get_attribute("height", "0")));

		if (bitmap_element.has_attribute("ascent")) 
			font_metrics.set_ascent(StringHelp::text_to_float(bitmap_element.get_attribute("ascent", "0")));

		if (bitmap_element.has_attribute("descent")) 
			font_metrics.set_descent(StringHelp::text_to_float(bitmap_element.get_attribute("descent", "0")));

		if (bitmap_element.has_attribute("internal_leading")) 
			font_metrics.set_internal_leading(StringHelp::text_to_float(bitmap_element.get_attribute("internal_leading", "0")));

		if (bitmap_element.has_attribute("external_leading")) 
			font_metrics.set_external_leading(StringHelp::text_to_float(bitmap_element.get_attribute("external_leading", "0")));

		if (bitmap_element.has_attribute("average_character_width")) 
			font_metrics.set_average_character_width(StringHelp::text_to_float(bitmap_element.get_attribute("average_character_width", "0")));

		if (bitmap_element.has_attribute("max_character_width")) 
			font_metrics.set_max_character_width(StringHelp::text_to_float(bitmap_element.get_attribute("max_character_width", "0")));

		if (bitmap_element.has_attribute("weight")) 
			font_metrics.set_weight(StringHelp::text_to_float(bitmap_element.get_attribute("weight", "0")));

		if (bitmap_element.has_attribute("overhang")) 
			font_metrics.set_overhang(StringHelp::text_to_float(bitmap_element.get_attribute("overhang", "0")));

		if (bitmap_element.has_attribute("digitized_aspect_x")) 
			font_metrics.set_digitized_aspect_x(StringHelp::text_to_float(bitmap_element.get_attribute("digitized_aspect_x", "0")));

		if (bitmap_element.has_attribute("digitized_aspect_y")) 
			font_metrics.set_digitized_aspect_y(StringHelp::text_to_float(bitmap_element.get_attribute("digitized_aspect_y", "0")));

		if (bitmap_element.has_attribute("italic")) 
			font_metrics.set_italic(StringHelp::text_to_bool(bitmap_element.get_attribute("italic", "0")));

		if (bitmap_element.has_attribute("underlined")) 
			font_metrics.set_underlined(StringHelp::text_to_bool(bitmap_element.get_attribute("underlined", "0")));

		if (bitmap_element.has_attribute("struck_out")) 
			font_metrics.set_struck_out(StringHelp::text_to_bool(bitmap_element.get_attribute("struck_out", "0")));

		if (bitmap_element.has_attribute("fixed_pitch")) 
			font_metrics.set_fixed_pitch(StringHelp::text_to_bool(bitmap_element.get_attribute("fixed_pitch", "0")));

		return Resource<Font>(Font_Sprite(spr_glyphs.get(), letters, spacelen, monospace, font_metrics));
	}
	else
	{
		throw Exception(string_format("Font resource %1 did not have a <bitmap> child element!", resource.get_name()));
	}
}

}

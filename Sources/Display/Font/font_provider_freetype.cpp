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
**    Magnus Norddahl
**    Mark Page
*/

#include "Display/precomp.h"
#include "font_provider_freetype.h"
#include "FontEngine/font_engine_freetype.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/2D/color.h"
#include "API/Core/XML/dom_element.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Freetype Construction:

FontProvider_Freetype::FontProvider_Freetype() : glyph_cache(), font_engine(0)
{
}

FontProvider_Freetype::~FontProvider_Freetype()
{
	free_font();
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Freetype Attributes:

FontMetrics FontProvider_Freetype::get_font_metrics()
{
	return glyph_cache.get_font_metrics();
}

Font_TextureGlyph *FontProvider_Freetype::get_glyph(GraphicContext &gc, unsigned int glyph)
{
	return glyph_cache.get_glyph(font_engine, gc, glyph);
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Freetype Operations:

void FontProvider_Freetype::draw_text(Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color)
{
	glyph_cache.draw_text(font_engine, canvas, xpos, ypos, text, color);
}

Size FontProvider_Freetype::get_text_size(GraphicContext &gc, const std::string &text)
{
	return glyph_cache.get_text_size(font_engine, gc, text);
}

void FontProvider_Freetype::set_font_metrics(const FontMetrics &metrics)
{
	glyph_cache.set_font_metrics(metrics);
}

void FontProvider_Freetype::set_texture_group(TextureGroup &new_texture_group)
{
	glyph_cache.set_texture_group(new_texture_group);
}

int FontProvider_Freetype::get_character_index(GraphicContext &gc, const std::string &text, const Point &point)
{
	return glyph_cache.get_character_index(font_engine, gc, text, point);
}

void FontProvider_Freetype::free_font()
{
	if (font_engine)
	{
		delete(font_engine);
		font_engine = NULL;
	}
}

void FontProvider_Freetype::load_font(const FontDescription &desc)
{
	std::string path = PathHelp::get_fullpath(desc.get_typeface_name(), PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(desc.get_typeface_name(), PathHelp::path_type_file);
	VirtualFileSystem vfs(path);
	IODevice file = vfs.get_root_directory().open_file_read(filename);
	load_font(desc, file);
}

void FontProvider_Freetype::load_font(const FontDescription &desc, const VirtualDirectory &directory)
{
	IODevice file = directory.open_file_read(desc.get_typeface_name());
	load_font(desc, file);
}

void FontProvider_Freetype::load_font(const std::string &resource_id, ResourceManager *resources)
{
	Resource resource = resources->get_resource(resource_id);
	if (resource.get_type() != "font")
		throw Exception(string_format("Resource '%1' is not of type 'font'", resource.get_name()));

	DomElement freetype_element = resource.get_element().named_item("freetype").to_element();
	if (freetype_element.is_null())
		throw Exception(string_format("Font resource '%1' has no 'freetype' child element", resource.get_name()));

	FontDescription desc;

	if (freetype_element.has_attribute("file"))
		desc.set_typeface_name(freetype_element.get_attribute("file"));
	else
		throw Exception(string_format("Font resource '%1' has no 'file' attribute", resource.get_name()));

	if (freetype_element.has_attribute("height"))
		desc.set_height(freetype_element.get_attribute_int("height", 0));
	else
		throw Exception(string_format("Font resource '%1' has no 'height' attribute", resource.get_name()));

	if (freetype_element.has_attribute("average_width"))
		desc.set_average_width(freetype_element.get_attribute_int("average_width", 0));

	if (freetype_element.has_attribute("anti_alias"))
		desc.set_anti_alias(freetype_element.get_attribute_bool("anti_alias", true));

	if (freetype_element.has_attribute("subpixel"))
		desc.set_subpixel(freetype_element.get_attribute_bool("subpixel", true));

	load_font(desc, resources->get_directory(resource));
}

void FontProvider_Freetype::load_font(const FontDescription &desc, IODevice &io_dev)
{
	free_font();

	if (desc.get_subpixel())
	{
		glyph_cache.enable_subpixel = true;
		glyph_cache.anti_alias = true;	// Implies anti_alias is set
	}
	else
	{
		glyph_cache.enable_subpixel = false;
		glyph_cache.anti_alias = desc.get_anti_alias();
	}

	// Load font from the opened file.
	font_engine = new FontEngine_Freetype(io_dev, desc.get_height(), desc.get_average_width());

	glyph_cache.font_metrics = font_engine->get_metrics();
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Freetype Implementation:

}

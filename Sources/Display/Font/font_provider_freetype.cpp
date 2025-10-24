/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Construction:

CL_FontProvider_Freetype::CL_FontProvider_Freetype() : glyph_cache(), font_engine(0)
{
}

CL_FontProvider_Freetype::~CL_FontProvider_Freetype()
{
	free_font();
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Attributes:

CL_FontMetrics CL_FontProvider_Freetype::get_font_metrics()
{
	return glyph_cache.get_font_metrics();
}

CL_Font_TextureGlyph *CL_FontProvider_Freetype::get_glyph(CL_GraphicContext &gc, unsigned int glyph)
{
	return glyph_cache.get_glyph(font_engine, gc, glyph);
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Operations:

void CL_FontProvider_Freetype::draw_text(CL_GraphicContext &gc, float xpos, float ypos, const CL_StringRef &text, const CL_Colorf &color)
{
	glyph_cache.draw_text(font_engine, gc, xpos, ypos, text, color);
}

CL_Size CL_FontProvider_Freetype::get_text_size(CL_GraphicContext &gc, const CL_StringRef &text)
{
	return glyph_cache.get_text_size(font_engine, gc, text);
}

void CL_FontProvider_Freetype::set_font_metrics(const CL_FontMetrics &metrics)
{
	glyph_cache.set_font_metrics(metrics);
}

void CL_FontProvider_Freetype::set_texture_group(CL_TextureGroup &new_texture_group)
{
	glyph_cache.set_texture_group(new_texture_group);
}

int CL_FontProvider_Freetype::get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point)
{
	return glyph_cache.get_character_index(font_engine, gc, text, point);
}

void CL_FontProvider_Freetype::destroy()
{
	delete this;
}

void CL_FontProvider_Freetype::free_font()
{
	if (font_engine)
	{
		delete(font_engine);
		font_engine = NULL;
	}
}

void CL_FontProvider_Freetype::load_font(const CL_FontDescription &desc)
{
	CL_String path = CL_PathHelp::get_fullpath(desc.get_typeface_name(), CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(desc.get_typeface_name(), CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_IODevice file = vfs.get_root_directory().open_file_read(filename);
	load_font(desc, file);
}

void CL_FontProvider_Freetype::load_font(const CL_FontDescription &desc, const CL_VirtualDirectory &directory)
{
	CL_IODevice file = directory.open_file_read(desc.get_typeface_name());
	load_font(desc, file);
}

void CL_FontProvider_Freetype::load_font(const CL_StringRef &resource_id, CL_ResourceManager *resources)
{
	CL_Resource resource = resources->get_resource(resource_id);
	if (resource.get_type() != "font")
		throw CL_Exception(cl_format("Resource '%1' is not of type 'font'", resource.get_name()));

	CL_DomElement freetype_element = resource.get_element().named_item("freetype").to_element();
	if (freetype_element.is_null())
		throw CL_Exception(cl_format("Font resource '%1' has no 'freetype' child element", resource.get_name()));

	CL_FontDescription desc;

	if (freetype_element.has_attribute("file"))
		desc.set_typeface_name(freetype_element.get_attribute("file"));
	else
		throw CL_Exception(cl_format("Font resource '%1' has no 'file' attribute", resource.get_name()));

	if (freetype_element.has_attribute("height"))
		desc.set_height(freetype_element.get_attribute_int("height", 0));
	else
		throw CL_Exception(cl_format("Font resource '%1' has no 'height' attribute", resource.get_name()));

	if (freetype_element.has_attribute("average_width"))
		desc.set_average_width(freetype_element.get_attribute_int("average_width", 0));

	if (freetype_element.has_attribute("anti_alias"))
		desc.set_anti_alias(freetype_element.get_attribute_bool("anti_alias", true));

	if (freetype_element.has_attribute("subpixel"))
		desc.set_subpixel(freetype_element.get_attribute_bool("subpixel", true));

	load_font(desc, resources->get_directory(resource));
}

void CL_FontProvider_Freetype::load_font(const CL_FontDescription &desc, CL_IODevice &io_dev)
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
	font_engine = new CL_FontEngine_Freetype(io_dev, desc.get_height(), desc.get_average_width());

	glyph_cache.font_metrics = font_engine->get_metrics();
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Implementation:

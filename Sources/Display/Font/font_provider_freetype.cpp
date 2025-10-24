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
#include "font_provider_freetype.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/draw.h"
#include "API/Core/Text/string_help.h"

#include "../2D/render_batch2d.h"
#include "../Render/graphic_context_impl.h"

#ifndef WIN32
#include "../X11/font_config.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Construction:

CL_FontProvider_Freetype::CL_FontProvider_Freetype(CL_GraphicContext &gc) : glyph_cache(gc), font_engine(0)
{
}

CL_FontProvider_Freetype::~CL_FontProvider_Freetype()
{
	free_font();
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Attributes:

CL_FontMetrics CL_FontProvider_Freetype::get_font_metrics(CL_GraphicContext &gc)
{
	return glyph_cache.get_font_metrics(gc);
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

void CL_FontProvider_Freetype::load_font(const CL_FontDescription &desc, CL_IODevice &io_dev)
{
	free_font();

	glyph_cache.anti_alias = true;	// Default, anti_alias enabled

	 // Load font from the opened file.
	font_engine = new CL_FontEngine_Freetype(io_dev, desc.get_height(), desc.get_average_width());

	if (desc.get_anti_alias_set())	// Anti-alias was set
	{
		glyph_cache.anti_alias = desc.get_anti_alias();	// Override the default
	}

	glyph_cache.font_metrics = font_engine->get_metrics();
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Implementation:

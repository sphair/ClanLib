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
**    Mark Page
*/

#include "Display/precomp.h"
#include "font_provider_system.h"

#ifdef WIN32
#include "FontEngine/font_engine_win32.h"
#elif defined(__APPLE__)
#include "FontEngine/font_engine_cocoa.h"
#else
#include "X11/font_engine_freetype.h"
#include "X11/font_provider_freetype.h"
#include "../Display/X11/font_config.h"
#endif

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Text/string_help.h"

#include "../2D/render_batch_triangle.h"
#include "../Render/graphic_context_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// FontProvider_System Construction:

FontProvider_System::FontProvider_System() : glyph_cache(), font_engine(0)
{
}

FontProvider_System::~FontProvider_System()
{
	free_font();
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_System Attributes:

FontMetrics FontProvider_System::get_font_metrics()
{
	return glyph_cache.get_font_metrics();
}

Font_TextureGlyph *FontProvider_System::get_glyph(GraphicContext &gc, unsigned int glyph)
{
	return glyph_cache.get_glyph(font_engine, gc, glyph);
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_System Operations:

void FontProvider_System::insert_glyph(GraphicContext &gc, unsigned int glyph, Subtexture &sub_texture, const Point &offset, const Point &increment)
{
	glyph_cache.insert_glyph(gc, glyph, sub_texture, offset, increment);
}

void FontProvider_System::insert_glyph(GraphicContext &gc, const std::string &text)
{
	glyph_cache.insert_glyph(font_engine, gc, text);
}

void FontProvider_System::draw_text(Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color)
{
	glyph_cache.draw_text(font_engine, canvas, xpos, ypos, text, color);
}

Size FontProvider_System::get_text_size(GraphicContext &gc, const std::string &text)
{
	return glyph_cache.get_text_size(font_engine, gc, text);
}

void FontProvider_System::set_font_metrics(const FontMetrics &metrics)
{
	glyph_cache.set_font_metrics(metrics);
}

void FontProvider_System::set_texture_group(TextureGroup &new_texture_group)
{
	glyph_cache.set_texture_group(new_texture_group);
}

int FontProvider_System::get_character_index(GraphicContext &gc, const std::string &text, const Point &point)
{
	return glyph_cache.get_character_index(font_engine, gc, text, point);
}

void FontProvider_System::free_font()
{
	if (font_engine)
	{
		delete(font_engine);
		font_engine = NULL;
	}
}

void FontProvider_System::load_font( GraphicContext &context, const FontDescription &desc, const std::string &filename)
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

#ifdef WIN32
	font_engine = new FontEngine_Win32(desc, filename);
	glyph_cache.font_metrics = font_engine->get_metrics();
    
#elif defined(__APPLE__)
    
    font_engine = new FontEngine_Cocoa(desc, filename);
    glyph_cache.font_metrics = font_engine->get_metrics();

#else
	std::string font_file_path = filename;
	if (font_file_path.empty())
	{
	    // Obtain the best matching font file from fontconfig.
		FontConfig &fc = FontConfig::instance();
		font_file_path = fc.match_font(desc);
	}

	std::string path = PathHelp::get_fullpath(font_file_path, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(font_file_path, PathHelp::path_type_file);
	FileSystem vfs(path);
	IODevice io_dev = vfs.open_file(filename);

	int average_width = desc.get_average_width();
	int height = desc.get_height();

	// Ensure width and height are positive
	if (average_width < 0) average_width =-average_width;
	if (height < 0) height =-height;

	font_engine = new FontEngine_Freetype(io_dev, average_width, height);

	glyph_cache.font_metrics = font_engine->get_metrics();

#endif
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_System Implementation:

}

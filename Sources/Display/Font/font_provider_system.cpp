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
#include "font_provider_system.h"

#ifdef USE_MS_FONT_RENDERER
#include "FontEngine/font_engine_win32.h"
#endif
#ifdef __APPLE__
#include "FontEngine/font_engine_cocoa.h"
#else
#include "FontEngine/font_engine_freetype.h"
#include "font_provider_freetype.h"
#endif

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
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

#ifndef WIN32
#include "../X11/font_config.h"
#endif

namespace clan
{

#ifndef WIN32
std::map<std::string, std::string > FontProvider_System::font_register_cache;
#endif

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

void FontProvider_System::insert_glyph(GraphicContext &gc, Font_System_Position &position, PixelBuffer &pixel_buffer)
{
	glyph_cache.insert_glyph(gc, position, pixel_buffer);
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

void FontProvider_System::load_font( GraphicContext &context, const FontDescription &desc)
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

#ifdef USE_MS_FONT_RENDERER

	font_engine = new FontEngine_Win32(desc);
	glyph_cache.font_metrics = font_engine->get_metrics();

#elif defined(WIN32)

	HFONT handle = CreateFont(
		desc.get_height(), desc.get_average_width(),
		(int) (desc.get_escapement() * 10 + 0.5),
		(int) (desc.get_orientation() * 10 + 0.5),
		desc.get_weight(),
		desc.get_italic() ? TRUE : FALSE,
		desc.get_underline() ? TRUE : FALSE,
		desc.get_strikeout() ? TRUE : FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		(desc.get_fixed_pitch() ? FIXED_CL_PITCH : DEFAULT_CL_PITCH) | FF_DONTCARE,
		StringHelp::utf8_to_ucs2(desc.get_typeface_name()).c_str());
	if (handle == 0)
		throw Exception("CreateFont failed");

	HDC dc = GetDC(0);
	HGDIOBJ old_font = SelectObject(dc, handle);
	DWORD font_file_size = GetFontData(dc, 0, 0, 0, 0);
	DataBuffer font_file(font_file_size);
	DWORD result = GetFontData(dc, 0, 0, font_file.get_data(), font_file.get_size());
	SelectObject(dc, old_font);
	ReleaseDC(0, dc);
	DeleteObject(handle);
	if (result == GDI_ERROR)
		throw Exception("GetFontData failed");

	IODevice_Memory font_iodevice(font_file);
	font_engine = new FontEngine_Freetype(font_iodevice, desc.get_height(), desc.get_average_width());
	glyph_cache.font_metrics = font_engine->get_metrics();
    
#elif defined(__APPLE__)
    
    font_engine = new FontEngine_Cocoa(desc);
    glyph_cache.font_metrics = font_engine->get_metrics();

#else

	FontDescription new_desc = get_registered_font(desc);

	std::string font_file_path = new_desc.get_typeface_name();

	// Try opening the font file.
	IODevice io_dev;
	try
	{
		io_dev = File(font_file_path, File::open_existing, File::access_read);
	}
	catch(Exception error)
	{
		throw Exception(string_format("Cannot open font file: \"%1\"", font_file_path));
	}

	float size_height =  new_desc.get_height();

	// Load font from the opened file.
	font_engine = new FontEngine_Freetype(io_dev, size_height, new_desc.get_average_width());
	glyph_cache.font_metrics = font_engine->get_metrics();


#endif
}

void FontProvider_System::register_font(const std::string &font_filename, const std::string &font_typeface)
{
#ifdef WIN32
	int fonts_added = AddFontResourceEx(StringHelp::utf8_to_ucs2(font_filename).c_str(), FR_PRIVATE|FR_NOT_ENUM, 0);
	if(fonts_added == 0)
		throw Exception("Unable to register font " + font_filename);
#else
	std::map<std::string, std::string >::iterator find_it;
	find_it = font_register_cache.find(font_typeface);
	if (find_it == font_register_cache.end())	// Ensure not already registered
	{
		font_register_cache[font_typeface] = font_filename;
	}
#endif
}


/////////////////////////////////////////////////////////////////////////////
// FontProvider_System Implementation:

#ifndef WIN32
FontDescription FontProvider_System::get_registered_font(const FontDescription &desc)
{
	int average_width = desc.get_average_width();
	int height = desc.get_height();

	FontDescription new_desc;
	new_desc.clone(desc);
	new_desc.set_average_width(average_width);
	new_desc.set_height(height);

	// Check for a registered font
	std::map<std::string, std::string >::iterator find_it;
	find_it = font_register_cache.find(desc.get_typeface_name());
	if (find_it != font_register_cache.end())	// Found the registered font
	{
		new_desc.set_typeface_name(find_it->second);
	}
	else
	{
#if !defined(__APPLE__)
        // Obtain the best matching font file from fontconfig.
		FontConfig &fc = FontConfig::instance();
		std::string font_file_path = fc.match_font(new_desc);
		new_desc.set_typeface_name(font_file_path);
#endif
	}
	return new_desc;
}
#endif

}

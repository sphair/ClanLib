/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "freetype_font.h"
#include "freetype_font_provider.h"

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font_metrics.h"
#include "font_cache_native.h"

#ifndef WIN32
#include "Display/X11/font_config.h"
#endif

std::map<CL_String, CL_String > CL_FontProvider_Freetype::font_register_cache;

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Construction:

CL_FontProvider_Freetype::CL_FontProvider_Freetype() : target(target), handle(0)
{
}

void CL_FontProvider_Freetype::load_font( const CL_StringRef &typeface_name, int height)
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	load_font(desc);
}

void CL_FontProvider_Freetype::load_font(const CL_FontDescription &desc)
{
	if (desc.get_anti_alias_set())	// Anti-alias was set
	{
		anti_alias = desc.get_anti_alias();	// Override the default
	}
	else
	{
		anti_alias = true;
	}

	int average_width = desc.get_average_width();
	int height = desc.get_height();

	// Ensure width and height are positive
	if (average_width < 0) average_width =-average_width;
	if (height < 0) height =-height;

	size_height = height;

	// Try opening the font file.
	CL_IODevice io_dev;
	try
	{
		io_dev = CL_File(desc.get_typeface_name(), CL_File::open_existing, CL_File::access_read);
	}
	catch(CL_Exception error)
	{
		throw CL_Exception(cl_format("Cannot open font file: \"./%1\"", desc.get_typeface_name()));
	}

        // Load font from the opened file.
	CL_FreetypeFontProvider &provider = CL_FreetypeFontProvider::instance();

	if (handle)
	{
		delete(handle);
		handle = NULL;
	}

	handle = provider.load_font(io_dev, height, average_width);

	unique_name = desc.get_typeface_name() + cl_format(
		cl_text("%1-%2-%3-%4-%5-%6-%7"),
		height,
		average_width,
		desc.get_escapement(),
		desc.get_orientation(),
		desc.get_weight(),
		(desc.get_italic() ? 1 : 0) |
		(desc.get_underline() ? 2 : 0) |
		(desc.get_strikeout() ? 4 : 0) |
		(desc.get_fixed_pitch() ? 8 : 0),
		anti_alias ? 1 : 0);
}

CL_FontProvider_Freetype::~CL_FontProvider_Freetype()
{
	if (handle)
	{
		delete(handle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Attributes:

CL_FreetypeFont *CL_FontProvider_Freetype::get_handle()
{
	return handle;
}

CL_FontMetrics CL_FontProvider_Freetype::get_font_metrics(CL_GraphicContext &gc)
{
	CL_FreetypeFont *vptr = get_handle();
	if (vptr)
	{
		return vptr->get_font_metrics();
	}

	return CL_FontMetrics(
		0,0,0,0,0,0,0,0,0,0,
		false, false, false,false);
}

CL_FontDescription CL_FontProvider_Freetype::get_registered_font(const CL_FontDescription &desc, bool font_size_win32)
{
	int average_width = desc.get_average_width();
	int height = desc.get_height();

	if (font_size_win32)
	{
		// Attempt to convert the point sizes (to match WIN32 with FreeType)
		if (average_width==0)	// Unset width
		{
			if (height <0)
			{
				average_width = -height;
				average_width = (average_width * 80) / 96;
			}
			else
			{
				// I do not know why this formula works,  but it seems to obtain the best result
				average_width = ( height * 80 * 80 ) / (96 * 96);
			}
		}

		if (height <0)
		{
			height = -height;
		}
		else
		{
			height = (height * 80) / 96;
		}
	}

	CL_FontDescription new_desc;
	new_desc.copy(desc);
	new_desc.set_average_width(average_width);
	new_desc.set_height(height);

	// Check for a registered font
	std::map<CL_String, CL_String >::iterator find_it;
	find_it = font_register_cache.find(desc.get_typeface_name());
	if (find_it != font_register_cache.end())	// Found the registered font
	{
		new_desc.set_typeface_name(find_it->second);
	}
	else
	{
#ifndef WIN32
	        // Obtain the best matching font file from fontconfig.
			CL_FontConfig &fc = CL_FontConfig::instance();
			CL_String font_file_path = fc.match_font(new_desc);
			new_desc.set_typeface_name(font_file_path);
#endif
	}
	return new_desc;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Operations:

void CL_FontProvider_Freetype::destroy()
{
	delete this;
}

FontGlyphCache *CL_FontProvider_Freetype::font_cache_load(CL_GraphicContextProvider *gc_provider, const CL_StringRef &text)
{
	FontGlyphCache *glyph_cache = cl_fontcache_native.get_glyph_cache(get_unique_name());

	if (text.length() == 0)
		return glyph_cache;

	FontGlyphRange range(text[0] + 1); // Make sure range does not match first char
	CL_FontPixelBuffer **glyphs = 0;
	CL_String::size_type length = text.length();
	const CL_String::char_type *t = text.data();
	for (CL_String::size_type p = 0; p < length; p++)
	{
		unsigned int c = t[p];
		if (c < range.start || c > range.end)
		{
			unsigned int start = (c & 0xffffff00);
			unsigned int end = start + 255;
			range = FontGlyphRange(start, end);

			std::map<FontGlyphRange, CL_FontPixelBuffer **>::iterator it = glyph_cache->glyphs.find(range);
			if (it == glyph_cache->glyphs.end())
			{
				glyphs = new CL_FontPixelBuffer*[256];
				for (int i = 0; i < 256; i++)
					glyphs[i] = 0;
				glyph_cache->glyphs[range] = glyphs;
			}
			else
			{
				glyphs = it->second;
			}
		}

		unsigned int glyph_index = c - range.start;
		if (glyphs[glyph_index] == 0)
		{
			// If a font does not have a handle (ie a bitmap font), create an empty one (for simplicity)
			if (!get_handle())
			{
				glyphs[glyph_index] = new CL_FontPixelBuffer();
			}
			else
			{
				glyphs[glyph_index] = new CL_FontPixelBuffer(get_font_glyph(gc_provider, c, anti_alias, CL_Colorf::white));
			}
		}
	}

	return glyph_cache;
}
void CL_FontProvider_Freetype::draw_text(CL_GraphicContext &gc, int x, int y, const CL_StringRef &text, const CL_Colorf &color)
{
	if (text.length() == 0)
		return;

	CL_GraphicContextProvider *gc_provider = (CL_GraphicContextProvider *) gc.get_provider();

	FontGlyphCache *glyph_cache = font_cache_load(gc_provider, text);
	if (glyph_cache == 0)
		return;

	FontGlyphRange range(text[0] + 1); // Make sure range does not match first char
	CL_FontPixelBuffer **glyphs = 0;

	int pos_x = 0;
	int pos_y = 0;
	CL_String::size_type length = text.length();
	const CL_String::char_type *t = text.data();
	for (CL_String::size_type p = 0; p < length; p++)
	{
		unsigned int c = t[p];
		if (c < range.start || c > range.end)
		{
			unsigned int start = (c & 0xffffff00);
			unsigned int end = start + 255;
			range = FontGlyphRange(start, end);

			std::map<FontGlyphRange, CL_FontPixelBuffer **>::iterator it = glyph_cache->glyphs.find(range);
			if (it == glyph_cache->glyphs.end())
				continue; // Glyph unknown - ignore it

			glyphs = it->second;
		}

		unsigned int glyph_index = c - range.start;
		if (glyphs[glyph_index] == 0)
			continue; // Glyph unknown - ignore it

		CL_FontPixelBuffer *pixelbuffer = glyphs[glyph_index];
		if (pixelbuffer->glyph)
		{
			if (!pixelbuffer->empty_buffer)
			{

				gc.draw_pixels(
					(float)x + pos_x + pixelbuffer->offset.x,
					(float)y + pos_y + pixelbuffer->offset.y,
					1.0f, 1.0f, pixelbuffer->buffer, color);
			}

			pos_x += pixelbuffer->increment.x;
			pos_y += pixelbuffer->increment.y;
		}
	}

}

CL_Size CL_FontProvider_Freetype::get_text_size(CL_GraphicContext &gc, const CL_StringRef &text)
{
	if (text.length() == 0)
		return CL_Size(0, 0);

	CL_GraphicContextProvider *gc_provider = (CL_GraphicContextProvider *) gc.get_provider();

	// Ensure that the font exists in the cache
	FontGlyphCache *glyph_cache = font_cache_load(gc_provider, text);
	if (glyph_cache == 0)
		return CL_Size(0, 0);

	FontGlyphRange range(text[0] + 1); // Make sure range does not match first char
	CL_FontPixelBuffer **glyphs = 0;

	int pos_x = 0;
	int pos_y = 0;

	// Calculate the font size using the same method as it is drawn
	CL_String::size_type length = text.length();
	const CL_String::char_type *t = text.data();
	for (CL_String::size_type p = 0; p < length; p++)
	{
		unsigned int c = t[p];
		if (c < range.start || c > range.end)
		{
			unsigned int start = (c & 0xffffff00);
			unsigned int end = start + 255;
			range = FontGlyphRange(start, end);

			std::map<FontGlyphRange, CL_FontPixelBuffer **>::iterator it = glyph_cache->glyphs.find(range);
			if (it == glyph_cache->glyphs.end())
				continue; // Glyph unknown - ignore it

			glyphs = it->second;
		}

		unsigned int glyph_index = c - range.start;
		if (glyphs[glyph_index] == 0)
			continue; // Glyph unknown - ignore it

		CL_FontPixelBuffer *pixelbuffer = glyphs[glyph_index];
		if (pixelbuffer->glyph)
		{
			pos_x += pixelbuffer->increment.x;
			pos_y += pixelbuffer->increment.y;
		}
	}

	return CL_Size(pos_x, size_height);
}

CL_FontPixelBuffer CL_FontProvider_Freetype::get_font_glyph(CL_GraphicContextProvider *gc_provider, int glyph, bool anti_alias, const CL_Colorf &color)
{
	CL_FontPixelBuffer font_buffer;

	CL_FreetypeFont *vptr = get_handle();
	if (!vptr) return font_buffer;

	font_buffer.glyph = glyph;

	font_buffer = vptr->create_pixelbuffer(glyph, anti_alias, color);
	return font_buffer;
}

void CL_FontProvider_Freetype::register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface)
{
	std::map<CL_String, CL_String >::iterator find_it;
	find_it = font_register_cache.find(font_typeface);
	if (find_it == font_register_cache.end())	// Ensure not already registered
	{
		font_register_cache[font_typeface] = font_filename;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Implementation:



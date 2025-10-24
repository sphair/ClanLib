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
**    Kevin Bluck
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "font_provider_win32.h"

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/draw.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font_metrics.h"
#include "font_cache_native.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Win32 Construction:

CL_FontProvider_Win32::CL_FontProvider_Win32() : glyph_cache(0), target(target), handle(0)
{
}

void CL_FontProvider_Win32::load_font( CL_GraphicContext &gc, const CL_StringRef &typeface_name, int height)
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	load_font(gc, desc);
}

void CL_FontProvider_Win32::load_font(CL_GraphicContext &gc, const CL_FontDescription &desc)
{
	if (handle)
	{
		throw CL_Exception(cl_text("Win32 font already loaded"));
	}

	anti_alias = true;	// Default, anti_alias enabled (may be modified by WIN32 later)
	underlined_font = desc.get_underline();
	strikeout_font = desc.get_strikeout();

	handle = CreateFont(
		desc.get_height(), desc.get_average_width(),
		(int) (desc.get_escapement() * 10 + 0.5),
		(int) (desc.get_orientation() * 10 + 0.5),
		desc.get_weight(),
		desc.get_italic() ? TRUE : FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		(desc.get_fixed_pitch() ? FIXED_PITCH : DEFAULT_PITCH) | FF_DONTCARE,
		desc.get_typeface_name().c_str());

	if (handle==NULL)
	{
		throw CL_Exception(cl_text("Unable to create font - Windows CreateFont() failed"));
	}

	// Calculate what windows says the font height size should be
	HDC hdc = ::GetDC(NULL);	//HDC hdc = target->get_drawable();
	int old_mode = SetMapMode(hdc, MM_TEXT);
	HGDIOBJ old_font = SelectObject( hdc, handle );
	SIZE size;
	TEXTMETRIC text_metrics;
	if (GetTextMetrics(hdc, &text_metrics))
	{
		if (text_metrics.tmHeight <= 18)
			anti_alias = false;
		size.cx = 0;
		size.cy = text_metrics.tmHeight;
	}
	else
	{
		CL_StringRef text(cl_text("."));
		GetTextExtentPoint32(hdc, text.data(), text.length(), &size);
	}

	OUTLINETEXTMETRIC outline_metric;
	GetOutlineTextMetrics(hdc, sizeof(outline_metric), &outline_metric);
	otms_underscore_position = outline_metric.otmsUnderscorePosition;
	otms_underscore_size = outline_metric.otmsUnderscoreSize;
	otms_strikeout_size = outline_metric.otmsStrikeoutSize;
	otms_strikeout_position = outline_metric.otmsStrikeoutPosition;

	SelectObject(hdc, old_font);
	SetMapMode(hdc, old_mode);
	::ReleaseDC(NULL, hdc);
	size_height = size.cy;

	if (desc.get_anti_alias_set())	// Anti-alias was set
	{
		anti_alias = desc.get_anti_alias();	// Override the default
	}

	unique_name = desc.get_typeface_name() + cl_format(
		cl_text("%1-%2-%3-%4-%5-%6-%7"),
		desc.get_height(),
		desc.get_average_width(),
		desc.get_escapement(),
		desc.get_orientation(),
		desc.get_weight(),
		(desc.get_italic() ? 1 : 0) |
		//(desc.get_underline() ? 2 : 0) |
		//(desc.get_strikeout() ? 4 : 0) |
		(desc.get_fixed_pitch() ? 8 : 0),
		anti_alias ? 1 : 0);

	glyph_cache = cl_fontcache_native.get_glyph_cache(get_unique_name());
}

CL_FontProvider_Win32::~CL_FontProvider_Win32()
{
	if (handle)
	{
		DeleteObject(handle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Win32 Attributes:

HFONT CL_FontProvider_Win32::get_handle()
{
	return handle;
}


CL_FontMetrics CL_FontProvider_Win32::get_font_metrics(CL_GraphicContext &gc)
{
	if (get_handle())
	{
		HDC hdc = ::GetDC(NULL);	//HDC hdc = target->get_drawable();
		int old_mode = SetMapMode(hdc, MM_TEXT);
		HGDIOBJ old_font = SelectObject(hdc, get_handle());

		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);

		SelectObject(hdc, old_font);
		SetMapMode(hdc, old_mode);
		::ReleaseDC(NULL, hdc);

		return CL_FontMetrics(
			tm.tmHeight,
			tm.tmAscent, 
			tm.tmDescent, 
			tm.tmInternalLeading,
			tm.tmExternalLeading,
			tm.tmAveCharWidth,
			tm.tmMaxCharWidth,
			tm.tmWeight,
			tm.tmOverhang, 
			tm.tmDigitizedAspectX,
			tm.tmDigitizedAspectY,
			tm.tmItalic != 0, 
			tm.tmUnderlined != 0, 
			tm.tmStruckOut != 0,
			tm.tmAveCharWidth == tm.tmMaxCharWidth);
	}

	return CL_FontMetrics(
		0,
		0, 
		0, 
		0,
		0,
		0,
		0,
		0, 
		0,
		0,
		false, 
		false, 
		false,
		false);
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Win32 Operations:

void CL_FontProvider_Win32::destroy()
{
	delete this;
}

void CL_FontProvider_Win32::draw_text(CL_GraphicContext &gc, int x, int y, const CL_StringRef &text, const CL_Colorf &color)
{
	if (text.length() == 0)
		return;

	CL_GraphicContextProvider *gc_provider = gc.get_provider();

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
	if (underlined_font)
	{
		CL_Draw::fill(gc, CL_Rect(x, y-otms_underscore_position, x + pos_x, y-otms_underscore_position+otms_underscore_size), color);
	}
	if (strikeout_font)
	{
		CL_Draw::fill(gc, CL_Rect(x, y-otms_strikeout_position-otms_strikeout_size, x + pos_x, y-otms_strikeout_position), color);
	}
}

FontGlyphCache *CL_FontProvider_Win32::font_cache_load(CL_GraphicContextProvider *gc_provider, const CL_StringRef &text)
{
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

CL_Size CL_FontProvider_Win32::get_text_size(CL_GraphicContext &gc, const CL_StringRef &text)
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

CL_FontPixelBuffer CL_FontProvider_Win32::get_font_glyph(CL_GraphicContextProvider *gc_provider, int glyph, bool anti_alias, const CL_Colorf &color)
{
	HDC hdc = ::GetDC(NULL);	//HDC hdc = target->get_drawable();
	int old_mode = SetMapMode(hdc, MM_TEXT);
	HGDIOBJ old_font = 0;
	old_font = SelectObject(hdc, get_handle());

	CL_FontPixelBuffer font_buffer;

	unsigned int r = (unsigned int) (color.get_red() * 255);
	unsigned int g = (unsigned int) (color.get_green() * 255);
	unsigned int b = (unsigned int) (color.get_blue() * 255);
	unsigned int a = (unsigned int) (color.get_alpha() * 255);
	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;
	if (a > 255)
		a = 255;
	unsigned int c = (r << 24) + (g << 16) + (b << 8);

	int pos_x = 0;
	int pos_y = 0;

	font_buffer.glyph = glyph;

	CL_DataBuffer glyph_buffer;

	if (anti_alias) // anti-alias font
	{
		GLYPHMETRICS glyph_metrics;
		MAT2 mat2;
		memset(&mat2, 0, sizeof(MAT2));
		mat2.eM11.value = 1;
		mat2.eM22.value = 1;
		DWORD buffer_size = GetGlyphOutline(hdc, glyph, GGO_GRAY8_BITMAP, &glyph_metrics, 0, 0, &mat2);
		if (buffer_size == GDI_ERROR)
		{
			ABC abc;
			if (GetCharABCWidths(hdc, glyph, glyph, &abc))
				pos_x += abc.abcA + abc.abcB + abc.abcC;
			goto failed;
		}

		if (buffer_size == 0)
		{
			pos_x += glyph_metrics.gmCellIncX;
			pos_y += glyph_metrics.gmCellIncY;
			goto failed;
		}
		glyph_buffer.set_size(buffer_size);
		DWORD result = GetGlyphOutline(hdc, glyph, GGO_GRAY8_BITMAP, &glyph_metrics, glyph_buffer.get_size(), glyph_buffer.get_data(), &mat2);
		if (result == GDI_ERROR)
		{
			ABC abc;
			if (GetCharABCWidths(hdc, glyph, glyph, &abc))
				pos_x += abc.abcA + abc.abcB + abc.abcC;
			goto failed;
		}

		DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 3) / 4;
		s_pitch *= 4;
		unsigned char *s = (unsigned char *) glyph_buffer.get_data();

		DWORD d_pitch = glyph_metrics.gmBlackBoxX * sizeof(DWORD);
		CL_PixelBuffer pixelbuffer(glyph_metrics.gmBlackBoxX, glyph_metrics.gmBlackBoxY, d_pitch, CL_PixelFormat::rgba8888);
		font_buffer.buffer = pixelbuffer;

		DWORD *d = (DWORD *) font_buffer.buffer.get_data();

		DWORD grayRamp[65];
		for (unsigned int i = 0; i < 65; i++)
		{
			unsigned int intensity = i * 255 / 64;
			grayRamp[i] = c + intensity;
		}

		for (DWORD py = 0; py < glyph_metrics.gmBlackBoxY; py++)
		{
			for (DWORD px = 0; px < glyph_metrics.gmBlackBoxX; px++)
			{
				DWORD gray = grayRamp[s[px + py*s_pitch]];
				d[px + py*glyph_metrics.gmBlackBoxX] = gray;
			}
		}

		font_buffer.offset.x = glyph_metrics.gmptGlyphOrigin.x;
		font_buffer.offset.y = -glyph_metrics.gmptGlyphOrigin.y;
		font_buffer.empty_buffer = false;
		pos_x = glyph_metrics.gmCellIncX;
		pos_y = glyph_metrics.gmCellIncY;
	}
	else
	{
		GLYPHMETRICS glyph_metrics;
		MAT2 mat2;
		memset(&mat2, 0, sizeof(MAT2));
		mat2.eM11.value = 1;
		mat2.eM22.value = 1;
		DWORD buffer_size = GetGlyphOutline(hdc, glyph, GGO_BITMAP, &glyph_metrics, 0, 0, &mat2);
		if (buffer_size == GDI_ERROR)
		{
			ABC abc;
			if (GetCharABCWidths(hdc, glyph, glyph, &abc))
				pos_x += abc.abcA + abc.abcB + abc.abcC;
			goto failed;
		}

		if (buffer_size == 0)
		{
			pos_x += glyph_metrics.gmCellIncX;
			pos_y += glyph_metrics.gmCellIncY;
			goto failed;
		}
		glyph_buffer.set_size(buffer_size);
		DWORD result = GetGlyphOutline(hdc, glyph, GGO_BITMAP, &glyph_metrics, glyph_buffer.get_size(), glyph_buffer.get_data(), &mat2);
		if (result == GDI_ERROR)
		{
			ABC abc;
			if (GetCharABCWidths(hdc, glyph, glyph, &abc))
				pos_x += abc.abcA + abc.abcB + abc.abcC;
			goto failed;
		}

		DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 31) / 32;
		s_pitch *= 4;
		unsigned char *s = (unsigned char *) glyph_buffer.get_data();

		DWORD d_pitch = glyph_metrics.gmBlackBoxX * sizeof(DWORD);

		CL_PixelBuffer pixelbuffer(glyph_metrics.gmBlackBoxX, glyph_metrics.gmBlackBoxY, d_pitch, CL_PixelFormat::rgba8888);
		font_buffer.buffer = pixelbuffer;
				
		DWORD *d = (DWORD *) font_buffer.buffer.get_data();

		for (DWORD py = 0; py < glyph_metrics.gmBlackBoxY; py++)
		{
			for (DWORD px = 0; px < glyph_metrics.gmBlackBoxX; px++)
			{
				DWORD gray = c + ((s[px/8 + py*s_pitch] >> (7-px%8)) & 1) * 255;
				d[px + py*glyph_metrics.gmBlackBoxX] = gray;
			}
		}

		font_buffer.offset.x = glyph_metrics.gmptGlyphOrigin.x;
		font_buffer.offset.y = -glyph_metrics.gmptGlyphOrigin.y;
		font_buffer.empty_buffer = false;
		pos_x = glyph_metrics.gmCellIncX;
		pos_y = glyph_metrics.gmCellIncY;
	}
failed:	
	SelectObject(hdc, old_font);
	SetMapMode(hdc, old_mode);
	::ReleaseDC(NULL, hdc);
	font_buffer.increment.x = pos_x;
	font_buffer.increment.y = pos_y;
	return font_buffer;

}

void CL_FontProvider_Win32::register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface)
{
	int fonts_added = AddFontResourceEx(font_filename.c_str(), FR_PRIVATE|FR_NOT_ENUM, 0);
	if(fonts_added == 0)
		throw CL_Exception("Unable to register font " + font_filename);
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Win32 Implementation:


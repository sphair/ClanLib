/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "font_engine.h"
#include "font_engine_win32.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"

CL_FontEngine_Win32::CL_FontEngine_Win32(const CL_FontDescription &desc)
: handle(0)
{
	handle = CreateFont(
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
		(desc.get_fixed_pitch() ? FIXED_PITCH : DEFAULT_PITCH) | FF_DONTCARE,
		CL_StringHelp::utf8_to_ucs2(desc.get_typeface_name()).c_str());
	if (handle == 0)
		throw CL_Exception("CreateFont failed");

	HDC dc = GetDC(0);
	int old_mode = SetMapMode(dc, MM_TEXT);
	HGDIOBJ old_font = SelectObject(dc, handle);
	BOOL result = GetTextMetrics(dc, &metrics);
	SelectObject(dc, old_font);
	SetMapMode(dc, old_mode);
	ReleaseDC(0, dc);
	if (result == FALSE)
	{
		DeleteObject(handle);
		handle = 0;
		throw CL_Exception("GetTextMetrics failed");
	}
}

CL_FontEngine_Win32::~CL_FontEngine_Win32()
{
	if (handle)
		DeleteObject(handle);
}

CL_FontMetrics CL_FontEngine_Win32::get_metrics()
{
	return CL_FontMetrics(
		(float)metrics.tmHeight,
		(float)metrics.tmAscent, 
		(float)metrics.tmDescent, 
		(float)metrics.tmInternalLeading,
		(float)metrics.tmExternalLeading,
		(float)metrics.tmAveCharWidth,
		(float)metrics.tmMaxCharWidth,
		(float)metrics.tmWeight,
		(float)metrics.tmOverhang, 
		(float)metrics.tmDigitizedAspectX,
		(float)metrics.tmDigitizedAspectY,
		metrics.tmItalic != 0, 
		metrics.tmUnderlined != 0, 
		metrics.tmStruckOut != 0,
		metrics.tmAveCharWidth == metrics.tmMaxCharWidth);
}

CL_FontPixelBuffer CL_FontEngine_Win32::get_font_glyph(int glyph, bool anti_alias, const CL_Colorf &color)
{
	if (anti_alias)
		return get_font_glyph_gray8(glyph, color);
	else
		return get_font_glyph_mono(glyph, color);
}

CL_FontPixelBuffer CL_FontEngine_Win32::get_font_glyph_lcd(int glyph, const CL_Colorf &color)
{
	CL_DataBuffer glyph_bitmap;
	GLYPHMETRICS glyph_metrics = { 0 };
	MAT2 matrix = { 0 };
	matrix.eM11.value = 3;
	matrix.eM22.value = 1;
	if (try_load_glyph_bitmap(glyph, GGO_GRAY8_BITMAP, matrix, glyph_bitmap, glyph_metrics))
	{
		CL_PixelBuffer pixelbuffer((glyph_metrics.gmBlackBoxX+2)/3, glyph_metrics.gmBlackBoxY, cl_rgba8);

		DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 3) / 4 * 4;
		unsigned char *s = (unsigned char *) glyph_bitmap.get_data();

		DWORD d_width = pixelbuffer.get_width();
		DWORD *d = (DWORD *) pixelbuffer.get_data();
		memset(d, 0, d_width*pixelbuffer.get_height()*4);

		CL_Color icolor = color;
		unsigned int c = (icolor.get_red() << 24) + (icolor.get_green() << 16) + (icolor.get_blue() << 8);

		for (DWORD py = 0; py < glyph_metrics.gmBlackBoxY; py++)
		{
			for (DWORD px = 0; px < glyph_metrics.gmBlackBoxX; px++)
			{
				DWORD gray = s[px + py*s_pitch];
				gray = (gray*255+32)/64;

				int offset = px/3 + py*d_width;
				DWORD component = 3-(px%3);
				DWORD component_mask = ((DWORD)0xff)<<(component*8);
				DWORD component_color = ((DWORD)0xff-gray)<<(component*8);

				if (gray > 0)
					d[offset] |= 0xff;

				d[offset] &= ~component_mask;
				d[offset] |= component_color;
			}
		}

		CL_FontPixelBuffer font_buffer;
		font_buffer.glyph = glyph;
		font_buffer.buffer = pixelbuffer;
		font_buffer.buffer_rect = pixelbuffer.get_size();
		font_buffer.offset.x = glyph_metrics.gmptGlyphOrigin.x/3;
		font_buffer.offset.y = -glyph_metrics.gmptGlyphOrigin.y;
		font_buffer.empty_buffer = false;
		font_buffer.increment.x = (glyph_metrics.gmCellIncX+2)/3;
		font_buffer.increment.y = glyph_metrics.gmCellIncY;
		return font_buffer;
	}
	else
	{
		return get_empty_font_glyph(glyph);
	}
}

CL_FontPixelBuffer CL_FontEngine_Win32::get_font_glyph_gray8(int glyph, const CL_Colorf &color)
{
	CL_DataBuffer glyph_bitmap;
	GLYPHMETRICS glyph_metrics = { 0 };
	MAT2 matrix = { 0 };
	matrix.eM11.value = 1;
	matrix.eM22.value = 1;
	if (try_load_glyph_bitmap(glyph, GGO_GRAY8_BITMAP, matrix, glyph_bitmap, glyph_metrics))
	{
		CL_PixelBuffer pixelbuffer(glyph_metrics.gmBlackBoxX, glyph_metrics.gmBlackBoxY, cl_rgba8);

		DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 3) / 4 * 4;
		unsigned char *s = (unsigned char *) glyph_bitmap.get_data();

		DWORD d_width = glyph_metrics.gmBlackBoxX;
		DWORD *d = (DWORD *) pixelbuffer.get_data();

		CL_Color icolor = color;
		unsigned int c = (icolor.get_red() << 24) + (icolor.get_green() << 16) + (icolor.get_blue() << 8);

		for (DWORD py = 0; py < glyph_metrics.gmBlackBoxY; py++)
		{
			for (DWORD px = 0; px < glyph_metrics.gmBlackBoxX; px++)
			{
				DWORD gray = s[px + py*s_pitch];
				d[px + py*d_width] = c + (gray*255+32)/64;
			}
		}

		CL_FontPixelBuffer font_buffer;
		font_buffer.glyph = glyph;
		font_buffer.buffer = pixelbuffer;
		font_buffer.buffer_rect = pixelbuffer.get_size();
		font_buffer.offset.x = glyph_metrics.gmptGlyphOrigin.x;
		font_buffer.offset.y = -glyph_metrics.gmptGlyphOrigin.y;
		font_buffer.empty_buffer = false;
		font_buffer.increment.x = glyph_metrics.gmCellIncX;
		font_buffer.increment.y = glyph_metrics.gmCellIncY;
		return font_buffer;
	}
	else
	{
		return get_empty_font_glyph(glyph);
	}
}

CL_FontPixelBuffer CL_FontEngine_Win32::get_font_glyph_mono(int glyph, const CL_Colorf &color)
{
	CL_DataBuffer glyph_bitmap;
	GLYPHMETRICS glyph_metrics = { 0 };
	MAT2 matrix = { 0 };
	matrix.eM11.value = 1;
	matrix.eM22.value = 1;
	if (try_load_glyph_bitmap(glyph, GGO_BITMAP, matrix, glyph_bitmap, glyph_metrics))
	{
		CL_PixelBuffer pixelbuffer(glyph_metrics.gmBlackBoxX, glyph_metrics.gmBlackBoxY, cl_rgba8);

		DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 31) / 32 * 4;
		unsigned char *s = (unsigned char *) glyph_bitmap.get_data();

		DWORD d_width = glyph_metrics.gmBlackBoxX;
		DWORD *d = (DWORD *) pixelbuffer.get_data();

		CL_Color icolor = color;
		unsigned int c = (icolor.get_red() << 24) + (icolor.get_green() << 16) + (icolor.get_blue() << 8);

		for (DWORD py = 0; py < glyph_metrics.gmBlackBoxY; py++)
		{
			for (DWORD px = 0; px < glyph_metrics.gmBlackBoxX; px++)
			{
				DWORD gray = c + ((s[px/8 + py*s_pitch] >> (7-px%8)) & 1) * 255;
				d[px + py*d_width] = gray;
			}
		}

		CL_FontPixelBuffer font_buffer;
		font_buffer.glyph = glyph;
		font_buffer.buffer = pixelbuffer;
		font_buffer.buffer_rect = pixelbuffer.get_size();
		font_buffer.offset.x = glyph_metrics.gmptGlyphOrigin.x;
		font_buffer.offset.y = -glyph_metrics.gmptGlyphOrigin.y;
		font_buffer.empty_buffer = false;
		font_buffer.increment.x = glyph_metrics.gmCellIncX;
		font_buffer.increment.y = glyph_metrics.gmCellIncY;
		return font_buffer;
	}
	else
	{
		return get_empty_font_glyph(glyph);
	}
}

bool CL_FontEngine_Win32::try_load_glyph_bitmap(int glyph, UINT format, MAT2 &matrix, CL_DataBuffer &glyph_bitmap, GLYPHMETRICS &glyph_metrics)
{
	HDC dc = GetDC(0);
	HGDIOBJ old_font = SelectObject(dc, handle);

	wchar_t text[2];
	text[0] = glyph;
	text[1] = 0;
	WORD indices[2] = {0};
	GetGlyphIndicesW(dc, text, 1, indices, GGI_MARK_NONEXISTING_GLYPHS);
	glyph = indices[0];
	format |= GGO_GLYPH_INDEX;

	bool result = false;
	DWORD bitmap_size = GetGlyphOutline(dc, glyph, format, &glyph_metrics, 0, 0, &matrix);
	if (bitmap_size != 0 && bitmap_size != GDI_ERROR)
	{
		CL_DataBuffer buffer(bitmap_size);
		bitmap_size = GetGlyphOutline(dc, glyph, format, &glyph_metrics, buffer.get_size(), buffer.get_data(), &matrix);
		if (bitmap_size != 0 && bitmap_size != GDI_ERROR)
		{
			glyph_bitmap = buffer;
			result = true;
		}
	}

	SelectObject(dc, old_font);
	ReleaseDC(0, dc);
	return result;
}

CL_FontPixelBuffer CL_FontEngine_Win32::get_empty_font_glyph(int glyph)
{
	HDC dc = GetDC(0);
	HGDIOBJ old_font = SelectObject(dc, handle);

	CL_FontPixelBuffer font_buffer;
	font_buffer.glyph = glyph;
	font_buffer.empty_buffer = true;
	ABC abc = { 0 };
	if (GetCharABCWidths(dc, glyph, glyph, &abc))
	{
		font_buffer.increment.x = abc.abcA + abc.abcB + abc.abcC;
		font_buffer.increment.y = 0;
	}
	else
	{
		font_buffer.increment.x = 0;
		font_buffer.increment.y = 0;
	}

	SelectObject(dc, old_font);
	ReleaseDC(0, dc);
	return font_buffer;
}

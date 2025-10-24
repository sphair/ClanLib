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
#include "font_engine.h"
#include "font_engine_win32.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/System/databuffer.h"

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
		desc.get_typeface_name().c_str());
	if (handle == 0)
		throw CL_Exception(cl_text("CreateFont failed"));

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
		throw CL_Exception(cl_text("GetTextMetrics failed"));
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
	HDC hdc = ::GetDC(NULL);
	int old_mode = SetMapMode(hdc, MM_TEXT);
	HGDIOBJ old_font = 0;
	old_font = SelectObject(hdc, handle);

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

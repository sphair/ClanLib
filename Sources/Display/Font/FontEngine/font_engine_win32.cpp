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
#include "font_engine.h"
#include "font_engine_win32.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"
#include "../glyph_outline.h"
#include "API/Core/Math/bezier_curve.h"

namespace clan
{

FontEngine_Win32::FontEngine_Win32(const FontDescription &desc)
: handle(0)
{
	handle = CreateFont(
		desc.get_height(), desc.get_average_width(),
		(int) (desc.get_escapement() * 10 + 0.5),
		(int) (desc.get_orientation() * 10 + 0.5),
		desc.get_weight(),
		desc.get_italic() ? TRUE : FALSE,
		FALSE,
		FALSE,
		decode_charset(desc.get_charset()),
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		(desc.get_fixed_pitch() ? FIXED_PITCH : DEFAULT_PITCH) | FF_DONTCARE,
		StringHelp::utf8_to_ucs2(desc.get_typeface_name()).c_str());
	if (handle == 0)
		throw Exception("CreateFont failed");

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
		throw Exception("GetTextMetrics failed");
	}
}

FontEngine_Win32::~FontEngine_Win32()
{
	if (handle)
		DeleteObject(handle);
}

FontMetrics FontEngine_Win32::get_metrics()
{
	return FontMetrics(
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

FontPixelBuffer FontEngine_Win32::get_font_glyph_standard(int glyph, bool anti_alias)
{
	if (anti_alias)
		return get_font_glyph_gray8(glyph);
	else
		return get_font_glyph_mono(glyph);
}

FontPixelBuffer FontEngine_Win32::get_font_glyph_subpixel(int glyph)
{
	return get_font_glyph_lcd(glyph);
}

FontPixelBuffer FontEngine_Win32::get_font_glyph_lcd(int glyph)
{
	HBITMAP old_bitmap;
	HFONT old_font;
	GLYPHMETRICS glyph_metrics = { 0 };
	HDC screen_dc = GetDC(0);
	old_font = (HFONT)SelectObject(screen_dc, handle);

	wchar_t text[2] = { glyph, 0 };
	WORD indices[2] = {0};
	GetGlyphIndicesW(screen_dc, text, 1, indices, GGI_MARK_NONEXISTING_GLYPHS);

	MAT2 matrix = { 0 };
	matrix.eM11.value = 1;
	matrix.eM22.value = 1;
	DWORD result = GetGlyphOutline(screen_dc, indices[0], GGO_GRAY8_BITMAP|GGO_GLYPH_INDEX, &glyph_metrics, 0, 0, &matrix);
	SelectObject(screen_dc, old_font);
	if (result == GDI_ERROR)
	{
		ReleaseDC(0, screen_dc);
		return get_empty_font_glyph(glyph);
	}

	Size bitmap_size(glyph_metrics.gmBlackBoxX+6, glyph_metrics.gmBlackBoxY);
	Point cursor(3-glyph_metrics.gmptGlyphOrigin.x, glyph_metrics.gmptGlyphOrigin.y);

	HDC dc = CreateCompatibleDC(screen_dc);
	HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, bitmap_size.width, bitmap_size.height);
	old_bitmap = (HBITMAP)SelectObject(dc, bitmap);

	HBRUSH brush = CreateSolidBrush(RGB(255,255,255));
	RECT rect = { 0, 0, bitmap_size.width, bitmap_size.height };
	FillRect(dc, &rect, brush);
	DeleteObject(brush);

	old_font = (HFONT)SelectObject(dc, handle);
	SetTextColor(dc, RGB(0,0,0));
	SetBkColor(dc, RGB(255,255,255));
	SetTextAlign(dc, TA_LEFT|TA_BASELINE|TA_NOUPDATECP);
	text[0] = indices[0];
	ExtTextOut(dc, cursor.x, cursor.y, ETO_GLYPH_INDEX, &rect, text, 1, 0);
	SelectObject(dc, old_font);

	SelectObject(dc, old_bitmap);
	DeleteDC(dc);

	BITMAPV5HEADER header = { 0 };
	header.bV5Size = sizeof(BITMAPV5HEADER);
	header.bV5Width = bitmap_size.width;
	header.bV5Height = -bitmap_size.height;
	header.bV5Planes = 1;
	header.bV5BitCount = 32;
	header.bV5Compression = BI_BITFIELDS;
	header.bV5RedMask   = 0xff000000;
	header.bV5GreenMask = 0x00ff0000;
	header.bV5BlueMask  = 0x0000ff00;
	header.bV5AlphaMask = 0x000000ff;
	header.bV5SizeImage = bitmap_size.height * 4;
	PixelBuffer pixelbuffer(bitmap_size.width, bitmap_size.height, tf_bgra8);
	int scanlines = GetDIBits(screen_dc, bitmap, 0, bitmap_size.height, pixelbuffer.get_data(), (LPBITMAPINFO)&header, DIB_RGB_COLORS);
	unsigned char *p = (unsigned char *)pixelbuffer.get_data();
	for (int i = 0; i < bitmap_size.width*bitmap_size.height; i++)
	{
		p[i*4+0] = 255-p[i*4+0];
		p[i*4+1] = 255-p[i*4+1];
		p[i*4+2] = 255-p[i*4+2];
		p[i*4+3] = 255;
	}

	DeleteObject(bitmap);
	ReleaseDC(0, screen_dc);

	FontPixelBuffer font_buffer;
	font_buffer.glyph = glyph;
	font_buffer.buffer = pixelbuffer;
	font_buffer.buffer_rect = pixelbuffer.get_size();
	font_buffer.offset.x = -cursor.x;
	font_buffer.offset.y = -cursor.y;
	font_buffer.empty_buffer = false;
	font_buffer.increment.x = glyph_metrics.gmCellIncX;
	font_buffer.increment.y = glyph_metrics.gmCellIncY;
	return font_buffer;
}

FontPixelBuffer FontEngine_Win32::get_font_glyph_gray8(int glyph)
{
	DataBuffer glyph_bitmap;
	GLYPHMETRICS glyph_metrics = { 0 };
	MAT2 matrix = { 0 };
	matrix.eM11.value = 1;
	matrix.eM22.value = 1;
	if (try_load_glyph_bitmap(glyph, GGO_GRAY8_BITMAP, matrix, glyph_bitmap, glyph_metrics))
	{
		PixelBuffer pixelbuffer(glyph_metrics.gmBlackBoxX, glyph_metrics.gmBlackBoxY, tf_rgba8);

		DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 3) / 4 * 4;
		unsigned char *s = (unsigned char *) glyph_bitmap.get_data();

		DWORD d_width = glyph_metrics.gmBlackBoxX;
		Vec4ub *d = pixelbuffer.get_data<Vec4ub>();

		for (DWORD py = 0; py < glyph_metrics.gmBlackBoxY; py++)
		{
			for (DWORD px = 0; px < glyph_metrics.gmBlackBoxX; px++)
			{
				DWORD gray = s[px + py*s_pitch];
				d[px + py*d_width] = Vec4ub(255, 255, 255, (gray * 255 + 32) / 64);
			}
		}

		FontPixelBuffer font_buffer;
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

FontPixelBuffer FontEngine_Win32::get_font_glyph_mono(int glyph)
{
	DataBuffer glyph_bitmap;
	GLYPHMETRICS glyph_metrics = { 0 };
	MAT2 matrix = { 0 };
	matrix.eM11.value = 1;
	matrix.eM22.value = 1;
	if (try_load_glyph_bitmap(glyph, GGO_BITMAP, matrix, glyph_bitmap, glyph_metrics))
	{
		PixelBuffer pixelbuffer(glyph_metrics.gmBlackBoxX, glyph_metrics.gmBlackBoxY, tf_rgba8);

		DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 31) / 32 * 4;
		unsigned char *s = (unsigned char *) glyph_bitmap.get_data();

		DWORD d_width = glyph_metrics.gmBlackBoxX;
		Vec4ub *d = pixelbuffer.get_data<Vec4ub>();

		for (DWORD py = 0; py < glyph_metrics.gmBlackBoxY; py++)
		{
			for (DWORD px = 0; px < glyph_metrics.gmBlackBoxX; px++)
			{
				d[px + py*d_width] = Vec4ub(255, 255, 255, ((s[px/8 + py*s_pitch] >> (7-px%8)) & 1) * 255);
			}
		}

		FontPixelBuffer font_buffer;
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

bool FontEngine_Win32::try_load_glyph_bitmap(int glyph, UINT format, MAT2 &matrix, DataBuffer &glyph_bitmap, GLYPHMETRICS &glyph_metrics)
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
		DataBuffer buffer(bitmap_size);
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

FontPixelBuffer FontEngine_Win32::get_empty_font_glyph(int glyph)
{
	HDC dc = GetDC(0);
	HGDIOBJ old_font = SelectObject(dc, handle);

	FontPixelBuffer font_buffer;
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

int FontEngine_Win32::decode_charset(FontDescription::Charset selected_charset)
{
	int charset = DEFAULT_CHARSET;
	switch (selected_charset)
	{
		case FontDescription::charset_default:
			charset = DEFAULT_CHARSET;
			break;
		case FontDescription::charset_ansi:
			charset = ANSI_CHARSET;
			break;
		case FontDescription::charset_baltic:
			charset = BALTIC_CHARSET;
			break;
		case FontDescription::charset_chinesebig5:
			charset = CHINESEBIG5_CHARSET;
			break;
		case FontDescription::charset_easteurope:
			charset = EASTEUROPE_CHARSET;
			break;
		case FontDescription::charset_gb2312:
			charset = GB2312_CHARSET;
			break;
		case FontDescription::charset_greek:
			charset = GREEK_CHARSET;
			break;
		case FontDescription::charset_hangul:
			charset = HANGUL_CHARSET;
			break;
		case FontDescription::charset_mac:
			charset = MAC_CHARSET;
			break;
		case FontDescription::charset_oem:
			charset = OEM_CHARSET;
			break;
		case FontDescription::charset_russian:
			charset = RUSSIAN_CHARSET;
			break;
		case FontDescription::charset_shiftjis:
			charset = SHIFTJIS_CHARSET;
			break;
		case FontDescription::charset_symbol:
			charset = SYMBOL_CHARSET;
			break;
		case FontDescription::charset_turkish:
			charset = TURKISH_CHARSET;
			break;
		case FontDescription::charset_vietnamese:
			charset = VIETNAMESE_CHARSET;
			break;
		case FontDescription::charset_johab:
			charset = JOHAB_CHARSET;
			break;
		case FontDescription::charset_arabic:
			charset = ARABIC_CHARSET;
			break;
		case FontDescription::charset_hebrew:
			charset = HEBREW_CHARSET;
			break;
		case FontDescription::charset_thai:
			charset = THAI_CHARSET;
			break;
	};
	return charset;

}

GlyphOutline *FontEngine_Win32::load_glyph_outline(int glyph)
{
	GLYPHMETRICS glyph_metrics = { 0 };
	MAT2 matrix = { 0 };
	matrix.eM11.value = 1;
	matrix.eM22.value = 1;

	HDC dc = GetDC(0);
	HGDIOBJ old_font = SelectObject(dc, handle);

	wchar_t text[2];
	text[0] = glyph;
	text[1] = 0;
	WORD indices[2] = {0};
	GetGlyphIndicesW(dc, text, 1, indices, GGI_MARK_NONEXISTING_GLYPHS);
	glyph = indices[0];
	int format = GGO_NATIVE | GGO_UNHINTED | GGO_GLYPH_INDEX;

	DataBuffer glyph_buffer;
	bool result = false;
	DWORD result_size = GetGlyphOutline(dc, glyph, format, &glyph_metrics, 0, 0, &matrix);
	if (result_size != 0 && result_size != GDI_ERROR)
	{
		DataBuffer buffer(result_size);
		result_size = GetGlyphOutline(dc, glyph, format, &glyph_metrics, buffer.get_size(), buffer.get_data(), &matrix);
		if (result_size != 0 && result_size != GDI_ERROR)
		{
			glyph_buffer = buffer;
			result = true;
		}
	}

	SelectObject(dc, old_font);
	ReleaseDC(0, dc);

	if (glyph_buffer.is_null())
		throw Exception("error loading glyph");

	TTPOLYGONHEADER * polygon_header = (TTPOLYGONHEADER *) glyph_buffer.get_data();
	char *data_end = (char *) polygon_header;
	data_end += glyph_buffer.get_size();

	GlyphOutline *outline = new GlyphOutline;		// TODO This should be a shared pointer

	while( (char *) (polygon_header+1) <= data_end)
	{
		if (polygon_header->dwType != TT_POLYGON_TYPE)
			throw Exception("invalid polygon type");

		BezierCurve bezier_curve;
		Pointf previous_point = PointFXtoPoint(polygon_header->pfxStart);
		Pointf initial_point = previous_point;
		bezier_curve.add_control_point( previous_point );

		GlyphContour *contour = new GlyphContour;	//TODO - Shared Pointer

		int curve_bytes = polygon_header->cb - sizeof(TTPOLYGONHEADER);
		if (curve_bytes < 0)
			throw Exception("invalid structure 1");

		TTPOLYCURVE *poly_curve = (TTPOLYCURVE *) (polygon_header + 1);

		// Update for next iteration (perfomed now for extra validation)
		polygon_header = (TTPOLYGONHEADER *) ( (char *) (polygon_header+1) + curve_bytes );
		if ( (char *) (polygon_header) > data_end)	// Ensure no overflow
			throw Exception("invalid structure 2");

		Pointf this_point;
		Pointf next_point;

		while(curve_bytes > 0)
		{
			int set_size = sizeof(TTPOLYCURVE) + sizeof(POINTFX) * (poly_curve->cpfx-1);
			curve_bytes -= set_size;
			TTPOLYCURVE *next_poly_curve = (TTPOLYCURVE *) ( (char *) (poly_curve) + set_size);
			if ( next_poly_curve < poly_curve || ( (char *) poly_curve > data_end ) )
				throw Exception("invalid structure 2");
	
			if (poly_curve->wType == TT_PRIM_LINE)
			{
				Pointf first_point = previous_point;
				for (int i = 0; i < poly_curve->cpfx; i++)
				{
					next_point = PointFXtoPoint(poly_curve->apfx[i]);
					contour->add_line_to(next_point);
				}
				previous_point = next_point;
			}
			else if (poly_curve->wType == TT_PRIM_QSPLINE)
			{
				for (int i = 0; i < poly_curve->cpfx; )
				{
					this_point = PointFXtoPoint(poly_curve->apfx[i++]);
	
					if (i == poly_curve->cpfx - 1)
					{
						next_point = PointFXtoPoint(poly_curve->apfx[i++]);
					}
					else
					{
						next_point = PointFXtoPoint(poly_curve->apfx[i]);
						next_point = Pointf((this_point.x + next_point.x) / 2.0f, (this_point.y + next_point.y) / 2.0f);
					}

					// conversion of a quadratic to a cubic

					// Cubic P1 in terms of Quadratic P0 and P1
					bezier_curve.add_control_point(previous_point.x + 2.0f * (this_point.x - previous_point.x) / 3.0f,  previous_point.y + 2.0f * (this_point.y - previous_point.y) / 3.0f);

					// Cubic P2 in terms of Qudartic P1 and P2
					bezier_curve.add_control_point(this_point.x + (next_point.x - this_point.x) / 3.0f,  this_point.y + (next_point.y - this_point.y) / 3.0f);

					// Cubic P3 is the on curve end point
					bezier_curve.add_control_point( next_point );

					previous_point = next_point;
				}
			}
			else
				throw Exception("unsupported curve type");

			poly_curve = next_poly_curve;
		}
		// TODO: Do we need to close the curve?
		contour->add_curve(bezier_curve);
		outline->add_contour(contour);
	}

	outline->advance_x = 0;	// HACK HACK HACK - This should not be here
	return outline;
}

}

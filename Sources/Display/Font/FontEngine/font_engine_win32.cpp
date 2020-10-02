/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Display/2D/path.h"
#include "API/Core/Math/bezier_curve.h"

namespace clan
{
	FontEngine_Win32::FontEngine_Win32(const FontDescription &desc, const std::string &typeface_name, float pixel_ratio) : pixel_ratio(pixel_ratio)
	{
		font_handle.engine = this;
		load_font(desc, typeface_name, pixel_ratio);
	}

	FontEngine_Win32::FontEngine_Win32(const FontDescription &desc, DataBuffer &font_databuffer, float pixel_ratio) : pixel_ratio(pixel_ratio)
	{
		font_handle.engine = this;

		if (font_databuffer.is_null())
			throw Exception("Attempt to load an empty font buffer");

		DWORD out_number_of_fonts = 0;
		HANDLE font_handle = AddFontMemResourceEx(font_databuffer.get_data(), font_databuffer.get_size(), 0, &out_number_of_fonts);
		if (out_number_of_fonts == 0)
			throw Exception("Unable to register font");

		std::string typeface_name = get_ttf_typeface_name(font_databuffer);
		if (typeface_name.empty())
			throw Exception("Unable to obtain typeface name");

		load_font(desc, typeface_name, pixel_ratio);
	}

	void FontEngine_Win32::load_font(const FontDescription &desc, const std::string &typeface_name, float pixel_ratio)
	{
		float device_font_size = std::abs(desc.get_height()) * pixel_ratio;
		float device_average_width = desc.get_average_width() * pixel_ratio;

		handle = CreateFont(
			-device_font_size,
			device_average_width,
			static_cast<int>(desc.get_escapement() * 10.0f + 0.5f),
			static_cast<int>(desc.get_orientation() * 10.0f + 0.5f),
			static_cast<int>(desc.get_weight()),
			desc.get_style() == FontStyle::italic ? TRUE : FALSE,
			FALSE,
			FALSE,
			decode_charset(desc.get_charset()),
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			StringHelp::utf8_to_ucs2(typeface_name).c_str());
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

		font_metrics = FontMetrics(
			metrics.tmHeight / pixel_ratio,
			metrics.tmAscent / pixel_ratio,
			metrics.tmDescent / pixel_ratio,
			metrics.tmInternalLeading / pixel_ratio,
			metrics.tmExternalLeading / pixel_ratio,
			desc.get_line_height(),		// Calculated in FontMetrics as tmHeight + tmExternalLeading if not specified
			pixel_ratio);

		font_description = desc.clone();
	}

	FontEngine_Win32::~FontEngine_Win32()
	{
		if (handle)
			DeleteObject(handle);
	}

	FontPixelBuffer FontEngine_Win32::get_font_glyph_standard(int glyph, bool anti_alias)
	{
		if (anti_alias)
			return get_font_glyph_gray8(glyph);
		else
			return get_font_glyph_mono(glyph);
	}

	FontPixelBuffer FontEngine_Win32::get_font_glyph(int glyph)
	{
		if (font_description.get_subpixel())
		{
			return get_font_glyph_subpixel(glyph);
		}
		else
		{
			return get_font_glyph_standard(glyph, font_description.get_anti_alias());
		}
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

		wchar_t text[2] = { static_cast<wchar_t>(glyph), 0 };
		WORD indices[2] = { 0 };
		GetGlyphIndicesW(screen_dc, text, 1, indices, GGI_MARK_NONEXISTING_GLYPHS);

		MAT2 matrix = { 0 };
		matrix.eM11.value = 1;
		matrix.eM22.value = 1;
		DWORD result = GetGlyphOutline(screen_dc, indices[0], GGO_GRAY8_BITMAP | GGO_GLYPH_INDEX, &glyph_metrics, 0, 0, &matrix);
		SelectObject(screen_dc, old_font);
		if (result == GDI_ERROR)
		{
			ReleaseDC(0, screen_dc);
			return get_empty_font_glyph(glyph);
		}

		Size bitmap_size(glyph_metrics.gmBlackBoxX + 6, glyph_metrics.gmBlackBoxY);
		Point cursor(3 - glyph_metrics.gmptGlyphOrigin.x, glyph_metrics.gmptGlyphOrigin.y);

		HDC dc = CreateCompatibleDC(screen_dc);
		HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, bitmap_size.width, bitmap_size.height);
		old_bitmap = (HBITMAP)SelectObject(dc, bitmap);

		HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
		RECT rect = { 0, 0, bitmap_size.width, bitmap_size.height };
		FillRect(dc, &rect, brush);
		DeleteObject(brush);

		old_font = (HFONT)SelectObject(dc, handle);
		SetTextColor(dc, RGB(0, 0, 0));
		SetBkColor(dc, RGB(255, 255, 255));
		SetTextAlign(dc, TA_LEFT | TA_BASELINE | TA_NOUPDATECP);
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
		header.bV5RedMask = 0xff000000;
		header.bV5GreenMask = 0x00ff0000;
		header.bV5BlueMask = 0x0000ff00;
		header.bV5AlphaMask = 0x000000ff;
		header.bV5SizeImage = bitmap_size.height * 4;
		PixelBuffer pixelbuffer(bitmap_size.width, bitmap_size.height, TextureFormat::bgra8);
		int scanlines = GetDIBits(screen_dc, bitmap, 0, bitmap_size.height, pixelbuffer.get_data(), (LPBITMAPINFO)&header, DIB_RGB_COLORS);
		unsigned char *p = (unsigned char *)pixelbuffer.get_data();
		for (int i = 0; i < bitmap_size.width*bitmap_size.height; i++)
		{
			p[i * 4 + 0] = 255 - p[i * 4 + 0];
			p[i * 4 + 1] = 255 - p[i * 4 + 1];
			p[i * 4 + 2] = 255 - p[i * 4 + 2];
			p[i * 4 + 3] = 255;
		}

		DeleteObject(bitmap);
		ReleaseDC(0, screen_dc);

		FontPixelBuffer font_buffer;
		font_buffer.glyph = glyph;
		font_buffer.buffer = pixelbuffer;
		font_buffer.buffer_rect = pixelbuffer.get_size();
		font_buffer.size = Sizef(pixelbuffer.get_width() / pixel_ratio, pixelbuffer.get_height() / pixel_ratio);
		font_buffer.offset.x = -cursor.x / pixel_ratio;
		font_buffer.offset.y = -cursor.y / pixel_ratio;
		font_buffer.empty_buffer = false;
		font_buffer.metrics.advance.width = glyph_metrics.gmCellIncX / pixel_ratio;
		font_buffer.metrics.advance.height = glyph_metrics.gmCellIncY / pixel_ratio;
		font_buffer.metrics.bbox_offset.x = glyph_metrics.gmptGlyphOrigin.x / pixel_ratio;
		font_buffer.metrics.bbox_offset.y = -glyph_metrics.gmptGlyphOrigin.y / pixel_ratio;
		font_buffer.metrics.bbox_size.width = glyph_metrics.gmBlackBoxX / pixel_ratio;
		font_buffer.metrics.bbox_size.height = glyph_metrics.gmBlackBoxY / pixel_ratio;
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
			PixelBuffer pixelbuffer(glyph_metrics.gmBlackBoxX, glyph_metrics.gmBlackBoxY, TextureFormat::rgba8);

			DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 3) / 4 * 4;
			unsigned char *s = (unsigned char *)glyph_bitmap.get_data();

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
			font_buffer.size = Sizef(pixelbuffer.get_width() / pixel_ratio, pixelbuffer.get_height() / pixel_ratio);
			font_buffer.offset.x = glyph_metrics.gmptGlyphOrigin.x / pixel_ratio;
			font_buffer.offset.y = -glyph_metrics.gmptGlyphOrigin.y / pixel_ratio;
			font_buffer.empty_buffer = false;
			font_buffer.metrics.advance.width = glyph_metrics.gmCellIncX / pixel_ratio;
			font_buffer.metrics.advance.height = glyph_metrics.gmCellIncY / pixel_ratio;
			font_buffer.metrics.bbox_offset.x = glyph_metrics.gmptGlyphOrigin.x / pixel_ratio;
			font_buffer.metrics.bbox_offset.y = -glyph_metrics.gmptGlyphOrigin.y / pixel_ratio;
			font_buffer.metrics.bbox_size.width = glyph_metrics.gmBlackBoxX / pixel_ratio;
			font_buffer.metrics.bbox_size.height = glyph_metrics.gmBlackBoxY / pixel_ratio;
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
			PixelBuffer pixelbuffer(glyph_metrics.gmBlackBoxX, glyph_metrics.gmBlackBoxY, TextureFormat::rgba8);

			DWORD s_pitch = (glyph_metrics.gmBlackBoxX + 31) / 32 * 4;
			unsigned char *s = (unsigned char *)glyph_bitmap.get_data();

			DWORD d_width = glyph_metrics.gmBlackBoxX;
			Vec4ub *d = pixelbuffer.get_data<Vec4ub>();

			for (DWORD py = 0; py < glyph_metrics.gmBlackBoxY; py++)
			{
				for (DWORD px = 0; px < glyph_metrics.gmBlackBoxX; px++)
				{
					d[px + py*d_width] = Vec4ub(255, 255, 255, ((s[px / 8 + py*s_pitch] >> (7 - px % 8)) & 1) * 255);
				}
			}

			FontPixelBuffer font_buffer;
			font_buffer.glyph = glyph;
			font_buffer.buffer = pixelbuffer;
			font_buffer.buffer_rect = pixelbuffer.get_size();
			font_buffer.size = Sizef(pixelbuffer.get_width() / pixel_ratio, pixelbuffer.get_height() / pixel_ratio);
			font_buffer.offset.x = glyph_metrics.gmptGlyphOrigin.x / pixel_ratio;
			font_buffer.offset.y = -glyph_metrics.gmptGlyphOrigin.y / pixel_ratio;
			font_buffer.empty_buffer = false;
			font_buffer.metrics.advance.width = glyph_metrics.gmCellIncX / pixel_ratio;
			font_buffer.metrics.advance.height = glyph_metrics.gmCellIncY / pixel_ratio;
			font_buffer.metrics.bbox_offset.x = glyph_metrics.gmptGlyphOrigin.x / pixel_ratio;
			font_buffer.metrics.bbox_offset.y = -glyph_metrics.gmptGlyphOrigin.y / pixel_ratio;
			font_buffer.metrics.bbox_size.width = glyph_metrics.gmBlackBoxX / pixel_ratio;
			font_buffer.metrics.bbox_size.height = glyph_metrics.gmBlackBoxY / pixel_ratio;
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

		wchar_t text[2] = { static_cast<wchar_t>(glyph), 0 };
		WORD indices[2] = { 0 };
		GetGlyphIndicesW(dc, text, 1, indices, GGI_MARK_NONEXISTING_GLYPHS);
		format |= GGO_GLYPH_INDEX;

		bool result = false;
		DWORD bitmap_size = GetGlyphOutline(dc, indices[0], format, &glyph_metrics, 0, 0, &matrix);
		if (bitmap_size != 0 && bitmap_size != GDI_ERROR)
		{
			DataBuffer buffer(bitmap_size);
			bitmap_size = GetGlyphOutline(dc, indices[0], format, &glyph_metrics, buffer.get_size(), buffer.get_data(), &matrix);
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
			font_buffer.metrics.advance.width = (abc.abcA + abc.abcB + abc.abcC) / pixel_ratio;
			font_buffer.metrics.bbox_offset = Pointf(0.0f, -1.0f);
			font_buffer.metrics.bbox_size = Sizef(font_buffer.metrics.advance.width, 1.0f);
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

	void FontEngine_Win32::load_glyph_path(unsigned int glyph_index, Path &path, GlyphMetrics &out_metrics)
	{
		path.set_fill_mode(PathFillMode::winding);

		GLYPHMETRICS glyph_metrics = { 0 };
		MAT2 matrix = { 0 };
		matrix.eM11.value = 1;
		matrix.eM22.value = 1;

		HDC dc = GetDC(0);
		HGDIOBJ old_font = SelectObject(dc, handle);

		wchar_t text[2];
		text[0] = glyph_index;
		text[1] = 0;
		WORD indices[2] = { 0 };
		GetGlyphIndicesW(dc, text, 1, indices, GGI_MARK_NONEXISTING_GLYPHS);
		glyph_index = indices[0];
		int format = GGO_NATIVE | GGO_UNHINTED | GGO_GLYPH_INDEX;

		DataBuffer glyph_buffer;
		bool result = false;
		DWORD result_size = GetGlyphOutline(dc, glyph_index, format, &glyph_metrics, 0, 0, &matrix);
		if (result_size != 0 && result_size != GDI_ERROR)
		{
			DataBuffer buffer(result_size);
			result_size = GetGlyphOutline(dc, glyph_index, format, &glyph_metrics, buffer.get_size(), buffer.get_data(), &matrix);
			if (result_size != 0 && result_size != GDI_ERROR)
			{
				glyph_buffer = buffer;
				result = true;
			}
		}

		SelectObject(dc, old_font);
		ReleaseDC(0, dc);

		if (glyph_buffer.is_null())
		{
			out_metrics.advance.width = glyph_metrics.gmCellIncX / pixel_ratio;
			out_metrics.advance.height = glyph_metrics.gmCellIncY / pixel_ratio;
			out_metrics.bbox_offset.x = glyph_metrics.gmptGlyphOrigin.x / pixel_ratio;
			out_metrics.bbox_offset.y = -glyph_metrics.gmptGlyphOrigin.y / pixel_ratio;
			out_metrics.bbox_size.width = glyph_metrics.gmBlackBoxX / pixel_ratio;
			out_metrics.bbox_size.height = glyph_metrics.gmBlackBoxY / pixel_ratio;
			return;
		}

		TTPOLYGONHEADER * polygon_header = (TTPOLYGONHEADER *)glyph_buffer.get_data();
		char *data_end = (char *)polygon_header;
		data_end += glyph_buffer.get_size();

		while ((char *)(polygon_header + 1) <= data_end)
		{
			if (polygon_header->dwType != TT_POLYGON_TYPE)
				throw Exception("invalid polygon type");

			path.move_to(to_point(polygon_header->pfxStart));

			int curve_bytes = polygon_header->cb - sizeof(TTPOLYGONHEADER);
			if (curve_bytes < 0)
				throw Exception("invalid structure 1");

			TTPOLYCURVE *poly_curve = (TTPOLYCURVE *)(polygon_header + 1);

			// Update for next iteration (perfomed now for extra validation)
			polygon_header = (TTPOLYGONHEADER *)((char *)(polygon_header + 1) + curve_bytes);
			if ((char *)(polygon_header) > data_end)	// Ensure no overflow
				throw Exception("invalid structure 2");

			Pointf this_point;
			Pointf next_point;

			while (curve_bytes > 0)
			{
				int set_size = sizeof(TTPOLYCURVE) + sizeof(POINTFX) * (poly_curve->cpfx - 1);
				curve_bytes -= set_size;
				TTPOLYCURVE *next_poly_curve = (TTPOLYCURVE *)((char *)(poly_curve)+set_size);
				if (next_poly_curve < poly_curve || ((char *)poly_curve > data_end))
					throw Exception("invalid structure 2");

				if (poly_curve->wType == TT_PRIM_LINE)
				{
					for (int i = 0; i < poly_curve->cpfx; i++)
					{
						next_point = to_point(poly_curve->apfx[i]);
						path.line_to(next_point);
					}
				}
				else if (poly_curve->wType == TT_PRIM_QSPLINE)
				{
					for (int i = 0; i < poly_curve->cpfx;)
					{
						this_point = to_point(poly_curve->apfx[i++]);

						if (i == poly_curve->cpfx - 1)
						{
							next_point = to_point(poly_curve->apfx[i++]);
						}
						else
						{
							next_point = to_point(poly_curve->apfx[i]);
							next_point = Pointf((this_point.x + next_point.x) / 2.0f, (this_point.y + next_point.y) / 2.0f);
						}

						path.bezier_to(this_point, next_point);
					}
				}
				else
					throw Exception("unsupported curve type");

				poly_curve = next_poly_curve;
			}
			path.close();
		}
		out_metrics.advance.width = glyph_metrics.gmCellIncX / pixel_ratio;
		out_metrics.advance.height = glyph_metrics.gmCellIncY / pixel_ratio;
		out_metrics.bbox_offset.x = glyph_metrics.gmptGlyphOrigin.x / pixel_ratio;
		out_metrics.bbox_offset.y = -glyph_metrics.gmptGlyphOrigin.y / pixel_ratio;
		out_metrics.bbox_size.width = glyph_metrics.gmBlackBoxX / pixel_ratio;
		out_metrics.bbox_size.height = glyph_metrics.gmBlackBoxY / pixel_ratio;

	}

	std::string FontEngine_Win32::get_ttf_typeface_name(DataBuffer &font_databuffer)
	{
		if (!font_databuffer.get_size())
			return std::string();

		const char *start_ptr = font_databuffer.get_data();
		unsigned int full_size = font_databuffer.get_size();

		const char *read_ptr = start_ptr;
		unsigned int size = full_size;

		size -= sizeof(ttf_offset_table);
		if (size <= 0)
			return std::string();
		ttf_offset_table offset_table = *reinterpret_cast<const ttf_offset_table *>(read_ptr);
		read_ptr += sizeof(ttf_offset_table);
		swap(offset_table.numTables);
		swap(offset_table.entrySelector);
		swap(offset_table.rangeShift);
		swap(offset_table.searchRange);
		swap(offset_table.version.major);
		swap(offset_table.version.minor);

		if (offset_table.numTables > 0xFFFF)	// Lets reject fonts that appear unusual
			return std::string();

		for (unsigned int record_count = 0; record_count < offset_table.numTables; record_count++)
		{
			size -= sizeof(ttf_table_record);
			if (size <= 0)
				return std::string();
			ttf_table_record table_record = *reinterpret_cast<const ttf_table_record *>(read_ptr);
			read_ptr += sizeof(ttf_table_record);
			swap(table_record.checkSum);
			swap(table_record.length);
			swap(table_record.offset);

			if (table_record.offset >= full_size)
				return std::string();

			if ((table_record.offset + table_record.length) > full_size)
				return std::string();

			if ((table_record.tag[0] == 'n') && (table_record.tag[1] == 'a') && (table_record.tag[2] == 'm') && (table_record.tag[3] == 'e'))
			{
				unsigned int record_size = table_record.length;
				const char *record_read_ptr = start_ptr + table_record.offset;
				if (record_size < sizeof(ttf_naming_table))
					return std::string();
				ttf_naming_table naming_table = *reinterpret_cast<const ttf_naming_table *>(record_read_ptr);
				swap(naming_table.count);
				swap(naming_table.format);
				swap(naming_table.stringOffset);

				if (naming_table.stringOffset >= record_size)
					return std::string();

				const char *string_base = record_read_ptr + naming_table.stringOffset;
				if (naming_table.count > 0xFFFF)	// Lets reject fonts that appear unusual
					return std::string();
				unsigned int string_base_size = record_size - naming_table.stringOffset;

				record_read_ptr += sizeof(ttf_naming_table);
				record_size -= sizeof(ttf_naming_table);
				for (unsigned int naming_record_count = 0; naming_record_count < naming_table.count; naming_record_count++)
				{
					record_size -= sizeof(ttf_naming_record);
					if (record_size <= 0)
						return std::string();
					ttf_naming_record naming_record = *reinterpret_cast<const ttf_naming_record *>(record_read_ptr);
					record_read_ptr += sizeof(ttf_naming_record);
					swap(naming_record.encodingID);
					swap(naming_record.languageID);
					swap(naming_record.length);
					swap(naming_record.nameID);
					swap(naming_record.offset);
					swap(naming_record.platformID);

					if (naming_record.offset >= string_base_size)
						return std::string();

					if (naming_record.length + naming_record.offset > string_base_size)
						return std::string();

					// From Microsoft Docs: 4 == Full font name
					if (naming_record.nameID == 4)
					{
						std::vector<char> buffer;
						buffer.resize(naming_record.length + 1);
						memcpy(&buffer[0], string_base + naming_record.offset, naming_record.length);
						buffer[naming_record.length] = 0;
						return &buffer[0];
					}
				}

				return std::string();
			}
		}

		return std::string();
	}

	HFONT FontHandle_Win32::hfont()
	{
		if (engine)
			return engine->handle;
		return HFONT(0);
	}

	FontHandle *FontEngine_Win32::get_handle()
	{
		return &font_handle;
	}
}

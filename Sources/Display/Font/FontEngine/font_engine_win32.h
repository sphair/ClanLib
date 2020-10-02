/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#pragma once

#include "font_engine.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Font/font_metrics.h"

namespace clan
{
	class DataBuffer;

	class FontEngine_Win32 : public FontEngine
	{
	public:
		FontEngine_Win32(const FontDescription &description, const std::string &typeface_name, float pixel_ratio);
		FontEngine_Win32(const FontDescription &description, DataBuffer &font_databuffer, float pixel_ratio);
		~FontEngine_Win32();

		bool is_automatic_recreation_allowed() const override { return true; }
		const FontMetrics &get_metrics() const override { return font_metrics; }
		FontPixelBuffer get_font_glyph(int glyph) override;
		FontPixelBuffer get_font_glyph_standard(int glyph, bool anti_alias);
		FontPixelBuffer get_font_glyph_subpixel(int glyph);
		const FontDescription &get_desc() const override { return font_description; }
		void load_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics) override;
		FontHandle *get_handle() override;

	private:
		void load_font(const FontDescription &desc, const std::string &typeface_name, float pixel_ratio);

		// Structure information from: http://www.microsoft.com/typography/otspec/otff.htm
		struct ttf_version
		{
			USHORT major;
			USHORT minor;
		};

		struct ttf_offset_table
		{
			ttf_version version;	// Fixed sfnt version 0x00010000 for version 1.0.
			USHORT numTables;	// Number of tables.
			USHORT searchRange;	// (Maximum power of 2 <= numTables) x 16.
			USHORT entrySelector;	// Log2(maximum power of 2 <= numTables).
			USHORT rangeShift;	// NumTables x 16-searchRange.
		};

		struct ttf_table_record
		{
			char tag[4];	// 4 -byte identifier.
			ULONG checkSum;	// CheckSum for this table.
			ULONG offset;	// Offset from beginning of TrueType font file.
			ULONG length;	// Length of this table.
		};

		struct ttf_naming_table
		{
			USHORT format;	// Format selector (=0).
			USHORT count;	// Number of name records.
			USHORT stringOffset;	// Offset to start of string storage (from start of table).
			// NameRecord nameRecord[count] The name records where count is the number of records.
		};

		struct ttf_naming_record
		{
			USHORT platformID;	// Platform ID.
			USHORT encodingID;	// Platform-specific encoding ID.
			USHORT languageID;	// Language ID.
			USHORT nameID;		// Name ID.
			USHORT length;		// String length (in bytes).
			USHORT offset;		// String offset from start of storage area (in bytes).
		};

		// Swap the endians
		void swap(USHORT &value) const { value = (value << 8) | (value >> 8); }
		void swap(ULONG &value) const { value = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF); value = (value << 16) | (value >> 16); }

		std::string get_ttf_typeface_name(DataBuffer &font_databuffer);

		inline Pointf to_point(POINTFX &fx)
		{
			return Pointf(to_float(fx.x), -to_float(fx.y));
		}

		inline float to_float(const FIXED &fixed)
		{
			double v = static_cast<int>(fixed.value) << 16 + static_cast<int>(fixed.fract);
			return static_cast<float>(v / 65536.0);
		}

		FontPixelBuffer get_font_glyph_lcd(int glyph);
		FontPixelBuffer get_font_glyph_gray8(int glyph);
		FontPixelBuffer get_font_glyph_mono(int glyph);
		bool try_load_glyph_bitmap(int glyph, UINT format, MAT2 &matrix, DataBuffer &glyph_bitmap, GLYPHMETRICS &glyph_metrics);
		FontPixelBuffer get_empty_font_glyph(int glyph);
		int decode_charset(FontDescription::Charset selected_charset);
		HFONT handle = 0;
		TEXTMETRIC metrics;

		FontDescription font_description;
		FontMetrics font_metrics;

		float pixel_ratio = 1.0f;

		FontHandle_Win32 font_handle;
		friend class FontHandle_Win32;
	};
}

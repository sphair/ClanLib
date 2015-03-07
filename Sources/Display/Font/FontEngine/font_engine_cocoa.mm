/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "font_engine_cocoa.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/2D/path.h"

namespace clan
{
	FontEngine_Cocoa::FontEngine_Cocoa(const FontDescription &desc, const std::string &typeface_name, float pixel_ratio)
	: pixel_ratio(pixel_ratio)
	{
        CFStringRef name = CFStringCreateWithCString(0, typeface_name.empty() ? "Helvetica" : typeface_name.c_str(), CFStringGetSystemEncoding());
		if (name == 0)
			throw Exception("CFStringCreateWithCString failed");
		
		CGFontRef cg_font = CGFontCreateWithFontName(name);
		
		CFRelease(name);
		name = 0;
		
		if (cg_font == 0)
			throw Exception("CGFontCreateWithFontName failed");
		
		handle = CTFontCreateWithGraphicsFont(cg_font, desc.get_height() * pixel_ratio, 0, 0);
		
		CFRelease(cg_font);
		cg_font = 0;
		
		if (handle == 0)
		{
			throw Exception("CTFontCreateWithGraphicsFont failed");
		}
		
		font_metrics = FontMetrics(
			(CTFontGetAscent(handle) + CTFontGetDescent(handle)) / pixel_ratio,
			(CTFontGetAscent(handle)) / pixel_ratio,
			(CTFontGetDescent(handle)) / pixel_ratio,
			0.0f,
			(CTFontGetLeading(handle)) / pixel_ratio,
			desc.get_line_height());
		
		font_description = desc.clone();
	}

	FontEngine_Cocoa::FontEngine_Cocoa(const FontDescription &desc, DataBuffer &font_databuffer, float pixel_ratio)
	: pixel_ratio(pixel_ratio)
	{
		DataBuffer *cgdata_databuffer = new DataBuffer(font_databuffer);
		
		CGDataProviderRef cg_fontdata = CGDataProviderCreateWithData(cgdata_databuffer, cgdata_databuffer->get_data(), cgdata_databuffer->get_size(),[](void *info, const void *data, size_t size) { delete (DataBuffer *)info; });
		
		if (cg_fontdata == 0)
		{
			delete cgdata_databuffer;
			throw Exception("CGDataProviderCreateWithData failed");
		}
		
		CGFontRef cg_font = CGFontCreateWithDataProvider(cg_fontdata);
		
		CFRelease(cg_fontdata);
		cg_fontdata = 0;
		
		if (cg_font == 0)
		{
			throw Exception("CGFontCreateWithDataProvider failed");
		}
		
		handle = CTFontCreateWithGraphicsFont(cg_font, desc.get_height() * pixel_ratio, 0, 0);
		
		CFRelease(cg_font);
		cg_font = 0;
		
		if (handle == 0)
		{
			throw Exception("CTFontCreateWithGraphicsFont failed");
		}

		font_metrics = FontMetrics(
			(CTFontGetAscent(handle) + CTFontGetDescent(handle)) / pixel_ratio,
			(CTFontGetAscent(handle)) / pixel_ratio,
			(CTFontGetDescent(handle)) / pixel_ratio,
			0.0f,
			(CTFontGetLeading(handle)) / pixel_ratio,
			desc.get_line_height());

		font_description = desc.clone();
	}

	FontEngine_Cocoa::~FontEngine_Cocoa()
	{
		if (handle)
			CFRelease(handle);
	}

	FontPixelBuffer FontEngine_Cocoa::get_font_glyph(int glyph)
	{
		// Retrieve glyph black box information:
		
		CGGlyph cgglyph = 0;
		UniChar c = glyph;
		bool found_char = CTFontGetGlyphsForCharacters(handle, &c, &cgglyph, 1);
		
		CGSize advance;
		CTFontGetAdvancesForGlyphs(handle, kCTFontDefaultOrientation, &cgglyph, &advance, 1);
		
		CGRect cg_bbox; // cg_bbox.origin is the lower left corner of the black box relative to the cursor on the baseline
		CTFontGetBoundingRectsForGlyphs(handle, kCTFontDefaultOrientation, &cgglyph, &cg_bbox, 1);
		
		float glyph_x = std::floor(cg_bbox.origin.x);
		float glyph_y = -std::floor(cg_bbox.origin.y + cg_bbox.size.height);
		int glyph_width = (int)std::ceil(cg_bbox.size.width + 2.0f);
		int glyph_height = (int)std::ceil(cg_bbox.size.height + 2.0f);
		
		// Render glyph into bitmap:
		
		PixelBuffer pixelbuffer(glyph_width, glyph_height, tf_bgra8);
		unsigned char *p = (unsigned char *)pixelbuffer.get_data();
		int len = pixelbuffer.get_width()*pixelbuffer.get_height();
		for (int i = 0; i < len*4; i++)
			p[i] = 255;
		
		CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
		
		CGContextRef context = CGBitmapContextCreate(pixelbuffer.get_data(), pixelbuffer.get_width(), pixelbuffer.get_height(), 8, pixelbuffer.get_width() * 4, colorspace, kCGImageAlphaPremultipliedFirst);
		
		CGContextSetAllowsFontSmoothing(context, font_description.get_subpixel() || font_description.get_anti_alias());
		CGContextSetAllowsAntialiasing(context, font_description.get_anti_alias());
		CGContextSetAllowsFontSubpixelQuantization(context, font_description.get_subpixel());
		CGContextSetAllowsFontSubpixelPositioning(context, false);
		
		CGPoint position;
		position.x = -std::floor(cg_bbox.origin.x);
		position.y = -std::floor(cg_bbox.origin.y + cg_bbox.size.height) + cg_bbox.size.height;
		
		CTFontDrawGlyphs(handle, &cgglyph, &position, 1, context);
		
		CGContextRelease(context);
		CGColorSpaceRelease(colorspace);
		
		for (int i = 0; i < len; i++)
		{
			p[i*4+0] = 255-p[i*4+0];
			p[i*4+1] = 255-p[i*4+1];
			p[i*4+2] = 255-p[i*4+2];
			p[i*4+3] = 255;
		}
		
		// Return glyph black box as a FontPixelBuffer:
		
		FontPixelBuffer font_buffer;
		font_buffer.glyph = glyph;
		font_buffer.buffer = pixelbuffer;
		font_buffer.buffer_rect = pixelbuffer.get_size();
		font_buffer.offset.x = glyph_x / pixel_ratio;
		font_buffer.offset.y = glyph_y / pixel_ratio;
		font_buffer.size = Sizef(pixelbuffer.get_width() / pixel_ratio, pixelbuffer.get_height() / pixel_ratio);
		font_buffer.empty_buffer = false;
		font_buffer.metrics.advance.width = advance.width / pixel_ratio;
		font_buffer.metrics.advance.height = advance.height / pixel_ratio;
		font_buffer.metrics.bbox_offset.x = cg_bbox.origin.x / pixel_ratio;
		font_buffer.metrics.bbox_offset.y = -(cg_bbox.origin.y + cg_bbox.size.height) / pixel_ratio;
		font_buffer.metrics.bbox_size.width = cg_bbox.size.width / pixel_ratio;
		font_buffer.metrics.bbox_size.height = cg_bbox.size.height / pixel_ratio;
		
		return font_buffer;
	}

	void FontEngine_Cocoa::load_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics)
	{
		throw Exception("Implement Me");
	}

}

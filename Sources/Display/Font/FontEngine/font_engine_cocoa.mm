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
		
		handle = CTFontCreateWithName(name, desc.get_height() * pixel_ratio, 0);
		
		CFRelease(name);
		name = 0;
		
		if (handle == 0)
		{
			throw Exception("CGFontCreateWithFontName failed");
		}
		
		if (desc.get_style() != FontStyle::normal || desc.get_weight() != FontWeight::normal)
		{
			// To do: maybe add "-Bold" and such to font name instead. traits seem to be somewhat limited and not well documented
			
			CTFontSymbolicTraits traits = 0;
			if (desc.get_weight() >= FontWeight::bold)
				traits |= kCTFontBoldTrait;
			if (desc.get_style() != FontStyle::normal)
				traits |= kCTFontItalicTrait;
			
			CTFontRef new_handle = CTFontCreateCopyWithSymbolicTraits(handle, desc.get_height() * pixel_ratio, 0, traits, kCTFontBoldTrait|kCTFontItalicTrait);
			
			if (new_handle != 0)
			{
				CFRelease(handle);
				handle = new_handle;
			}
		}
		
		font_metrics = FontMetrics(
			(CTFontGetAscent(handle) + CTFontGetDescent(handle)) / pixel_ratio,
			(CTFontGetAscent(handle)) / pixel_ratio,
			(CTFontGetDescent(handle)) / pixel_ratio,
			0.0f,
			(CTFontGetLeading(handle)) / pixel_ratio,
			desc.get_line_height(),
			pixel_ratio);
		
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
			desc.get_line_height(),
			pixel_ratio);

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
		
		CGRect cg_bbox;
		CTFontGetBoundingRectsForGlyphs(handle, kCTFontDefaultOrientation, &cgglyph, &cg_bbox, 1);
        
        // Black box in bitmap coordinates:
        
        float cursor_x = std::ceil(cg_bbox.origin.x);
        float cursor_y = -std::ceil(cg_bbox.origin.y) + 1;
        
        int glyph_width = std::ceil(cursor_x + cg_bbox.origin.x + cg_bbox.size.width) + 1;
        int glyph_height = std::ceil(cursor_y - cg_bbox.origin.y + cg_bbox.size.height) + 1;
        
        int glyph_x = -cursor_x;
        int glyph_y = -(glyph_height - cursor_y);
        
        CGPoint position;
        position.x = cursor_x; // axis right
        position.y = cursor_y; // axis up
        
        // Check for empty glyphs:
        
        if (glyph_width <= 0 || glyph_height <= 0)
        {
            FontPixelBuffer font_buffer;
            font_buffer.glyph = glyph;
            font_buffer.empty_buffer = true;
            font_buffer.metrics.advance.width = advance.width / pixel_ratio;
            font_buffer.metrics.advance.height = advance.height / pixel_ratio;
            font_buffer.metrics.bbox_offset.x = -cg_bbox.origin.x / pixel_ratio;
            font_buffer.metrics.bbox_offset.y = -(cg_bbox.origin.y + cg_bbox.size.height) / pixel_ratio;
            font_buffer.metrics.bbox_size.width = cg_bbox.size.width / pixel_ratio;
            font_buffer.metrics.bbox_size.height = cg_bbox.size.height / pixel_ratio;
            return font_buffer;
        }
		
		// Render glyph into bitmap:
        
		PixelBuffer pixelbuffer(glyph_width, glyph_height, tf_bgra8);
		unsigned char *p = (unsigned char *)pixelbuffer.get_data();
		int len = pixelbuffer.get_width()*pixelbuffer.get_height();
		for (int i = 0; i < len*4; i++)
			p[i] = 255;
		
		CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
		CGFloat black_components[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		CGColorRef black = CGColorCreate(colorspace, black_components);
		
		CGContextRef context = CGBitmapContextCreate(pixelbuffer.get_data(), pixelbuffer.get_width(), pixelbuffer.get_height(), 8, pixelbuffer.get_width() * 4, colorspace, kCGImageAlphaPremultipliedLast);
		
		CGContextSetAllowsFontSmoothing(context, font_description.get_subpixel() || font_description.get_anti_alias());
		CGContextSetAllowsAntialiasing(context, font_description.get_anti_alias());
		CGContextSetAllowsFontSubpixelQuantization(context, font_description.get_subpixel());
		CGContextSetAllowsFontSubpixelPositioning(context, false);
		CGContextSetFillColorWithColor(context, black);
		
		CTFontDrawGlyphs(handle, &cgglyph, &position, 1, context);
		
		CFRelease(black);
		CGContextRelease(context);
		CGColorSpaceRelease(colorspace);
		
		for (int i = 0; i < len; i++)
		{
			p[i*4+0] = 255-p[i*4+0];
			p[i*4+1] = 255-p[i*4+1];
			p[i*4+2] = 255-p[i*4+2];
			p[i*4+3] = 255;

#if 0 /* Debug black box positions */
			bool l = i % glyph_width == 0;
			bool r = i % glyph_width == glyph_width - 1;
			bool t = i < glyph_width;
			bool b = i >= glyph_width * (glyph_height - 1);
			if (l || r || t || b)
				p[i*4+1] = 255;
#endif
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
		font_buffer.metrics.bbox_offset.x = -cg_bbox.origin.x / pixel_ratio;
		font_buffer.metrics.bbox_offset.y = -(cg_bbox.origin.y + cg_bbox.size.height) / pixel_ratio;
		font_buffer.metrics.bbox_size.width = cg_bbox.size.width / pixel_ratio;
		font_buffer.metrics.bbox_size.height = cg_bbox.size.height / pixel_ratio;
		
		return font_buffer;
	}

	void FontEngine_Cocoa::load_glyph_path(unsigned int glyph_index, Path &path, GlyphMetrics &metrics)
	{
		// Retrieve glyph black box information:
		
		CGGlyph cgglyph = 0;
		UniChar c = glyph_index;
		bool found_char = CTFontGetGlyphsForCharacters(handle, &c, &cgglyph, 1);
		
		CGSize advance;
		CTFontGetAdvancesForGlyphs(handle, kCTFontDefaultOrientation, &cgglyph, &advance, 1);
		
		CGRect cg_bbox;
		CTFontGetBoundingRectsForGlyphs(handle, kCTFontDefaultOrientation, &cgglyph, &cg_bbox, 1);
		
		// Glyph metrics:
		
		metrics.advance.width = advance.width / pixel_ratio;
		metrics.advance.height = advance.height / pixel_ratio;
		metrics.bbox_offset.x = -cg_bbox.origin.x / pixel_ratio;
		metrics.bbox_offset.y = -(cg_bbox.origin.y + cg_bbox.size.height) / pixel_ratio;
		metrics.bbox_size.width = cg_bbox.size.width / pixel_ratio;
		metrics.bbox_size.height = cg_bbox.size.height / pixel_ratio;
		
		// Retrieve path information:
		
		CGPathRef cg_path = CTFontCreatePathForGlyph(handle, cgglyph, 0);
		if (cg_path == 0)
			return;
		
		path.set_fill_mode(PathFillMode::winding);
		
		LoadGlyphPathInfo info(path, pixel_ratio);
		
		CGPathApply(cg_path, &info, [](void *info_ptr, const CGPathElement *element)
		{
			LoadGlyphPathInfo *info = (LoadGlyphPathInfo*)info_ptr;
			
			switch (element->type)
			{
				case kCGPathElementMoveToPoint:
					info->path.move_to(to_pointf(element->points[0]) * info->pixel_ratio);
					break;
					
				case kCGPathElementAddLineToPoint:
					info->path.line_to(to_pointf(element->points[0]) * info->pixel_ratio);
					break;
					
				case kCGPathElementAddQuadCurveToPoint:
					info->path.bezier_to(to_pointf(element->points[0]) * info->pixel_ratio, to_pointf(element->points[1]) * info->pixel_ratio);
					break;
					
				case kCGPathElementAddCurveToPoint:
					info->path.bezier_to(to_pointf(element->points[0]) * info->pixel_ratio, to_pointf(element->points[1]) * info->pixel_ratio, to_pointf(element->points[2]) * info->pixel_ratio);
					break;
					
				case kCGPathElementCloseSubpath:
					info->path.close();
					break;
					
				default:
					break;
			}
		});
		
		CFRelease(cg_path);
	}
	
	Pointf FontEngine_Cocoa::to_pointf(const CGPoint &p)
	{
		return Pointf(p.x, p.y);
	}

}

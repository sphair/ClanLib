/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

namespace clan
{

FontEngine_Cocoa::FontEngine_Cocoa(const FontDescription &desc)
: handle(0)
{
    CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, desc.get_typeface_name().c_str(), kCFStringEncodingUTF8);
    handle = CTFontCreateWithName(name, desc.get_height(), 0);
    CFRelease(name);
    if (handle == 0)
        throw Exception(string_format("Unable to create font %1", desc.get_typeface_name()));
}

FontEngine_Cocoa::~FontEngine_Cocoa()
{
	if (handle)
        CFRelease(handle);
}

FontMetrics FontEngine_Cocoa::get_metrics()
{
	return FontMetrics(
		CTFontGetSize(handle),
		CTFontGetAscent(handle),
		CTFontGetDescent(handle), 
		CTFontGetLeading(handle),
		0.0f /* CTFontGetExternalLeading(handle)*/,
		0.0f/*CTFontGetAverageCharWidth(handle)*/,
		0.0f /*CTFontGetMaxCharWidth(handle)*/,
		400 /* weight */,
		0 /* overhang */, 
		1.0f /* tmDigitizedAspectX */,
		1.0f /* tmDigitizedAspectY */,
		false /* tmItalic */, 
		false /* tmUnderlined */, 
		false /* tmStruckOut */,
		false/*CTFontGetAverageCharWidth(handle) == CTFontGetMaxCharWidth(handle)*/);
}

FontPixelBuffer FontEngine_Cocoa::get_font_glyph_standard(int glyph, bool anti_alias)
{
	return get_font_glyph_lcd(glyph);
}

FontPixelBuffer FontEngine_Cocoa::get_font_glyph_subpixel(int glyph)
{
    return get_font_glyph_standard(glyph, true);
}

FontPixelBuffer FontEngine_Cocoa::get_font_glyph_lcd(int glyph)
{
    // To do: CGRect and CGPoint return values in floats. Simply casting them to integers
    // cause rounding issues with the baseline alignment and the size of the black box.
    // As a temporary hack we extend the black box by 2 pixels but this does not fix the
    // baseline problem.
    
    CGGlyph cgglyph = 0;
    UniChar c = glyph;
    bool found_char = CTFontGetGlyphsForCharacters(handle, &c, &cgglyph, 1);
    
    CGRect bbox;
    CGRect result = CTFontGetBoundingRectsForGlyphs(handle, kCTFontDefaultOrientation, &cgglyph, &bbox, 1);
    //if (result == CGRectNull)
    //    return get_empty_font_glyph(glyph);
    
    PixelBuffer pixelbuffer(bbox.size.width+2, bbox.size.height+2, tf_bgra8);
	unsigned char *p = (unsigned char *)pixelbuffer.get_data();
    int len = pixelbuffer.get_width()*pixelbuffer.get_height();
	for (int i = 0; i < len*4; i++)
        p[i] = 255;
    
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    
    CGContextRef context = CGBitmapContextCreate(pixelbuffer.get_data(), pixelbuffer.get_width(), pixelbuffer.get_height(), 8, pixelbuffer.get_width() * 4, colorspace, kCGImageAlphaPremultipliedFirst);
    
    CGPoint position;
    position.x = -bbox.origin.x+1;
    position.y = -bbox.origin.y+1;
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
    
    CGSize advance;
    CTFontGetAdvancesForGlyphs(handle, kCTFontDefaultOrientation, &cgglyph, &advance, 1);
    
	FontPixelBuffer font_buffer;
	font_buffer.glyph = glyph;
	font_buffer.buffer = pixelbuffer;
	font_buffer.buffer_rect = pixelbuffer.get_size();
	font_buffer.offset.x = bbox.origin.x;
	font_buffer.offset.y = -bbox.origin.y - bbox.size.height;
	font_buffer.empty_buffer = false;
    font_buffer.increment.x = advance.width;
    font_buffer.increment.y = advance.height;
	return font_buffer;
}

FontPixelBuffer FontEngine_Cocoa::get_empty_font_glyph(int glyph)
{
	FontPixelBuffer font_buffer;
	font_buffer.glyph = glyph;
	font_buffer.empty_buffer = true;
    
    CGGlyph cgglyph = glyph;
    CGSize advance;
    CTFontGetAdvancesForGlyphs(handle, kCTFontDefaultOrientation, &cgglyph, &advance, 1);
    font_buffer.increment.x = advance.width;
    font_buffer.increment.y = advance.height;
    
	return font_buffer;
}

Shape2D FontEngine_Cocoa::load_glyph_outline(int c, int &out_advance_x);
{
	throw Exception("Implement Me");
}

}

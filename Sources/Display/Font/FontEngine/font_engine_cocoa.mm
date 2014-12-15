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

void fontProviderReleaseData (
                                void *info,
                                const void *data,
                                size_t size
                              )
{
    char* rawbytes = (char*)info;
    delete[] rawbytes;
}

CGFloat getLineHeightForFont(CTFontRef iFont)
{
    CGFloat lineHeight = 0.0;
    
   
    // Get the ascent from the font, already scaled for the font's size
    lineHeight += CTFontGetAscent(iFont);
    
    // Get the descent from the font, already scaled for the font's size
    lineHeight += CTFontGetDescent(iFont);
    
    // Get the leading from the font, already scaled for the font's size
    lineHeight += CTFontGetLeading(iFont);
    
    return lineHeight;
}
    
    
// Creates a CTFont with the given CGFont and pixel size. Ownership is
// transferred to the caller.
//
// Note: This code makes use of pixel sizes (rather than view coordinate sizes)
CTFontRef CreateCTFontWithPixelSize(CGFontRef cgFont,
                                    const int target_pixel_size) {
    // Epsilon value used for comparing font sizes.
    const CGFloat kEpsilon = 0.001;
    // The observed pixel to points ratio for Lucida Grande on 10.6. Other fonts
    // have other ratios and the documentation doesn't provide a guarantee that
    // the relation is linear. So this ratio is used as a first try before
    // falling back to the bisection method.
    const CGFloat kPixelsToPointsRatio = 0.849088;
    
 
    
    // First, try using |kPixelsToPointsRatio|.
    CGFloat point_size = target_pixel_size * kPixelsToPointsRatio;
    CTFontRef ct_font(CTFontCreateWithGraphicsFont(cgFont, point_size, NULL,NULL));
    CGFloat actual_pixel_size = getLineHeightForFont(ct_font);
    if (std::fabs(actual_pixel_size - target_pixel_size) < kEpsilon){
        return ct_font;
    }
    
    // |kPixelsToPointsRatio| wasn't correct. Use the bisection method to find the
    // right size.
    
    // First, find the initial bisection range, so that the point size that
    // corresponds to |target_pixel_size| is between |lo| and |hi|.
    CGFloat lo = 0;
    CGFloat hi = point_size;
    while (actual_pixel_size < target_pixel_size) {
        lo = hi;
        hi *= 2;
        CFRelease(ct_font);
        ct_font = CTFontCreateWithGraphicsFont(cgFont, hi, NULL,NULL);
        actual_pixel_size = getLineHeightForFont(ct_font);
    }
    
    // Now, bisect to find the right size.
    while (lo < hi) {
        point_size = (hi - lo) * 0.5 + lo;
        CFRelease(ct_font);
        ct_font  = CTFontCreateWithGraphicsFont(cgFont, point_size, NULL,NULL);
        actual_pixel_size = getLineHeightForFont(ct_font);
        if (std::fabs(actual_pixel_size - target_pixel_size) < kEpsilon)
            break;
        if (target_pixel_size > actual_pixel_size)
            lo = point_size;
        else
            hi = point_size;
    }
    
    return ct_font;
}
    
void FontEngine_Cocoa::load_font(const FontDescription& desc, const std::string& filename, IODevice& file){
    char * rawBytes = new char[file.get_size()]; // TODO: A better version would use CGDataProviderCreateDirect and give it callbacks to load from vfs itself.
    file.read(rawBytes,file.get_size());
    // Then, create a data provider
    CGDataProviderRef dataProvider =  CGDataProviderCreateWithData (
                                                                    rawBytes,
                                                                    rawBytes,
                                                                    file.get_size(),
                                                                    fontProviderReleaseData
                                                                    );
    // Now use our data provider to load a CGFont
    CGFontRef theCGFont = CGFontCreateWithDataProvider(dataProvider);
    
#if APPROX_FONT_SIZE
    //The real way to do this unfortunately involves a lot of
    // creating and destroying the font and narrowing in on the right
    // pixel size. So I cheat and approximate.
    const CGFloat ptSize = float(desc.get_height()) * 0.7f; // The 0.7f is rough.
    handle = CTFontCreateWithGraphicsFont(theCGFont, ptSize, NULL,NULL);
#else
    handle = CreateCTFontWithPixelSize(theCGFont,desc.get_height());
#endif
    
    CGRect bbox = CTFontGetBoundingBox(handle);
    avg_glyph_width = bbox.size.width / CTFontGetGlyphCount(handle);

    CFRelease(dataProvider);
    CFRelease(theCGFont);
    //delete[] rawBytes;
    if (handle == 0)
        throw Exception(string_format("Unable to create font %1", desc.get_typeface_name()));
    
}
    
FontEngine_Cocoa::FontEngine_Cocoa(const FontDescription &desc, const std::string &filename, FileSystem& vfs)
: handle(0)
{

    // First load our file into memory
    IODevice file = vfs.open_file(filename);
    load_font(desc,filename,file);

	font_metrics = FontMetrics(
		getLineHeightForFont(handle),
		CTFontGetAscent(handle),
		CTFontGetDescent(handle), 
		CTFontGetLeading(handle),
		0.0f/* CTFontGetExternalLeading(handle)*/,
		avg_glyph_width,
		0.0f /*CTFontGetMaxCharWidth(handle)*/,
		400 /* weight */,
		0 /* overhang */, 
		1.0f /* tmDigitizedAspectX */,
		1.0f /* tmDigitizedAspectY */,
		false /* tmItalic */, 
		false /* tmUnderlined */, 
		false /* tmStruckOut */,
		false/*CTFontGetAverageCharWidth(handle) == CTFontGetMaxCharWidth(handle)*/);

	font_description = desc.clone();

}

FontEngine_Cocoa::~FontEngine_Cocoa()
{
	if (handle)
        CFRelease(handle);
}

FontPixelBuffer FontEngine_Cocoa::get_font_glyph(int glyph)
{
	if (font_description.get_subpixel())
	{
		return get_font_glyph_subpixel(glyph);
	}
	else
	{
		get_font_glyph_standard(glyph, font_description.get_anti_alias());
	}
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

	font_buffer.metrics.advance.width = advance.width;
	font_buffer.metrics.advance.height = advance.width;
	//FIXME = font_buffer.metrics.black_box
	
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
	font_buffer.metrics.advance.width = advance.width;
	font_buffer.metrics.advance.height = advance.width;
	//FIXME = font_buffer.metrics.black_box
    
	return font_buffer;
}

void FontEngine_Cocoa::load_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics)
{
	throw Exception("Implement Me");
}

}

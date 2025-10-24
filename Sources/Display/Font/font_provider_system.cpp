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
#include "font_provider_system.h"

#ifndef WIN32	// Not WIN32
#include "freetype_font.h"
#include "freetype_font_provider.h"
#include "font_provider_freetype.h"
#endif

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/draw.h"
#include "API/Core/Text/string_help.h"

#include "../2D/sprite_render_batch.h"
#include "../Render/graphic_context_impl.h"

#ifndef WIN32
#include "../X11/font_config.h"
#endif

#ifndef WIN32
std::map<CL_String, CL_String > CL_FontProvider_Texture::font_register_cache;
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Texture Construction:

CL_FontProvider_Texture::CL_FontProvider_Texture(CL_GraphicContext &gc) : handle(0)
{
	glyph_list.reserve(256);

	// Note, the user can specify a different texture group size using set_texture_group()
	texture_group = CL_TextureGroup(gc, CL_Size(256,256));

	// Set default font metrics
	font_metrics = CL_FontMetrics(
		0,0, 0, 0,0,0,0,0, 0,0,
		false, false, false, false);

	anti_alias = false;
}

CL_FontProvider_Texture::~CL_FontProvider_Texture()
{
	free_font();

	for (int cnt = 0; cnt < glyph_list.size(); cnt++)
		delete glyph_list[cnt];
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Texture Attributes:

#ifdef WIN32
HFONT CL_FontProvider_Texture::get_handle()
{
	return handle;
}
#else
CL_FreetypeFont *CL_FontProvider_Texture::get_handle()
{
	return handle;
}
#endif

CL_FontMetrics CL_FontProvider_Texture::get_font_metrics(CL_GraphicContext &gc)
{
	return font_metrics;
}

CL_Size CL_FontProvider_Texture::get_text_size(CL_GraphicContext &gc, const CL_StringRef &text)
{
	int width = 0;

	for (CL_String::size_type p = 0; p < text.length(); p++)
	{
		CL_Font_System_Glyph *gptr = get_glyph(gc, text[p]);
		if (gptr == NULL) continue;
		width += gptr->increment.x;
	}
	int height;
	if (width == 0)
	{
		height = 0;
	}
	else
	{
		height = size_height;
	}

	return (CL_Size(width, height));
}

CL_Font_System_Glyph *CL_FontProvider_Texture::get_glyph(CL_GraphicContext &gc, unsigned int glyph)
{
	std::vector< CL_Font_System_Glyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return &(*glyph_list[cnt]);
	}

	// If glyph does not exist and a system font exists, create one automatically
	if (handle)
	{
		insert_glyph(gc, glyph);

		// Search for the glyph again
		size = glyph_list.size();
		for (int cnt=0; cnt<size; cnt++)
		{
			if (glyph_list[cnt]->glyph == glyph)
				return &(*glyph_list[cnt]);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Texture Operations:

int CL_FontProvider_Texture::get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point)
{
	int dest_x = 0;
	int dest_y = 0;

	int character_counter = 0;

	CL_FontMetrics fm = get_font_metrics(gc);
	int font_height = fm.get_height();
	int font_external_leading = fm.get_external_leading();

	std::vector<CL_TempString> lines = CL_StringHelp::split_text(text, cl_text("\n"), false);
	for (std::vector<CL_TempString>::size_type i=0; i<lines.size(); i++)
	{
		int xpos = dest_x;
		int ypos = dest_y;

		CL_TempString &textline = lines[i];
		CL_String::size_type string_length = textline.length();

		// Scan the string
		for (CL_String::size_type p = 0; p < string_length; p++)
		{
			CL_Font_System_Glyph *gptr = get_glyph(gc, textline[p]);
			if (gptr == NULL) continue;

			CL_Rect position(xpos, ypos - font_height, CL_Size(gptr->increment.x, gptr->increment.y + font_height + font_external_leading));
			if (position.contains(point))
				return ((int) p) + character_counter;
		
			xpos += gptr->increment.x;
			ypos += gptr->increment.y;
		}

		dest_y += font_height + font_external_leading;

		character_counter += string_length + 1;		// (Including the '\n')

	}
	return -1;	// Not found
}

void CL_FontProvider_Texture::destroy()
{
	delete this;
}

void CL_FontProvider_Texture::free_font()
{
	if (handle)
	{
#ifdef WIN32
		DeleteObject(handle);
#else		
		delete(handle);
#endif
		handle = NULL;
	}
}

CL_FontPixelBuffer CL_FontProvider_Texture::get_font_glyph(CL_GraphicContext &gc, int glyph, bool anti_alias, const CL_Colorf &color)
{
	if (!handle)
	{
		throw CL_Exception("Must load a system font before importing glyphs");
	}

#ifdef WIN32
	HDC hdc = ::GetDC(NULL);
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
#else	// Not WIN32

	CL_FontPixelBuffer font_buffer;

	CL_FreetypeFont *vptr = get_handle();
	if (!vptr) return font_buffer;

	font_buffer.glyph = glyph;

	font_buffer = vptr->create_pixelbuffer(glyph, anti_alias, color);
	return font_buffer;
#endif
}

void CL_FontProvider_Texture::insert_glyph(CL_GraphicContext &gc, CL_Font_System_Position &position, CL_PixelBuffer &pixel_buffer)
{
	unsigned int glyph = position.glyph;

	// Search for duplicated glyph's, if found silently ignore them
	std::vector< CL_Font_System_Glyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return;
	}

	CL_Font_System_Glyph *font_glyph = new(CL_Font_System_Glyph);
	
	glyph_list.push_back(font_glyph);
	font_glyph->glyph = glyph;
	font_glyph->offset = CL_Point(position.x_offset, position.y_offset);
	font_glyph->increment = CL_Point(position.x_increment, position.y_increment);

	if ( (position.width > 0 ) && (position.height > 0) )
	{
		font_glyph->empty_buffer = false;
		font_glyph->subtexture = texture_group.add(gc, CL_Size(position.width, position.height));
		font_glyph->texture = font_glyph->subtexture.get_texture(); // cached for performance reasons

		CL_PixelBuffer buffer = pixel_buffer.get_subimage(CL_Rect(position.x_pos, position.y_pos, position.width + position.x_pos, position.height + position.y_pos));

		font_glyph->texture.set_subimage(font_glyph->subtexture.get_geometry().left, font_glyph->subtexture.get_geometry().top, buffer);

	}
	else
	{
		font_glyph->empty_buffer = true;
	}
}

void CL_FontProvider_Texture::insert_glyph(CL_GraphicContext &gc, CL_FontPixelBuffer &pb)
{
	// Search for duplicated glyph's, if found silently ignore them
	std::vector< CL_Font_System_Glyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == pb.glyph)
			return ;
	}

	CL_Font_System_Glyph *font_glyph = new(CL_Font_System_Glyph);
	
	glyph_list.push_back(font_glyph);
	font_glyph->glyph = pb.glyph;
	font_glyph->empty_buffer = pb.empty_buffer;
	font_glyph->offset = pb.offset;
	font_glyph->increment = pb.increment;

	if (!pb.empty_buffer)
	{
		font_glyph->subtexture = texture_group.add(gc, CL_Size(pb.buffer.get_width(), pb.buffer.get_height()));
		font_glyph->texture = font_glyph->subtexture.get_texture(); // cached for performance reasons
		font_glyph->texture.set_subimage(font_glyph->subtexture.get_geometry().left, font_glyph->subtexture.get_geometry().top, pb.buffer);
	}
}

void CL_FontProvider_Texture::insert_glyph(CL_GraphicContext &gc, int glyph)
{
	CL_FontPixelBuffer pb = get_font_glyph(gc, glyph, anti_alias, CL_Colorf::white);

	if (pb.glyph)	// Ignore invalid glyphs
	{
		insert_glyph(gc, pb);
	}
}

void CL_FontProvider_Texture::insert_glyph(CL_GraphicContext &gc, const CL_StringRef &text)
{
	for (CL_String::size_type p = 0; p < text.length(); ++p)
	{
		insert_glyph(gc, text[p]);
	}
}

void CL_FontProvider_Texture::load_font( CL_GraphicContext &context, const CL_FontDescription &desc)
{
	free_font();

	anti_alias = true;	// Default, anti_alias enabled (may be modified by WIN32 later)

#ifdef WIN32
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

	if (handle==NULL)
	{
		throw CL_Exception(cl_text("Unable to create font - Windows CreateFont() failed"));
	}

	// Calculate what windows says the font height size should be
	HDC hdc = ::GetDC(NULL);
	int old_mode = SetMapMode(hdc, MM_TEXT);
	HGDIOBJ old_font = SelectObject( hdc, handle );
	SIZE size;
	CL_StringRef text(cl_text("."));
	GetTextExtentPoint32(hdc, text.data(), text.length(), &size);
	TEXTMETRIC text_metrics;
	if (GetTextMetrics(hdc, &text_metrics))
	{
		if (text_metrics.tmHeight <= 18)
			anti_alias = false;
	}

	SelectObject(hdc, old_font);
	SetMapMode(hdc, old_mode);
	::ReleaseDC(NULL, hdc);
	size_height = size.cy;

#else
	if (handle) delete handle;

	CL_FontDescription new_desc = get_registered_font(desc);

	CL_String font_file_path = new_desc.get_typeface_name();

	// Try opening the font file.
	CL_IODevice io_dev;
	try
	{
		io_dev = CL_File(font_file_path, CL_File::open_existing, CL_File::access_read);
	}
	catch(CL_Exception error)
	{
		throw CL_Exception(cl_text(cl_format("Cannot open font file: \"%1\"", font_file_path)));
	}

	size_height =  new_desc.get_height();

        // Load font from the opened file.
	CL_FreetypeFontProvider &provider = CL_FreetypeFontProvider::instance();
	handle = provider.load_font(io_dev, size_height, new_desc.get_average_width());


#endif
	if (desc.get_anti_alias_set())	// Anti-alias was set
	{
		anti_alias = desc.get_anti_alias();	// Override the default
	}

	write_font_metrics(context);
}

void CL_FontProvider_Texture::draw_text(CL_GraphicContext &gc, int xpos, int ypos, const CL_StringRef &text, const CL_Colorf &color) 
{
	CL_String::size_type string_length = text.length();
	if (string_length==0)
	{
		return;
	}

	CL_SpriteRenderBatch *batcher = &gc.impl->sprite_batcher;

	// Scan the string
	for (CL_String::size_type p = 0; p < string_length; p++)
	{
		CL_Font_System_Glyph *gptr = get_glyph(gc, text[p]);
		if (gptr == NULL) continue;

		if (!gptr->empty_buffer)
		{
			int xp = xpos + gptr->offset.x;
			int yp = ypos + gptr->offset.y;

			CL_Rect tg = gptr->subtexture.get_geometry();
			CL_Rectf dest_size((float)xp, (float)yp, (float)xp + tg.get_width(), (float)yp + tg.get_height());
			batcher->draw_image(gc, tg, dest_size, color, gptr->texture);
		}
		xpos += gptr->increment.x;
		ypos += gptr->increment.y;
	}
}

void CL_FontProvider_Texture::set_font_metrics(const CL_FontMetrics &metrics)
{
	font_metrics = metrics;
}

void CL_FontProvider_Texture::set_texture_group(CL_TextureGroup &new_texture_group)
{
	if (new_texture_group.is_null())
	{
		throw CL_Exception(cl_text("Specified texture group is not valid"));
	}

	if (!glyph_list.empty())
	{
		throw CL_Exception(cl_text("Cannot specify a new texture group after the font has been used"));
	}

	texture_group = new_texture_group;
}

void CL_FontProvider_Texture::register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface)
{
#ifdef WIN32
	int fonts_added = AddFontResourceEx(font_filename.c_str(), FR_PRIVATE|FR_NOT_ENUM, 0);
	if(fonts_added == 0)
		throw CL_Exception(cl_text("Unable to register font " + font_filename));
#else
	std::map<CL_String, CL_String >::iterator find_it;
	find_it = font_register_cache.find(font_typeface);
	if (find_it == font_register_cache.end())	// Ensure not already registered
	{
		font_register_cache[font_typeface] = font_filename;
	}
#endif
}


/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Texture Implementation:

void CL_FontProvider_Texture::write_font_metrics(CL_GraphicContext &gc)
{
#ifdef WIN32
	if (get_handle())
	{
		HDC hdc = ::GetDC(NULL);
		int old_mode = SetMapMode(hdc, MM_TEXT);
		HGDIOBJ old_font = SelectObject(hdc, get_handle());

		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);

		SelectObject(hdc, old_font);
		SetMapMode(hdc, old_mode);

		font_metrics = CL_FontMetrics(
			(float)tm.tmHeight,
			(float)tm.tmAscent, 
			(float)tm.tmDescent, 
			(float)tm.tmInternalLeading,
			(float)tm.tmExternalLeading,
			(float)tm.tmAveCharWidth,
			(float)tm.tmMaxCharWidth,
			(float)tm.tmWeight,
			(float)tm.tmOverhang, 
			(float)tm.tmDigitizedAspectX,
			(float)tm.tmDigitizedAspectY,
			tm.tmItalic != 0, 
			tm.tmUnderlined != 0, 
			tm.tmStruckOut != 0,
			tm.tmAveCharWidth == tm.tmMaxCharWidth);

		::ReleaseDC(NULL, hdc); 
	}
#else
	CL_FreetypeFont *vptr = get_handle();
	if (vptr)
	{
		font_metrics = vptr->get_font_metrics();
	}
#endif
}

#ifndef WIN32
CL_FontDescription CL_FontProvider_Texture::get_registered_font(const CL_FontDescription &desc)
{
	int average_width = desc.get_average_width();
	int height = desc.get_height();

	// Attempt to convert the point sizes (to match WIN32 with FreeType)
	if (average_width==0)	// Unset width
	{
		if (height <0)
		{
			average_width = -height;
			average_width = (average_width * 80) / 96;
		}
		else
		{
			// I do not know why this formula works,  but it seems to obtain the best result
			average_width = ( height * 80 * 80 ) / (96 * 96);
		}
	}

	if (height <0)
	{
		height = -height;
	}
	else
	{
		height = (height * 80) / 96;
	}

	CL_FontDescription new_desc;
	new_desc.copy(desc);
	new_desc.set_average_width(average_width);
	new_desc.set_height(height);

	// Check for a registered font
	std::map<CL_String, CL_String >::iterator find_it;
	find_it = font_register_cache.find(desc.get_typeface_name());
	if (find_it != font_register_cache.end())	// Found the registered font
	{
		new_desc.set_typeface_name(find_it->second);
	}
	else
	{
        // Obtain the best matching font file from fontconfig.
		CL_FontConfig &fc = CL_FontConfig::instance();
		CL_String font_file_path = fc.match_font(new_desc);
		new_desc.set_typeface_name(font_file_path);
	}
	return new_desc;
}
#endif

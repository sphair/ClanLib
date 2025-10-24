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
#include "font_provider_freetype.h"
#include "freetype_font.h"
#include "freetype_font_provider.h"
#include "font_provider_freetype.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
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

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Construction:

CL_FontProvider_Freetype::CL_FontProvider_Freetype(CL_GraphicContext &gc) : handle(0)
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

CL_FontProvider_Freetype::~CL_FontProvider_Freetype()
{
	free_font();

	for (int cnt = 0; cnt < glyph_list.size(); cnt++)
		delete glyph_list[cnt];
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Attributes:

CL_FreetypeFont *CL_FontProvider_Freetype::get_handle()
{
	return handle;
}

CL_FontMetrics CL_FontProvider_Freetype::get_font_metrics(CL_GraphicContext &gc)
{
	return font_metrics;
}

CL_Size CL_FontProvider_Freetype::get_text_size(CL_GraphicContext &gc, const CL_StringRef &text)
{
	int width = 0;

	for (CL_String::size_type p = 0; p < text.length(); p++)
	{
		CL_Font_Freetype_Glyph *gptr = get_glyph(gc, text[p]);
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

CL_Font_Freetype_Glyph *CL_FontProvider_Freetype::get_glyph(CL_GraphicContext &gc, unsigned int glyph)
{
	std::vector< CL_Font_Freetype_Glyph * >::size_type size = glyph_list.size();
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
// CL_FontProvider_Freetype Operations:

int CL_FontProvider_Freetype::get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point)
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
			CL_Font_Freetype_Glyph *gptr = get_glyph(gc, textline[p]);
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

void CL_FontProvider_Freetype::destroy()
{
	delete this;
}

void CL_FontProvider_Freetype::free_font()
{
	if (handle)
	{
		delete(handle);
		handle = NULL;
	}
}

CL_FontPixelBuffer CL_FontProvider_Freetype::get_font_glyph(CL_GraphicContext &gc, int glyph, bool anti_alias, const CL_Colorf &color)
{
	if (!handle)
	{
		throw CL_Exception("Must load a system font before importing glyphs");
	}

	CL_FontPixelBuffer font_buffer;

	CL_FreetypeFont *vptr = get_handle();
	if (!vptr) return font_buffer;

	font_buffer.glyph = glyph;

	font_buffer = vptr->create_pixelbuffer(glyph, anti_alias, color);
	return font_buffer;
}

void CL_FontProvider_Freetype::insert_glyph(CL_GraphicContext &gc, CL_FontPixelBuffer &pb)
{
	// Search for duplicated glyph's, if found silently ignore them
	std::vector< CL_Font_Freetype_Glyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == pb.glyph)
			return ;
	}

	CL_Font_Freetype_Glyph *font_glyph = new(CL_Font_Freetype_Glyph);
	
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

void CL_FontProvider_Freetype::insert_glyph(CL_GraphicContext &gc, int glyph)
{
	CL_FontPixelBuffer pb = get_font_glyph(gc, glyph, anti_alias, CL_Colorf::white);

	if (pb.glyph)	// Ignore invalid glyphs
	{
		insert_glyph(gc, pb);
	}
}

void CL_FontProvider_Freetype::load_font(const CL_FontDescription &desc)
{
	CL_String path = CL_PathHelp::get_fullpath(desc.get_typeface_name(), CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(desc.get_typeface_name(), CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_IODevice file = vfs.get_root_directory().open_file_read(filename);
	load_font(desc, file);
}

void CL_FontProvider_Freetype::load_font(const CL_FontDescription &desc, const CL_VirtualDirectory &directory)
{
	CL_IODevice file = directory.open_file_read(desc.get_typeface_name());
	load_font(desc, file);
}

void CL_FontProvider_Freetype::load_font(const CL_FontDescription &desc, CL_IODevice &io_dev)
{
	free_font();

	anti_alias = true;	// Default, anti_alias enabled

	size_height =  desc.get_height();

	 // Load font from the opened file.
	CL_FreetypeFontProvider &provider = CL_FreetypeFontProvider::instance();
	handle = provider.load_font(io_dev, size_height, desc.get_average_width());

	if (desc.get_anti_alias_set())	// Anti-alias was set
	{
		anti_alias = desc.get_anti_alias();	// Override the default
	}

	font_metrics = handle->get_font_metrics();
}

void CL_FontProvider_Freetype::draw_text(CL_GraphicContext &gc, int xpos, int ypos, const CL_StringRef &text, const CL_Colorf &color) 
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
		CL_Font_Freetype_Glyph *gptr = get_glyph(gc, text[p]);
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

void CL_FontProvider_Freetype::set_texture_group(CL_TextureGroup &new_texture_group)
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

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Freetype Implementation:

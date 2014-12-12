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
#include "glyph_cache.h"
#include "FontEngine/font_engine.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Image/pixel_buffer_help.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/utf8_reader.h"
#include "../2D/render_batch_triangle.h"
#include "../Render/graphic_context_impl.h"
#include "API/Display/2D/canvas.h"
#include "../2D/canvas_impl.h"
#include "API/Display/Font/glyph_metrics.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GlyphCache Construction:

GlyphCache::GlyphCache()
{
	glyph_list.reserve(256);
	texture_group = TextureGroup(Size(256,256));
}

GlyphCache::~GlyphCache()
{
	for (int cnt = 0; cnt < glyph_list.size(); cnt++)
		delete glyph_list[cnt];
}

/////////////////////////////////////////////////////////////////////////////
// GlyphCache Attributes:

Font_TextureGlyph *GlyphCache::get_glyph(Canvas &canvas, FontEngine *font_engine, unsigned int glyph)
{
	std::vector< Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return &(*glyph_list[cnt]);
	}

	// If glyph does not exist, create one automatically
	FontPixelBuffer pb = enable_subpixel ? font_engine->get_font_glyph_subpixel(glyph) : font_engine->get_font_glyph_standard(glyph, anti_alias);
	if (pb.glyph)	// Ignore invalid glyphs
		insert_glyph(canvas, pb);

	// Search for the glyph again
	size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return &(*glyph_list[cnt]);
	}

	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
// GlyphCache Operations:

void GlyphCache::draw(FontEngine *font_engine, Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color)
{
	RenderBatchTriangle *batcher = canvas.impl->batcher.get_triangle_batcher();
	GraphicContext &gc = canvas.get_gc();

	Pointf pos = canvas.grid_fit(position);
	float offset_x = 0;
	float offset_y = 0;
	int line_spacing = static_cast<int>(font_metrics.get_line_height() + 0.5f);
	UTF8_Reader reader(text.data(), text.length());
	while (!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();

		if (glyph == '\n')
		{
			offset_x = 0;
			offset_y += line_spacing;
			continue;
		}

		Font_TextureGlyph *gptr = get_glyph(canvas, font_engine, glyph);
		if (gptr)
		{
			if (!gptr->texture.is_null())
			{
				float xp = offset_x + pos.x + gptr->offset.x;
				float yp = offset_y + pos.y + gptr->offset.y;

				Rectf dest_size(xp, yp, Sizef(gptr->geometry.get_size()));
				if (enable_subpixel)
				{
					batcher->draw_glyph_subpixel(canvas, gptr->geometry, dest_size, color, gptr->texture);
				}
				else
				{
					batcher->draw_image(canvas, gptr->geometry, dest_size, color, gptr->texture);
				}
			}
			offset_x += gptr->metrics.advance.width;
			offset_y += gptr->metrics.advance.height;

		}
	}
}

GlyphMetrics GlyphCache::get_metrics(FontEngine *font_engine, Canvas &canvas, unsigned int glyph)
{
	Font_TextureGlyph *gptr = get_glyph(canvas, font_engine, glyph);
	if (gptr)
	{
		return gptr->metrics;
	}
	return GlyphMetrics();
}

void GlyphCache::get_glyph_path(FontEngine *font_engine, unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics)
{
	return font_engine->load_glyph_path(glyph_index, out_path, out_metrics);
}

void GlyphCache::insert_glyph(Canvas &canvas, FontPixelBuffer &pb)
{
	// Search for duplicated glyph's, if found silently ignore them
	std::vector< Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == pb.glyph)
			return ;
	}

	auto font_glyph = new Font_TextureGlyph();
	
	glyph_list.push_back(font_glyph);
	font_glyph->glyph = pb.glyph;
	font_glyph->offset = pb.offset;
	font_glyph->metrics = pb.metrics;

	if (!pb.empty_buffer)
	{
		PixelBuffer buffer_with_border = PixelBufferHelp::add_border(pb.buffer, glyph_border_size, pb.buffer_rect);
		GraphicContext gc = canvas.get_gc();
		Subtexture sub_texture = texture_group.add(gc, Size(buffer_with_border.get_width(), buffer_with_border.get_height() ));
		font_glyph->texture = sub_texture.get_texture();
		font_glyph->geometry = Rect(sub_texture.get_geometry().left + glyph_border_size, sub_texture.get_geometry().top + glyph_border_size, pb.buffer_rect.get_size() );
		sub_texture.get_texture().set_subimage(gc, sub_texture.get_geometry().left, sub_texture.get_geometry().top, buffer_with_border, buffer_with_border.get_size());
	}
}

void GlyphCache::insert_glyph(Canvas &canvas, unsigned int glyph, Subtexture &sub_texture, const Point &offset, const GlyphMetrics &glyph_metrics)
{
	// Search for duplicated glyph's, if found silently ignore them
	std::vector< Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return;
	}

	auto font_glyph = new Font_TextureGlyph();
	
	glyph_list.push_back(font_glyph);
	font_glyph->glyph = glyph;
	font_glyph->offset = offset;
	font_glyph->metrics = glyph_metrics;

	if ( (sub_texture.get_geometry().get_width() > 0 ) && (sub_texture.get_geometry().get_height() > 0) )
	{
		font_glyph->texture = sub_texture.get_texture();
		font_glyph->geometry = sub_texture.get_geometry();
	}

}

/////////////////////////////////////////////////////////////////////////////
// GlyphCache Implementation:

}

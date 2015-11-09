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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/Resources/display_cache.h"
#include "API/Core/IOData/path_help.h"
#include "Display/2D/canvas_impl.h"
#include "Display/Font/FontEngine/font_engine.h"
#include "Display/2D/sprite_impl.h"
#include "font_draw_subpixel.h"
#include "Display/Font/glyph_cache.h"
#include "Display/Font/path_cache.h"

namespace clan
{
	void Font_DrawSubPixel::init(GlyphCache *cache, FontEngine *engine)
	{
		glyph_cache = cache;
		font_engine = engine;
	}

	GlyphMetrics Font_DrawSubPixel::get_metrics(Canvas &canvas, unsigned int glyph)
	{
		return glyph_cache->get_metrics(font_engine, canvas, glyph);
	}

	void Font_DrawSubPixel::draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color, float line_spacing)
	{
		float offset_x = 0;
		float offset_y = 0;
		UTF8_Reader reader(text.data(), text.length());
		RenderBatchTriangle *batcher = canvas.impl->batcher.get_triangle_batcher();

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

			Font_TextureGlyph *gptr = glyph_cache->get_glyph(canvas, font_engine, glyph);
			if (gptr)
			{
				if (!gptr->texture.is_null())
				{
					float xp = offset_x + position.x + gptr->offset.x;
					float yp = offset_y + position.y + gptr->offset.y;
					Pointf pos = canvas.grid_fit(Pointf(xp, yp));

					Rectf dest_size(pos, gptr->size);
					batcher->draw_glyph_subpixel(canvas, gptr->geometry, dest_size, color, gptr->texture);
				}
				offset_x += gptr->metrics.advance.width;
				offset_y += gptr->metrics.advance.height;
			}
		}
	}
}

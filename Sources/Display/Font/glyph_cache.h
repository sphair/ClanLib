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

#pragma once

#include "API/Display/Font/font.h"
#include "API/Display/Font/glyph_metrics.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/texture_2d.h"
#include <list>
#include <map>

namespace clan
{
	class Colorf;
	class TextureGroup;
	class FontEngine;
	class Font_TextureGlyph;
	class Subtexture;
	class FontPixelBuffer;
	class Path;
	class RenderBatchTriangle;

	/// \brief Font texture format (holds a pixel buffer containing a glyph)
	class Font_TextureGlyph
	{
	public:
		Font_TextureGlyph() : glyph(0) { };

		/// \brief Glyph this pixel buffer refers to.
		unsigned int glyph;

		/// \brief The pixel buffer containing the glyph.
		///
		/// This maybe a null texture
		Texture2D texture;

		/// \brief Geometry of the glyph inside the subtexture (excluding the border)
		Rect geometry;

		/// \brief Offset to draw the font to buffer
		/** For example:
			x = pos_x + pixelbuffer.offset.x
			y = pos_y + pixelbuffer.offset.y*/
		Pointf offset;

		/// \brief Glyph size in device independent pixels (96 dpi)
		Sizef size;

		GlyphMetrics metrics;
	};

	class GlyphCache
	{
	public:
		GlyphCache();
		virtual ~GlyphCache();

		/// \brief Get a glyph. Returns NULL if the glyph was not found
		Font_TextureGlyph *get_glyph(Canvas &canvas, FontEngine *font_engine, unsigned int glyph);

		GlyphMetrics get_metrics(FontEngine *font_engine, Canvas &canvas, unsigned int glyph);

		void insert_glyph(Canvas &canvas, unsigned int glyph, Subtexture &sub_texture, const Pointf &offset, const Sizef &size, const GlyphMetrics &glyph_metrics);
		void insert_glyph(Canvas &canvas, FontPixelBuffer &pb);

		void set_texture_group(TextureGroup &new_texture_group);

	private:
		std::vector<std::unique_ptr<Font_TextureGlyph>> glyph_list;
		TextureGroup texture_group;

		static const int glyph_border_size = 1;
	};
}

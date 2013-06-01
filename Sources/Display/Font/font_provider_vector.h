/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/shape2d.h"
#include "API/Display/Render/texture_2d.h"
#include <list>
#include <map>

namespace clan
{

class FreetypeFont;
class FontEngine;

class FontProvider_Vector : public FontProvider
{
/// \name Construction
/// \{

public:
	FontProvider_Vector();
	virtual ~FontProvider_Vector();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns information about the current font.
	virtual FontMetrics get_font_metrics();

/// \}
/// \name Operations
/// \{
public:
	/// \brief Print text on gc.
	void draw_text(Canvas &canvas, float x, float y, const std::string &text, const Colorf &color);

	/// \brief Calculate size of text string.
	Size get_text_size(GraphicContext &gc, const std::string &text);

	/// \brief Set to draw filled (default)
	void set_filled(bool enable);

	void get_glyphs(
		const std::string &text,
		int *out_glyphs,
		float *out_interspacing_x,
		float *out_interspacing_y);

	void draw_glyphs(
		Canvas &canvas,
		float x,
		float y,
		int *glyphs,
		int length,
		bool filled,
		float *interspacing_x,
		float *interspacing_y,
		const Colorf &color = Colorf::white);

	int get_character_index(GraphicContext &gc, const std::string &text, const Point &point);

	void load_font(const FontDescription &desc, const std::string &filename);
	Rectf get_bounding_box(const std::string &reference_string);

	void set_texture(const Texture2D &src_texture, const Rectf &bounding_rect, const Rectf &texture_rect);

	const std::vector<Vec2f> &get_glyph_filled(unsigned int glyph);
	const std::vector< std::vector<Vec2f> > &get_glyph_outline(unsigned int glyph);

/// \}
/// \name Implementation
/// \{
private:
	void store_in_char_cache(unsigned int glyph);

	struct vector_glyph
	{
		std::vector<Vec2f> primitives_array;
		std::vector< std::vector<Vec2f> > primitives_array_outline;
		std::vector<Vec2f> texture_positions;
		Rectf calculated_bounding_rect;	// texture_positions calculated using this rect
		Rectf calculated_texture_rect;	// texture_positions calculated using this rect

		//Note advance_x is wrong.  For freetype, it should use...
		//out_interspacing_x[i] = font_engine->get_advance_x( text[i] );
		//out_interspacing_x[i] += font_engine->get_kerning( text[i], text[i+1] );
		int advance_x;
	};
	
	void draw_prim_array(Canvas &canvas, vector_glyph &vg, const Colorf &color);

	std::map<int, vector_glyph> char_cache;

	FontMetrics metrics;

	FontEngine *font_engine;

	int size_height;

	bool is_filled;

	Texture2D current_texture;
	Rectf current_bounding_rect;
	Rectf current_texture_rect;
/// \}
};

}

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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/glyph_metrics.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/path.h"
#include "API/Display/Render/texture_2d.h"
#include <list>
#include <map>

namespace clan
{

class FreetypeFont;
class FontEngine;
class Brush;

class PathFont_Impl
{
/// \name Construction
/// \{

public:
	PathFont_Impl();
	virtual ~PathFont_Impl();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns information about the current font.
	FontMetrics get_font_metrics();

/// \}
/// \name Operations
/// \{
public:
	void draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Brush &brush);

	GlyphMetrics get_metrics(Canvas &canvas, unsigned int glyph);
	GlyphMetrics measure_text(Canvas &canvas, const std::string &string);

	void load_font(const FontDescription &desc, const std::string &filename, FileSystem fs);

	void set_height(float value) { scaled_height = value / 100.0f; }

/// \}
/// \name Implementation
/// \{
private:
	void store_in_char_cache(unsigned int glyph);

	struct vector_glyph
	{
		Path path;
		GlyphMetrics metrics;
	};

	std::map<unsigned int, vector_glyph> char_cache;

	FontMetrics font_metrics;

	FontEngine *font_engine;

	float scaled_height;	// Relative to 100.0f

/// \}
};

}

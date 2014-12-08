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

#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/glyph_metrics.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/shape2d.h"
#include "API/Display/Render/texture_2d.h"
#include <list>
#include <map>
#include "glyph_cache.h"

namespace clan
{

class FontEngine;

class Font_Impl
{
public:
	Font_Impl();
	~Font_Impl();
	
	void load_font( GraphicContext &context, const FontDescription &desc, const std::string &filename);
	void load_font( GraphicContext &context, const FontDescription &desc, const std::string &filename, FileSystem& fs);
	void load_font( Canvas &canvas, Sprite &sprite, const std::string &glyph_list, int spacelen, bool monospace, const FontMetrics &metrics);
	FontMetrics get_font_metrics();
	int get_character_index(GraphicContext &gc, const std::string &text, const Point &point);

	/// \brief Loads a font from a XML resource definition
	static Font load(Canvas &canvas, const FontDescription &reference_desc, const std::string &id, const XMLResourceDocument &doc, std::function<Resource<Sprite>(Canvas &, const std::string &)> cb_get_sprite);

	GlyphCache glyph_cache;
	FontEngine *font_engine;

private:
	void free_font();

};

}

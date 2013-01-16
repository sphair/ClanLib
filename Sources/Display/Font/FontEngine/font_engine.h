/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#pragma once

#include <memory>

namespace clan
{

class FontMetrics;
class FontPixelBuffer;
class Colorf;
class GlyphOutline;

class FontEngine
{
public:
	virtual ~FontEngine() { }
	virtual FontMetrics get_metrics() = 0;

	virtual FontPixelBuffer get_font_glyph_standard(int glyph, bool anti_alias) = 0;
	virtual FontPixelBuffer get_font_glyph_subpixel(int glyph) = 0;
	virtual std::shared_ptr<GlyphOutline> load_glyph_outline(int glyph_index, int &out_advance_x) = 0;

};

}

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
#include "API/Display/Font/font_face.h"
#include "API/Display/Render/texture_2d.h"
#include <list>
#include <map>
#include "glyph_cache.h"

namespace clan
{

class FontEngine;

class Font_Cache
{
public:
	Font_Cache(std::shared_ptr<FontEngine> &new_engine) : engine(new_engine), glyph_cache(std::make_shared<GlyphCache>()) {}
	std::shared_ptr<FontEngine> engine;
	std::shared_ptr<GlyphCache> glyph_cache;
};

class FontFace_Impl
{
public:
	FontFace_Impl();
	~FontFace_Impl();

	void load_font(const FontDescription &desc, std::string filename, FileSystem fs);
	void load_font(Canvas &canvas, Sprite &sprite, const std::string &glyph_list, int spacelen, bool monospace, const FontMetrics &metrics);

	Font_Cache get_font(const FontDescription &desc);
	Font_Cache get_last_font();

private:
	TextureGroup texture_group;		// Shared texture group between glyph cache's
	std::vector<Font_Cache> font_cache;

};

}
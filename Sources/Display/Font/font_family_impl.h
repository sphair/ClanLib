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
#include "API/Display/Font/font_family.h"
#include "API/Display/Render/texture_2d.h"
#include <list>
#include <map>
#include "glyph_cache.h"
#include "path_cache.h"

namespace clan
{

class FontEngine;

class Font_Selected
{
public:
	// This structure contains the FontDescription options that are selectable in clan::Font
	float height = 20.0f;
	FontWeight weight = FontWeight::normal;
	FontStyle style = FontStyle::normal;
};

class Font_Cache
{
public:
	Font_Cache() {}
	Font_Cache(std::shared_ptr<FontEngine> &new_engine) : engine(new_engine), glyph_cache(std::make_shared<GlyphCache>()), path_cache(std::make_shared<PathCache>()) {}
	std::shared_ptr<FontEngine> engine;
	std::shared_ptr<GlyphCache> glyph_cache;
	std::shared_ptr<PathCache> path_cache;
};

class FontFamily_Impl
{
public:
	FontFamily_Impl(const std::string &family_name);
	~FontFamily_Impl();

	const std::string &get_family_name() const { return family_name; }

	void load_font(const FontDescription &desc, DataBuffer &font_databuffer);
	void load_font(Canvas &canvas, Sprite &sprite, const std::string &glyph_list, int spacelen, bool monospace, const FontMetrics &metrics);

	// Returns null engine if font not found
	Font_Cache get_font(const Font_Selected &desc);

	// Find font and copy it using the revised description
	Font_Cache copy_font(const Font_Selected &desc);

private:
	std::string family_name;
	TextureGroup texture_group;		// Shared texture group between glyph cache's
	std::vector<Font_Cache> font_cache;

};

}

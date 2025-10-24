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
#include "font_cache_native.h"
#include "API/Display/Font/font.h"

CL_FontCache_Native cl_fontcache_native;

CL_FontCache_Native::CL_FontCache_Native()
{
}

CL_FontCache_Native::~CL_FontCache_Native()
{
	free_font_cache();
}

FontGlyphCache *CL_FontCache_Native::get_glyph_cache(const CL_StringRef &unique_name)
{
	std::map<CL_String, FontGlyphCache*>::iterator it_cache = font_cache.find(unique_name);
	if (it_cache != font_cache.end())
	{
		return it_cache->second;
	}
	else
	{
		FontGlyphCache *glyph_cache = new FontGlyphCache;
		font_cache[unique_name] = glyph_cache;
		return glyph_cache;
	}
}

void CL_FontCache_Native::free_font_cache()
{
	std::map<CL_String, FontGlyphCache*>::iterator it_cache;
	for (it_cache = font_cache.begin(); it_cache != font_cache.end(); ++it_cache)
	{
		std::map<FontGlyphRange, CL_FontPixelBuffer **>::iterator it_glyphs;
		for (it_glyphs = it_cache->second->glyphs.begin(); it_glyphs != it_cache->second->glyphs.end(); ++it_glyphs)
		{
			unsigned int range = it_glyphs->first.end - it_glyphs->first.start;
			for (unsigned int index = 0; index < range; index++)
				delete it_glyphs->second[index];
			delete[] it_glyphs->second;
		}
		delete it_cache->second;
	}
	font_cache.clear();
}


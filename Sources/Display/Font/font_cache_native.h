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

#pragma once


#include <map>
#include "API/Display/Font/font.h"

class CL_GraphicContextProvider;
class CL_FontPixelBuffer;
class CL_FontCache_Native;

extern CL_FontCache_Native cl_fontcache_native;

struct FontGlyphRange
{
	FontGlyphRange()
	: start(0), end(0)
	{
	}

	FontGlyphRange(unsigned int start, unsigned int end)
	: start(start), end(end)
	{
	}

	FontGlyphRange(unsigned int c)
	: start(c), end(c)
	{
	}

	FontGlyphRange(const FontGlyphRange &other)
	: start(other.start), end(other.end)
	{
	}

	bool operator <(const FontGlyphRange &other) const
	{
		return other.start < start;
	}

	bool operator ==(const FontGlyphRange &other) const
	{
		return other.start == start && other.end == end;
	}

	unsigned int start;
	unsigned int end;
};

struct FontGlyphCache
{
	std::map<FontGlyphRange, CL_FontPixelBuffer **> glyphs;
};

class CL_FontCache_Native
{
public:
	CL_FontCache_Native();
	~CL_FontCache_Native();

	FontGlyphCache *get_glyph_cache(const CL_StringRef &unique_name);
	void free_font_cache();

private:
	std::map<CL_String, FontGlyphCache*> font_cache;
};




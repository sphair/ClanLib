/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "font_engine.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"

class CL_DataBuffer;

class CL_FontEngine_Win32 : public CL_FontEngine
{
public:
	CL_FontEngine_Win32(const CL_FontDescription &description);
	~CL_FontEngine_Win32();

	CL_FontMetrics get_metrics();
	CL_FontPixelBuffer get_font_glyph(int glyph, bool anti_alias, const CL_Colorf &color);

private:
	CL_FontPixelBuffer get_font_glyph_lcd(int glyph, const CL_Colorf &color);
	CL_FontPixelBuffer get_font_glyph_gray8(int glyph, const CL_Colorf &color);
	CL_FontPixelBuffer get_font_glyph_mono(int glyph, const CL_Colorf &color);
	bool try_load_glyph_bitmap(int glyph, UINT format, MAT2 &matrix, CL_DataBuffer &glyph_bitmap, GLYPHMETRICS &glyph_metrics);
	CL_FontPixelBuffer get_empty_font_glyph(int glyph);

	HFONT handle;
	TEXTMETRIC metrics;
};

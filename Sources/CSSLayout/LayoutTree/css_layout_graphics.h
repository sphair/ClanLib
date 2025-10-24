/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
*/

#pragma once

#include "API/Core/Math/rect.h"
#include "API/Display/2D/image.h"
#include "API/CSSLayout/css_layout.h"

class CL_CSSBoxProperties;
class CL_Image;
class CL_Font;
class CL_FontMetrics;
class CL_CSSResourceCache;

class CL_CSSLayoutGraphics
{
public:
	CL_CSSLayoutGraphics(CL_GraphicContext &gc, CL_CSSResourceCache *cache, const CL_Rect &viewport, CL_CSSLayout::ClipWrapper *clip_wrapper = 0);
	~CL_CSSLayoutGraphics();

	CL_Image &get_image(const CL_String &url);
	CL_Font get_font(const CL_CSSBoxProperties &properties);
	CL_FontMetrics get_font_metrics(CL_Font &font);

	void fill(const CL_Rect &box, const CL_Colorf &color);
	void dot_vertical(const CL_Rect &box, const CL_Colorf &color);
	void dot_horizontal(const CL_Rect &box, const CL_Colorf &color);
	void dash_vertical(const CL_Rect &box, const CL_Colorf &color);
	void dash_horizontal(const CL_Rect &box, const CL_Colorf &color);
	void draw_image(CL_Image &image, int x, int y);
	void draw_image(CL_Image &image, const CL_Rect &box);
	void draw_image(CL_Image &image, const CL_Rect &box, const CL_Rect &src_box);
	void draw_text(CL_Font &font, int x, int y, const CL_StringRef &text, const CL_Colorf &color);
	CL_Size get_text_size(CL_Font &font, const CL_StringRef &text);

	void push_cliprect(const CL_Rect &box);
	void pop_cliprect();

private:
	CL_GraphicContext gc;
	CL_CSSResourceCache *cache;
	CL_Rect viewport;
	CL_CSSLayout::ClipWrapper *clip_wrapper;
};

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
*/

#pragma once

#include "API/Core/Math/rect.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/canvas.h"
#include "API/CSSLayout/css_layout.h"

namespace clan
{

class CSSBoxProperties;
class Image;
class Font;
class FontMetrics;
class CSSResourceCache;

class CSSLayoutGraphics
{
public:
	CSSLayoutGraphics(Canvas &canvas, CSSResourceCache *cache, const Rect &viewport, CSSLayout::ClipWrapper *clip_wrapper = 0);
	~CSSLayoutGraphics();

	Image &get_image(const std::string &url);
	Font get_font(const CSSBoxProperties &properties);
	FontMetrics get_font_metrics(Font &font);

	void fill(const Rect &box, const Colorf &color);
	void dot_vertical(const Rect &box, const Colorf &color);
	void dot_horizontal(const Rect &box, const Colorf &color);
	void dash_vertical(const Rect &box, const Colorf &color);
	void dash_horizontal(const Rect &box, const Colorf &color);
	void draw_image(Image &image, int x, int y);
	void draw_image(Image &image, const Rect &box);
	void draw_image(Image &image, const Rect &box, const Rect &src_box);
	void draw_text(Font &font, int x, int y, const std::string &text, const Colorf &color);
	Size get_text_size(Font &font, const std::string &text);

	void push_cliprect(const Rect &box);
	void pop_cliprect();

private:
	Canvas canvas;
	CSSResourceCache *cache;
	Rect viewport;
	CSSLayout::ClipWrapper *clip_wrapper;
};

}

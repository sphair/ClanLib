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

#include "CSSLayout/precomp.h"
#include "css_layout_graphics.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSLayoutGraphics::CSSLayoutGraphics(Canvas &canvas, CSSResourceCache *cache, const Rect &viewport, CSSLayout::ClipWrapper *clip_wrapper)
: canvas(canvas), cache(cache), viewport(viewport), clip_wrapper(clip_wrapper)
{
}

CSSLayoutGraphics::~CSSLayoutGraphics()
{
}

Image &CSSLayoutGraphics::get_image(const std::string &url)
{
	return cache->get_image(canvas, url);
}

Font CSSLayoutGraphics::get_font(const CSSBoxProperties &properties)
{
	return cache->get_font(canvas, properties);
}

FontMetrics CSSLayoutGraphics::get_font_metrics(Font &font)
{
	return font.get_font_metrics();
}

void CSSLayoutGraphics::fill(const Rect &box, const Colorf &color)
{
	canvas.fill(box, color);
}

void CSSLayoutGraphics::dot_vertical(const Rect &box, const Colorf &color)
{
}

void CSSLayoutGraphics::dot_horizontal(const Rect &box, const Colorf &color)
{
}

void CSSLayoutGraphics::dash_vertical(const Rect &box, const Colorf &color)
{
}

void CSSLayoutGraphics::dash_horizontal(const Rect &box, const Colorf &color)
{
}

void CSSLayoutGraphics::draw_image(Image &image, int x, int y)
{
	image.draw(canvas, x + viewport.left, y + viewport.top);
}

void CSSLayoutGraphics::draw_image(Image &image, const Rect &box)
{
	Rect box2 = box;
	box2.translate(viewport.left, viewport.top);
	image.draw(canvas, box2);
}

void CSSLayoutGraphics::draw_image(Image &image, const Rect &box, const Rect &src_box)
{
	Rect box2 = box;
	box2.translate(viewport.left, viewport.top);
	image.draw(canvas, src_box, box2);
}

void CSSLayoutGraphics::draw_text(Font &font, int x, int y, const std::string &text, const Colorf &color)
{
	font.draw_text(canvas, x + viewport.left, y + viewport.top, text, color);
}

Size CSSLayoutGraphics::get_text_size(Font &font, const std::string &text)
{
	return font.get_text_size(canvas, text);
}

void CSSLayoutGraphics::push_cliprect(const Rect &box)
{
	Rect box2 = box;
	box2.translate(viewport.left, viewport.top);
	if (clip_wrapper)
		clip_wrapper->push_cliprect(canvas, box2);
	else
		canvas.push_cliprect(box2);
}

void CSSLayoutGraphics::pop_cliprect()
{
	if (clip_wrapper)
		clip_wrapper->pop_cliprect(canvas);
	else
		canvas.pop_cliprect();
}

}

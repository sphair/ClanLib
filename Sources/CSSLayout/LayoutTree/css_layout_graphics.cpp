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

#include "CSSLayout/precomp.h"
#include "css_layout_graphics.h"
#include "../css_resource_cache.h"

CL_CSSLayoutGraphics::CL_CSSLayoutGraphics(CL_GraphicContext &gc, CL_CSSResourceCache *cache, const CL_Rect &viewport, CL_CSSLayout::ClipWrapper *clip_wrapper)
: gc(gc), cache(cache), viewport(viewport), clip_wrapper(clip_wrapper)
{
}

CL_CSSLayoutGraphics::~CL_CSSLayoutGraphics()
{
}

CL_Image &CL_CSSLayoutGraphics::get_image(const CL_String &url)
{
	return cache->get_image(gc, url);
}

CL_Font CL_CSSLayoutGraphics::get_font(const CL_CSSBoxProperties &properties)
{
	return cache->get_font(gc, properties);
}

CL_FontMetrics CL_CSSLayoutGraphics::get_font_metrics(CL_Font &font)
{
	return font.get_font_metrics();
}

void CL_CSSLayoutGraphics::fill(const CL_Rect &box, const CL_Colorf &color)
{
	CL_Draw::fill(gc, box, color);
}

void CL_CSSLayoutGraphics::dot_vertical(const CL_Rect &box, const CL_Colorf &color)
{
}

void CL_CSSLayoutGraphics::dot_horizontal(const CL_Rect &box, const CL_Colorf &color)
{
}

void CL_CSSLayoutGraphics::dash_vertical(const CL_Rect &box, const CL_Colorf &color)
{
}

void CL_CSSLayoutGraphics::dash_horizontal(const CL_Rect &box, const CL_Colorf &color)
{
}

void CL_CSSLayoutGraphics::draw_image(CL_Image &image, int x, int y)
{
	image.draw(gc, x + viewport.left, y + viewport.top);
}

void CL_CSSLayoutGraphics::draw_image(CL_Image &image, const CL_Rect &box)
{
	CL_Rect box2 = box;
	box2.translate(viewport.left, viewport.top);
	image.draw(gc, box2);
}

void CL_CSSLayoutGraphics::draw_image(CL_Image &image, const CL_Rect &box, const CL_Rect &src_box)
{
	CL_Rect box2 = box;
	box2.translate(viewport.left, viewport.top);
	image.draw(gc, src_box, box2);
}

void CL_CSSLayoutGraphics::draw_text(CL_Font &font, int x, int y, const CL_StringRef &text, const CL_Colorf &color)
{
	font.draw_text(gc, x + viewport.left, y + viewport.top, text, color);
}

CL_Size CL_CSSLayoutGraphics::get_text_size(CL_Font &font, const CL_StringRef &text)
{
	return font.get_text_size(gc, text);
}

void CL_CSSLayoutGraphics::push_cliprect(const CL_Rect &box)
{
	CL_Rect box2 = box;
	box2.translate(viewport.left, viewport.top);
	if (clip_wrapper)
		clip_wrapper->push_cliprect(gc, box2);
	else
		gc.push_cliprect(box2);
}

void CL_CSSLayoutGraphics::pop_cliprect()
{
	if (clip_wrapper)
		clip_wrapper->pop_cliprect(gc);
	else
		gc.pop_cliprect();
}

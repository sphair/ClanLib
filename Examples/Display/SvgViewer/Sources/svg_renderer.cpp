/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "precomp.h"
#include "svg_renderer.h"

SvgRenderer::SvgRenderer(clan::Canvas &canvas) : canvas(canvas)
{
}

void SvgRenderer::render(clan::DomElement &svg_element)
{
	// To do: parse viewBox, width and height

	for (auto child = svg_element.get_first_child_element(); !child.is_null(); child = child.get_next_sibling_element())
	{
		visit(child);
	}
}

void SvgRenderer::g(clan::DomElement &e)
{
	clan::Mat4f old_transform = canvas.get_transform();
	canvas.mult_transform(get_transform(e));

	for (auto child = e.get_first_child_element(); !child.is_null(); child = child.get_next_sibling_element())
	{
		visit(child);
	}

	canvas.set_transform(old_transform);
}

void SvgRenderer::line(clan::DomElement &e)
{
	float x0 = e.get_attribute_float("x0");
	float y0 = e.get_attribute_float("y0");
	float x1 = e.get_attribute_float("x1");
	float y1 = e.get_attribute_float("y1");
	render_path(clan::Path::line(x0, y0, x1, y1), e);
}

void SvgRenderer::polyline(clan::DomElement &e)
{
}

void SvgRenderer::rect(clan::DomElement &e)
{
	float x = e.get_attribute_float("x");
	float y = e.get_attribute_float("y");
	float width = e.get_attribute_float("width");
	float height = e.get_attribute_float("height");
	render_path(clan::Path::rect(x, y, width, height), e);
}

void SvgRenderer::circle(clan::DomElement &e)
{
	float cx = e.get_attribute_float("cx");
	float cy = e.get_attribute_float("cy");
	float r = e.get_attribute_float("r");
	render_path(clan::Path::circle(cx, cy, r), e);
}

void SvgRenderer::ellipse(clan::DomElement &e)
{
	float cx = e.get_attribute_float("cx");
	float cy = e.get_attribute_float("cy");
	float rx = e.get_attribute_float("rx");
	float ry = e.get_attribute_float("ry");
	render_path(clan::Path::ellipse(cx, cy, rx, ry), e);
}

void SvgRenderer::polygon(clan::DomElement &e)
{
}

void SvgRenderer::path(clan::DomElement &e)
{
	std::string data = e.get_attribute("d");
}

void SvgRenderer::text(clan::DomElement &e)
{
}

void SvgRenderer::image(clan::DomElement &e)
{
}

void SvgRenderer::render_path(const clan::Path &path, clan::DomElement &e)
{
	SvgPaint paint = get_paint(e);
	if (paint.fill)
		canvas.fill(path, paint.brush);
	if (paint.stroke)
		canvas.stroke(path, paint.pen);
}

clan::Mat4f SvgRenderer::get_transform(clan::DomElement &e)
{
	std::string transform = e.get_attribute("transform");
	return clan::Mat4f::identity();
}

SvgPaint SvgRenderer::get_paint(clan::DomElement &e)
{
	SvgPaint paint;
	return paint;
}

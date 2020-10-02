/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "svg_tree.h"
#include "svg_attribute_reader.h"
#include "svg_transform_scope.h"

SvgTreeBuilder::SvgTreeBuilder(clan::Canvas &canvas) : canvas(canvas), node(std::make_shared<SvgNode>())
{
}

void SvgTreeBuilder::build(clan::DomElement &e)
{
	// To do: parse viewBox, width and height

	for (auto child = e.get_first_child_element(); !child.is_null(); child = child.get_next_sibling_element())
	{
		visit(child);
	}
}

void SvgTreeBuilder::g(clan::DomElement &e)
{
	SvgTreeBuilder group(canvas);
	group.node->transform = SvgTransformScope::parse_transform(e, group.node->transform_active);
	group.build(e);
	
	node->nodes.push_back(group.node);
}

void SvgTreeBuilder::line(clan::DomElement &e)
{
	float x0 = (float)SvgAttributeReader::single_length(e, "x0");
	float y0 = (float)SvgAttributeReader::single_length(e, "y0");
	float x1 = (float)SvgAttributeReader::single_length(e, "x1");
	float y1 = (float)SvgAttributeReader::single_length(e, "y1");
	auto path = clan::Path::line(x0, y0, x1, y1);
	render_path(path, e);
}

void SvgTreeBuilder::polyline(clan::DomElement &e)
{
}

void SvgTreeBuilder::rect(clan::DomElement &e)
{
	float x = (float)SvgAttributeReader::single_length(e, "x");
	float y = (float)SvgAttributeReader::single_length(e, "y");
	float width = (float)SvgAttributeReader::single_length(e, "width");
	float height = (float)SvgAttributeReader::single_length(e, "height");
	if (width != 0.0f && height != 0.0f)
	{
		auto path = clan::Path::rect(x, y, width, height);
		render_path(path, e);
	}
}

void SvgTreeBuilder::circle(clan::DomElement &e)
{
	float cx = (float)SvgAttributeReader::single_length(e, "cx");
	float cy = (float)SvgAttributeReader::single_length(e, "cy");
	float r = (float)SvgAttributeReader::single_length(e, "r");
	if (r != 0.0f)
	{
		auto path = clan::Path::circle(cx, cy, r);
		render_path(path, e);
	}
}

void SvgTreeBuilder::ellipse(clan::DomElement &e)
{
	float cx = (float)SvgAttributeReader::single_length(e, "cx");
	float cy = (float)SvgAttributeReader::single_length(e, "cy");
	float rx = (float)SvgAttributeReader::single_length(e, "rx");
	float ry = (float)SvgAttributeReader::single_length(e, "ry");
	if (rx != 0.0f && ry != 0.0f)
	{
		auto path = clan::Path::ellipse(cx, cy, rx, ry);
		render_path(path, e);
	}
}

void SvgTreeBuilder::polygon(clan::DomElement &e)
{
}

void SvgTreeBuilder::path(clan::DomElement &e)
{
	SvgAttributeReader data(e, "d");

	clan::Path path;

	try
	{
		double last_x = 0.0;
		double last_y = 0.0;
		double last_cp_x = 0.0f;
		double last_cp_y = 0.0f;

		while (!data.is_end())
		{
			char command = data.get_path_command();
			bool absolute = (command >= 'A' && command <= 'Z');
			if (command != 'Z' && command != 'z' && !data.is_number()) data.parse_error("unexpected path data");
			switch (command)
			{
			case 'M': // Move to (abs)
			case 'm': // Move to (rel)
			{
				double mx = data.get_sequence_number();
				double my = data.get_sequence_number();
				if (!absolute)
				{
					mx += last_x;
					my += last_y;
				}
				path.move_to((float)mx, (float)my);
				last_x = mx;
				last_y = my;
				last_cp_x = last_x;
				last_cp_y = last_y;
				while (data.is_sequence_number())
				{
					double lx = data.get_sequence_number();
					double ly = data.get_sequence_number();
					if (!absolute)
					{
						lx += last_x;
						ly += last_y;
					}
					path.line_to((float)lx, (float)ly);
					last_x = lx;
					last_y = ly;
					last_cp_x = last_x;
					last_cp_y = last_y;
				}
				break;
			}
			case 'Z': // Close path
			case 'z':
				path.close();
				break;
			case 'L': // Line to (abs)
			case 'l': // Line to (rel)
				do
				{
					double x = data.get_sequence_number();
					double y = data.get_sequence_number();
					if (!absolute)
					{
						x += last_x;
						y += last_y;
					}
					path.line_to((float)x, (float)y);
					last_x = x;
					last_y = y;
					last_cp_x = last_x;
					last_cp_y = last_y;
				} while (data.is_sequence_number());
				break;
			case 'H': // Horizontal line to (abs)
			case 'h': // Horizontal line to (rel)
				do
				{
					double x = data.get_sequence_number();
					if (!absolute)
						x += last_x;
					path.line_to((float)x, (float)last_y);
					last_x = x;
					last_cp_x = last_x;
				} while (data.is_sequence_number());
				break;
			case 'V': // Vertical line to (abs)
			case 'v': // Vertical line to (rel)
				do
				{
					double y = data.get_number();
					if (!absolute)
						y += last_y;
					path.line_to((float)last_x, (float)y);
					last_y = y;
					last_cp_y = last_y;
				} while (data.is_sequence_number());
				break;
			case 'C': // Cubic curve to (abs)
			case 'c': // Cubic curve to (rel)
				do
				{
					double x1 = data.get_sequence_number();
					double y1 = data.get_sequence_number();
					double x2 = data.get_sequence_number();
					double y2 = data.get_sequence_number();
					double x = data.get_sequence_number();
					double y = data.get_sequence_number();
					if (!absolute)
					{
						x1 += last_x;
						y1 += last_y;
						x2 += last_x;
						y2 += last_y;
						x += last_x;
						y += last_y;
					}
					path.bezier_to(clan::Pointf((float)x1, (float)y1), clan::Pointf((float)x2, (float)y2), clan::Pointf((float)x, (float)y));
					last_x = x;
					last_y = y;
					last_cp_x = x2;
					last_cp_y = y2;
				} while (data.is_sequence_number());
				break;
			case 'S': // Shorthand/smooth cubic curve to (abs)
			case 's': // Shorthand/smooth cubic curve to (rel)
				do
				{
					double x2 = data.get_sequence_number();
					double y2 = data.get_sequence_number();
					double x = data.get_sequence_number();
					double y = data.get_sequence_number();
					if (!absolute)
					{
						x2 += last_x;
						y2 += last_y;
						x += last_x;
						y += last_y;
					}
					double x1 = last_cp_x + x - last_x;
					double y1 = last_cp_y + y - last_y;
					path.bezier_to(clan::Pointf((float)x1, (float)y1), clan::Pointf((float)x2, (float)y2), clan::Pointf((float)x, (float)y));
					last_x = x;
					last_y = y;
					last_cp_x = x2;
					last_cp_y = y2;
				} while (data.is_sequence_number());
				break;
			case 'Q': // Quadratic curve to (abs)
			case 'q': // Quadratic curve to (rel)
				do
				{
					double x1 = data.get_sequence_number();
					double y1 = data.get_sequence_number();
					double x = data.get_sequence_number();
					double y = data.get_sequence_number();
					if (!absolute)
					{
						x1 += last_x;
						y1 += last_y;
						x += last_x;
						y += last_y;
					}
					path.bezier_to(clan::Pointf((float)x1, (float)y1), clan::Pointf((float)x, (float)y));
					last_x = x;
					last_y = y;
					last_cp_x = x1;
					last_cp_y = y1;
				} while (data.is_sequence_number());
				break;
			case 'T': // Shorthand/smooth quadratic curve to (abs)
			case 't': // Shorthand/smooth quadratic curve to (rel)
				do
				{
					double x = data.get_sequence_number();
					double y = data.get_sequence_number();
					if (!absolute)
					{
						x += last_x;
						y += last_y;
					}
					double x1 = last_cp_x + x - last_x;
					double y1 = last_cp_y + y - last_y;
					path.bezier_to(clan::Pointf((float)x1, (float)y1), clan::Pointf((float)x, (float)y));
					last_x = x;
					last_y = y;
					last_cp_x = x1;
					last_cp_y = y1;
				} while (data.is_sequence_number());
				break;
			case 'A': // Elliptical arc (abs)
			case 'a': // Elliptical arc (rel)
				do
				{
					double rx = data.get_sequence_number();
					double ry = data.get_sequence_number();
					double x_axis_rotate = data.get_sequence_number();
					double y_axis_rotate = data.get_sequence_number();
					double large_arc_flag = data.get_sequence_number();
					double sweep_flag = data.get_sequence_number();
					double x = data.get_sequence_number();
					double y = data.get_sequence_number();
					last_x = x;
					last_y = y;
					last_cp_x = last_x;
					last_cp_y = last_y;
				} while (data.is_sequence_number());
				break;
			default:
				break;
			}
		}
	}
	catch (clan::Exception &)
	{
	}

	render_path(path, e);
}

void SvgTreeBuilder::text(clan::DomElement &e)
{
}

void SvgTreeBuilder::image(clan::DomElement &e)
{
}

void SvgTreeBuilder::render_path(clan::Path &path, clan::DomElement &e)
{
	auto path_node = std::make_shared<SvgNode>();
	path_node->transform = SvgTransformScope::parse_transform(e, path_node->transform_active);

	/*
	SvgAttributeReader clip_path(e, "clip-path");
	SvgAttributeReader clip_rule(e, "clip-rule");

	SvgAttributeReader mask(e, "mask");

	SvgAttributeReader marker(e, "marker"); // shorthand
	SvgAttributeReader marker_start(e, "marker-start");
	SvgAttributeReader marker_mid(e, "marker-mid");
	SvgAttributeReader marker_end(e, "marker-end");
	*/

	SvgAttributeReader opacity(e, "opacity");
	SvgAttributeReader visibility(e, "visibility");

	SvgAttributeReader fill(e, "fill", true);
	SvgAttributeReader fill_opacity(e, "fill-opacity");
	SvgAttributeReader fill_rule(e, "fill-rule");

	SvgAttributeReader stroke(e, "stroke");
	SvgAttributeReader stroke_dasharray(e, "stroke-dasharray");
	SvgAttributeReader stroke_dashoffset(e, "stroke-dashoffset");
	SvgAttributeReader stroke_linecap(e, "stroke-linecap");
	SvgAttributeReader stroke_linejoin(e, "stroke-linejoin");
	SvgAttributeReader stroke_miterlimit(e, "stroke-miterlimit");
	SvgAttributeReader stroke_opacity(e, "stroke-opacity");
	SvgAttributeReader stroke_width(e, "stroke-width");

	if (fill_rule.is_keyword("evenodd"))
		path.set_fill_mode(clan::PathFillMode::alternate);
	else
		path.set_fill_mode(clan::PathFillMode::winding);

	if (!fill.is_keyword("none"))
	{
		clan::Brush brush = clan::Brush::solid_rgb8(0, 0, 0);
		if (fill.is_color())
		{
			brush.color = fill.get_color();
		}

		path_node->brush = brush;
		path_node->fill = true;
	}

	if (!stroke.is_keyword("none"))
	{
		double width = 1.0;
		if (stroke_width.is_length())
			width = stroke_width.get_length();

		path_node->pen = clan::Pen(clan::Colorf::white, (float)width);
		path_node->stroke = true;
	}

	if (path_node->stroke || path_node->fill)
		path_node->path = path;

	node->nodes.push_back(path_node);
}

/////////////////////////////////////////////////////////////////////////////

void SvgNode::render(clan::Canvas &canvas)
{
	SvgTransformScope transform(canvas, this->transform, transform_active);

	for (auto &child : nodes)
		child->render(canvas);

	if (stroke && fill)
	{
		path.stroke(canvas, pen);
		path.fill(canvas, brush);
	}
	else if (stroke)
	{
		path.stroke(canvas, pen);
	}
	else if (fill)
	{
		path.fill(canvas, brush);
	}
}

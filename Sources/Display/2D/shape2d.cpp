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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/2D/shape2d.h"
#include "shape2d_impl.h"
#include "API/Core/Math/ear_clip_result.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font_vector.h"
#include "API/Core/Math/bezier_curve.h"
#include "API/Core/Math/line_segment.h"
#include <vector>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Shape2D Construction:

Shape2D::Shape2D() : impl(new Shape2D_Impl())
{
}

Shape2D::~Shape2D()
{
}

/////////////////////////////////////////////////////////////////////////////
// Shape2D Attributes:


/////////////////////////////////////////////////////////////////////////////
// Shape2D Operations:

void Shape2D::add_path(Path2D &path)
{
	impl->add_path(path);
}

void Shape2D::get_triangles(std::vector<Vec2f> &out_primitives_array, PolygonOrientation orientation) const
{
	impl->get_triangles(out_primitives_array, orientation);
}

void Shape2D::get_outline(std::vector< std::vector<Vec2f> > &out_primitives_array_outline) const
{
	impl->get_outline(out_primitives_array_outline);
}

void Shape2D::add_circle(float center_x, float center_y, float radius, bool reverse)
{
	add_ellipse(Pointf(center_x, center_y), Pointf(radius, radius), reverse);

}

void Shape2D::add_ellipse(float center_x, float center_y, float radius_x, float radius_y, bool reverse)
{
	add_ellipse(Pointf(center_x, center_y), Pointf(radius_x, radius_y), reverse);

}

void Shape2D::add_circle(const Pointf &center, float radius, bool reverse)
{
	add_ellipse(center, Pointf(radius, radius), reverse);
}

void Shape2D::add_ellipse(const Pointf &center, const Pointf &radius, bool reverse)
{
	float offset_x = 0;
	float offset_y = 0;

	int max_radius = max(radius.x, radius.y);

	int rotationcount = max(5, (max_radius - 3));
	float halfpi = 1.5707963267948966192313216916398f;
	float turn = halfpi / rotationcount;

	offset_x = center.x;
	offset_y = -center.y;

	Path2D path;

	rotationcount *= 4;

	std::vector<Pointf> points;
	points.resize(rotationcount);

	for(int i = 0; i < rotationcount ; i++)
	{
		float pos1 = radius.x * cos(i * turn);
		float pos2 = radius.y * sin(i * turn);

		points[i].x = (center.x + pos1);
		points[i].y = (center.y + pos2);
	}

	path.add_line_to(points);

	if (reverse)
		path.reverse();

	add_path(path);
}

void Shape2D::add_rounded_rect(const Pointf &origin, const Sizef &size, float cap_rounding, bool reverse)
{
	add_rounded_rect(origin, size, cap_rounding, Angle(), reverse);
}

void Shape2D::add_rounded_line(const Pointf &start, const Pointf &end, float line_width, float cap_rounding, bool reverse)
{
	float distance = start.distance(end);
	Sizef size(distance, line_width);

	Angle angle(start.angle_line(end));

	Pointf center(start.x + size.width / 2.0f,  start.y + size.height / 2.0f );
	Vec2f rotated = start;
	rotated.rotate(center, angle);

	Vec2f origin( start.x + (start.x - rotated.x), start.y + (start.y - rotated.y));
	rotated = Vec2f(0.0f, line_width/2.0f);
	rotated.rotate(Vec2f(), angle);
	origin.x -= rotated.x;
	origin.y -= rotated.y;

	add_rounded_rect(origin, size, cap_rounding, angle, reverse);
}

void Shape2D::add_rounded_rect(const Pointf &origin, const Sizef &size, float cap_rounding, const Angle &angle, bool reverse)
{
	// sanitize rounding values
	float tmp_rounding = cap_rounding;
	float min_rounding = min(size.width/2.0f, size.height/2.0f);
	if( tmp_rounding >= (min_rounding-0.01f) ) // 0.01: hysterezis for floating point errors
	{
		tmp_rounding = min_rounding-0.01f; // avoid duplicating curve endpoints 
	}

	Path2D path;
	Pointf center(origin.x + size.width / 2.0f,  origin.y + size.height / 2.0f);

	// top right curve
	Shape2D_Impl::add_rotated_curve(path, center, angle,
		Pointf(origin.x + size.width-tmp_rounding, origin.y),
		Pointf(origin.x + size.width, origin.y),
		Pointf(origin.x + size.width, origin.y + tmp_rounding));

	// bottom right curve
	Shape2D_Impl::add_rotated_curve(path, center, angle, 
		Pointf(origin.x + size.width, origin.y + size.height-tmp_rounding),
		Pointf(origin.x + size.width, origin.y + size.height),
		Pointf(origin.x + size.width-tmp_rounding, origin.y + size.height));
	
	// bottom left curve
	Shape2D_Impl::add_rotated_curve(path, center, angle, 
		Pointf(origin.x + tmp_rounding, origin.y + size.height),
		Pointf(origin.x, origin.y + size.height),
		Pointf(origin.x, origin.y + size.height-tmp_rounding));

	// top left curve
	Shape2D_Impl::add_rotated_curve(path, center, angle, 
		Pointf(origin.x, origin.y +tmp_rounding),
		Pointf(origin.x, origin.y),
		Pointf(origin.x + tmp_rounding, origin.y));
	
	if (reverse)
		path.reverse();

	add_path(path);
}

void Shape2D::add_rect(const Rectf &rect, bool reverse)
{
	add_rect(rect, Angle(), reverse);
}

void Shape2D::add_rect(const Rectf &rect, const Angle &angle, bool reverse)
{
	Path2D path;

	Pointf point_1(rect.left, rect.top);
	Pointf point_2(rect.right, rect.top);
	Pointf point_3(rect.right, rect.bottom);
	Pointf point_4(rect.left, rect.bottom);

	if (angle.to_radians() != 0.0f)
	{
		Pointf center = rect.get_center();
		point_1.rotate(center, angle);
		point_2.rotate(center, angle);
		point_3.rotate(center, angle);
		point_4.rotate(center, angle);
	}

	path.add_line_to(point_1);
	path.add_line_to(point_2);
	path.add_line_to(point_3);
	path.add_line_to(point_4);

	if (reverse)
		path.reverse();

	add_path(path);
}


/////////////////////////////////////////////////////////////////////////////
// Shape2D Implementation:

}

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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/Math/line_math.h"
#include "API/Core/Math/line_segment.h"
#include "API/Core/Math/bezier_curve.h"
#include "API/Display/2D/path.h"
#include "path_impl.h"
#include <cfloat>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Path_Impl Construction:

Path_Impl::Path_Impl()
{
}

Path_Impl::~Path_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// Path_Impl Attributes:

bool Path_Impl::is_hole(PolygonOrientation polygon_orientation)
{
	PolygonOrientation orientation = get_orientation();

	return orientation == polygon_orientation;
}

bool Path_Impl::is_inside_contour(const Path &other) const
{
	if( other.is_point_inside( contour_points[0] ) ) // if one point is inside then all other have to be also.
		return true;

	return false;
}

bool Path_Impl::is_point_inside(const Pointf &P) const
{
	LineSegment2f lineX( Pointf(P.x, P.y + FLT_EPSILON), Pointf(P.x + 100000.0f, P.y + FLT_EPSILON) );

	int num_intersections = 0;

	for( unsigned int i=0; i < contour_points.size(); ++i )
	{
		unsigned int size = contour_points.size();

		LineSegment2f line2(contour_points[ i % size], contour_points[(i+1) % size]);

		if( lineX.intersects(line2, false) )
		{
			num_intersections++;
		}
	}

	if( num_intersections % 2 )
		return true;
	
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// Path_Impl Operations:

void Path_Impl::add_curve(BezierCurve &curve)
{
	std::vector<Pointf> points = curve.generate_curve_points(Angle::from_degrees(10));
	
	for( unsigned int i = 0; i<points.size() - 1; i++ )
	{
		contour_points.push_back( points[i] );
	}
}

void Path_Impl::add_line_to(const Pointf &point )
{
	contour_points.push_back(point);
}

/////////////////////////////////////////////////////////////////////////////
// Path_Impl Implementation:

PolygonOrientation Path_Impl::get_orientation()
{
	float sum = 0;

	unsigned int size = contour_points.size();

	for( unsigned int i = 0; i < size; i++ )
	{
		const Pointf &p1 = contour_points[ i ];
		const Pointf &p2 = contour_points[ (i+1) % size ];

		sum += (p1.x*p2.y - p2.x*p1.y);
	}

	if( sum > 0.0 )
		return cl_counter_clockwise;
	return cl_clockwise;
}

}

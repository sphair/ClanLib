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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "bezier_curve_impl.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Math/angle.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BezierCurve_Impl Construction:

CL_BezierCurve_Impl::CL_BezierCurve_Impl()
{
}

CL_BezierCurve_Impl::~CL_BezierCurve_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BezierCurve_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BezierCurve_Impl Operations:

std::vector<CL_Pointf> CL_BezierCurve_Impl::subdivide_bezier(float t_start, float t_end ) const
{
	std::vector<CL_Pointf> points;

	float t_center = (t_start+t_end)/2.0f;

	CL_Pointf sp = get_point_relative(t_start);
	CL_Pointf ep = get_point_relative(t_end);
	CL_Pointf cp = get_point_relative(t_center);

	CL_Vec4f sp2cp( (cp.x-sp.x), (cp.y-sp.y) );
	CL_Vec4f cp2ep( (ep.x-cp.x), (ep.y-cp.y) );

	if (sp2cp.angle3(cp2ep).to_radians() > split_angle_rad)
	{
		points = subdivide_bezier(t_start, t_center);
		std::vector<CL_Pointf> subpoints = subdivide_bezier(t_center, t_end);
		points.insert(points.end(), subpoints.begin(), subpoints.end());
	}
	else
	{
		points.push_back(cp);
		points.push_back(ep);
	}

	return points;
}

CL_Pointf CL_BezierCurve_Impl::get_point_relative(float pos) const
{
	// Perform deCasteljau iterations:
	// (linear interpolate between the control points)
	std::vector<CL_Pointf>::size_type j, N, i;
	float a = pos;
	float b = 1.0f - pos;
	P = control_points;
	N = control_points.size();
	if (N == 0)
		return CL_Pointf();
	for (j = N-1; j > 0; j--)
	{
		for (i = 0; i < j; i++)
		{
			P[i].x = b*P[i].x + a*P[i+1].x;
			P[i].y = b*P[i].y + a*P[i+1].y;
		}
	}
	return P[0];
}

std::vector<CL_Pointf> CL_BezierCurve_Impl::generate_curve_points(const CL_Angle &split_angle)
{
	std::vector<CL_Pointf> points;
/*
	for (float i = 0.0; i < 1.0; i += 0.01)
	{
		points.push_back(get_point_relative(i));
	}
	points.push_back(get_point_relative(1.0));
*/

	split_angle_rad = split_angle.to_radians(); 

	points.push_back( get_point_relative(0.0) );

	std::vector<CL_Pointf> sub_points = subdivide_bezier(0.0, 0.5);
	points.insert( points.end(), sub_points.begin(), sub_points.end() );

	sub_points = subdivide_bezier(0.5, 1.0);
	points.insert( points.end(), sub_points.begin(), sub_points.end() );

	return points;
}


/////////////////////////////////////////////////////////////////////////////
// CL_BezierCurve_Impl Implementation:

/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Emanuel Greisen
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Display/Collision/outline_math.h"
#include "API/Display/Collision/outline_circle.h"
#include "API/Core/Math/line_math.h"
#include "API/Core/Math/pointset_math.h"
#include "API/Core/Math/point.h"


//    Variant of minimum enclosing disc routines, now with radius cap.

void CL_OutlineMath::minimum_enclosing_sub_circle(
	CL_OutlineCircle &smalldisc,
	const std::vector<CL_Pointf> &points,
	float maxradius)
{
	int real_i_indx = smalldisc.end % points.size();
	// Get first disc (between the first two points)
	smalldisc.position = CL_LineMath::midpoint(points[smalldisc.start], points[real_i_indx]);
	smalldisc.radius   = points[smalldisc.start].distance(points[real_i_indx]) / 2.0f;
	while(smalldisc.end < points.size())
	{
		// Add next one
		smalldisc.end++;
		real_i_indx = smalldisc.end % points.size();
		// only enlargen the circle if points[i] is not already contained
		if(smalldisc.position.distance(points[real_i_indx]) > smalldisc.radius)
		{
			// Break if we have already exceeded the radius
			if(smalldisc.radius > maxradius)
			{
				smalldisc.end--;
				return;
			}
			CL_OutlineMath::minimum_enclosing_sub_circle_with_1point(smalldisc, points);
		}
	}
}


void CL_OutlineMath::minimum_enclosing_sub_circle_with_1point(
	CL_OutlineCircle &smalldisc,
	const std::vector<CL_Pointf> &points)
{
	int real_i_indx = smalldisc.end % points.size();
	// Get first disc (between the first point and `points[i]`)
	smalldisc.position = CL_LineMath::midpoint(points[smalldisc.start], points[real_i_indx]);
	smalldisc.radius   = points[smalldisc.start].distance(points[real_i_indx]) / 2.0f;
	//for(int j = smalldisc.start; j < smalldisc.end; j++)
	for(unsigned int j = smalldisc.start+1; j < smalldisc.end; j++)
	{
		int real_j_indx = j % points.size();
		// only enlargen the circle if points[j] is not already contained
		if(smalldisc.position.distance(points[real_j_indx]) > smalldisc.radius)
		{
			CL_OutlineMath::minimum_enclosing_sub_circle_with_2points(smalldisc, points, j);
		}
	}
}


void CL_OutlineMath::minimum_enclosing_sub_circle_with_2points(
	CL_OutlineCircle &smalldisc,
	const std::vector<CL_Pointf> &points,
	unsigned int j)
{
	int real_i_indx = smalldisc.end % points.size();
	int real_j_indx = j % points.size();

	// Get first disc (between `points[j]` and `points[i]`)
	smalldisc.position = CL_LineMath::midpoint(points[real_j_indx], points[real_i_indx]);
	smalldisc.radius   = points[real_j_indx].distance(points[real_i_indx]) / 2.0f;
	for(unsigned int k = smalldisc.start; k < j; k++)
	{
		if(k == smalldisc.end || k == j)
			continue;
		int real_k_indx = k % points.size();
		// only enlargen the circle if points[k] is not already contained
		if(smalldisc.position.distance(points[real_k_indx]) > smalldisc.radius)
		{
			CL_Circlef tmp_disc;
			tmp_disc.position = smalldisc.position;
			tmp_disc.radius = smalldisc.radius;
			CL_PointSetMath::minimum_disc_with_3points(tmp_disc, points, real_i_indx, j, real_k_indx);
			smalldisc.position = tmp_disc.position;
			smalldisc.radius = float(tmp_disc.radius);
		}
	}
}




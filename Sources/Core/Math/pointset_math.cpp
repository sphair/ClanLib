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
**    Emanuel Greisen
*/

#include "Core/precomp.h"
#include "API/Core/Math/pointset_math.h"
#include "API/Core/Math/circle.h"
#include "API/Core/Math/line_math.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/line.h"
#include <climits>

namespace clan
{
	Circlef PointSetMath::minimum_enclosing_disc(const std::vector<Pointf> &points)
	{
		Circlef smalldisc;

		if (points.size() == 0)
		{
			// ERROR !!!!
			smalldisc.position = Pointf(0.0, 0.0);
			smalldisc.radius = 0.0;
		}
		else if (points.size() == 1)
		{
			smalldisc.position = points[0];
			smalldisc.radius = 0.0;
		}
		else
		{
			int start = 0;
			int end = points.size() - 1;
			//TODO: random permutation of the vector...

			// Calculate the disc
			calculate_minimum_enclosing_disc(smalldisc, points, start, end);
		}

		return smalldisc;
	}

	std::vector<Pointf> PointSetMath::convex_hull_from_polygon(std::vector<Pointf> &points)
	{
		/**
		* OPTIMIZE: we could make it only work in the "points"-vector, instead of returning
		*           the resulting convex hull. That would save memory, and such.
		*/

		// First we find the point with the lowest x-value (left most point, must be on the convex hull)
		unsigned int leftpoint = 0;
		unsigned int rightpoint = 0;
		unsigned int i;
		for (i = 1; i < points.size(); i++)
		{
			if ((points[leftpoint].x == points[i].x && points[i].y > points[leftpoint].y)
				|| (points[i].x < points[leftpoint].x))
			{
				leftpoint = i;
			}
			if ((points[rightpoint].x == points[i].x && points[i].y < points[rightpoint].y)
				|| (points[i].x > points[rightpoint].x))
			{
				rightpoint = i;
			}
		}

		// init our convex hull
		std::vector<Pointf> hull;
		hull.clear();
		hull.push_back(points[leftpoint]);

		// Keep track of the right end-point
		Pointf rightp(points[rightpoint]);
		Pointf leftp(points[leftpoint]);

		// Now we start at the left point, and walk down to generate
		// the lower half of the convex hull
		i = (leftpoint + 1) % points.size();
		for (; i != rightpoint; i = (i + 1) % points.size())
		{
			// Only insert the point if it is on the convex hull
			if (LineMath::point_right_of_line(hull.back(), points[i], rightp) < 0.0)
			{
				hull.push_back(points[i]);

				// remove any left-turns behind us
				while (hull.size() > 2 &&
					LineMath::point_right_of_line(hull[hull.size() - 3], hull[hull.size() - 2], hull[hull.size() - 1]) >= 0.0)
				{
					// Erase the second backmost point
					hull[hull.size() - 2] = hull[hull.size() - 1];
					hull.pop_back();
				}
			}
		}

		// Add the right-point (it's on the convex hull for sure)
		hull.push_back(points[rightpoint]);

		// Now we start at the right point, and walk up to generate
		// the upper half of the convex hull
		i = (rightpoint + 1) % points.size();
		for (; i != leftpoint; i = (i + 1) % points.size())
		{
			// Only insert the point if it is on the convex hull
			if (LineMath::point_right_of_line(hull.back(), points[i], leftp) < 0.0)
			{
				hull.push_back(points[i]);

				// remove any left-turns behind us
				while (hull.size() > 2 &&
					LineMath::point_right_of_line(hull[hull.size() - 3], hull[hull.size() - 2], hull[hull.size() - 1]) >= 0.0)
				{
					// Erase the second backmost point
					hull[hull.size() - 2] = hull[hull.size() - 1];
					hull.pop_back();
				}
			}
		}

		return hull;
	}

	void PointSetMath::calculate_minimum_enclosing_disc(
		Circlef &smalldisc,
		const std::vector<Pointf> &points,
		int start, int end)
	{
		// Get first disc (between the first two points)
		smalldisc.position = LineMath::midpoint(points[start], points[start + 1]);
		smalldisc.radius = points[start].distance(points[start + 1]) / 2.0f;

		// Now start the loop
		for (int i = start + 2; i <= end; i++)
		{
			// only enlargen the circle if points[i] is not already contained
			if (smalldisc.position.distance(points[i]) > smalldisc.radius)
			{
				minimum_disc_with_1point(smalldisc, points, start, i);
			}
		}
	}

	void PointSetMath::minimum_disc_with_1point(
		Circlef &smalldisc,
		const std::vector<Pointf> &points,
		int start,
		unsigned int i)
	{
		// Get first disc (between the first point and `points[i]`)
		smalldisc.position = LineMath::midpoint(points[start], points[i]);
		smalldisc.radius = points[start].distance(points[i]) / 2.0f;

		for (unsigned int j = start + 1; j < i; j++)
		{
			// only enlargen the circle if points[i] is not already contained
			if (smalldisc.position.distance(points[j]) > smalldisc.radius)
			{
				minimum_disc_with_2points(smalldisc, points, start, i, j);
			}
		}
	}

	void PointSetMath::minimum_disc_with_2points(
		Circlef &smalldisc,
		const std::vector<Pointf> &points,
		int start,
		unsigned int i,
		unsigned int j)
	{
		// Get first disc (between `points[i]` and `points[j]`)
		smalldisc.position = LineMath::midpoint(points[i], points[j]);
		smalldisc.radius = points[i].distance(points[j]) / 2.0f;

		for (unsigned int k = start; k < j; k++)
		{
			if (k == i || k == j)
				continue;

			// only enlargen the circle if points[i] is not already contained
			if (smalldisc.position.distance(points[k]) > smalldisc.radius)
			{
				minimum_disc_with_3points(smalldisc, points, i, j, k);
			}
		}
	}

	void PointSetMath::minimum_disc_with_3points(
		Circlef &smalldisc,
		const std::vector<Pointf> &points,
		unsigned int i,
		unsigned int j,
		unsigned int k)
	{
		// There is only one circle with all three points on its boundary.

		// Find center:
		// http://astronomy.swin.edu.au/~pbourke/geometry/circlefrom3/
		// 

		Pointf ji_mid = LineMath::midpoint(points[i], points[j]);
		Pointf ji_norm = ji_mid + Pointf(points[i].y - ji_mid.y, -(points[i].x - ji_mid.x));
		Pointf ki_mid = LineMath::midpoint(points[k], points[i]);
		Pointf ki_norm = ki_mid + Pointf(points[k].y - ki_mid.y, -(points[k].x - ki_mid.x));

		Line2f line_a(ji_mid, ji_norm);
		Line2f line_b(ki_mid, ki_norm);

		bool did_intersect;
		smalldisc.position = line_a.get_intersection(line_b, did_intersect);

		// Since (i,j,k) are all on the circle, just get distance to one of them
		smalldisc.radius = smalldisc.position.distance(points[i]);
	}

	Rect PointSetMath::bounding_box(const std::vector<Pointf> &points)
	{
		Rect R;
		R.left = INT_MAX;
		R.top = INT_MAX;
		R.right = INT_MIN;
		R.bottom = INT_MIN;

		std::vector<Pointf>::const_iterator it;
		for (it = points.begin(); it != points.end(); ++it)
		{
			if ((*it).x < R.left)
				R.left = (*it).x;
			else if ((*it).x > R.right)
				R.right = (*it).x;
			if ((*it).y < R.top)
				R.top = (*it).y;
			else if ((*it).y > R.bottom)
				R.bottom = (*it).y;
		}

		return R;
	}

	// Descending date sorting function
	struct PointAngleSorter
	{
		Pointf basepoint;
		PointAngleSorter(const Pointf &p) : basepoint(p){};

		bool operator()(const Pointf &p1, const Pointf &p2)
		{
			return atan2(basepoint.x - p1.x, basepoint.y - p1.y) < atan2(basepoint.x - p2.x, basepoint.y - p2.y);
		}
	};
}


#pragma once

#include "plane_equation.h"
#include "camera.h"

class Frustrum
{
public:
	static Frustrum from_matrix(CL_Mat4f matrix)
	{
		Frustrum frustrum;
		frustrum.planes[0] = PlaneEquation::left_frustum(matrix);
		frustrum.planes[1] = PlaneEquation::right_frustum(matrix);
		frustrum.planes[2] = PlaneEquation::top_frustum(matrix);
		frustrum.planes[3] = PlaneEquation::bottom_frustum(matrix);
		frustrum.planes[4] = PlaneEquation::near_frustum(matrix);
		frustrum.planes[5] = PlaneEquation::far_frustum(matrix);
		for (int i = 0; i < 6; i++)
			frustrum.planes[i].normalize();
		return frustrum;
	}

	static Frustrum from_camera(const Camera &camera, float z_near, float z_far)
	{
		CL_Mat4f proj = camera.get_projection(z_near, z_far);
		CL_Mat4f view = camera.get_modelview();
		CL_Mat4f proj_view = proj.multiply(view);
		return from_matrix(proj_view);
	}

	enum IntersectionResult
	{
		outside_plane,
		inside_plane,
		intersects_plane
	};

	IntersectionResult intersect_sphere(const CL_Vec3f &center, float radius) const
	{
		for (int i = 0; i < 6; i++)
		{
			float distance = planes[i].distance(center);
			if (distance < -radius)
				return outside_plane;
			else if (distance > -radius && distance < radius)
				return intersects_plane;
		}
		return inside_plane;
	}

	IntersectionResult intersect_box(const CL_Vec3f *corners) const
	{
		// test all 8 corners against the 6 sides 
		// if all points are behind 1 specific plane, we are out
		// if we are in with all points, then we are fully in

		int total_in = 0;
		for (int p = 0; p < 6; p++)
		{
			int in_count = 8;
			bool point_in = true;

			for (int i = 0; i < 8; i++)
			{
				if (planes[p].distance(corners[i]) < 0)
				{
					point_in = false;
					in_count--;
				}
			}

			// were all the points outside of plane p?
			if (in_count == 0)
				return outside_plane;

			// check if they were all on the right side of the plane
			if (point_in)
				total_in++;
		}
		// so if total_in is 6, then all are inside the view
		if (total_in == 6)
			return inside_plane;
		// we must be partly in then otherwise
		return intersects_plane;
	}

	PlaneEquation planes[6];
};

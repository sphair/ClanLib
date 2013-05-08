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
*/

#include "Scene3D/precomp.h"
#include "portal_clipping.h"

namespace clan
{

PortalClipping::PortalClipping(const ClippingFrustum &frustum, const Mat4f &world_to_projection)
: world_to_projection(world_to_projection), projection_to_world(Mat4f::inverse(world_to_projection)), frustum(frustum)
{
}

Rectf PortalClipping::intersect(Rectf box, const std::vector<Vec3f> &points) const
{
	std::vector<Vec2f> edges;
	find_edges(points, edges);
	box.overlap(project(edges));
	return box;
}

Rectf PortalClipping::project(const std::vector<Vec2f> &points) const
{
	Rectf b;
	for (size_t i = 0; i < points.size(); i++)
	{
		const Vec2f &pos_normalized = points[i];
		if (i == 0)
		{
			b = Rectf(pos_normalized.x, pos_normalized.y, pos_normalized.x, pos_normalized.y);
		}
		else
		{
			b.left = min(b.left, pos_normalized.x);
			b.top = min(b.top, pos_normalized.y);
			b.right = max(b.right, pos_normalized.x);
			b.bottom = max(b.bottom, pos_normalized.y);
		}
	}
	return b;
}

void PortalClipping::find_edges(const std::vector<Vec3f> &points, std::vector<Vec2f> &out_edges) const
{
	// Reduce convex polygon to triangles by treating it as a triangle fan:
	out_edges.clear();
	for (size_t i = 1; i + 1 < points.size(); i++)
		add_triangle_edges(points, 0, i, i + 1, out_edges);
}

void PortalClipping::add_triangle_edges(const std::vector<Vec3f> &points, int v0, int v1, int v2, std::vector<Vec2f> &out_edges) const
{
	// Convert to clip spaces:
	Vec4f clip_points[4];
	clip_points[0] = world_to_projection * Vec4f(points[v0], 1.0f);
	clip_points[1] = world_to_projection * Vec4f(points[v1], 1.0f);
	clip_points[2] = world_to_projection * Vec4f(points[v2], 1.0f);
	clip_points[3] = clip_points[0]; // Loop list to make it easier to work with

	for (int i = 0; i < 3; i++)
	{
		Vec4f p0 = clip_points[i];
		Vec4f p1 = clip_points[i + 1];

		float clip_distances0[6] =
		{
			p0.w - p0.x,
			p0.x - p0.w,
			p0.w - p0.y,
			p0.y - p0.w,
			p0.w - p0.z,
			p0.z - p0.w
		};

		float clip_distances1[6] =
		{
			p1.w - p1.x,
			p1.x - p1.w,
			p1.w - p1.y,
			p1.y - p1.w,
			p1.w - p1.z,
			p1.z - p1.w
		};

		bool inside = true;
		for (int k = 0; k < 6; k++)
		{
			if (clip_distances0[k] < 0.0f && clip_distances1[k] < 0.0f)
			{
				inside = false;
				break;
			}
			else if (clip_distances0[k] < 0.0f || clip_distances1[k] < 0.0f)
			{
				float t = -clip_distances1[k] / (clip_distances1[k] - clip_distances0[k]);
				Vec4f p2 = mix(p0, p1, t);
				if (clip_distances0[k] < 0.0f)
					p0 = p2;
				if (clip_distances1[k] < 0.0f)
					p1 = p2;
			}
		}

		if (inside)
		{
			out_edges.push_back(Vec2f(p0) / p0.w);
			out_edges.push_back(Vec2f(p1) / p1.w);
		}
	}
}

}


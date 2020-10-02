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

#include "Display/precomp.h"
#include "path_renderer.h"
#include <algorithm>

namespace clan
{
	void PathRenderer::begin(float x, float y)
	{
		start_x = last_x = x;
		start_y = last_y = y;
	}

	void PathRenderer::end(bool close)
	{
	}

	void PathRenderer::quadratic_bezier(float qcp1_x, float qcp1_y, float qcp2_x, float qcp2_y)
	{
		float qcp0_x = last_x;
		float qcp0_y = last_y;

		// Convert to cubic:
		float cp1_x = qcp0_x + 2.0f * (qcp1_x - qcp0_x) / 3.0f;
		float cp1_y = qcp0_y + 2.0f * (qcp1_y - qcp0_y) / 3.0f;
		float cp2_x = qcp1_x + (qcp2_x - qcp1_x) / 3.0f;
		float cp2_y = qcp1_y + (qcp2_y - qcp1_y) / 3.0f;
		float cp3_x = qcp2_x;
		float cp3_y = qcp2_y;
		cubic_bezier(cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y);
	}

	void PathRenderer::cubic_bezier(float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y)
	{
		float cp0_x = last_x;
		float cp0_y = last_y;

		float estimated_length =
			std::sqrt((cp1_x - cp0_x) * (cp1_x - cp0_x) + (cp1_y - cp0_y) * (cp1_y - cp0_y)) +
			std::sqrt((cp1_x - cp0_x) * (cp1_x - cp0_x) + (cp1_y - cp0_y) * (cp1_y - cp0_y)) +
			std::sqrt((cp1_x - cp0_x) * (cp1_x - cp0_x) + (cp1_y - cp0_y) * (cp1_y - cp0_y));

		float min_segs = 10.0f;
		float segs = estimated_length / 5.0f;
		int steps = (int)std::ceil(std::sqrt(segs * segs * 0.3f + min_segs));
		for (int i = 0; i < steps; i++)
		{
			//clan::Pointf sp = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, i / (float)steps);
			clan::Pointf ep = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, (i + 1) / (float)steps);
			line(ep.x, ep.y);
		}

		// http://ciechanowski.me/blog/2014/02/18/drawing-bezier-curves/
		// http://antigrain.com/research/adaptive_bezier/  (best method, unfortunately GPL example code)
	}

	void PathRenderer::subdivide_bezier(int level, float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y, float t0, float t1)
	{
		const float split_angle_cos = 0.99f;

		float tc = (t0 + t1) * 0.5f;

		clan::Pointf sp = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, t0);
		clan::Pointf cp = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, tc);
		clan::Pointf ep = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, t1);

		clan::Vec2f sp2cp(cp.x - sp.x, cp.y - sp.y);
		clan::Vec2f cp2ep(ep.x - cp.x, ep.y - cp.y);

		float dot = clan::Vec2f::dot(sp2cp.normalize(), cp2ep.normalize());
		if (dot < split_angle_cos && level < 15)
		{
			subdivide_bezier(level + 1, cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, t0, tc);
			subdivide_bezier(level + 1, cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, tc, t1);
		}
		else
		{
			line(ep.x, ep.y);
		}
	}

	clan::Pointf PathRenderer::point_on_bezier(float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y, float t)
	{
		const int num_cp = 4;

		float cp_x[4] = { cp0_x, cp1_x, cp2_x, cp3_x };
		float cp_y[4] = { cp0_y, cp1_y, cp2_y, cp3_y };

		// Perform deCasteljau iterations:
		// (linear interpolate between the control points)
		float a = 1.0f - t;
		float b = t;
		for (int j = num_cp - 1; j > 0; j--)
		{
			for (int i = 0; i < j; i++)
			{
				cp_x[i] = a * cp_x[i] + b * cp_x[i + 1];
				cp_y[i] = a * cp_y[i] + b * cp_y[i + 1];
			}
		}

		return clan::Pointf(cp_x[0], cp_y[0]);
	}
}

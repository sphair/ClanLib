
/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/Math/frustum_planes.h"

namespace clan
{
	FrustumPlanes::FrustumPlanes()
	{
	}

	FrustumPlanes::FrustumPlanes(const Mat4f &world_to_projection)
	{
		planes[0] = near_frustum_plane(world_to_projection);
		planes[1] = far_frustum_plane(world_to_projection);
		planes[2] = left_frustum_plane(world_to_projection);
		planes[3] = right_frustum_plane(world_to_projection);
		planes[4] = top_frustum_plane(world_to_projection);
		planes[5] = bottom_frustum_plane(world_to_projection);
	}

	Vec4f FrustumPlanes::left_frustum_plane(const Mat4f &matrix)
	{
		Vec4f plane(
			matrix[3 + 0 * 4] + matrix[0 + 0 * 4],
			matrix[3 + 1 * 4] + matrix[0 + 1 * 4],
			matrix[3 + 2 * 4] + matrix[0 + 2 * 4],
			matrix[3 + 3 * 4] + matrix[0 + 3 * 4]);
		plane /= plane.length3();
		return plane;
	}

	Vec4f FrustumPlanes::right_frustum_plane(const Mat4f &matrix)
	{
		Vec4f plane(
			matrix[3 + 0 * 4] - matrix[0 + 0 * 4],
			matrix[3 + 1 * 4] - matrix[0 + 1 * 4],
			matrix[3 + 2 * 4] - matrix[0 + 2 * 4],
			matrix[3 + 3 * 4] - matrix[0 + 3 * 4]);
		plane /= plane.length3();
		return plane;
	}

	Vec4f FrustumPlanes::top_frustum_plane(const Mat4f &matrix)
	{
		Vec4f plane(
			matrix[3 + 0 * 4] - matrix[1 + 0 * 4],
			matrix[3 + 1 * 4] - matrix[1 + 1 * 4],
			matrix[3 + 2 * 4] - matrix[1 + 2 * 4],
			matrix[3 + 3 * 4] - matrix[1 + 3 * 4]);
		plane /= plane.length3();
		return plane;
	}

	Vec4f FrustumPlanes::bottom_frustum_plane(const Mat4f &matrix)
	{
		Vec4f plane(
			matrix[3 + 0 * 4] + matrix[1 + 0 * 4],
			matrix[3 + 1 * 4] + matrix[1 + 1 * 4],
			matrix[3 + 2 * 4] + matrix[1 + 2 * 4],
			matrix[3 + 3 * 4] + matrix[1 + 3 * 4]);
		plane /= plane.length3();
		return plane;
	}

	Vec4f FrustumPlanes::near_frustum_plane(const Mat4f &matrix)
	{
		Vec4f plane(
			matrix[3 + 0 * 4] + matrix[2 + 0 * 4],
			matrix[3 + 1 * 4] + matrix[2 + 1 * 4],
			matrix[3 + 2 * 4] + matrix[2 + 2 * 4],
			matrix[3 + 3 * 4] + matrix[2 + 3 * 4]);
		plane /= plane.length3();
		return plane;
	}

	Vec4f FrustumPlanes::far_frustum_plane(const Mat4f &matrix)
	{
		Vec4f plane(
			matrix[3 + 0 * 4] - matrix[2 + 0 * 4],
			matrix[3 + 1 * 4] - matrix[2 + 1 * 4],
			matrix[3 + 2 * 4] - matrix[2 + 2 * 4],
			matrix[3 + 3 * 4] - matrix[2 + 3 * 4]);
		plane /= plane.length3();
		return plane;
	}
}

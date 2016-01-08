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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/Math/ear_clip_result.h"
#include "API/Core/Math/ear_clip_triangulator.h"

namespace clan
{
	class EarClipResult_Impl
	{
	public:
		EarClipResult_Impl(int num_triangles)
		{
			if (num_triangles <= 0)
				return;

			triangles.reserve(num_triangles);
		}

		void add_triangle(const EarClipTriangulator_Triangle &tri)
		{
			triangles.push_back(tri);
		}

		std::vector<EarClipTriangulator_Triangle> triangles;
	};

	EarClipResult::EarClipResult(int num_triangles)
		: impl(std::make_shared<EarClipResult_Impl>(num_triangles))
	{
	}

	EarClipResult::~EarClipResult()
	{
	}

	std::vector<EarClipTriangulator_Triangle> &EarClipResult::get_triangles()
	{
		return impl->triangles;
	}

	EarClipTriangulator_Triangle &EarClipResult::get_triangle(int index)
	{
		return impl->triangles[index];
	}
}

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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/Math/triangle_math.h"
#include "API/Core/Math/ear_clip_triangulator.h"
#include "API/Core/Math/ear_clip_result.h"
#include "ear_clip_triangulator_impl.h"
#include <algorithm>

namespace clan
{
	EarClipTriangulator::EarClipTriangulator()
		: impl(std::make_shared<EarClipTriangulator_Impl>())
	{
	}

	EarClipTriangulator::~EarClipTriangulator()
	{
	}

	std::vector<Pointf> EarClipTriangulator::get_vertices()
	{
		return impl->get_vertices();
	}

	int EarClipTriangulator::get_vertice_count()
	{
		return impl->get_vertice_count();
	}

	PolygonOrientation EarClipTriangulator::calculate_polygon_orientation()
	{
		return impl->calculate_polygon_orientation();
	}

	void EarClipTriangulator::add_vertex(float x, float y)
	{
		impl->add_vertex(x, y);
	}

	void EarClipTriangulator::add_vertex(const Pointf &P)
	{
		impl->add_vertex(P.x, P.y);
	}

	void EarClipTriangulator::clear()
	{
		impl->clear();
	}

	void EarClipTriangulator::set_orientation(PolygonOrientation orientation)
	{
		impl->set_orientation(orientation);
	}

	EarClipResult EarClipTriangulator::triangulate()
	{
		return impl->triangulate();
	}

	void EarClipTriangulator::begin_hole()
	{
		impl->begin_hole();
	}

	void EarClipTriangulator::end_hole()
	{
		impl->end_hole();
	}
}

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
*/

#include "Core/precomp.h"
#include "API/Core/Math/outline_triangulator.h"
#include "outline_triangulator_generic.h"

namespace clan
{
	OutlineTriangulator::OutlineTriangulator() : impl(std::make_shared<OutlineTriangulator_Impl>())
	{
	}

	OutlineTriangulator::~OutlineTriangulator()
	{
	}

	void OutlineTriangulator::next_contour()
	{
		if (impl->current_contour.vertices.empty()) return;
		impl->current_polygon.contours.push_back(impl->current_contour);
		impl->current_contour = OutlineTriangulator_Contour();
	}

	void OutlineTriangulator::next_polygon()
	{
		next_contour();
		if (impl->current_polygon.contours.empty()) return;
		impl->polygons.push_back(impl->current_polygon);
		impl->current_polygon = OutlineTriangulator_Polygon();
	}

	void OutlineTriangulator::add_vertex(float x, float y, void *data)
	{
		OutlineTriangulator_Vertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.data = data;
		impl->current_contour.vertices.push_back(vertex);
	}

	void OutlineTriangulator::generate()
	{
		next_polygon();
		if (impl->polygons.empty()) return;
		impl->triangulate();
	}
}

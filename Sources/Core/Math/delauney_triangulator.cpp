/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Core/Math/delauney_triangulator.h"
#include "delauney_triangulator_generic.h"

namespace clan
{
	DelauneyTriangulator::DelauneyTriangulator() : impl(std::make_shared<DelauneyTriangulator_Impl>())
	{
	}

	DelauneyTriangulator::~DelauneyTriangulator()
	{
	}

	const std::vector<DelauneyTriangulator_Vertex> &DelauneyTriangulator::get_vertices() const
	{
		return impl->input_vertices;
	}

	const std::vector<DelauneyTriangulator_Triangle> &DelauneyTriangulator::get_triangles() const
	{
		return impl->triangles;
	}

	void DelauneyTriangulator::add_vertex(float x, float y, void *data)
	{
		DelauneyTriangulator_Vertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.data = data;
		impl->input_vertices.push_back(vertex);
	}

	void DelauneyTriangulator::generate()
	{
		if (impl->input_vertices.empty()) return;
		impl->triangulate();
	}
}

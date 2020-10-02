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

#pragma once

#include <vector>

namespace clan
{
	class LinkedVertice
	{
	public:
		LinkedVertice() : x(0), y(0), is_ear(0), previous(nullptr), next(nullptr)
		{
			return;
		}

		LinkedVertice(float x, float y) : x(x), y(y), is_ear(0), previous(nullptr), next(nullptr)
		{
			return;
		}

		float x, y;
		bool is_ear;
		LinkedVertice *previous;
		LinkedVertice *next;
	};

	class EarClipTriangulator_Impl
	{
	public:
		EarClipTriangulator_Impl();
		virtual ~EarClipTriangulator_Impl();

		std::vector<Pointf> get_vertices();

		int get_vertice_count() { return vertex_count; }

		/// \brief Determine the orientation of the polygon.
		PolygonOrientation calculate_polygon_orientation();

		void add_vertex(float x, float y);
		void set_orientation(PolygonOrientation orientation);

		void clear();

		EarClipResult triangulate();

		void begin_hole();
		void end_hole();

	private:
		bool is_reflex(const LinkedVertice &v);
		bool is_ear(const LinkedVertice &v);
		void create_lists(bool create_ear_list);

		void set_bridge_vertice_offset(
			LinkedVertice *target,
			Pointf split_point,
			float split_point_rel,
			LinkedVertice *segment_start,
			LinkedVertice *segment_end,
			int direction);
		PolygonOrientation orientation;
		std::vector<LinkedVertice *> vertices;
		std::vector<LinkedVertice *> hole;
		std::vector<LinkedVertice *> *target_array;

		std::vector<LinkedVertice *> ear_list;

		int vertex_count;
	};
}

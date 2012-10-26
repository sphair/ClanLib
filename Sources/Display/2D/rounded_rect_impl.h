/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "API/Core/Math/point.h"
#include "API/Core/Math/triangle_math.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/2D/gradient.h"
#include "API/Display/2D/color.h"
#include "API/Display/Render/primitives_array.h"
#include <vector>

namespace clan
{

class Gradient;
class Colorf;
class Canvas;

class RoundedRect_Impl
{
public:
	RoundedRect_Impl();
	~RoundedRect_Impl();

	void draw(Canvas &canvas, const Pointf &position, const Colorf &color, Origin origin);
	void fill(Canvas &canvas, const Pointf &position, const Colorf &color, Origin origin);
	void fill(Canvas &canvas, const Pointf &position, const Gradient &gradient, Origin origin);

	Pointf cp_tl, cp_tr, cp_bl, cp_br;
	Sizef round_tl, round_tr, round_bl, round_br;
	Sizef size;
	float rounding;

	bool outline_needs_update;
	bool triangulation_needs_update;

private:
	void calculate_outline_points();
	void triangulate();

	Colorf point_on_triangle_to_color(
		const Trianglef &triangle,
		const Colorf &c1, const Colorf &c2, const Colorf &c3,
		const Pointf &P );

	Pointf simultaneous_2(
		float a1, float b1, float c1,
		float a2, float b2, float c2 );

	float determinant_2x2(float a, float b, float c, float d);

	std::vector<Pointf> outline;

	std::vector<Vec2f> triangle_positions;
	VertexArrayVector<Vec2f> gpu_positions;
	VertexArrayVector<Vec4f> gpu_colors;
	size_t num_vectors;

	Gradient gradient;
};

}

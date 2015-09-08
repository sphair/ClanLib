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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Math/line_math.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{
	float LineMath::point_right_of_line(float x, float y, float *line)
	{
		const float &Ax = line[0];
		const float &Ay = line[1];
		const float &Bx = line[2];
		const float &By = line[3];

		return (Bx - Ax) * (y - Ay) - (x - Ax) * (By - Ay);
	}

	float LineMath::point_right_of_line(float x, float y, float Ax, float Ay, float Bx, float By)
	{
		return (Bx - Ax) * (y - Ay) - (x - Ax) * (By - Ay);
	}

	float LineMath::point_right_of_line(const Pointf &A, const Pointf &B, const Pointf &P)
	{
		return (B.x - A.x) * (P.y - A.y) - (P.x - A.x) * (B.y - A.y);
	}

	// return the midpoint on the line from A to B
	Pointf LineMath::midpoint(const Pointf &A, const Pointf &B)
	{
		return Pointf((A.x + B.x) / 2.0f, (A.y + B.y) / 2.0f);
	}

	Pointf LineMath::closest_point(const Pointf &P, const Pointf &A, const Pointf &B)
	{
		float u = closest_point_relative(P, A, B);
		return Pointf(A.x + u*(B.x - A.x), A.y + u*(B.y - A.y));
	}

	float LineMath::closest_point_relative(const Pointf &P, const Pointf &A, const Pointf &B)
	{
		if (A == B)
			return 1.0;

		float u = ((P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y)) / (pow2(B.x - A.x) + pow2(B.y - A.y));

		if (u < 0.0) return 0.0;
		if (u > 1.0) return 1.0;

		return u;
	}
}

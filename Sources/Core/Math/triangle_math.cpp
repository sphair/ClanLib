/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include <cmath>
#include "API/Core/Math/triangle_math.h"

/*
	---- from comp.graphics.algorithms FAQ ----

	calculate the barycentric coordinates
	for a triangle {(x1,y1), (x2,y2), (x3,y3)} and some point (x0,y0)

	Then if b1, b2, and b3 are all > 0, (x0,y0) is strictly inside the triangle;

	if bi = 0 and the other two coordinates are positive,
	(x0,y0) lies on the edge opposite (xi,yi);
	
	if bi and bj = 0, (x0,y0) lies on (xk,yk); if bi < 0,
	(x0,y0) lies outside the edge opposite (xi,yi);

	if all three coordinates are negative, something else is wrong.
*/

bool CL_TriangleMath::point_inside_triangle(const CL_Pointf &P, float *vertices, bool on_edge_is_outside)
{
	return point_inside_triangle(P.x, P.y, vertices, on_edge_is_outside);
}

bool CL_TriangleMath::point_inside_triangle( float px, float py, float *vertices, bool on_edge_is_outside)
{
	const float &tx0 = vertices[0];
	const float &ty0 = vertices[1];
	const float &tx1 = vertices[2];
	const float &ty1 = vertices[3];
	const float &tx2 = vertices[4];
	const float &ty2 = vertices[5];

	float b0 =  (tx1 - tx0) * (ty2 - ty0) - (tx2 - tx0) * (ty1 - ty0);
	float b1 = ((tx1 - px)  * (ty2 - py)  - (tx2 - px)  * (ty1 - py)) / b0;
	float b2 = ((tx2 - px)  * (ty0 - py)  - (tx0 - px)  * (ty2 - py)) / b0;
	float b3 = ((tx0 - px)  * (ty1 - py)  - (tx1 - px)  * (ty0 - py)) / b0;
	
	if( (b1 > 0 && b2 > 0 && b3 > 0) ) // strictly inside
	{
		return true;
	}
	
	if( on_edge_is_outside == false ) // on edge?
		if( (b1>=0 && b2>=0 && b3>=0) && (b1+b2+b3) >= 0 )  
			return true;
	
	return false;
}

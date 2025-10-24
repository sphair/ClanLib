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
#include "API/Core/Math/quad.h"

#include <cmath>

#define cl_min(a,b) ((a < b) ? a : b)
#define cl_max(a,b) ((a > b) ? a : b)

int CL_Quad::get_width() const
{
	return cl_max(cl_max(cl_max(x1, x2), x3), x4) 
		- cl_min(cl_min(cl_min(x1, x2), x3), x4);
}

int CL_Quad::get_height() const
{
	return cl_max(cl_max(cl_max(y1, y2), y3), y4) 
		- cl_min(cl_min(cl_min(y1, y2), y3), y4);
}

CL_Rect CL_Quad::get_bounds() const
{ return CL_Rect( 
		cl_min(cl_min(cl_min(x1, x2), x3), x4), 
		cl_min(cl_min(cl_min(y1, y2), y3), y4), 
		cl_max(cl_max(cl_max(x1, x2), x3), x4), 
		cl_max(cl_max(cl_max(y1, y2), y3), y4)); 
}

void CL_Quad::rotate(const CL_Point &hotspot, float angle)
{
	//Find the rotated positions of each corner
	CL_Point p1 = CL_Point(x1, y1).rotate(hotspot, angle);
	CL_Point p2 = CL_Point(x2, y2).rotate(hotspot, angle);
	CL_Point p3 = CL_Point(x3, y3).rotate(hotspot, angle);
	CL_Point p4 = CL_Point(x4, y4).rotate(hotspot, angle);
	
	x1 = p1.x; y1 = p1.y;
	x2 = p2.x; y2 = p2.y;
	x3 = p3.x; y3 = p3.y;
	x4 = p4.x; y4 = p4.y;
}

void CL_Quad::scale(const CL_Point &hotspot, float sx, float sy)
{
	x1 = (int)((x1-hotspot.x)*sx+hotspot.x); y1 = (int)((y1-hotspot.y)*sy+hotspot.y);
	x2 = (int)((x2-hotspot.x)*sx+hotspot.x); y2 = (int)((y2-hotspot.y)*sy+hotspot.y);
	x3 = (int)((x3-hotspot.x)*sx+hotspot.x); y3 = (int)((y3-hotspot.y)*sy+hotspot.y);
	x4 = (int)((x4-hotspot.x)*sx+hotspot.x); y4 = (int)((y4-hotspot.y)*sy+hotspot.y);
}

void CL_Quad::scale(float sx, float sy)
{
	x1 = (int)(x1*sx); y1 = (int)(y1*sy);
	x2 = (int)(x2*sx); y2 = (int)(y2*sy);
	x3 = (int)(x3*sx); y3 = (int)(y3*sy);
	x4 = (int)(x4*sx); y4 = (int)(y4*sy);
}

CL_Point CL_Quad::center() const
{
	CL_Rect r = get_bounds();
	return CL_Point((r.left+r.right)/2, (r.top+r.bottom)/2);
}

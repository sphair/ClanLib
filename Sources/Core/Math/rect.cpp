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
#include "API/Core/Math/rect.h"

#include <cmath>

#define cl_min(a,b) ((a < b) ? a : b)
#define cl_max(a,b) ((a > b) ? a : b)

CL_Rect CL_Rect::get_rot_bounds(const CL_Point &hotspot, float angle) const
{
	//Find the rotated positions of each corner
	CL_Rect retVal(*this);
	CL_Point ul = CL_Point(retVal.left, retVal.top).rotate(hotspot, angle);
	CL_Point ur = CL_Point(retVal.right, retVal.top).rotate(hotspot, angle);
	CL_Point ll = CL_Point(retVal.left, retVal.bottom).rotate(hotspot, angle);
	CL_Point lr = CL_Point(retVal.right, retVal.bottom).rotate(hotspot, angle);
	
	//Use the sidemost corners as the bounds of the new rectangle
	retVal.left = cl_min(cl_min(ul.x, ur.x), cl_min(ll.x, lr.x));
	retVal.right = cl_max(cl_max(ul.x, ur.x), cl_max(ll.x, lr.x));
	retVal.top = cl_min(cl_min(ul.y, ur.y), cl_min(ll.y, lr.y));
	retVal.bottom = cl_max(cl_max(ul.y, ur.y), cl_max(ll.y, lr.y));
	
	return retVal;
}

CL_Rect CL_Rect::get_rot_bounds(CL_Origin origin, int x, int y, float angle) const
{
	return get_rot_bounds(
		CL_Point(left, top) + calc_origin(origin, get_size()) + CL_Point(x, y),
		angle);
}

CL_Rectf CL_Rectf::get_rot_bounds(const CL_Pointf &hotspot, float angle) const
{
	//Find the rotated positions of each corner
	CL_Rectf retVal(*this);
	CL_Pointf ul = CL_Pointf(retVal.left, retVal.top).rotate(hotspot, angle);
	CL_Pointf ur = CL_Pointf(retVal.right, retVal.top).rotate(hotspot, angle);
	CL_Pointf ll = CL_Pointf(retVal.left, retVal.bottom).rotate(hotspot, angle);
	CL_Pointf lr = CL_Pointf(retVal.right, retVal.bottom).rotate(hotspot, angle);
	
	//Use the sidemost corners as the bounds of the new rectangle
	retVal.left = cl_min(cl_min(ul.x, ur.x), cl_min(ll.x, lr.x));
	retVal.right = cl_max(cl_max(ul.x, ur.x), cl_max(ll.x, lr.x));
	retVal.top = cl_min(cl_min(ul.y, ur.y), cl_min(ll.y, lr.y));
	retVal.bottom = cl_max(cl_max(ul.y, ur.y), cl_max(ll.y, lr.y));
	
	return retVal;
}

CL_Rectf CL_Rectf::get_rot_bounds(CL_Origin origin, float x, float y, float angle) const
{
	return get_rot_bounds(
		CL_Pointf(left, top) + calc_origin(origin, get_size()) + CL_Pointf(x, y),
		angle);
}

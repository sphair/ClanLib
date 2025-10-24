/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Kenneth Gangstoe
**    Harry Storbacka
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/Math/rect.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Rectx<Type> Attributes:

template<typename Type>
CL_Rectx<Type> CL_Rectx<Type>::get_rot_bounds(const CL_Vec2<Type> &hotspot, const CL_Angle &angle) const
{
	//Find the rotated positions of each corner
	CL_Rectx<Type> retVal(*this);
	CL_Vec2<Type> ul(retVal.left, retVal.top);
	ul.rotate(hotspot, angle);
	CL_Vec2<Type> ur(retVal.right, retVal.top);
	ur.rotate(hotspot, angle);
	CL_Vec2<Type> ll(retVal.left, retVal.bottom);
	ll.rotate(hotspot, angle);
	CL_Vec2<Type> lr (retVal.right, retVal.bottom);
	lr.rotate(hotspot, angle);
	
	//Use the sidemost corners as the bounds of the new rectangle
	retVal.left = cl_min(cl_min(ul.x, ur.x), cl_min(ll.x, lr.x));
	retVal.right = cl_max(cl_max(ul.x, ur.x), cl_max(ll.x, lr.x));
	retVal.top = cl_min(cl_min(ul.y, ur.y), cl_min(ll.y, lr.y));
	retVal.bottom = cl_max(cl_max(ul.y, ur.y), cl_max(ll.y, lr.y));
	
	return retVal;
}

template<typename Type>
CL_Rectx<Type> CL_Rectx<Type>::get_rot_bounds(CL_Origin origin, Type x, Type y, const CL_Angle &angle) const
{
	return get_rot_bounds(
		CL_Vec2<Type>(left, top) + CL_Vec2<Type>::calc_origin(origin, get_size()) + CL_Vec2<Type>(x, y),
		angle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Rectx<Type> Operations:


/////////////////////////////////////////////////////////////////////////////
// CL_Rectx<Type> Implementation:

// Explicit instantiate the versions we use:

template class CL_Rectx<int>;
template class CL_Rectx<float>;
template class CL_Rectx<double>;



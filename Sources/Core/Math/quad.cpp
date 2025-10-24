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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/Math/quad.h"
#include "API/Core/Math/rect.h"

template<typename Type>
Type CL_Quadx<Type>::get_width() const
{
	return cl_max(cl_max(cl_max(p.x, q.x), r.x), s.x) 
		- cl_min(cl_min(cl_min(p.x, q.x), r.x), s.x);
}

template<typename Type>
Type CL_Quadx<Type>::get_height() const
{
	return cl_max(cl_max(cl_max(p.y, q.y), r.y), s.y) 
		- cl_min(cl_min(cl_min(p.y, q.y), r.y), s.y);
}

template<typename Type>
CL_Rect CL_Quadx<Type>::get_bounds() const
{ return CL_Rect( 
		cl_min(cl_min(cl_min(p.x, q.x), r.x), s.x), 
		cl_min(cl_min(cl_min(p.y, q.y), r.y), s.y), 
		cl_max(cl_max(cl_max(p.x, q.x), r.x), s.x), 
		cl_max(cl_max(cl_max(p.y, q.y), r.y), s.y)); 
}

template<typename Type>
CL_Quadx<Type> &CL_Quadx<Type>::rotate(const CL_Vec2<Type> &hotspot, const CL_Angle &angle)
{
	p.rotate(hotspot, angle);
	q.rotate(hotspot, angle);
	r.rotate(hotspot, angle);
	s.rotate(hotspot, angle);
	return *this;
}

template<typename Type>
CL_Quadx<Type> &CL_Quadx<Type>::scale(const CL_Vec2<Type> &hotspot, float sx, float sy)
{
	p.x = (Type)((p.x-hotspot.x)*sx+hotspot.x); p.y = (Type)((p.y-hotspot.y)*sy+hotspot.y);
	q.x = (Type)((q.x-hotspot.x)*sx+hotspot.x); q.y = (Type)((q.y-hotspot.y)*sy+hotspot.y);
	r.x = (Type)((r.x-hotspot.x)*sx+hotspot.x); r.y = (Type)((r.y-hotspot.y)*sy+hotspot.y);
	s.x = (Type)((s.x-hotspot.x)*sx+hotspot.x); s.y = (Type)((s.y-hotspot.y)*sy+hotspot.y);
	return *this;
}

template<typename Type>
CL_Quadx<Type> &CL_Quadx<Type>::scale(float sx, float sy)
{
	p.x = (Type)(p.x*sx); p.y = (Type)(p.y*sy);
	q.x = (Type)(q.x*sx); q.y = (Type)(q.y*sy);
	r.x = (Type)(r.x*sx); r.y = (Type)(r.y*sy);
	s.x = (Type)(s.x*sx); s.y = (Type)(s.y*sy);
	return *this;
}

template<typename Type>
CL_Vec2<Type> CL_Quadx<Type>::center() const
{
	CL_Rect r = get_bounds();
	return CL_Vec2<Type>((r.left+r.right)/2, (r.top+r.bottom)/2);
}

template<typename Type>
CL_Quadx<Type> &CL_Quadx<Type>::apply_alignment(CL_Origin origin, Type x, Type y)
{
	CL_Vec2<Type> offset = CL_Vec2<Type>::calc_origin(origin, get_size());
	offset.x -= x;
	offset.y -= y;

	p.x += offset.x;
	p.y += offset.y;
	q.x += offset.x;
	q.y += offset.y;
	r.x += offset.x;
	r.y += offset.y;
	s.x += offset.x;
	s.y += offset.y;
	return *this;
}

// Explicit instantiate the versions we use:
template class CL_Quadx<int>;
template class CL_Quadx<float>;
template class CL_Quadx<double>;

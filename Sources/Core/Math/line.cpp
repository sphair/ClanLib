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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Math/line.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Math/rect.h"

template<typename Type>
CL_Vec2<Type> CL_Line2x<Type>::get_intersection( const CL_Line2x<Type> &second, bool &intersect ) const
{
	Type denominator = (q.x - p.x) * (second.q.y - second.p.y) - (second.q.x - second.p.x) * (q.y - p.y );
	if (denominator == ((Type) 0))
	{
		intersect = false;
		return CL_Vec2<Type>();
	}
	intersect = true;

	Type numerator = (second.p.x - p.x) * (q.y - p.y) - (q.x - p.x) * (second.p.y - p.y);

	CL_Vec2<Type> result;
	result.x = (second.p.x) + ( numerator * ( second.q.x - second.p.x ) ) / denominator;
	result.y = (second.p.y) + ( numerator * ( second.q.y - second.p.y ) ) / denominator;
	return result;
}

template<typename Type>
CL_Vec3<Type> CL_Line3x<Type>::get_intersection( const CL_Line3x<Type> &second, bool &intersect, Type range ) const
{
	Type denominator = (q.x - p.x) * (second.q.y - second.p.y) - (second.q.x - second.p.x) * (q.y - p.y );
	if (denominator == ((Type) 0))
	{
		intersect = false;
		return CL_Vec3<Type>();
	}

	Type numerator = (second.p.x - p.x) * (q.y - p.y) - (q.x - p.x) * (second.p.y - p.y);

	CL_Vec3<Type> result;
	result.x = (second.p.x) + ( numerator * ( second.q.x - second.p.x ) ) / denominator;
	result.y = (second.p.y) + ( numerator * ( second.q.y - second.p.y ) ) / denominator;
	result.z = (second.p.z) + ( numerator * ( second.q.z - second.p.z ) ) / denominator;

	// Check for intersection
	Type difference = (q.x - p.x) * (second.p.z - p.z) - (q.z - p.z) * (second.p.x - p.x);
	difference += numerator * ( (second.q.z - second.p.z) * (q.x - p.x) - (q.z - p.z) * (second.q.x - second.p.x) ) / denominator;
	difference /= (q.x - p.x);

	// Ensure positive difference
	if (difference < ((Type) 0))
	{
		difference = -difference;
	}

	if (difference <= range)
	{
		intersect = true;
	}
	else
	{
		intersect = false;
	}
	return result;
}

// Explicit instantiate the versions we use:

template class CL_Line2x<int>;
template class CL_Line2x<float>;
template class CL_Line2x<double>;

template class CL_Line3x<int>;
template class CL_Line3x<float>;
template class CL_Line3x<double>;

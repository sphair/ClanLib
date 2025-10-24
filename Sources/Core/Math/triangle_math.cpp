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
**    Harry Storbacka
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/Math/triangle_math.h"

template<typename Type>
bool CL_Trianglex<Type>::point_inside( const CL_Vec2<Type> &point ) const
{
	Type b0 =  (q.x - p.x) * (r.y - p.y) - (r.x - p.x) * (q.y - p.y);
	Type b1 = ((q.x - point.x)  * (r.y - point.y)  - (r.x - point.x)  * (q.y - point.y)) / b0;
	Type b2 = ((r.x - point.x)  * (p.y - point.y)  - (p.x - point.x)  * (r.y - point.y)) / b0;
	Type b3 = ((p.x - point.x)  * (q.y - point.y)  - (q.x - point.x)  * (p.y - point.y)) / b0;
	
	Type zero = (Type) 0;
	if( (b1>zero && b2>zero && b3>zero) || ((b1>=zero && b2>=zero && b3>=zero) && b1+b2+b3 > zero) )
		return true;
	
	return false;
}

// Explicit instantiate the versions we use:
template class CL_Trianglex<int>;
template class CL_Trianglex<float>;
template class CL_Trianglex<double>;

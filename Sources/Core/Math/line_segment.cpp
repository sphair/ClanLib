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
**    Harry Storbacka
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Math/line_segment.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Math/rect.h"

template<typename Type>
CL_Vec2<Type> CL_LineSegment2x<Type>::normal() const
{
	CL_Vec2<Type> N;
	N.x = -(q.y-p.y);
	N.y = q.x-p.x;

	Type len = sqrt(N.x*N.x + N.y*N.y);
	N.x /= len;
	N.y /= len;

	return N;
}

// For integers
template<>
CL_Vec2i CL_LineSegment2x<int>::normal() const
{
	CL_LineSegment2f line( CL_Vec2f( (float) p.x, (float) p.y), CL_Vec2f( (float) q.x, (float) q.y) );
	CL_Vec2f n = line.normal();
	CL_Vec2i vec((int) (n.x * 256.0f), (int) (n.y * 256.0f));
	return vec;
}

/*	    From comp.graphics.algorithms FAQ	 
	+---------------------------------------+

    Let A,B,C,D be 2-space position vectors.  Then the directed line
    segments AB & CD are given by:

        AB=A+r(B-A), r in [0,1]
        CD=C+s(D-C), s in [0,1]

    If AB & CD intersect, then

        A+r(B-A)=C+s(D-C), or

        Ax+r(Bx-Ax)=Cx+s(Dx-Cx)
        Ay+r(By-Ay)=Cy+s(Dy-Cy)  for some r,s in [0,1]

    Solving the above for r and s yields

            (Ay-Cy)(Dx-Cx)-(Ax-Cx)(Dy-Cy)
        r = -----------------------------  (eqn 1)
            (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

            (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
        s = -----------------------------  (eqn 2)
            (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

    Let P be the position vector of the intersection point, then

        P=A+r(B-A) or

        Px=Ax+r(Bx-Ax)
        Py=Ay+r(By-Ay)

    By examining the values of r & s, you can also determine some
    other limiting conditions:

        If 0<=r<=1 & 0<=s<=1, intersection exists
            r<0 or r>1 or s<0 or s>1 line segments do not intersect

        If the denominator in eqn 1 is zero, AB & CD are parallel
        If the numerator in eqn 1 is also zero, AB & CD are collinear.
*/

template<typename Type>
CL_Vec2<Type> CL_LineSegment2x<Type>::get_intersection( const CL_LineSegment2x<Type> &second, bool &intersect) const
{
	Type denominator = ((q.x-p.x)*(second.q.y-second.p.y)-(q.y-p.y)*(second.q.x-second.p.x));

	if( denominator == ((Type)0) )
	{
		intersect = false;
		return p;
	}
	
	Type r = ((p.y-second.p.y)*(second.q.x-second.p.x)-(p.x-second.p.x)*(second.q.y-second.p.y)) / denominator;
	Type s = ((p.y-second.p.y)*(q.x-p.x)-(p.x-second.p.x)*(q.y-p.y)) / denominator;
	
	// We use the open interval [0;1) or (0;1] depending on the direction of CD
	if(second.p.y < second.q.y)
	{
		if( ! ( (s >= ((Type) 0)  && s < ((Type) 1)) && (r >= ((Type) 0) && r <= ((Type) 1)) ) )
		{
			intersect = false;
			return p;
		}
	}
	else
	{
		if( ! ( (s > ((Type) 0)  && s <= ((Type) 1)) && (r >= ((Type) 0) && r <= ((Type) 1)) ) )
		{
			intersect = false;
			return p;
		}
	}
	
	intersect = true;
	return CL_Vec3<Type>(p.x+r*(q.x-p.x), p.y+r*(q.y-p.y));
}

template<typename Type>
bool CL_LineSegment2x<Type>::intersects( const CL_LineSegment2x<Type> &second,	bool collinear_intersect ) const
{
	Type denominator = ((q.x-p.x)*(second.q.y-second.p.y)-(q.y-p.y)*(second.q.x-second.p.x));
	
	if( denominator == ((Type) 0) ) // parallel
	{
		if( (p.y-second.p.y)*(second.q.x-second.p.x)-(p.x-second.p.x)*(second.q.y-second.p.y) == ((Type) 0) ) // collinear
		{
			if( collinear_intersect )
				return true;
			else
				return false; 
		}
		return false;
	}
	
	Type r = ((p.y-second.p.y)*(second.q.x-second.p.x)-(p.x-second.p.x)*(second.q.y-second.p.y)) / denominator;
	Type s = ((p.y-second.p.y)*(q.x-p.x)-(p.x-second.p.x)*(q.y-p.y)) / denominator;
	
	// We use the open interval [0;1) or (0;1] depending on the direction of CD
	if(second.p.y < second.q.y)
	{
		if( (s >= ((Type) 0)  && s < ((Type) 1)) && (r >= ((Type) 0) && r <= ((Type) 1)) )
			return true;
	}
	else
	{
		if( (s > ((Type) 0)  && s <= ((Type) 1)) && (r >= ((Type) 0) && r <= ((Type) 1)) )
			return true;
	}
	
	return false;
}


// Collinear points are points that all lie on the same line. 
template<typename Type>
bool CL_LineSegment2x<Type>::collinear(const CL_LineSegment2x<Type> &second) const
{
	Type denominator = ((q.x-p.x)*(second.q.y-second.p.y)-(q.y-p.y)*(second.q.x-second.p.x));	
	Type numerator = ((p.y-second.p.y)*(second.q.x-second.p.x)-(p.x-second.p.x)*(second.q.y-second.p.y));

	if( denominator == ((Type)0) && numerator == ((Type)0) )
		return true;

	return false;
}

/* ----- from comp.graphics.algorithms FAQ ------

  Distance to line (Ax,Ay->Bx,By) from point x,y

	L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 )

	    (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
	r = -------------------------------
	                 L^2
*/

template<typename Type>
Type CL_LineSegment2x<Type>::point_distance(const CL_Vec2<Type> &point)
{
	Type L = cl_pow2(q.x-p.x) + cl_pow2(q.y-p.y);
	Type r = ((point.x-p.x)*(q.x-p.x)+(point.y-p.y)*(q.y-p.y)) / L;
	
	if( r <= 0 || r >= 1 )
	{
		return cl_min( point.distance(p), point.distance(q) );
	}
	
	Type s = ((p.y-point.y)*(q.x-p.x)-(p.x-point.x)*(q.y-p.y)) / L;

	s *= sqrt(L);

	if (s < ((Type)0))
		s = -s;

	return s;
}

// For integers
template<>
int CL_LineSegment2x<int>::point_distance(const CL_Vec2i &point)
{
	int L = cl_pow2(q.x-p.x) + cl_pow2(q.y-p.y);
	int r = ((point.x-p.x)*(q.x-p.x)+(point.y-p.y)*(q.y-p.y)) / L;
	
	if( r <= 0 || r >= 1 )
	{
		return cl_min( point.distance(p), point.distance(q) );
	}
	
	int s = ((p.y-point.y)*(q.x-p.x)-(p.x-point.x)*(q.y-p.y)) / L;

	s *= (int) (sqrt((float) L) + 0.5f);

	if (s < 0)
		s = -s;

	return s;
}

template<typename Type>
Type CL_LineSegment3x<Type>::point_distance(const CL_Vec3<Type> &point, CL_Vec3<Type> &dest_intercept) const
{
	CL_Vec3<Type> ap(point.x - p.x, point.y - p.y, point.z - p.z );
	CL_Vec3<Type> ab(q.x - p.x, q.y - p.y, q.z - p.z);

	Type ac_dot_ab = (ap.x * ab.x) + (ap.y * ab.y) + (ap.z * ab.z);
	Type length_ab_sqr = (ab.x * ab.x) + (ab.y * ab.y) + (ab.z * ab.z);

	if (length_ab_sqr == ( (Type) 0 ) )
	{
		// Point is on the line
		dest_intercept = point;
		return (Type) 0;
	}

	Type t = ac_dot_ab / length_ab_sqr;

	// Clamp to segment
	if (t < ((Type) 0)) t = (Type) 0;
	else if (t > ((Type) 1)) t = (Type) 1;

	CL_Vec3<Type> intercept;
	intercept.x = p.x + ab.x * t;
	intercept.y = p.y + ab.y * t;
	intercept.z = p.z + ab.z * t;

	Type length = (point.x - intercept.x) * (point.x - intercept.x);
	length += (point.y - intercept.y) * (point.y - intercept.y);
	length += (point.z - intercept.z) * (point.z - intercept.z);

	dest_intercept = intercept;

	return sqrt(length);
}

// For integers
template<>
int CL_LineSegment3x<int>::point_distance(const CL_Vec3i &point, CL_Vec3i &dest_intercept) const
{
	CL_Vec3i ap(point.x - p.x, point.y - p.y, point.z - p.z );
	CL_Vec3i ab(q.x - p.x, q.y - p.y, q.z - p.z);

	int ac_dot_ab = (ap.x * ab.x) + (ap.y * ab.y) + (ap.z * ab.z);
	int length_ab_sqr = (ab.x * ab.x) + (ab.y * ab.y) + (ab.z * ab.z);

	if (length_ab_sqr == (  0 ) )
	{
		// Point is on the line
		dest_intercept = point;
		return  0;
	}

	int t = ac_dot_ab / length_ab_sqr;

	// Clamp to segment
	if (t < ( 0)) t =  0;
	else if (t > ( 1)) t =  1;

	CL_Vec3i intercept;
	intercept.x = p.x + ab.x * t;
	intercept.y = p.y + ab.y * t;
	intercept.z = p.z + ab.z * t;

	int length = (point.x - intercept.x) * (point.x - intercept.x);
	length += (point.y - intercept.y) * (point.y - intercept.y);
	length += (point.z - intercept.z) * (point.z - intercept.z);

	dest_intercept = intercept;

	return (int) (sqrt( (float) length) + 0.5f);
}


#define CL_LINECLIP_TOP 1
#define CL_LINECLIP_RIGHT 2
#define CL_LINECLIP_BOTTOM 4
#define CL_LINECLIP_LEFT 8

#define CL_CREATE_REGION_CODE(point_x, point_y, region_code) \
	region_code = 0; \
	if (point_y < rect.top) region_code = CL_LINECLIP_TOP; \
	else if (point_y > rect.bottom) region_code = CL_LINECLIP_BOTTOM; \
	if (point_x > rect.right) region_code |= CL_LINECLIP_RIGHT; \
	else if (point_x < rect.left) region_code |= CL_LINECLIP_LEFT;

template<typename Type>
CL_LineSegment2x<Type> &CL_LineSegment2x<Type>::clip(const CL_Rectx<Type> &rect, bool &clipped)
{
	// Implementing the Cohen-Sutherland line clip algorithm

	int region_code_p = 0;
	int region_code_q = 0;

	CL_CREATE_REGION_CODE(p.x, p.y, region_code_p);
	CL_CREATE_REGION_CODE(q.x, q.y, region_code_q);

	while(true)
	{
		if ( ! (region_code_p | region_code_q) )	// All inside
		{
			clipped = true;
			break;
		}
		if ( region_code_p & region_code_q )	// Regions outside the rect
		{
			clipped = false;
			break;
		}

		// Calculate the line segment to clip from an outside point to an intersection with clip edge
		Type x, y;

		// At least one endpoint is outside the clip rectangle; pick it.
		int region_code_out = region_code_p ? region_code_p: region_code_q;

		// Now find the intersection point;
		// use formulas y = y0 + slope * (x - x0), x = x0 + (1/slope)* (y - y0)
		if (region_code_out & CL_LINECLIP_TOP)			// point is above the clip rectangle
		{
			x = p.x + (q.x - p.x) * (rect.top - p.y)/(q.y - p.y);
			y = rect.top;
		}
		else if (region_code_out & CL_LINECLIP_BOTTOM)	// point is below the clip rectangle
		{
			x = p.x + (q.x - p.x) * (rect.bottom - p.y)/(q.y - p.y);
			y = rect.bottom;
		}
		else if (region_code_out & CL_LINECLIP_RIGHT)	// point is to the right of clip rectangle
		{
			y = p.y + (q.y - p.y) * (rect.right - p.x)/(q.x - p.x);
			x = rect.right;
		}
		else /* if (region_code_out & CL_LINECLIP_LEFT) */	// point is to the left of clip rectangle
		{
			y = p.y + (q.y - p.y) * (rect.left - p.x)/(q.x - p.x);
			x = rect.left;
		}

		// Move outside point to intersection point to clip and get ready for next pass.
		if (region_code_out == region_code_p)
		{
			p.x = x;
			p.y = y;
			CL_CREATE_REGION_CODE(x, y, region_code_p);
		}
		else 
		{
			q.x = x;
			q.y = y;
			CL_CREATE_REGION_CODE(x, y, region_code_q);
		}

	}

	return *this;
}

// Explicit instantiate the versions we use:

template class CL_LineSegment2x<int>;
template class CL_LineSegment2x<float>;
template class CL_LineSegment2x<double>;

template class CL_LineSegment3x<int>;
template class CL_LineSegment3x<float>;
template class CL_LineSegment3x<double>;




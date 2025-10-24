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
#include "API/Core/Math/line_math.h"

template<typename T> inline T pow2(T value) { return value*value; }
template<typename T> inline T cl_min(T a, T b) { if(a < b) return a; return b; }
template<typename T> inline T cl_max(T a, T b) { if(a > b) return a; return b; }

/* ----- from comp.graphics.algorithms FAQ ------

  Distance to line (Ax,Ay->Bx,By) from point x,y

	L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 )

	    (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
	r = -------------------------------
	                 L^2
*/

float CL_LineMath::distance_to_line(const CL_Pointf &P, float *line)
{
	return distance_to_line( P.x, P.y, line );
}

float CL_LineMath::distance_to_line(float x, float y, float *line)
{
	const float &Ax = line[0];
	const float &Ay = line[1];
	const float &Bx = line[2];
	const float &By = line[3];

	float L = sqrt( pow2(Bx-Ax) + pow2(By-Ay) );
	float r = ((x-Ax)*(Bx-Ax)+(y-Ay)*(By-Ay))/pow2(L);
	
	if( r <= 0 || r >= 1 )
	{
		CL_Pointf p(x,y);
		CL_Pointf A(Ax,Ay);
		CL_Pointf B(Bx,By);
		
		return cl_min( p.distance(A), p.distance(B) );
	}
	
	float s = ((Ay-y)*(Bx-Ax)-(Ax-x)*(By-Ay)) / pow2(L);
	return fabs(s)*L;
}

// Collinear points are points that all lie on the same line. 

bool CL_LineMath::collinear( float *lineA, float *lineB )
{
	const float &Ax = lineA[0];
	const float &Ay = lineA[1];
	const float &Bx = lineA[2];
	const float &By = lineA[3];

	const float &Cx = lineB[0];
	const float &Cy = lineB[1];
	const float &Dx = lineB[2];
	const float &Dy = lineB[3];
	
	float denominator = ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx));	
	float numerator = ((Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy));

	if( denominator == 0 && numerator == 0 )
		return true;

	return false;
}

float CL_LineMath::point_right_of_line( float x, float y, float *line )
{
	const float &Ax = line[0];
	const float &Ay = line[1];
	const float &Bx = line[2];
	const float &By = line[3];

	return (Bx-Ax) * (y-Ay) - (x-Ax) * (By-Ay);
}

float CL_LineMath::point_right_of_line(float x, float y, float Ax, float Ay, float Bx, float By)
{
	return (Bx-Ax) * (y-Ay) - (x-Ax) * (By-Ay);
}

float CL_LineMath::point_right_of_line( const CL_Pointf &A, const CL_Pointf &B, const CL_Pointf &P )
{
	return (B.x-A.x) * (P.y-A.y) - (P.x-A.x) * (B.y-A.y);
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
*/

CL_Pointf CL_LineMath::get_intersection( float *lineA, float *lineB )
{
	const float &Ax = lineA[0];
	const float &Ay = lineA[1];
	const float &Bx = lineA[2];
	const float &By = lineA[3];

	const float &Cx = lineB[0];
	const float &Cy = lineB[1];
	const float &Dx = lineB[2];
	const float &Dy = lineB[3];

	float denominator = ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx));

	if( denominator == 0 )
		return CL_Pointf(Ax,Ay); 
	
	float r = ((Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy)) / denominator;
	
	CL_Pointf P;
	P.x=Ax+r*(Bx-Ax);
	P.y=Ay+r*(By-Ay);
	
	return P;
}

/*
	---- from comp.graphics.algorithms FAQ ----

    By examining the values of r & s, you can also determine some
    other limiting conditions:

        If 0<=r<=1 & 0<=s<=1, intersection exists
            r<0 or r>1 or s<0 or s>1 line segments do not intersect

        If the denominator in eqn 1 is zero, AB & CD are parallel
        If the numerator in eqn 1 is also zero, AB & CD are collinear.
*/

bool CL_LineMath::intersects( 
	float *lineA,
	float *lineB,
	bool collinear_intersect )
{
	const float &Ax = lineA[0];
	const float &Ay = lineA[1];
	const float &Bx = lineA[2];
	const float &By = lineA[3];

	const float &Cx = lineB[0];
	const float &Cy = lineB[1];
	const float &Dx = lineB[2];
	const float &Dy = lineB[3];

	float denominator = ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx));
	
	if( denominator == 0.0f ) // parallell
	{
		if( (Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy) == 0.0f ) // collinear
		{
			if( collinear_intersect )
				return true;
			else
				return false; 
		}
		return false;
	}
	
	float r = ((Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy)) / denominator;
	float s = ((Ay-Cy)*(Bx-Ax)-(Ax-Cx)*(By-Ay)) / denominator;
	
	// We use the open interval [0;1) or (0;1] depending on the direction of CD
	if(Cy < Dy)
	{
		if( (s >= 0.0f  && s < 1.0f) && (r >= 0.0f && r <= 1.0f) )
			return true;
	}
	else
	{
		if( (s > 0.0f  && s <= 1.0f) && (r >= 0.0f && r <= 1.0f) )
			return true;
	}
	
	return false;
}


// return the midpoint on the line from A to B
CL_Pointf CL_LineMath::midpoint( const CL_Pointf &A, const CL_Pointf &B )
{
	return CL_Pointf( (A.x+B.x)/2.0f, (A.y+B.y)/2.0f );
}

// return the normal vector of the line
CL_Pointf CL_LineMath::normal( const CL_Pointf &A, const CL_Pointf &B )
{
	return CL_LineMath::normal(A.x, A.y, B.x, B.y);
}

CL_Pointf CL_LineMath::normal( float x1, float y1, float x2, float y2 )
{
	CL_Pointf N;
	N.x = -1 * (y2-y1);
	N.y = x2-x1;

	float len = sqrt(N.x*N.x + N.y*N.y);
	N.x /= len;
	N.y /= len;

	return N;
}

CL_Pointf CL_LineMath::normal( float *line )
{
	CL_Pointf N;
	N.x = -1 * (line[3]-line[1]);
	N.y = line[2]-line[0];

	float len = sqrt(N.x*N.x + N.y*N.y);
	N.x /= len;
	N.y /= len;

	return N;
}

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

//! clanCore="Math"
//! header=core.h

#ifndef header_line_math
#define header_line_math

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#include "point.h"

//: Math operations on 2D lines.
//- !group=Core/Math!
//- !header=core.h!
class CL_API_CORE CL_LineMath
{
 public:
// Operations:
	//: Return the distance from a point to a line.
	//param float x: X coordinate of point.
	//param float y: Y coordinate of point.
	//param CL_Pointf point: The point.
	//param float *line: Pointer to float array with elements {x1,y1,x2,y2}.
	static float distance_to_line(const CL_Pointf &point, float *line);
	static float distance_to_line(float x, float y, float *line);

	//: Return true if two line segments intersect.
	//param float *lineA: First line. Pointer to float array with elements {x1,y1,x2,y2}.
	//param float *lineB: Second line. Pointer to float array with elements {x1,y1,x2,y2}.
	//param bool collinear_intersect: set if a collision is reported when all points are on the same line.
	static bool intersects( float *lineA, float *lineB, bool collinear_intersect=false );

	//: Return true if two line segments are collinear. (All points are on the same line.)
	//param float *lineA: First line. Pointer to float array with elements {x1,y1,x2,y2}.
	//param float *lineB: Second line. Pointer to float array with elements {x1,y1,x2,y2}.
	static bool collinear( float *lineA, float *lineB );

	//: Return the intersection point of two lines.
	//param float *lineA: First line. Pointer to float array with elements {x1,y1,x2,y2}.
	//param float *lineB: Second line. Pointer to float array with elements {x1,y1,x2,y2}.
	static CL_Pointf get_intersection( float *lineA, float *lineB );	

	//: Return [<0, 0, >0] if the Point P is right, on or left of the line trough A,B
	//param float x: x coordinate of the point being tested.
	//param float y: y coordinate of the point being tested.
	//param float *line: The line. A pointer to a float array with the elements {x1,y1,x2,y2}.
	//param float line_x1, line_y1: First point defining the line.
	//param float line_x2, line_y2: Second point defining the line.
	//param CL_Pointf P: Test point.
	//param CL_Pointf A: First point defining the line.
	//param CL_Pointf B: Second point defining the line.
	static float point_right_of_line( float x, float y, float *line );

	static float point_right_of_line( float x, float y, float line_x1, float line_y1, float line_x2, float line_y2 );

	static float point_right_of_line( const CL_Pointf &A, const CL_Pointf &B, const CL_Pointf &P );

	//: Return the midpoint of the line from point A to point B.
	//param CL_Pointf A: point A
	//param CL_Pointf B: point B
	static CL_Pointf midpoint( const CL_Pointf &A, const CL_Pointf &B );

	//: Return the normal vector of the line from point A to point B.
	//param CL_Pointf A: point A
	//param CL_Pointf B: point B
	//param float *line: pointer to line vertices {x1, y1, x2, y2}
	static CL_Pointf normal( const CL_Pointf &A, const CL_Pointf &B );

	static CL_Pointf normal( float x1, float y1, float x2, float y2 );

	static CL_Pointf normal( float *line );

	
};

#endif

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

#ifndef header_quad
#define header_quad

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "rect.h"
#include "size.h"
#include "point.h"
#include "origin.h"

//: 2D (x1, y1, x2, y2, x3, y3, x4, y4) quad structure.
//- !group=Core/Math!
//- !header=core.h!
class CL_API_CORE CL_Quad
{
//! Construction:
public:
	//: Constructs a quad.
	//param x1: x-coord for first point of quad.
	//param y1: y-coord for first point of quad.
	//param x2: x-coord for second point of quad.
	//param y2: y-coord for second of quad.
	//param x3: x-coord for third point of quad.
	//param y3: y-coord for third point of quad.
	//param x4: x-coord for forth point of quad.
	//param y4: y-coord for forth point of quad.
	//param rect: Rectangle used to initialize the quad.
	CL_Quad() { return; }

	CL_Quad(int new_x1, int new_y1, int new_x2, int new_y2,
		int new_x3, int new_y3, int new_x4, int new_y4)
	{ x1 = new_x1; y1 = new_y1; x2 = new_x2; y2 = new_y2; 
		x3 = new_x3; y3 = new_y3; x4 = new_x4; y4 = new_y4; 
	}

	CL_Quad(const CL_Rect &rect)
	{ x1 = rect.left; y1 = rect.top; x2 = rect.right; y2 = rect.top; 
		x3 = rect.right; y3 = rect.bottom; x4 = rect.left; y4 = rect.bottom; 
	}

	CL_Quad(const CL_Quad &q)
	{ x1 = q.x1; y1 = q.y1; x2 = q.x2; y2 = q.y2; 
		x3 = q.x3; y3 = q.y3; x4 = q.x4; y4 = q.y4; 
	}

	//: Quad += Quad operator.
	CL_Quad &operator+=(const CL_Quad &q)
	{ x1 += q.x1; y1 += q.y1; x2 += q.x2; y2 += q.y2; 
		x3 += q.x3; y3 += q.y3; x4 += q.x4; y4 += q.y4; 
		return *this;
	}

	//: Quad -= Quad operator.
	CL_Quad &operator-=(const CL_Quad &q)
	{ x1 -= q.x1; y1 -= q.y1; x2 -= q.x2; y2 -= q.y2; 
		x3 -= q.x3; y3 -= q.y3; x4 -= q.x4; y4 -= q.y4; 
		return *this;
	}
	
	//: Quad += Point operator.
	CL_Quad &operator+=(const CL_Point &p)
	{ x1 += p.x; y1 += p.y; x2 += p.x; y2 += p.y; 
		x3 += p.x; y3 += p.y; x4 += p.x; y4 += p.y; 
		return *this;
	}

	//: Quad -= Point operator.
	CL_Quad &operator-=(const CL_Point &p)
	{ x1 -= p.x; y1 -= p.y; x2 -= p.x; y2 -= p.y; 
		x3 -= p.x; y3 -= p.y; x4 -= p.x; y4 -= p.y; 
		return *this;
	}

	//: Quad + Quad operator.
	CL_Quad operator+(const CL_Quad &q) const
	{ return CL_Quad(x1 + q.x1, y1 + q.y1, x2 + q.x2, y2 + q.y2,
		x3 + q.x3, y3 + q.y3, x4 + q.x4, y4 + q.y4); }

	//: Quad - Quad operator.
	CL_Quad operator-(const CL_Quad &q) const
	{ return CL_Quad(x1 - q.x1, y1 - q.y1, x2 - q.x2, y2 - q.y2,
		x3 - q.x3, y3 - q.y3, x4 - q.x4, y4 - q.y4); }

	//: Quad + Point operator.
	CL_Quad operator+(const CL_Point &p) const
	{ return CL_Quad(x1 + p.x, y1 + p.y, x2 + p.x, y2 + p.y,
		x3 + p.x, y3 + p.y, x4 + p.x, y4 + p.y); }

	//: Quad - Point operator.
	CL_Quad operator-(const CL_Point &p) const
	{ return CL_Quad(x1 - p.x, y1 - p.y, x2 - p.x, y2 - p.y,
		x3 - p.x, y3 - p.y, x4 - p.x, y4 - p.y); }

	//: Quad == Quad operator.
	bool operator==(const CL_Quad &q) const
	{ return (x1 == q.x1 && y1 == q.y1 && x2 == q.x2 && y2 == q.y2 &&
		x3 == q.x3 && y3 == q.y3 && x4 == q.x4 && y4 == q.y4); }

	//: Quad != Quad operator.
	bool operator!=(const CL_Quad &q) const
	{ return (x1 != q.x1 || y1 != q.y1 || x2 != q.x2 || y2 != q.y2 ||
		x3 != q.x3 || y3 != q.y3 || x4 != q.x4 || y4 != q.y4); }

//! Attributes:
public:
	//: X1-coordinate.
	int x1;
	
	//: Y1-coordinate.
	int y1;
	
	//: X2-coordinate.
	int x2;
	
	//: Y2-coordinate.
	int y2;
	
	//: X3-coordinate.
	int x3;
	
	//: Y3-coordinate.
	int y3;
	
	//: X4-coordinate.
	int x4;
	
	//: Y4-coordinate.
	int y4;
	
	//: Returns the width of the quad.
	int get_width() const;
	
	//: Returns the height of the quad.
	int get_height() const;

	//: Returns the size of the rectangle.
	CL_Size get_size() const { return CL_Size(get_width(), get_height()); }
	
	//: Returns the bounding box of the quad as a CL_Rect
	//param hotspot: Point to rotate around.
	//param origin: Determines the hotspot point within the rectangle
	//param x, y: Offsets applied negatively to the hotspot point
	//param angle: Angle to rotate in degrees.
	CL_Rect get_bounds() const;
	
//! Operations:
public:
	//: Rotates the Quad
	//param hotspot: Point to rotate around.
	//param angle: Angle to rotate in degrees.
	void rotate(const CL_Point &hotspot, float angle);
	
	//: Scale the Quad
	//param sx: Scale value in x-axis
	//param sy: Scale value in y-axis
	//param hotspot: Point to scale around.
	void scale(float sx, float sy);
	void scale(const CL_Point &hotspot, float sx, float sy);
	
	//: Returns the center point of the quad
	CL_Point center() const;
	
	//: Applies an origin and offset pair to this rectangle
	//param origin: The new origin to adjust to from default upper-left position
	//param x, y: Offsets applied negatively to each corner of the rectangle
	void apply_alignment(CL_Origin origin, int x, int y)
	{
		CL_Point offset = calc_origin(origin, get_size());
		offset.x -= x;
		offset.y -= y;
		
		x1 += offset.x;
		y1 += offset.y;
		x2 += offset.x;
		y2 += offset.y;
		x3 += offset.x;
		y3 += offset.y;
		x4 += offset.x;
		y4 += offset.y;
	}
};

#endif

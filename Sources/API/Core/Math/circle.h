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
**    Harry Storbacka
**    (if your name is missing here, please add it)
*/

//! clanCore="Math"
//! header=core.h

#ifndef header_circle
#define header_circle

#if _MSC_VER > 1000
#pragma once
#endif

class CL_Circlef;

//: Circle (point,radius) structure.
//- !group=Core/Math!
//- !header=core.h!
class CL_Circle
{
// Construction:
public:
	//: Constructs a Circle.
	//param x: Initial x value.
	//param y: Initial y value.
	//param radius: Circle radius.
	//param p: Point to use for initial values.
	CL_Circle()
	{ return; }

	CL_Circle(int x, int y, int radius)
	: position(x,y), radius(radius) { return; }

	CL_Circle(const CL_Point &p, int radius)
	: position(p), radius(radius) { return; }

	explicit CL_Circle(const CL_Circlef& other);

// Attributes:
public:
	//: Circle center point
	CL_Point position;

	//: Circle radius
	int radius;
};


class CL_Circlef
{
// Construction:
public:
	//: Constructs a Circle.
	//param x: Initial x value.
	//param y: Initial y value.
	//param radius: Circle radius.
	//param p: Point to use for initial values.
	CL_Circlef()
	{ return; }

	CL_Circlef(double x, double y, double radius)
	: position((float)x,(float)y), radius(radius) { return; }

	CL_Circlef(const CL_Pointf &p, double radius)
	: position(p), radius(radius) { return; }

// Attributes:
public:
	//: Circle center point
	CL_Pointf position;

	//: Circle radius
	double radius;
};

inline CL_Circle::CL_Circle(const CL_Circlef &other)
: position(other.position), radius(int(other.radius+0.5))
{
	return;
}

#endif

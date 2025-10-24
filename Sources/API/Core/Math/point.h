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

#ifndef header_point
#define header_point

#if _MSC_VER > 1000
#pragma once
#endif

#include "math.h"

class CL_Pointf;

//: 2D (x,y) point structure.
//- !group=Core/Math!
//- !header=core.h!
class CL_Point
{
// Construction:
public:
	//: Constructs a point.
	//param x: Initial x value.
	//param y: Initial y value.
	//param p: Point to use for initial values.
	CL_Point()
	{ return; }

	CL_Point(int x, int y)
	: x(x), y(y) { }

	CL_Point(const CL_Point &p)
	{ x = p.x; y = p.y; }

	explicit CL_Point(const CL_Pointf& p);

// Operations:
public:
	//: Return a rotated version of this point.
	//param hotspot: The point around which to rotate.
	//param angle: The amount of degrees to rotate by, clockwise.
	CL_Point rotate(
		const CL_Point &hotspot,
		float angle) const
	{
		//Move the hotspot to 0,0
		CL_Point r(x - hotspot.x, y - hotspot.y);
		
		//Do some Grumbel voodoo.

		// Because MSVC sucks ass wrt standards compliance, it gets it own special function calls
		#ifdef _MSC_VER
		const float c = (float) sqrt((float)r.x*(float)r.x + (float)r.y*(float)r.y);
		const float nw = (float)(atan2((float)r.y, (float)r.x) + ((angle + 180) * M_PI / 180));
		r.x = (int)((sin(1.5 * M_PI - nw) * c) + 0.5) + hotspot.x;
		r.y = -(int)((sin(nw) * c) + 0.5) + hotspot.y;
		#else
		const float c = (float) std::sqrt((float)r.x*(float)r.x + (float)r.y*(float)r.y);
		const float nw = (float)(std::atan2((float)r.y, (float)r.x) + ((angle + 180) * M_PI / 180));
 		r.x = (int)((std::sin(1.5 * M_PI - nw) * c) + 0.5) + hotspot.x;
		r.y = -(int)((std::sin(nw) * c) + 0.5) + hotspot.y;
		#endif

		return r;
	}

	//: Return the distance to another point.
	//param CL_Point &p: The other point.
	int distance( const CL_Point &p ) const
	{
		#ifdef _MSC_VER
    	return int(sqrt(double((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y))) + 0.5f);
		#else
    	return int(std::sqrt(double((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y))) + 0.5f);
		#endif
	}
	
	//: Translate point.
	CL_Point &operator+=(const CL_Point &p)
	{ x += p.x; y += p.y; return *this; }

	//: Translate point negatively.
	CL_Point &operator-=(const CL_Point &p)
	{ x -= p.x; y -= p.y; return *this; }
	
	//: Point + Point operator.
	CL_Point operator+(const CL_Point &p) const
	{ return CL_Point(x + p.x, y + p.y); }

	//: Point - Point operator.
	CL_Point operator-(const CL_Point &p) const
	{ return CL_Point(x - p.x, y - p.y); }

	//: Point == Point operator (deep compare)
	bool operator==(const CL_Point &p) const
	{ return (x == p.x) && (y == p.y); }

	//: Point != Point operator (deep compare)
	bool operator!=(const CL_Point &p) const
	{ return (x != p.x) || (y != p.y); }

// Attributes:
public:
	//: X coordinate.
	int x;

	//: Y coordinate.
	int y;
};

//: 2D (x,y) floating point point structure.
class CL_Pointf
{
// Construction:
public:
	//: Constructs a point.
	//param x: Initial x value.
	//param y: Initial y value.
	//param p: Point to use for initial values.
	CL_Pointf()
	{ return; }
	
	CL_Pointf(const CL_Point& p) 
		: x((float)p.x), y((float)p.y)
	{}

	CL_Pointf(float x, float y)
	: x(x), y(y) { }
	
	CL_Pointf(const CL_Pointf &p)
	{ x = p.x; y = p.y; }
	
// Operations:
public:
	//: Return a rotated version of this point.
	//param hotspot: The point around which to rotate.
	//param angle: The amount of degrees to rotate by, clockwise.
	CL_Pointf rotate(
		const CL_Pointf &hotspot,
		float angle) const
	{
		//Move the hotspot to 0,0
		CL_Pointf r(x - hotspot.x, y - hotspot.y);
		
		//Do some Grumbel voodoo.
		// MSVC is doesn't recognize std::sin and friends
		#ifdef _MSC_VER
		const float c = (float)(sqrt(r.x*r.x + r.y*r.y));

		const float nw = (float)((float)atan2(r.y, r.x) + ((angle + 180) * M_PI / 180));

		r.x = ((float)(sin(1.5 * M_PI - nw)) * c) + hotspot.x;
		r.y = -((float)(sin(nw)) * c) + hotspot.y;
		#else
 		const float c = (float)(std::sqrt(r.x*r.x + r.y*r.y));

		const float nw = (float)((float)std::atan2(r.y, r.x) + ((angle + 180) * M_PI / 180));

		r.x = ((float)(std::sin(1.5 * M_PI - nw)) * c) + hotspot.x;
		r.y = -((float)(std::sin(nw)) * c) + hotspot.y;
		#endif
		
		return r;
	}

	//: Return the distance to another point.
	//param CL_Pointf &p: The other point.
	float distance( const CL_Pointf &p ) const
	{
		#ifdef _MSC_VER
    	return (float) sqrt((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y));
		#else
    	return std::sqrt((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y));
		#endif
	}

	//: Translate point.
	CL_Pointf &operator+=(const CL_Pointf &p)
	{ x += p.x; y += p.y; return *this; }
	
	//: Translate point negatively.
	CL_Pointf &operator-=(const CL_Pointf &p)
	{ x -= p.x; y -= p.y; return *this; }
	
	//: Point + Point operator.
	CL_Pointf operator+(const CL_Pointf &p) const
	{ return CL_Pointf(x + p.x, y + p.y); }

	//: Point - Point operator.
	CL_Pointf operator-(const CL_Pointf &p) const
	{ return CL_Pointf(x - p.x, y - p.y); }

	//: Point == Point operator (deep compare)
	bool operator==(const CL_Pointf &p) const
	{ return (x == p.x) && (y == p.y); }

	//: Point != Point operator (deep compare)
	bool operator!=(const CL_Pointf &p) const
	{ return (x != p.x) || (y != p.y); }

// Attributes:
public:
	//: X coordinate.
	float x;
	
	//: Y coordinate.
	float y;
};


inline CL_Point::CL_Point(const CL_Pointf& p)
  : x(static_cast<int>(p.x)),
    y(static_cast<int>(p.y))
{}

#endif

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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

//! clanDisplay="Collision"
//! header=display.h

#ifndef header_contour
#define header_contour

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "outline_circle.h"

//: Collision detection contour.
//- !group=Display/Collision !
//- !header=display.h!
//- <p>A outline consists of contours, which hold the actual outline geometry</p>
class CL_Contour
{
//! Construction:
 public:
	//: Construct a contour
	CL_Contour() : is_inside_contour(false) {};
	~CL_Contour() {};

//! Attributes:
 public:
	// Points forming the countour.
	std::vector<CL_Pointf> points;

	// boolean specifying if this contour is inside-out (the inside of a hollow polygon)
	// if that is the case, then the collision-test will skip the inside_contour-test (because you can
	// be inside this one, without causing a collision)
	bool is_inside_contour;

	// Circles encapsulating a part of the outline.
	// If two circles arent intersecting, none of the lines inside them
	// collide either.
	std::vector<CL_OutlineCircle> sub_circles;
};

#endif

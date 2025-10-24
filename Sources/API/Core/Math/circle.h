/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "point.h"

class CL_Circlef;

/// \brief Circle (point,radius) structure.
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Circle
{
// Construction:
public:
	/// \brief Constructs a Circle.
	CL_Circle()
	{ return; }

	/// \brief Constructs a Circle.
	///
	/// \param x = Initial x value.
	/// \param y = Initial y value.
	/// \param radius = Circle radius.
	CL_Circle(int x, int y, int radius)
	: position(x,y), radius(radius) { return; }

	/// \brief Constructs a Circle.
	///
	/// \param p = Point to use for initial values.
	/// \param radius = Circle radius.
	CL_Circle(const CL_Point &p, int radius)
	: position(p), radius(radius) { return; }

	explicit CL_Circle(const CL_Circlef& other);

// Attributes:
public:
	/// \brief Circle center point
	CL_Point position;

	/// \brief Circle radius
	int radius;
};

/// \brief Circle (point,radius) structure.
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Circlef
{
// Construction:
public:
	/// \brief Constructs a Circle.
	CL_Circlef()
	{ return; }

	/// \brief Constructs a Circle.
	///
	/// \param x = Initial x value.
	/// \param y = Initial y value.
	/// \param radius = Circle radius.
	CL_Circlef(float x, float y, float radius)
	: position((float)x,(float)y), radius(radius) { return; }

	/// \brief Constructs a Circle.
	///
	/// \param p = Point to use for initial values.
	/// \param radius = Circle radius.
	CL_Circlef(const CL_Pointf &p, float radius)
	: position(p), radius(radius) { return; }

// Attributes:
public:
	/// \brief Circle center point
	CL_Pointf position;

	/// \brief Circle radius
	float radius;
};

inline CL_Circle::CL_Circle(const CL_Circlef &other)
: position(other.position), radius(int(other.radius+0.5))
{
	return;
}

/// \}

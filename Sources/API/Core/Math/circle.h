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

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "point.h"

/// \brief Circle
///
/// These circle templates are defined for: int (CL_Circle), float (CL_Circlef), double (CL_Circled)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Circlex
{
public:
	/// \brief Circle center point
	CL_Vec2<Type> position;

	/// \brief Circle radius
	Type radius;
	
	CL_Circlex(Type x, Type y, Type radius)	: position(x,y), radius(radius) { }
	CL_Circlex(const CL_Vec2<Type> &p, Type radius) : position(p), radius(radius) { }
	CL_Circlex() : radius( (Type) 0 )  { }
	CL_Circlex(const CL_Circlex<Type> &copy) { position = copy.position; radius = copy.radius;}

/// \name Attributes
/// \{
public:

	bool is_inside( const CL_Vec2<Type> &point ) {return radius >= position.distance(point);}

/// \}
/// \name Operators
/// \{
public:
	/// \brief = operator.
	CL_Circlex<Type> &operator = (const CL_Circlex<Type>& copy) { position = copy.position; radius = copy.radius; return *this; }

	/// \brief == operator.
	bool operator == (const CL_Circlex<Type>& circle) const {return ((position == circle.position) && (radius == circle.radius));}

	/// \brief != operator.
	bool operator != (const CL_Circlex<Type>& circle) const {return ((position != circle.position) || (radius != circle.radius));}
/// \}
};

/// \brief Circle - Integer
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Circle : public CL_Circlex<int>
{
public:
	CL_Circle(int x, int y, int radius) : CL_Circlex<int>(x, y, radius) { }
	CL_Circle(const CL_Vec2<int> &p, int radius) : CL_Circlex<int>(p, radius) { }
	CL_Circle() : CL_Circlex<int>(){ }
	CL_Circle(const CL_Circlex<int> &copy) : CL_Circlex<int>(copy){ }
};

/// \brief Circle - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Circlef : public CL_Circlex<float>
{
public:
	CL_Circlef(float x, float y, float radius) : CL_Circlex<float>(x, y, radius) { }
	CL_Circlef(const CL_Vec2<float> &p, float radius) : CL_Circlex<float>(p, radius) { }
	CL_Circlef() : CL_Circlex<float>(){ }
	CL_Circlef(const CL_Circlex<float> &copy) : CL_Circlex<float>(copy){ }
};

/// \brief Circle - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Circled : public CL_Circlex<double>
{
public:
	CL_Circled(double x, double y, double radius) : CL_Circlex<double>(x, y, radius) { }
	CL_Circled(const CL_Vec2<double> &p, double radius) : CL_Circlex<double>(p, radius) { }
	CL_Circled() : CL_Circlex<double>(){ }
	CL_Circled(const CL_Circlex<double> &copy) : CL_Circlex<double>(copy){ }
};

/// \}

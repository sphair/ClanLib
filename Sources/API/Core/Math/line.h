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
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"

template<typename Type>
class CL_Line2x;

template<typename Type>
class CL_Line3x;

template<typename Type>
class CL_Rectx;

template<typename Type>
class CL_Vec2;

class CL_Angle;

/// \brief 3D line
///
/// These line templates are defined for: int (CL_Line3), float (CL_Line3f), double (CL_Line3d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Line3x
{
public:
	CL_Vec3<Type> p;
	CL_Vec3<Type> q;

	CL_Line3x() { }
	CL_Line3x(const CL_Line3x<Type> &copy) { p = copy.p; q = copy.q;}
	CL_Line3x(const CL_Vec3<Type> &point_p, const CL_Vec3<Type> &point_q) { p = point_p; q = point_q; }

/// \name Attributes
/// \{
public:
	/// \brief Return the intersection of this and other line
	///
	/// \param second = The second line to use
	/// \param intersect = On Return: true if the lines intersect, false if the lines are parallel
	/// \param range = Rounding error delta, to use to judge whether of not the lines intersect
	/// \return The point
	CL_Vec3<Type> get_intersection( const CL_Line3x<Type> &second, bool &intersect, Type range = (Type) 0.5 ) const;

/// \}
/// \name Operators
/// \{
public:
	/// \brief = operator.
	CL_Line3x<Type> &operator = (const CL_Line3x<Type>& copy) { p = copy.p; q = copy.q; return *this; }

	/// \brief == operator.
	bool operator == (const CL_Line3x<Type>& line) const {return ((p == line.p) && (q == line.q));}

	/// \brief != operator.
	bool operator != (const CL_Line3x<Type>& line) const {return ((p != line.p) || (q != line.q));}
/// \}
};

/// \brief 2D line
///
/// These line templates are defined for: int (CL_Line2i), float (CL_Line2f), double (CL_Line2d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Line2x
{
public:
	/// \brief First point on the line
	CL_Vec2<Type> p;

	// \brief Another point on the line
	CL_Vec2<Type> q;

	CL_Line2x() { }
	CL_Line2x(const CL_Line2x<Type> &copy) { p = copy.p; q = copy.q;}
	CL_Line2x(const CL_Vec2<Type> &point_p, const CL_Vec2<Type> &point_q) { p = point_p; q = point_q; }
	CL_Line2x(const CL_Vec2<Type> &point_p, Type gradient) { p = point_p; q.x = (Type) 1; q.y = gradient; }

/// \name Attributes
/// \{
public:
	/// \brief Return the intersection of this and other line
	///
	/// \param second = The second line to use
	/// \param intersect = On Return: true if the lines intersect, false if the lines are parallel
	/// \return The point
	CL_Vec2<Type> get_intersection( const CL_Line2x<Type> &second, bool &intersect ) const;

	/// \brief Return [<0, 0, >0] if the Point P is right, on or left of the line trough A,B
	///
	/// \param point = The point
	/// \return Value representing - left (>0), centre (=0), or right (<0)
	Type point_right_of_line( CL_Vec2<Type> point ) const {return (q.x - p.x) * (point.y - p.y) - (point.x - p.x) * (q.y - p.y);}

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Operators
/// \{
public:
	/// \brief = operator.
	CL_Line2x<Type> &operator = (const CL_Line2x<Type>& copy) { p = copy.p; q = copy.q; return *this; }

	/// \brief == operator.
	bool operator == (const CL_Line2x<Type>& line) const {return ((p == line.p) && (q == line.q));}

	/// \brief != operator.
	bool operator != (const CL_Line2x<Type>& line) const {return ((p != line.p) || (q != line.q));}
/// \}
};

/// \brief 2D line - Integer
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Line2 : public CL_Line2x<int>
{
public:
	CL_Line2() : CL_Line2x<int>() { }
	CL_Line2(const CL_Line2x<int> &copy) : CL_Line2x<int>(copy) { }
	CL_Line2(const CL_Vec2<int> &point_p, const CL_Vec2<int> &point_q) : CL_Line2x<int>(point_p, point_q) { }
	CL_Line2(const CL_Vec2<int> &point_p, int gradient) : CL_Line2x<int>(point_p, gradient) { }
};

/// \brief 2D line - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Line2f : public CL_Line2x<float>
{
public:
	CL_Line2f() : CL_Line2x<float>() { }
	CL_Line2f(const CL_Line2x<float> &copy) : CL_Line2x<float>(copy) { }
	CL_Line2f(const CL_Vec2<float> &point_p, const CL_Vec2<float> &point_q) : CL_Line2x<float>(point_p, point_q) { }
	CL_Line2f(const CL_Vec2<float> &point_p, float gradient) : CL_Line2x<float>(point_p, gradient) { }
};

/// \brief 2D line - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Line2d : public CL_Line2x<double>
{
public:
	CL_Line2d() : CL_Line2x<double>() { }
	CL_Line2d(const CL_Line2x<double> &copy) : CL_Line2x<double>(copy) { }
	CL_Line2d(const CL_Vec2<double> &point_p, const CL_Vec2<double> &point_q) : CL_Line2x<double>(point_p, point_q) { }
	CL_Line2d(const CL_Vec2<double> &point_p, double gradient) : CL_Line2x<double>(point_p, gradient) { }
};

/// \brief 3D line - Integer
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Line3 : public CL_Line3x<int>
{
public:
	CL_Line3() : CL_Line3x<int>() { }
	CL_Line3(const CL_Line3x<int> &copy) : CL_Line3x<int>(copy) { }
	CL_Line3(const CL_Vec3<int> &point_p, const CL_Vec3<int> &point_q) : CL_Line3x<int>(point_p, point_q) { }
};

/// \brief 3D line - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Line3f : public CL_Line3x<float>
{
public:
	CL_Line3f() : CL_Line3x<float>() { }
	CL_Line3f(const CL_Line3x<float> &copy) : CL_Line3x<float>(copy) { }
	CL_Line3f(const CL_Vec3<float> &point_p, const CL_Vec3<float> &point_q) : CL_Line3x<float>(point_p, point_q) { }
};

/// \brief 3D line - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Line3d : public CL_Line3x<double>
{
public:
	CL_Line3d() : CL_Line3x<double>() { }
	CL_Line3d(const CL_Line3x<double> &copy) : CL_Line3x<double>(copy) { }
	CL_Line3d(const CL_Vec3<double> &podouble_p, const CL_Vec3<double> &podouble_q) : CL_Line3x<double>(podouble_p, podouble_q) { }
};

/// \}

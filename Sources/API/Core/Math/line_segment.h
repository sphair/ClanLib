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
class CL_LineSegment2x;

template<typename Type>
class CL_LineSegment3x;

template<typename Type>
class CL_Vec2;

template<typename Type>
class CL_Vec3;

template<typename Type>
class CL_Rectx;

class CL_Angle;

/// \brief 3D line segment
///
/// A line segment has a start point and an end point\n
/// These line templates are defined for: int (CL_LineSegment3), float (CL_LineSegment3f), double (CL_LineSegment3d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_LineSegment3x
{
public:
	/// \brief Start point on the line
	CL_Vec3<Type> p;

	// \brief End point on the line
	CL_Vec3<Type> q;

	CL_LineSegment3x() { }
	CL_LineSegment3x(const CL_LineSegment3x<Type> &copy) { p = copy.p; q = copy.q;}
	CL_LineSegment3x(const CL_Vec3<Type> &point_p, const CL_Vec3<Type> &point_q) { p = point_p; q = point_q; }

/// \name Attributes
/// \{
public:
	/// \brief Get the midpoint of this line
	///
	/// \return The midpoint
	CL_Vec3<Type> get_midpoint() const { return CL_Vec3<Type>( (q.x + p.x)/((Type)2), (q.y + p.y)/((Type)2), (q.z + p.z)/((Type)2) ); };

	/// \brief Calculate the distance from a line segment to a point
	///
	/// \param point = The point
	/// \param dest_intercept = On Return: The point on the line closest to the point
	/// \return The Distance
	Type point_distance(const CL_Vec3<Type> &point, CL_Vec3<Type> &dest_intercept) const;

/// \}
/// \name Operators
/// \{
public:
	/// \brief = operator.
	CL_LineSegment3x<Type> &operator = (const CL_LineSegment3x<Type>& copy) { p = copy.p; q = copy.q; return *this; }

	/// \brief == operator.
	bool operator == (const CL_LineSegment3x<Type>& line) const {return ((p == line.p) && (q == line.q));}

	/// \brief != operator.
	bool operator != (const CL_LineSegment3x<Type>& line) const {return ((p != line.p) || (q != line.q));}
/// \}
};

/// \brief 2D line segment
///
/// A line segment has a start point and an end point\n
/// These line templates are defined for: int (CL_LineSegment2i), float (CL_LineSegment2f), double (CL_LineSegment2d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_LineSegment2x
{
public:
	/// \brief Start point on the line
	CL_Vec2<Type> p;

	// \brief End point on the line
	CL_Vec2<Type> q;

	CL_LineSegment2x() { }
	CL_LineSegment2x(const CL_LineSegment2x<Type> &copy) { p = copy.p; q = copy.q;}
	CL_LineSegment2x(const CL_Vec2<Type> &point_p, const CL_Vec2<Type> &point_q) { p = point_p; q = point_q; }

/// \name Attributes
/// \{
public:
	/// \brief Get the midpoint of this line
	///
	/// \return The midpoint
	CL_Vec2<Type> get_midpoint() const { return CL_Vec2<Type>( (q.x + p.x)/((Type)2), (q.y + p.y)/((Type)2) ); };

	/// \brief Return the distance from a point to a line.
	///
	/// \param point = The point.
	Type point_distance(const CL_Vec2<Type> &point);

	/// \brief Return true if two line segments are collinear. (All points are on the same line.)
	///
	/// \param second = The second line to check with
	/// \return true = They are collinear
	bool collinear(const CL_LineSegment2x<Type> &second) const;

	/// \brief Return true if two line segments intersect.
	///
	/// \param second = Second line.
	/// \param collinear_intersect = true if a collision is reported when all points are on the same line.
	/// \return true = Intersects
	bool intersects( const CL_LineSegment2x<Type> &second, bool collinear_intersect ) const;

	/// \brief Return the intersection point of two lines.
	///
	/// \param second = Second line.
	/// \param intersect = On Return: The intercept. If the lines are parallel, this contains this line's first point
	/// \return true if the lines intersect, false if the lines are parallel
	CL_Vec2<Type> get_intersection( const CL_LineSegment2x<Type> &second, bool &intersect) const;

	/// \brief Return [<0, 0, >0] if the Point P is right, on or left of the line trough A,B
	///
	/// \param point = The point
	/// \return Value representing - left (>0), centre (=0), or right (<0)
	Type point_right_of_line( const CL_Vec2<Type> &point ) const {return (q.x - p.x) * (point.y - p.y) - (point.x - p.x) * (q.y - p.y);}

	/// \brief Return the normal vector of the line from point A to point B.
	///
	/// When using CL_Vec2i, the vector is an 8 bit fraction (multiplied by 256)
	///
	/// \return The normal vector
	CL_Vec2<Type> normal() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Clip this line to a rectangle
	///
	/// If clipping was not successful, this object is undefined
	///
	/// \param rect = Rectangle to clip to
	/// \param clipped = On Return: true if the line could be clipped, false if line exists outside the rectangle
	/// \return reference to this object
	CL_LineSegment2x<Type> &clip(const CL_Rectx<Type> &rect, bool &clipped);

/// \}
/// \name Operators
/// \{
public:
	/// \brief = operator.
	CL_LineSegment2x<Type> &operator = (const CL_LineSegment2x<Type>& copy) { p = copy.p; q = copy.q; return *this; }

	/// \brief == operator.
	bool operator == (const CL_LineSegment2x<Type>& line) const {return ((p == line.p) && (q == line.q));}

	/// \brief != operator.
	bool operator != (const CL_LineSegment2x<Type>& line) const {return ((p != line.p) || (q != line.q));}
/// \}
};

/// \brief 2D line segment - Integer
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineSegment2 : public CL_LineSegment2x<int>
{
public:
	CL_LineSegment2() : CL_LineSegment2x<int>() {}
	CL_LineSegment2(const CL_LineSegment2x<int> &copy) : CL_LineSegment2x<int>(copy) {}
	CL_LineSegment2(const CL_Vec2<int> &point_p, const CL_Vec2<int> &point_q) : CL_LineSegment2x<int>(point_p, point_q) {}
};

/// \brief 2D line segment - Float
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineSegment2f : public CL_LineSegment2x<float>
{
public:
	CL_LineSegment2f() : CL_LineSegment2x<float>() {}
	CL_LineSegment2f(const CL_LineSegment2x<float> &copy) : CL_LineSegment2x<float>(copy) {}
	CL_LineSegment2f(const CL_Vec2<float> &point_p, const CL_Vec2<float> &point_q) : CL_LineSegment2x<float>(point_p, point_q) {}
};

/// \brief 2D line segment - Double
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineSegment2d : public CL_LineSegment2x<double>
{
public:
	CL_LineSegment2d() : CL_LineSegment2x<double>() {}
	CL_LineSegment2d(const CL_LineSegment2x<double> &copy) : CL_LineSegment2x<double>(copy) {}
	CL_LineSegment2d(const CL_Vec2<double> &point_p, const CL_Vec2<double> &point_q) : CL_LineSegment2x<double>(point_p, point_q) {}
};

/// \brief 3D line segment - Integer
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineSegment3 : public CL_LineSegment3x<int>
{
public:
	CL_LineSegment3() : CL_LineSegment3x<int>() {}
	CL_LineSegment3(const CL_LineSegment3x<int> &copy) : CL_LineSegment3x<int>(copy) {}
	CL_LineSegment3(const CL_Vec3<int> &point_p, const CL_Vec3<int> &point_q) : CL_LineSegment3x<int>(point_p, point_q) {}
};

/// \brief 3D line segment - Float
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineSegment3f : public CL_LineSegment3x<float>
{
public:
	CL_LineSegment3f() : CL_LineSegment3x<float>() {}
	CL_LineSegment3f(const CL_LineSegment3x<float> &copy) : CL_LineSegment3x<float>(copy) {}
	CL_LineSegment3f(const CL_Vec3<float> &point_p, const CL_Vec3<float> &point_q) : CL_LineSegment3x<float>(point_p, point_q) {}
};

/// \brief 3D line segment - Double
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineSegment3d : public CL_LineSegment3x<double>
{
public:
	CL_LineSegment3d() : CL_LineSegment3x<double>() {}
	CL_LineSegment3d(const CL_LineSegment3x<double> &copy) : CL_LineSegment3x<double>(copy) {}
	CL_LineSegment3d(const CL_Vec3<double> &point_p, const CL_Vec3<double> &point_q) : CL_LineSegment3x<double>(point_p, point_q) {}
};

/// \}


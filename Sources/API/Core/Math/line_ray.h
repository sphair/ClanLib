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
class CL_LineRay2x;

template<typename Type>
class CL_LineRay3x;

template<typename Type>
class CL_Vec2;

template<typename Type>
class CL_Vec3;

class CL_Angle;

/// \brief 3D line ray
///
/// A ray has a start point and a direction\n
/// These line templates are defined for: int (CL_LineRay3), float (CL_LineRay3f), double (CL_LineRay3d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_LineRay3x
{
public:
	/// \brief Start point on the line ray
	CL_Vec3<Type> p;

	// \brief Direction of the ray
	CL_Vec3<Type> v;

	CL_LineRay3x() { }
	CL_LineRay3x(const CL_LineRay3x<Type> &copy) { p = copy.p; v = copy.v;}
	CL_LineRay3x(const CL_Vec3<Type> &point_p, const CL_Vec3<Type> &direction_v) { p = point_p; v = direction_v; }

/// \name Attributes
/// \{

public:

/// \}
/// \name Operators
/// \{

public:

	/// \brief = operator.
	CL_LineRay3x<Type> &operator = (const CL_LineRay3x<Type>& copy) { p = copy.p; v = copy.v; return *this; }

	/// \brief == operator.
	bool operator == (const CL_LineRay3x<Type>& line) const {return ((p == line.p) && (v == line.v));}

	/// \brief != operator.
	bool operator != (const CL_LineRay3x<Type>& line) const {return ((p != line.p) || (v != line.v));}
/// \}
};

/// \brief 2D line ray
///
/// A ray has a start point and a direction\n
/// These line templates are defined for: int (CL_LineRay2i), float (CL_LineRay2f), double (CL_LineRay2d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_LineRay2x
{
public:
	/// \brief Start point on the line ray
	CL_Vec2<Type> p;

	// \brief Direction of the ray
	CL_Vec2<Type> v;

	CL_LineRay2x() { }
	CL_LineRay2x(const CL_LineRay2x<Type> &copy) { p = copy.p; v = copy.v;}
	CL_LineRay2x(const CL_Vec2<Type> &point_p, const CL_Vec2<Type> &direction_v) { p = point_p; v = direction_v; }

/// \name Attributes
/// \{

public:

/// \}
/// \name Operators
/// \{

public:

	/// \brief = operator.
	CL_LineRay2x<Type> &operator = (const CL_LineRay2x<Type>& copy) { p = copy.p; v = copy.v; return *this; }

	/// \brief == operator.
	bool operator == (const CL_LineRay2x<Type>& line) const {return ((p == line.p) && (v == line.v));}

	/// \brief != operator.
	bool operator != (const CL_LineRay2x<Type>& line) const {return ((p != line.p) || (v != line.v));}
/// \}
};

/// \brief 2D line ray - Integer
///
/// A ray has a start point and a direction\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineRay2 : public CL_LineRay2x<int>
{
public:
	CL_LineRay2() : CL_LineRay2x<int>() {}
	CL_LineRay2(const CL_LineRay2x<int> &copy) : CL_LineRay2x<int>(copy) {}
	CL_LineRay2(const CL_Vec2<int> &point_p, const CL_Vec2<int> &direction_v) : CL_LineRay2x<int>(point_p, direction_v) {}

};

/// \brief 2D line ray - Float
///
/// A ray has a start point and a direction\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineRay2f : public CL_LineRay2x<float>
{
public:
	CL_LineRay2f() : CL_LineRay2x<float>() {}
	CL_LineRay2f(const CL_LineRay2x<float> &copy) : CL_LineRay2x<float>(copy) {}
	CL_LineRay2f(const CL_Vec2<float> &point_p, const CL_Vec2<float> &direction_v) : CL_LineRay2x<float>(point_p, direction_v) {}

};

/// \brief 2D line ray - Double
///
/// A ray has a start point and a direction\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineRay2d : public CL_LineRay2x<double>
{
public:
	CL_LineRay2d() : CL_LineRay2x<double>() {}
	CL_LineRay2d(const CL_LineRay2x<double> &copy) : CL_LineRay2x<double>(copy) {}
	CL_LineRay2d(const CL_Vec2<double> &point_p, const CL_Vec2<double> &direction_v) : CL_LineRay2x<double>(point_p, direction_v) {}

};

/// \brief 3D line ray - Integer
///
/// A ray has a start point and a direction\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineRay3 : public CL_LineRay3x<int>
{
public:
	CL_LineRay3() : CL_LineRay3x<int>() {}
	CL_LineRay3(const CL_LineRay3x<int> &copy) : CL_LineRay3x<int>(copy) {}
	CL_LineRay3(const CL_Vec3<int> &point_p, const CL_Vec3<int> &direction_v) : CL_LineRay3x<int>(point_p, direction_v) {}

};

/// \brief 3D line ray - Float
///
/// A ray has a start point and a direction\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineRay3f : public CL_LineRay3x<float>
{
public:
	CL_LineRay3f() : CL_LineRay3x<float>() {}
	CL_LineRay3f(const CL_LineRay3x<float> &copy) : CL_LineRay3x<float>(copy) {}
	CL_LineRay3f(const CL_Vec3<float> &point_p, const CL_Vec3<float> &direction_v) : CL_LineRay3x<float>(point_p, direction_v) {}

};

/// \brief 3D line ray - Double
///
/// A ray has a start point and a direction\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_LineRay3d : public CL_LineRay3x<double>
{
public:
	CL_LineRay3d() : CL_LineRay3x<double>() {}
	CL_LineRay3d(const CL_LineRay3x<double> &copy) : CL_LineRay3x<double>(copy) {}
	CL_LineRay3d(const CL_Vec3<double> &point_p, const CL_Vec3<double> &direction_v) : CL_LineRay3x<double>(point_p, direction_v) {}

};

/// \}

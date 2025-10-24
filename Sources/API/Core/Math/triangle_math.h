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

#include "../api_core.h"
#include "vec3.h"

/// \brief Triangles
///
/// These triangle templates are defined for: int (CL_Triangle), float (CL_Trianglef), double (CL_Triangled)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Trianglex
{
public:
	/// \brief First triangle point
	CL_Vec2<Type> p;

	// \brief Second triangle point
	CL_Vec2<Type> q;

	// \brief Third triangle point
	CL_Vec2<Type> r;

	CL_Trianglex() { }
	CL_Trianglex(const CL_Trianglex<Type> &copy) { p = copy.p; q = copy.q; r = copy.r;}
	CL_Trianglex(const CL_Vec2<Type> &point_p, const CL_Vec2<Type> &point_q, const CL_Vec2<Type> &point_r) { p = point_p; q = point_q; r = point_r;}

/// \name Attributes
/// \{

public:

	/// \brief Return true if the point is inside the triangle.
	///
	/// \param point = point to test.
	/// \return true if the point is inside the triangle
	bool point_inside(const CL_Vec2<Type> &point) const;

/// \}
/// \name Operators
/// \{

public:

	/// \brief = operator.
	CL_Trianglex<Type> &operator = (const CL_Trianglex<Type>& copy) { p = copy.p; q = copy.q; r = copy.r; return *this; }

	/// \brief == operator.
	bool operator == (const CL_Trianglex<Type>& triangle) const {return ((p == triangle.p) && (q == triangle.q) && (r == triangle.r));}

	/// \brief != operator.
	bool operator != (const CL_Trianglex<Type>& triangle) const {return ((p != triangle.p) || (q != triangle.q) || (r != triangle.r));}
/// \}
};

/// \brief Triangles - Integer
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Triangle : public CL_Trianglex<int>
{
public:
	CL_Triangle() { }
	CL_Triangle(const CL_Trianglex<int> &copy) : CL_Trianglex<int>(copy) {}
	CL_Triangle(const CL_Vec2<int> &point_p, const CL_Vec2<int> &point_q, const CL_Vec2<int> &point_r) : CL_Trianglex<int>(point_p, point_q, point_r) {}
};

/// \brief Triangles - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Trianglef : public CL_Trianglex<float>
{
public:
	CL_Trianglef() { }
	CL_Trianglef(const CL_Trianglex<float> &copy) : CL_Trianglex<float>(copy) {}
	CL_Trianglef(const CL_Vec2<float> &point_p, const CL_Vec2<float> &point_q, const CL_Vec2<float> &point_r) : CL_Trianglex<float>(point_p, point_q, point_r) {}
};

/// \brief Triangles - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Triangled : public CL_Trianglex<double>
{
public:
	CL_Triangled() { }
	CL_Triangled(const CL_Trianglex<double> &copy) : CL_Trianglex<double>(copy) {}
	CL_Triangled(const CL_Vec2<double> &point_p, const CL_Vec2<double> &point_q, const CL_Vec2<double> &point_r) : CL_Trianglex<double>(point_p, point_q, point_r) {}
};

/// \}


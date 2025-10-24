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
**    Magnus Norddahl
**    Mark Page
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "rect.h"
#include "size.h"
#include "point.h"
#include "origin.h"

/// \brief 2D quad structure.
///
/// These quads templates are defined for: int (CL_Quad), float (CL_Quadf), double (CL_Quadd)
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_API_CORE CL_Quadx
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a quad.
	CL_Quadx() { }

	/// \brief Constructs a quad.
	///
	/// \param new_p = coord for first point of quad.
	/// \param new_q = coord for second point of quad.
	/// \param new_r = coord for third point of quad.
	/// \param new_s = coord for forth point of quad.
	CL_Quadx(const CL_Vec2<Type> &new_p, const CL_Vec2<Type> &new_q, const CL_Vec2<Type> &new_r, const CL_Vec2<Type> &new_s)
	{ p = new_p; q = new_q;	r = new_r; s = new_s; }

	/// \brief Constructs a quad.
	///
	/// \param rect Rectangle used to initialize the quad. 
	/// <p>The corners of the rect are mapped to the quad member variables as follows:
	///   <ul>
	///     <li>Top-left: p</li>
	///     <li>Top-right: q</li>
	///     <li>Bottom-right: r</li>
	///     <li>Bottom-left: s</li>
	///   </ul>
	/// </p>
	CL_Quadx(const CL_Rectx<Type> &rect)
	{ p.x = rect.left; p.y = rect.top; q.x = rect.right; q.y = rect.top;
		r.x = rect.right; r.y = rect.bottom; s.x = rect.left; s.y = rect.bottom;
	}

	/// \brief Constructs a quad.
	///
	/// \param quad = Quad used to initialize this quad.
	CL_Quadx(const CL_Quadx<Type> &quad)
	{ p = quad.p; q = quad.q; r = quad.r; s = quad.s; }

	/// \brief Quad += Quad operator.
	CL_Quadx<Type> &operator+=(const CL_Quadx<Type> &quad)
	{ p += quad.p; q += quad.q;  r += quad.r; s += quad.s; return *this; }

	/// \brief Quad -= Quad operator.
	CL_Quadx<Type> &operator-=(const CL_Quadx<Type> &quad)
	{ p -= quad.p; q -= quad.q;  r -= quad.r; s -= quad.s; return *this; }

	/// \brief Quad += Point operator.
	CL_Quadx<Type> &operator+=(const CL_Vec2<Type> &point)
	{ p += point; q += point; r += point; s += point; return *this;	}

	/// \brief Quad -= Point operator.
	CL_Quadx<Type> &operator-=(const CL_Vec2<Type> &point)
	{ p -= point; q -= point; r -= point; s -= point; return *this;	}

	/// \brief Quad + Quad operator.
	CL_Quadx<Type> operator+(const CL_Quadx<Type> &quad) const
	{ return CL_Quadx(p + quad.p, q + quad.q, r + quad.r, s + quad.s); }

	/// \brief Quad - Quad operator.
	CL_Quadx<Type> operator-(const CL_Quadx<Type> &quad) const
	{ return CL_Quadx(p - quad.p, q - quad.q, r - quad.r, s - quad.s); }

	/// \brief Quad + Point operator.
	CL_Quadx<Type> operator+(const CL_Vec2<Type>  &point) const
	{ return CL_Quadx(p + point, q + point,  r + point, s + point); }

	/// \brief Quad - Point operator.
	CL_Quadx<Type> operator-(const CL_Vec2<Type>  &point) const
	{ return CL_Quadx(p - point, q - point,  r - point, s - point); }

	/// \brief Quad == Quad operator.
	bool operator==(const CL_Quadx<Type> &quad) const
	{ return (p == quad.p && q == quad.q && r == quad.r && s == quad.s); }

	/// \brief Quad != Quad operator.
	bool operator!=(const CL_Quadx<Type> &quad) const
	{ return (p != quad.p || q != quad.q || r != quad.r || s != quad.s); }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief First Point
	CL_Vec2<Type> p;

	/// \brief Second Point
	CL_Vec2<Type> q;

	/// \brief Third Point
	CL_Vec2<Type> r;

	/// \brief Fourth Point
	CL_Vec2<Type> s;

	/// \brief Returns the width of the quad.
	Type get_width() const;

	/// \brief Returns the height of the quad.
	Type get_height() const;

	/// \brief Returns the size of the rectangle.
	CL_Sizex<Type> get_size() const { return CL_Sizex<Type>(get_width(), get_height()); }

	/// \brief Returns the bounding box of the quad as a CL_Rect
	CL_Rect get_bounds() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Rotates the Quad
	///
	/// \param hotspot Point to rotate around.
	/// \param angle Angle to rotate in degrees.
	///
	/// \return reference to this object
	CL_Quadx<Type> &rotate(const CL_Vec2<Type>  &hotspot, const CL_Angle &angle);

	/// \brief Scale the Quad
	///
	/// \param sx = Scale value in x-axis
	/// \param sy = Scale value in y-axis
	///
	/// \return reference to this object
	CL_Quadx<Type> &scale(float sx, float sy);

	/// \brief Scale the Quad
	///
	/// \param sx = Scale value in x-axis
	/// \param sy = Scale value in y-axis
	/// \param hotspot = Point to scale around.
	///
	/// \return reference to this object
	CL_Quadx<Type> &scale(const CL_Vec2<Type>  &hotspot, float sx, float sy);

	/// \brief Returns the center point of the quad
	CL_Vec2<Type> center() const;

	/// \brief Applies an origin and offset pair to this rectangle
	///
	/// \param origin The new origin to adjust to from default upper-left position
	/// \param x, y Offsets applied negatively to each corner of the rectangle
	///
	/// \return reference to this object
	CL_Quadx<Type> &apply_alignment(CL_Origin origin, Type x, Type y);

/// \}
};

/// \brief 2D quad structure - Integer
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Quad : public CL_Quadx<int>
{
public:
	CL_Quad() : CL_Quadx<int>() {}
	CL_Quad(const CL_Vec2<int> &new_p, const CL_Vec2<int> &new_q, const CL_Vec2<int> &new_r, const CL_Vec2<int> &new_s) : CL_Quadx<int>(new_p, new_q, new_r, new_s) {}
	CL_Quad(const CL_Rect &rect) : CL_Quadx<int>(rect) {}
	CL_Quad(const CL_Quadx<int> &quad) : CL_Quadx<int>(quad) {}
};

/// \brief 2D quad structure - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Quadf : public CL_Quadx<float>
{
public:
	CL_Quadf() : CL_Quadx<float>() {}
	CL_Quadf(const CL_Vec2<float> &new_p, const CL_Vec2<float> &new_q, const CL_Vec2<float> &new_r, const CL_Vec2<float> &new_s) : CL_Quadx<float>(new_p, new_q, new_r, new_s) {}
	CL_Quadf(const CL_Rectf &rect) : CL_Quadx<float>(rect) {}
	CL_Quadf(const CL_Quadx<float> &quad) : CL_Quadx<float>(quad) {}
};

/// \brief 2D quad structure - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Quadd : public CL_Quadx<double>
{
public:
	CL_Quadd() : CL_Quadx<double>() {}
	CL_Quadd(const CL_Vec2<double> &new_p, const CL_Vec2<double> &new_q, const CL_Vec2<double> &new_r, const CL_Vec2<double> &new_s) : CL_Quadx<double>(new_p, new_q, new_r, new_s) {}
	CL_Quadd(const CL_Rectd &rect) : CL_Quadx<double>(rect) {}
	CL_Quadd(const CL_Quadx<double> &quad) : CL_Quadx<double>(quad) {}
};

/// \}

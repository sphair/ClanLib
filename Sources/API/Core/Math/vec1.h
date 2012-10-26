/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Harry Storbacka
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "cl_math.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

namespace clan
{

template<typename Type>
class Vec1;

template<typename Type>
class Vec2;

template<typename Type>
class Vec3;

template<typename Type>
class Vec4;

template<typename Type>
class Mat2;

template<typename Type>
class Mat3;

template<typename Type>
class Mat4;

template<typename Type>
class Sizex;

template<typename Type>
class Pointx;

class Angle;

/// \brief 1D vector
///
/// These vector templates are defined for: \n
/// char (Vec1c), unsigned char (Vec1uc), short (Vec1s), \n
/// unsigned short (Vec1us), int (Vec1i), unsigned int (Vec1ui), float  Vec1f), double (Vec1d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class Vec1
{
public:
	typedef Type datatype;

	union { Type x; Type s; Type r; };

	Vec1() : x(0) { }
	explicit Vec1(const Vec2<Type> &copy) { x = copy.x; }
	explicit Vec1(const Vec3<Type> &copy) { x = copy.x; }
	explicit Vec1(const Vec4<Type> &copy) { x = copy.x; }
	explicit Vec1(const Type &p1) : x(p1) { }

	/// \brief Rounds all components on a vector
	///
	/// Uses Asymmetric Arithmetic Rounding
	///
	/// \param vector = Vector to use
	/// \return rounded vector
	static Vec1<Type> round(const Vec1<Type>& vector) { Vec1 dest; dest.x = (Type) floor(vector.x+0.5); return dest; }

/// \name Attributes
/// \{

public:
	/// \brief Rounds all components on this vector.
	///
	/// Uses Asymmetric Arithmetic Rounding
	/// \return reference to this object
	Vec1<Type> &round() { x = (Type) floor(x+0.5); return *this;}

/// \}
/// \name Operators
/// \{

public:
	const Type &operator[](unsigned int i) const { return ((Type *) this)[i]; }
	Type &operator[](unsigned int i) { return ((Type *) this)[i]; }
	operator Type *() { return (Type *) this; }
	operator Type * const() const { return (Type * const) this; }

	/// \brief += operator.
	void operator += (const Vec1<Type>& vector) { x+= vector.x; }

	/// \brief += operator.
	void operator += ( Type value) { x+= value; }

	/// \brief -= operator.
	void operator -= (const Vec1<Type>& vector) { x-= vector.x; }

	/// \brief -= operator.
	void operator -= ( Type value) { x-= value; }

	/// \brief - operator.
	Vec1<Type> operator - () const {return Vec1<Type>(-x);}

	/// \brief *= operator.
	void operator *= (const Vec1<Type>& vector) { x*= vector.x; }

	/// \brief *= operator.
	void operator *= ( Type value) { x*= value; }

	/// \brief /= operator.
	void operator /= (const Vec1<Type>& vector) { x/= vector.x; }

	/// \brief /= operator.
	void operator /= ( Type value) { x/= value; }

	/// \brief = operator.
	Vec1<Type> &operator = (const Vec1<Type>& vector) { x = vector.x; return *this; }

	/// \brief == operator.
	bool operator == (const Vec1<Type>& vector) const {return ((x == vector.x));}

	/// \brief != operator.
	bool operator != (const Vec1<Type>& vector) const {return ((x != vector.x));}

	/// \brief < operator.
	bool operator < (const Vec1<Type>& vector) const { return x < vector.x; }
/// \}
};

/// \brief + operator.
template<typename Type>
Vec1<Type> operator + (const Vec1<Type>& v1, const Vec1<Type>& v2) {return Vec1<Type>(v1.x + v2.x);}

/// \brief + operator.
template<typename Type>
Vec1<Type> operator + (Type s, const Vec1<Type>& v) {return Vec1<Type>(s + v.x);}

/// \brief + operator.
template<typename Type>
Vec1<Type> operator + (const Vec1<Type>& v, Type s) {return Vec1<Type>(v.x + s);}

/// \brief - operator.
template<typename Type>
Vec1<Type> operator - (const Vec1<Type>& v1, const Vec1<Type>& v2) {return Vec1<Type>(v1.x - v2.x);}

/// \brief - operator.
template<typename Type>
Vec1<Type> operator - (Type s, const Vec1<Type>& v) {return Vec1<Type>(s - v.x);}

/// \brief - operator.
template<typename Type>
Vec1<Type> operator - (const Vec1<Type>& v, Type s) {return Vec1<Type>(v.x - s);}

/// \brief * operator.
template<typename Type>
Vec1<Type> operator * (const Vec1<Type>& v1, const Vec1<Type>& v2) {return Vec1<Type>(v1.x * v2.x);}

/// \brief * operator.
template<typename Type>
Vec1<Type> operator * (Type s, const Vec1<Type>& v) {return Vec1<Type>(s * v.x);}

/// \brief * operator.
template<typename Type>
Vec1<Type> operator * (const Vec1<Type>& v, Type s) {return Vec1<Type>(v.x * s);}

/// \brief / operator.
template<typename Type>
Vec1<Type> operator / (const Vec1<Type>& v1, const Vec1<Type>& v2) {return Vec1<Type>(v1.x / v2.x);}

/// \brief / operator.
template<typename Type>
Vec1<Type> operator / (Type s, const Vec1<Type>& v) {return Vec1<Type>(s / v.x);}

/// \brief / operator.
template<typename Type>
Vec1<Type> operator / (const Vec1<Type>& v, Type s) {return Vec1<Type>(v.x / s);}

typedef Vec1<unsigned char> Vec1ub;
typedef Vec1<char> Vec1b;
typedef Vec1<unsigned short> Vec1us;
typedef Vec1<short> Vec1s;
typedef Vec1<unsigned int> Vec1ui;
typedef Vec1<int> Vec1i;
typedef Vec1<float> Vec1f;
typedef Vec1<double> Vec1d;

}

/// \}

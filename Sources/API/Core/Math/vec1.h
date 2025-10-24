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

template<typename Type>
class CL_Vec1;

template<typename Type>
class CL_Vec2;

template<typename Type>
class CL_Vec3;

template<typename Type>
class CL_Vec4;

template<typename Type>
class CL_Mat2;

template<typename Type>
class CL_Mat3;

template<typename Type>
class CL_Mat4;

template<typename Type>
class CL_Sizex;

template<typename Type>
class CL_Pointx;

class CL_Angle;

/// \brief 1D vector
///
/// These vector templates are defined for: \n
/// char (CL_Vec1c), unsigned char (CL_Vec1uc), short (CL_Vec1s), \n
/// unsigned short (CL_Vec1us), int (CL_Vec1i), unsigned int (CL_Vec1ui), float  CL_Vec1f), double (CL_Vec1d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Vec1
{
public:
	typedef Type datatype;

	union { Type x; Type s; Type r; };

	CL_Vec1() : x(0) { }
	CL_Vec1(const CL_Vec2<Type> &copy) { x = copy.x; }
	CL_Vec1(const CL_Vec3<Type> &copy) { x = copy.x; }
	CL_Vec1(const CL_Vec4<Type> &copy) { x = copy.x; }
	CL_Vec1(const Type &p1) : x(p1) { }

	/// \brief Rounds all components on a vector
	///
	/// Uses Asymmetric Arithmetic Rounding
	///
	/// \param vector = Vector to use
	/// \return rounded vector
	static CL_Vec1<Type> round(const CL_Vec1<Type>& vector) { CL_Vec1 dest; dest.x = (Type) floor(vector.x+0.5); return dest; }

/// \name Attributes
/// \{

public:
	/// \brief Rounds all components on this vector.
	///
	/// Uses Asymmetric Arithmetic Rounding
	/// \return reference to this object
	CL_Vec1<Type> &round() { x = (Type) floor(x+0.5); return *this;}

/// \}
/// \name Operators
/// \{

public:
	const Type &operator[](unsigned int i) const { return ((Type *) this)[i]; }
	Type &operator[](unsigned int i) { return ((Type *) this)[i]; }
	operator Type *() { return (Type *) this; }
	operator Type * const() const { return (Type * const) this; }

	/// \brief += operator.
	void operator += (const CL_Vec1<Type>& vector) { x+= vector.x; }

	/// \brief += operator.
	void operator += ( Type value) { x+= value; }

	/// \brief + operator.
	CL_Vec1<Type> operator + (const CL_Vec1<Type>& vector) const {return CL_Vec1<Type>(vector.x + x);}

	/// \brief + operator.
	CL_Vec1<Type> operator + (Type value) const {return CL_Vec1<Type>(value + x);}

	/// \brief -= operator.
	void operator -= (const CL_Vec1<Type>& vector) { x-= vector.x; }

	/// \brief -= operator.
	void operator -= ( Type value) { x-= value; }

	/// \brief - operator.
	CL_Vec1<Type> operator - (const CL_Vec1<Type>& vector) const {return CL_Vec1<Type>(x - vector.x);}

	/// \brief - operator.
	CL_Vec1<Type> operator - (Type value) const {return CL_Vec1<Type>(x - value);}

	/// \brief *= operator.
	void operator *= (const CL_Vec1<Type>& vector) { x*= vector.x; }

	/// \brief *= operator.
	void operator *= ( Type value) { x*= value; }

	/// \brief * operator.
	CL_Vec1<Type> operator * (const CL_Vec1<Type>& vector) const {return CL_Vec1<Type>(vector.x * x);}

	/// \brief * operator.
	CL_Vec1<Type> operator * (Type value) const {return CL_Vec1<Type>(value * x);}

	/// \brief /= operator.
	void operator /= (const CL_Vec1<Type>& vector) { x/= vector.x; }

	/// \brief /= operator.
	void operator /= ( Type value) { x/= value; }

	/// \brief / operator.
	CL_Vec1<Type> operator / (const CL_Vec1<Type>& vector) const {return CL_Vec1<Type>(x / vector.x);}

	/// \brief / operator.
	CL_Vec1<Type> operator / (Type value) const {return CL_Vec1<Type>(x / value);}

	/// \brief = operator.
	CL_Vec1<Type> &operator = (const CL_Vec1<Type>& vector) { x = vector.x; return *this; }

	/// \brief == operator.
	bool operator == (const CL_Vec1<Type>& vector) const {return ((x == vector.x));}

	/// \brief != operator.
	bool operator != (const CL_Vec1<Type>& vector) const {return ((x != vector.x));}
/// \}
};

typedef CL_Vec1<unsigned char> CL_Vec1ub;
typedef CL_Vec1<char> CL_Vec1b;
typedef CL_Vec1<unsigned short> CL_Vec1us;
typedef CL_Vec1<short> CL_Vec1s;
typedef CL_Vec1<unsigned int> CL_Vec1ui;
typedef CL_Vec1<int> CL_Vec1i;
typedef CL_Vec1<float> CL_Vec1f;
typedef CL_Vec1<double> CL_Vec1d;

/// \}

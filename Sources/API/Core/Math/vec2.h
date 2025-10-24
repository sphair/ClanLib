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
#include "vec1.h"
#include "vec3.h"
#include "vec4.h"
#include "origin.h"

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

/// \brief 2D vector
///
/// These vector templates are defined for:\n
/// char (CL_Vec2c), unsigned char (CL_Vec2uc), short (CL_Vec2s),\n
/// unsigned short (CL_Vec2us), int (CL_Vec2i), unsigned int (CL_Vec2ui), float (CL_Vec2f), double (CL_Vec2d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Vec2
{
public:
	typedef Type datatype;

	union { Type x; Type s; Type r; };
	union { Type y; Type t; Type g; };

	CL_Vec2() : x(0), y(0) { }
	CL_Vec2(const CL_Vec1<Type> &copy) { x = copy.x; y = 0;}
	CL_Vec2(const CL_Vec3<Type> &copy) { x = copy.x; y = copy.y;}
	CL_Vec2(const CL_Vec4<Type> &copy) { x = copy.x; y = copy.y;}
	CL_Vec2(const Type &p1, const Type &p2 = 0) : x(p1), y(p2) { }
	CL_Vec2(const CL_Pointx<int> &point);
	CL_Vec2(const CL_Pointx<float> &point);
	CL_Vec2(const CL_Pointx<double> &point);

	CL_Vec2(const CL_Vec2<double> &copy);
	CL_Vec2(const CL_Vec2<float> &copy);
	CL_Vec2(const CL_Vec2<int> &copy);

	/// \brief Normalizes a vector
	///
	/// Operates in the native datatype
	///
	/// \param vector = Vector to use
	/// \return normalized vector
	static CL_Vec2<Type> normalize(const CL_Vec2<Type>& vector);

	/// \brief Dot products a vector with an other vector.
	///
	/// Operates in the native datatype
	///
	/// \param vector_1 = First vector used for the dot product.
	/// \param vector_2 = Second vector used for the dot product.
	/// \return The resulting dot product of the two vectors.
	static Type dot(const CL_Vec2<Type>& vector_1, const CL_Vec2<Type>& vector_2) { return vector_1.x*vector_2.x + vector_1.y*vector_2.y; }

	/// \brief Rounds all components on a vector
	///
	/// Uses Asymmetric Arithmetic Rounding
	///
	/// \param vector = Vector to use
	/// \return The rounded vector
	static CL_Vec2<Type> round(const CL_Vec2<Type>& vector);

	/// \brief Rotate a vector around another point.
	///
	/// \param vector = Vector to use
	/// \param hotspot The point around which to rotate.
	/// \param angle = Angle to rotate.
	static CL_Vec2<Type> rotate(const CL_Vec2<Type>& vector, const CL_Vec2<Type>& hotspot, const CL_Angle &angle);

	/// \brief Returns the anchor point for the origin within the dimensions of the size structure.
	///
	/// \param origin = The origin
	/// \param size = The size
	/// \return The point
	static CL_Pointx<Type> calc_origin(CL_Origin origin, const CL_Sizex<Type> &size);

/// \name Attributes
/// \{

public:

	/// \brief Returns the length (magnitude) of this vector.
	///
	/// Operates in the native datatype
	///
	/// \return the length of the vector
	Type length() const;

	/// \brief Normalizes this vector
	///
	/// Operates in the native datatype
	///
	/// \return reference to this object
	CL_Vec2<Type> &normalize();

	/// \brief Dot products this vector with an other vector.
	///
	/// Operates in the native datatype
	///
	/// \param vector Second vector used for the dot product.
	///
	/// \return The resulting dot product of the two vectors.
	Type dot(const CL_Vec2<Type>& vector) const {return x*vector.x + y*vector.y;}

	/// \brief Calculate the angle between this vector and an other vector.
	///
	/// \param vector = Second vector used to calculate angle.
	///
	/// \return The angle between the two vectors.
	CL_Angle angle(const CL_Vec2<Type>& vector) const;

	/// \brief Calculate the distance between this vector and an other vector.
	///
	/// \param vector = Second vector used to calculate distance.
	///
	/// \return The distance between the two vectors.
	Type distance(const CL_Vec2<Type>& vector) const;

	/// \brief Rounds all components of this vector
	///
	/// Uses Asymmetric Arithmetic Rounding
	///
	/// \return reference to this object
	CL_Vec2<Type> &round();

	/// \brief Rotate this vector around another point.
	///
	/// \param hotspot The point around which to rotate.
	/// \param angle = Angle to rotate.
	///
	/// \return reference to this object
	CL_Vec2<Type> &rotate(const CL_Vec2<Type>& hotspot, const CL_Angle &angle);

	/// \brief Rounds a value for the datatype
	///
	/// For doubles and floats, this function does not round.
	/// \param value = Value to round
	/// \return The rounded value
	Type round_value(float value) const;

/// \}
/// \name Operators
/// \{

public:
	const Type &operator[](unsigned int i) const { return ((Type *) this)[i]; }
	Type &operator[](unsigned int i) { return ((Type *) this)[i]; }
	operator Type *() { return (Type *) this; }
	operator Type * const() const { return (Type * const) this; }

	/// \brief += operator.
	void operator += (const CL_Vec2<Type>& vector) { x+= vector.x; y+= vector.y; }

	/// \brief += operator.
	void operator += ( Type value) { x+= value; y+= value; }

	/// \brief + operator.
	CL_Vec2<Type> operator + (const CL_Vec2<Type>& vector) const {return CL_Vec2<Type>(vector.x + x, vector.y + y);}

	/// \brief + operator.
	CL_Vec2<Type> operator + (Type value) const {return CL_Vec2<Type>(value + x, value + y);}

	/// \brief -= operator.
	void operator -= (const CL_Vec2<Type>& vector) { x-= vector.x; y-= vector.y; }

	/// \brief -= operator.
	void operator -= ( Type value) { x-= value; y-= value; }

	/// \brief - operator.
	CL_Vec2<Type> operator - (const CL_Vec2<Type>& vector) const {return CL_Vec2<Type>(x - vector.x, y - vector.y);}

	/// \brief - operator.
	CL_Vec2<Type> operator - (Type value) const {return CL_Vec2<Type>(x - value, y - value);}

	/// \brief - operator.
	CL_Vec2<Type> operator - () const {return CL_Vec2<Type>(-x , -y);}

	/// \brief *= operator.
	void operator *= (const CL_Vec2<Type>& vector) { x*= vector.x; y*= vector.y; }

	/// \brief *= operator.
	void operator *= ( Type value) { x*= value; y*= value; }

	/// \brief * operator.
	CL_Vec2<Type> operator * (const CL_Vec2<Type>& vector) const {return CL_Vec2<Type>(vector.x * x, vector.y * y);}

	/// \brief * operator.
	CL_Vec2<Type> operator * (Type value) const {return CL_Vec2<Type>(value * x, value * y);}

	/// \brief /= operator.
	void operator /= (const CL_Vec2<Type>& vector) { x/= vector.x; y/= vector.y; }

	/// \brief /= operator.
	void operator /= ( Type value) { x/= value; y/= value; }

	/// \brief / operator.
	CL_Vec2<Type> operator / (const CL_Vec2<Type>& vector) const {return CL_Vec2<Type>(x / vector.x, y / vector.y);}

	/// \brief / operator.
	CL_Vec2<Type> operator / (Type value) const {return CL_Vec2<Type>(x / value, y / value);}

	/// \brief = operator.
	CL_Vec2<Type> &operator = (const CL_Vec2<Type>& vector) { x = vector.x; y = vector.y; return *this; }

	/// \brief == operator.
	bool operator == (const CL_Vec2<Type>& vector) const {return ((x == vector.x) && (y == vector.y));}

	/// \brief != operator.
	bool operator != (const CL_Vec2<Type>& vector) const {return ((x != vector.x) || (y != vector.y));}
/// \}
};

template<typename Type>
CL_Vec2<Type> operator * (const CL_Vec2<Type>& v, const CL_Mat2<Type>& matrix)
{
	return CL_Vec2<Type>(
		matrix[0*2+0]*v.x + matrix[0*2+1]*v.y,
		matrix[1*2+0]*v.x + matrix[1*2+1]*v.y);
}

template<typename Type>
CL_Vec2<Type> operator * (const CL_Mat2<Type>& matrix, const CL_Vec2<Type>& v)
{
	return CL_Vec2<Type>(
		matrix[0*2+0]*v.x + matrix[1*2+0]*v.y,
		matrix[0*2+1]*v.x + matrix[1*2+1]*v.y);
}

//////////////////////////////////////////////////////////////////////////

template<>
inline CL_Vec2<unsigned char>::CL_Vec2(const CL_Vec2<float> &copy) { x = (unsigned char) floor(copy.x +0.5f); y = (unsigned char) floor(copy.y + 0.5f); }

template<>
inline CL_Vec2<unsigned char>::CL_Vec2(const CL_Vec2<double> &copy) { x = (unsigned char) floor(copy.x+0.5); y = (unsigned char) floor(copy.y+0.5); }

template<>
inline CL_Vec2<unsigned char>::CL_Vec2(const CL_Vec2<int> &copy) { x = (unsigned char) copy.x; y = (unsigned char) copy.y; }

template<>
inline CL_Vec2<char>::CL_Vec2(const CL_Vec2<float> &copy) { x = (char) floor(copy.x +0.5f); y = (char) floor(copy.y + 0.5f); }

template<>
inline CL_Vec2<char>::CL_Vec2(const CL_Vec2<double> &copy) { x = (char) floor(copy.x+0.5); y = (char) floor(copy.y+0.5); }

template<>
inline CL_Vec2<char>::CL_Vec2(const CL_Vec2<int> &copy) { x = (char) copy.x; y = (char) copy.y; }

template<>
inline CL_Vec2<unsigned short>::CL_Vec2(const CL_Vec2<float> &copy) { x = (unsigned short) floor(copy.x +0.5f); y = (unsigned short) floor(copy.y + 0.5f); }

template<>
inline CL_Vec2<unsigned short>::CL_Vec2(const CL_Vec2<double> &copy) { x = (unsigned short) floor(copy.x+0.5); y = (unsigned short) floor(copy.y+0.5); }

template<>
inline CL_Vec2<unsigned short>::CL_Vec2(const CL_Vec2<int> &copy) { x = (unsigned short) copy.x; y = (unsigned short) copy.y; }

template<>
inline CL_Vec2<short>::CL_Vec2(const CL_Vec2<float> &copy) { x = (short) floor(copy.x +0.5f); y = (short) floor(copy.y + 0.5f); }

template<>
inline CL_Vec2<short>::CL_Vec2(const CL_Vec2<double> &copy) { x = (short) floor(copy.x+0.5); y = (short) floor(copy.y+0.5); }

template<>
inline CL_Vec2<short>::CL_Vec2(const CL_Vec2<int> &copy) { x = (short) copy.x; y = (short) copy.y; }

template<>
inline CL_Vec2<int>::CL_Vec2(const CL_Vec2<float> &copy) { x = (int) floor(copy.x +0.5f); y = (int) floor(copy.y + 0.5f); }

template<>
inline CL_Vec2<int>::CL_Vec2(const CL_Vec2<double> &copy) { x = (int) floor(copy.x+0.5); y = (int) floor(copy.y+0.5); }

template<>
inline CL_Vec2<int>::CL_Vec2(const CL_Vec2<int> &copy) { x = (int) copy.x; y = (int) copy.y; }

template<>
inline CL_Vec2<unsigned int>::CL_Vec2(const CL_Vec2<float> &copy) { x = (unsigned int) floor(copy.x +0.5f); y = (unsigned int) floor(copy.y + 0.5f); }

template<>
inline CL_Vec2<unsigned int>::CL_Vec2(const CL_Vec2<double> &copy) { x = (unsigned int) floor(copy.x+0.5); y = (unsigned int) floor(copy.y+0.5); }

template<>
inline CL_Vec2<unsigned int>::CL_Vec2(const CL_Vec2<int> &copy) { x = (unsigned int) copy.x; y = (unsigned int) copy.y; }

template<>
inline CL_Vec2<float>::CL_Vec2(const CL_Vec2<float> &copy) { x = (float) copy.x; y = (float) copy.y; }

template<>
inline CL_Vec2<float>::CL_Vec2(const CL_Vec2<double> &copy) { x = (float) copy.x; y = (float) copy.y; }

template<>
inline CL_Vec2<float>::CL_Vec2(const CL_Vec2<int> &copy) { x = (float) copy.x; y = (float) copy.y; }

template<>
inline CL_Vec2<double>::CL_Vec2(const CL_Vec2<float> &copy) { x = (double) copy.x; y = (double) copy.y; }

template<>
inline CL_Vec2<double>::CL_Vec2(const CL_Vec2<double> &copy) { x = (double) copy.x; y = (double) copy.y; }

template<>
inline CL_Vec2<double>::CL_Vec2(const CL_Vec2<int> &copy) { x = (double) copy.x; y = (double) copy.y; }

//////////////////////////////////////////////////////////////////////////

typedef CL_Vec2<unsigned char> CL_Vec2ub;
typedef CL_Vec2<char> CL_Vec2b;
typedef CL_Vec2<unsigned short> CL_Vec2us;
typedef CL_Vec2<short> CL_Vec2s;
typedef CL_Vec2<unsigned int> CL_Vec2ui;
typedef CL_Vec2<int> CL_Vec2i;
typedef CL_Vec2<float> CL_Vec2f;
typedef CL_Vec2<double> CL_Vec2d;

/// \}

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
#include "vec2.h"
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

/// \brief 3D vector
///
/// These vector templates are defined for:\n
/// char (CL_Vec3c), unsigned char (CL_Vec3uc), short (CL_Vec3s),\n
/// unsigned short (CL_Vec3us), int (CL_Vec3i), unsigned int (CL_Vec3ui), float (CL_Vecdf), double (CL_Vec3d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Vec3
{
public:
	typedef Type datatype;

	union { Type x; Type s; Type r; };
	union { Type y; Type t; Type g; };
	union { Type z; Type u; Type b; };

	CL_Vec3() : x(0), y(0), z(0) { }
	CL_Vec3(const CL_Vec2<Type> &copy) { x = copy.x; y = copy.y; z = 0; }
	CL_Vec3(const CL_Vec1<Type> &copy) { x = copy.x; y = 0; z = 0; }
	CL_Vec3(const CL_Vec4<Type> &copy) { x = copy.x; y = copy.y; z = copy.z; }

	CL_Vec3(const CL_Vec3<double> &copy);
	CL_Vec3(const CL_Vec3<float> &copy);
	CL_Vec3(const CL_Vec3<int> &copy);

	CL_Vec3(const Type &p1, const Type &p2 = 0, const Type &p3 = 0) : x(p1), y(p2), z(p3) { }

	/// \brief Normalizes a vector
	///
	/// \param vector = Vector to use
	///
	/// Operates in the native datatype
	static CL_Vec3<Type> normalize(const CL_Vec3<Type>& vector);

	/// \brief Dot products between two vectors.
	///
	/// \return The dot product
	static Type dot(const CL_Vec3<Type>& vector1, const CL_Vec3<Type>& vector2) { return vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z; }

	/// \brief Calculate the cross product between two vectors.
	///
	/// \param vector1 = The first vector
	/// \param vector2 = The second vector
	/// \return The cross product
	static CL_Vec3<Type> cross(const CL_Vec3<Type>& vector1, const CL_Vec3<Type>& vector2);

	/// \brief Rotate a vector around an axis. Same as glRotate[f|d](angle, a);
	///
	/// \param vector = Vector to use
	/// \param angle = Angle to rotate.
	/// \param axis = Rotation axis.
	/// \return The rotated vector
	static CL_Vec3<Type> rotate(const CL_Vec3<Type>& vector, const CL_Angle &angle, const CL_Vec3<Type>& axis);

	/// \brief Rounds all components on a vector
	///
	/// Uses Asymmetric Arithmetic Rounding
	/// \param vector = Vector to use
	/// \return The rounded vector
	static CL_Vec3<Type> round(const CL_Vec3<Type>& vector);

/// \name Attributes
/// \{

public:
	/// \brief Returns the length (magnitude) of this vector.
	///
	/// Operates in the native datatype
	/// \return the length of the vector
	Type length() const;

	/// \brief Normalizes this vector
	///
	/// Operates in the native datatype
	/// \return reference to this object
	CL_Vec3<Type> &normalize();

	/// \brief Dot products this vector with an other vector.
	///
	/// Operates in the native datatype
	///
	/// \param vector Second vector used for the dot product.
	/// \return The resulting dot product of the two vectors.
	Type dot(const CL_Vec3<Type>& vector) const { return x*vector.x + y*vector.y + z*vector.z; }

	/// \brief Calculate the angle between this vector and an other vector.
	///
	/// \param vector Second vector used to calculate angle.
	///
	/// \return The angle between the two vectors.
	CL_Angle angle(const CL_Vec3<Type>& vector) const;

	/// \brief Calculate the distance between this vector and an other vector.
	///
	/// \param vector = Second vector used to calculate distance.
	///
	/// \return The distance between the two vectors.
	Type distance(const CL_Vec3<Type>& vector) const;

	/// \brief Calculate the cross product between this vector and an other vector.
	///
	/// Operates in the native datatype
	/// \param vector = Second vector used to perform the calculation.
	/// \return reference to this object
	CL_Vec3<Type> &cross(const CL_Vec3<Type>& vector);

	/// \brief Rotate this vector around an axis. Same as glRotate[f|d](angle, a);
	///
	/// \param angle Angle to rotate.
	/// \param axis Rotation axis.
	/// \return reference to this object
	CL_Vec3<Type> &rotate(const CL_Angle &angle, const CL_Vec3<Type>& axis);

	/// \brief Rounds all components on this vector
	///
	//// Uses Asymmetric Arithmetic Rounding
	/// \return reference to this object
	CL_Vec3<Type> &round();

/// \}
/// \name Operators
/// \{

public:
	const Type &operator[](unsigned int i) const { return ((Type *) this)[i]; }
	Type &operator[](unsigned int i) { return ((Type *) this)[i]; }
	operator Type *() { return (Type *) this; }
	operator Type * const() const { return (Type * const) this; }

	/// \brief += operator.
	void operator += (const CL_Vec3<Type>& vector) { x+= vector.x; y+= vector.y; z+= vector.z; }

	/// \brief += operator.
	void operator += ( Type value) { x+= value; y+= value; z+= value; }

	/// \brief + operator.
	CL_Vec3<Type> operator + (const CL_Vec3<Type>& vector) const {return CL_Vec3<Type>(vector.x + x, vector.y + y, vector.z + z);}

	/// \brief + operator.
	CL_Vec3<Type> operator + (Type value) const {return CL_Vec3<Type>(value + x, value + y, value + z);}

	/// \brief -= operator.
	void operator -= (const CL_Vec3<Type>& vector) { x-= vector.x; y-= vector.y; z-= vector.z; }

	/// \brief -= operator.
	void operator -= ( Type value) { x-= value; y-= value; z-= value; }

	/// \brief - operator.
	CL_Vec3<Type> operator - (const CL_Vec3<Type>& vector) const {return CL_Vec3<Type>(x - vector.x, y - vector.y, z - vector.z);}

	/// \brief - operator.
	CL_Vec3<Type> operator - (Type value) const {return CL_Vec3<Type>(x - value, y - value, z - value);}

	/// \brief *= operator.
	void operator *= (const CL_Vec3<Type>& vector) { x*= vector.x; y*= vector.y; z*= vector.z; }

	/// \brief *= operator.
	void operator *= ( Type value) { x*= value; y*= value; z*= value; }

	/// \brief * operator.
	CL_Vec3<Type> operator * (const CL_Vec3<Type>& vector) const {return CL_Vec3<Type>(vector.x * x, vector.y * y, vector.z * z);}

	/// \brief * operator.
	CL_Vec3<Type> operator * (Type value) const {return CL_Vec3<Type>(value * x, value * y, value * z);}

	/// \brief /= operator.
	void operator /= (const CL_Vec3<Type>& vector) { x/= vector.x; y/= vector.y; z/= vector.z; }

	/// \brief /= operator.
	void operator /= ( Type value) { x/= value; y/= value; z/= value; }

	/// \brief / operator.
	CL_Vec3<Type> operator / (const CL_Vec3<Type>& vector) const {return CL_Vec3<Type>(x / vector.x, y / vector.y, z / vector.z);}

	/// \brief / operator.
	CL_Vec3<Type> operator / (Type value) const {return CL_Vec3<Type>(x / value, y / value, z / value);}

	/// \brief = operator.
	CL_Vec3<Type> &operator = (const CL_Vec3<Type>& vector) { x = vector.x; y = vector.y; z = vector.z; return *this; }

	/// \brief == operator.
	bool operator == (const CL_Vec3<Type>& vector) const {return ((x == vector.x) && (y == vector.y) && (z == vector.z));}

	/// \brief != operator.
	bool operator != (const CL_Vec3<Type>& vector) const {return ((x != vector.x) || (y != vector.y) || (z != vector.z));}
/// \}
};

/// Matrix is assumed to be in the Column-Major matrix format (opengl native)\n
/// Note: "vec = vector * matrix"  is different to "vec = matrix * vector"
template<typename Type>
CL_Vec3<Type> operator * (const CL_Vec3<Type>& v, const CL_Mat3<Type>& matrix)
{
	return CL_Vec3<Type>(
		matrix[0*3+0]*v.x + matrix[0*3+1]*v.y + matrix[0*3+2]*v.z,
		matrix[1*3+0]*v.x + matrix[1*3+1]*v.y + matrix[1*3+2]*v.z,
		matrix[2*3+0]*v.x + matrix[2*3+1]*v.y + matrix[2*3+2]*v.z);
}

/// Matrix is assumed to be in the Column-Major matrix format (opengl native)\n
/// Note: "vec = vector * matrix"  is different to "vec = matrix * vector"
template<typename Type>
CL_Vec3<Type> operator * (const CL_Mat3<Type>& matrix, const CL_Vec3<Type>& v)
{
	return CL_Vec3<Type>(
		matrix[0*3+0]*v.x + matrix[1*3+0]*v.y + matrix[2*3+0]*v.z,
		matrix[0*3+1]*v.x + matrix[1*3+1]*v.y + matrix[2*3+1]*v.z,
		matrix[0*3+2]*v.x + matrix[1*3+2]*v.y + matrix[2*3+2]*v.z);
}

template<>
inline CL_Vec3<unsigned char>::CL_Vec3(const CL_Vec3<float> &copy) { x = (unsigned char) floor(copy.x +0.5f); y = (unsigned char) floor(copy.y + 0.5f); z = (unsigned char) floor(copy.z + 0.5f); }

template<>
inline CL_Vec3<unsigned char>::CL_Vec3(const CL_Vec3<double> &copy) { x = (unsigned char) floor(copy.x+0.5); y = (unsigned char) floor(copy.y+0.5); z = (unsigned char) floor(copy.z + 0.5); }

template<>
inline CL_Vec3<unsigned char>::CL_Vec3(const CL_Vec3<int> &copy) { x = (unsigned char) copy.x; y = (unsigned char) copy.y; z = (unsigned char) copy.z; }

template<>
inline CL_Vec3<char>::CL_Vec3(const CL_Vec3<float> &copy) { x = (char) floor(copy.x +0.5f); y = (char) floor(copy.y + 0.5f); z = (char) floor(copy.z + 0.5f); }

template<>
inline CL_Vec3<char>::CL_Vec3(const CL_Vec3<double> &copy) { x = (char) floor(copy.x+0.5); y = (char) floor(copy.y+0.5); z = (char) floor(copy.z + 0.5); }

template<>
inline CL_Vec3<char>::CL_Vec3(const CL_Vec3<int> &copy) { x = (char) copy.x; y = (char) copy.y; z = (char) copy.z; }

template<>
inline CL_Vec3<unsigned short>::CL_Vec3(const CL_Vec3<float> &copy) { x = (unsigned short) floor(copy.x +0.5f); y = (unsigned short) floor(copy.y + 0.5f); z = (unsigned short) floor(copy.z + 0.5f); }

template<>
inline CL_Vec3<unsigned short>::CL_Vec3(const CL_Vec3<double> &copy) { x = (unsigned short) floor(copy.x+0.5); y = (unsigned short) floor(copy.y+0.5); z = (unsigned short) floor(copy.z + 0.5); }

template<>
inline CL_Vec3<unsigned short>::CL_Vec3(const CL_Vec3<int> &copy) { x = (unsigned short) copy.x; y = (unsigned short) copy.y; z = (unsigned short) copy.z; }

template<>
inline CL_Vec3<short>::CL_Vec3(const CL_Vec3<float> &copy) { x = (short) floor(copy.x +0.5f); y = (short) floor(copy.y + 0.5f); z = (short) floor(copy.z + 0.5f); }

template<>
inline CL_Vec3<short>::CL_Vec3(const CL_Vec3<double> &copy) { x = (short) floor(copy.x+0.5); y = (short) floor(copy.y+0.5); z = (short) floor(copy.z + 0.5); }

template<>
inline CL_Vec3<short>::CL_Vec3(const CL_Vec3<int> &copy) { x = (short) copy.x; y = (short) copy.y; z = (short) copy.z; }

template<>
inline CL_Vec3<int>::CL_Vec3(const CL_Vec3<float> &copy) { x = (int) floor(copy.x +0.5f); y = (int) floor(copy.y + 0.5f); z = (int) floor(copy.z + 0.5f); }

template<>
inline CL_Vec3<int>::CL_Vec3(const CL_Vec3<double> &copy) { x = (int) floor(copy.x+0.5); y = (int) floor(copy.y+0.5); z = (int) floor(copy.z + 0.5); }

template<>
inline CL_Vec3<int>::CL_Vec3(const CL_Vec3<int> &copy) { x = (int) copy.x; y = (int) copy.y; z = (int) copy.z; }

template<>
inline CL_Vec3<unsigned int>::CL_Vec3(const CL_Vec3<float> &copy) { x = (unsigned int) floor(copy.x +0.5f); y = (unsigned int) floor(copy.y + 0.5f); z = (unsigned int) floor(copy.z + 0.5f); }

template<>
inline CL_Vec3<unsigned int>::CL_Vec3(const CL_Vec3<double> &copy) { x = (unsigned int) floor(copy.x+0.5); y = (unsigned int) floor(copy.y+0.5); z = (unsigned int) floor(copy.z + 0.5); }

template<>
inline CL_Vec3<unsigned int>::CL_Vec3(const CL_Vec3<int> &copy) { x = (unsigned int) copy.x; y = (unsigned int) copy.y; z = (unsigned int) copy.z; }

template<>
inline CL_Vec3<float>::CL_Vec3(const CL_Vec3<float> &copy) { x = (float) copy.x; y = (float) copy.y; z = (float) copy.z; }

template<>
inline CL_Vec3<float>::CL_Vec3(const CL_Vec3<double> &copy) { x = (float) copy.x; y = (float) copy.y; z = (float) copy.z; }

template<>
inline CL_Vec3<float>::CL_Vec3(const CL_Vec3<int> &copy) { x = (float) copy.x; y = (float) copy.y; z = (float) copy.z; }

template<>
inline CL_Vec3<double>::CL_Vec3(const CL_Vec3<float> &copy) { x = (double) copy.x; y = (double) copy.y; z = (double) copy.z; }

template<>
inline CL_Vec3<double>::CL_Vec3(const CL_Vec3<double> &copy) { x = (double) copy.x; y = (double) copy.y; z = (double) copy.z; }

template<>
inline CL_Vec3<double>::CL_Vec3(const CL_Vec3<int> &copy) { x = (double) copy.x; y = (double) copy.y; z = (double) copy.z; }

typedef CL_Vec3<unsigned char> CL_Vec3ub;
typedef CL_Vec3<char> CL_Vec3b;
typedef CL_Vec3<unsigned short> CL_Vec3us;
typedef CL_Vec3<short> CL_Vec3s;
typedef CL_Vec3<unsigned int> CL_Vec3ui;
typedef CL_Vec3<int> CL_Vec3i;
typedef CL_Vec3<float> CL_Vec3f;
typedef CL_Vec3<double> CL_Vec3d;

/// \}

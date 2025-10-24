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
#include "vec3.h"

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

/// \brief 4D vector
///
/// These vector templates are defined for:\n
/// char (CL_Vec4c), unsigned char (CL_Vec4uc), short (CL_Vec4s),\n
/// unsigned short (CL_Vec4us), int (CL_Vec4i), unsigned int (CL_Vec4ui), float (CL_Vec4f), double (CL_Vec4d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Vec4
{
public:
	typedef Type datatype;

	union { Type x; Type s; Type r; };
	union { Type y; Type t; Type g; };
	union { Type z; Type u; Type b; };
	union { Type w; Type v; Type a; };

	CL_Vec4() : x(0), y(0), z(0), w(0) { }
	CL_Vec4(const CL_Vec1<Type> &copy) { x = copy.x; y = 0; z = 0; w = 0; }
	CL_Vec4(const CL_Vec2<Type> &copy) { x = copy.x; y = copy.y; z = 0; w = 0; }
	CL_Vec4(const CL_Vec3<Type> &copy) { x = copy.x; y = copy.y; z = copy.z; w = 0; }
	CL_Vec4(const Type &p1, const Type &p2 = 0, const Type &p3 = 0, const Type &p4 = 0) : x(p1), y(p2), z(p3), w(p4) { }

	/// \brief Normalizes a vector (not taking into account the w ordinate)
	///
	/// Operates in the native datatype
	/// \param vector = The vector to use
	/// \return the normalized vector
	static CL_Vec4<Type> normalize3(const CL_Vec4<Type> &vector);

	/// \brief Normalizes a vector (taking into account the w ordinate)
	///
	/// Operates in the native datatype
	/// \param vector = The vector to use
	/// \return the normalized vector
	static CL_Vec4<Type> normalize4(const CL_Vec4<Type> &vector);

	/// \brief Dot products between two vectors (not taking into account the w ordinate).
	///
	/// Operates in the native datatype
	///
	/// \param vector1 First vector used for the dot product.
	/// \param vector2 Second vector used for the dot product.
	/// \return The resulting dot product of the two vectors.
	static Type dot3(const CL_Vec4<Type>& vector1, const CL_Vec4<Type>& vector2) { return vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z; }

	/// \brief Dot products between two vectors (taking into account the w ordinate).
	///
	/// Operates in the native datatype
	///
	/// \param vector1 First vector used for the dot product.
	/// \param vector2 Second vector used for the dot product.
	/// \return The resulting dot product of the two vectors.
	static Type dot4(const CL_Vec4<Type>& vector1, const CL_Vec4<Type>& vector2) { return vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z + vector1.w*vector2.w; }

	/// \brief Calculate the cross product between two vectors (not taking into account the w ordinate).
	///
	/// \vector1 = The first vector
	/// \vector2 = The second vector
	/// \return = Cross product
	static CL_Vec4<Type> cross3(const CL_Vec4<Type>& vector1, const CL_Vec4<Type>& vector2);

	/// \brief Rotate a vector around an axis. Same as glRotate[f|d](angle, a);
	///
	/// Not taking into account the w ordinate\n
	///  The w ordinate is not modified
	///
	/// \param vector = The vector to use
	/// \param angle Angle to rotate
	/// \param axis Rotation axis.
	/// \return The rotated vector
	static CL_Vec4<Type> rotate3(const CL_Vec4<Type>& vector, const CL_Angle &angle, const CL_Vec4<Type>& axis);

	/// \brief Rounds all components on a vector.
	///
	/// Includes the w ordinate\n
	/// Uses Asymmetric Arithmetic Rounding
	/// \param vector = The vector to use
	/// \return The rounded vector
	static CL_Vec4<Type> round(const CL_Vec4<Type>& vector);

/// \name Attributes
/// \{

public:
	/// \brief Returns the length (magnitude) of this vector (not taking into account the w ordinate).
	///
	/// Operates in the native datatype
	///
	/// \return the length of the vector */
	Type length3() const;

	/// \brief Returns the length (magnitude) of this vector (taking into account the w ordinate).
	///
	/// Operates in the native datatype
	///
	/// \return the length of the vector */
	Type length4() const;

	/// \brief Normalizes this vector (not taking into account the w ordinate)
	///
	/// Operates in the native datatype
	/// \return reference to this object
	CL_Vec4<Type> &normalize3();

	/// \brief Normalizes this vector (taking into account the w ordinate)
	///
	/// Operates in the native datatype
	/// \return reference to this object
	CL_Vec4<Type> &normalize4();

	/// \brief Dot products this vector with an other vector (not taking into account the w ordinate).
	///
	/// Operates in the native datatype
	///
	/// \param vector Second vector used for the dot product.
	/// \return The resulting dot product of the two vectors.
	Type dot3(const CL_Vec4<Type>& vector) const {return x*vector.x + y*vector.y + z*vector.z;}

	/// \brief Dot products this vector with an other vector (taking into account the w ordinate).
	///
	/// Operates in the native datatype
	///
	/// \param vector Second vector used for the dot product.
	/// \return The resulting dot product of the two vectors.
	Type dot4(const CL_Vec4<Type>& vector) const {return x*vector.x + y*vector.y + z*vector.z + w*vector.w;}

	/// \brief Calculate the angle between this vector and an other vector (not taking into account the w ordinate).
	///
	/// \param vector = Second vector used to calculate angle.
	///
	/// \return The angle between the two vectors.
	CL_Angle angle3(const CL_Vec4<Type>& vector) const;

	/// \brief Calculate the distance between this vector and an other vector (not taking into account the w ordinate).
	///
	/// \param vector = Second vector used to calculate distance.
	///
	/// \return The distance between the two vectors.
	Type distance3(const CL_Vec4<Type>& vector) const;

	/// \brief Calculate the distance between this vector and an other vector (taking into account the w ordinate).
	///
	/// \param vector = Second vector used to calculate distance.
	///
	/// \return The distance between the two vectors.
	Type distance4(const CL_Vec4<Type>& vector) const;

	/// \brief Calculate the cross product between this vector and an other vector (not taking into account the w ordinate).
	///
	/// Operates in the native datatype
	///
	/// \param vector Second vector used to perform the calculation.
	/// \return reference to this object
	CL_Vec4<Type> &cross3(const CL_Vec4<Type>& vector);

	/// \brief Rotate this vector around an axis. Same as glRotate[f|d](angle, a);
	///
	/// Not taking into account the w ordinate\n
	///  The w ordinate is not modified
	///
	/// \param angle Angle to rotate
	/// \param axis Rotation axis.
	/// \return reference to this object
	CL_Vec4<Type> &rotate3(const CL_Angle &angle, const CL_Vec4<Type>& axis);

	/// \brief Rounds all components on this vector.
	///
	/// Includes the w ordinate\n
	/// Uses Asymmetric Arithmetic Rounding
	/// \return reference to this object
	CL_Vec4<Type> &round();

/// \}
/// \name Operators
/// \{

public:
	const Type &operator[](unsigned int i) const { return ((Type *) this)[i]; }
	Type &operator[](unsigned int i) { return ((Type *) this)[i]; }
	operator Type *() { return (Type *) this; }
	operator Type * const() const { return (Type * const) this; }

	/// \brief += operator.
	void operator += (const CL_Vec4<Type>& vector) { x+= vector.x; y+= vector.y; z+= vector.z; w+= vector.w; }

	/// \brief += operator.
	void operator += ( Type value) { x+= value; y+= value; z+= value; w+= value; }

	/// \brief + operator.
	CL_Vec4<Type> operator + (const CL_Vec4<Type>& vector) const {return CL_Vec4<Type>(vector.x + x, vector.y + y, vector.z + z, vector.w + w);}

	/// \brief + operator.
	CL_Vec4<Type> operator + (Type value) const {return CL_Vec4<Type>(value + x, value + y, value + z, value + w);}

	/// \brief -= operator.
	void operator -= (const CL_Vec4<Type>& vector) { x-= vector.x; y-= vector.y; z-= vector.z; w-= vector.w; }

	/// \brief -= operator.
	void operator -= ( Type value) { x-= value; y-= value; z-= value; w-= value; }

	/// \brief - operator.
	CL_Vec4<Type> operator - (const CL_Vec4<Type>& vector) const {return CL_Vec4<Type>(x - vector.x, y - vector.y, z - vector.z, w - vector.w);}

	/// \brief - operator.
	CL_Vec4<Type> operator - (Type value) const {return CL_Vec4<Type>(x - value, y - value, z - value, w - value);}

	/// \brief *= operator.
	void operator *= (const CL_Vec4<Type>& vector) { x*= vector.x; y*= vector.y; z*= vector.z; w*= vector.w; }

	/// \brief *= operator.
	void operator *= ( Type value) { x*= value; y*= value; z*= value; w*= value; }

	/// \brief * operator.
	CL_Vec4<Type> operator * (const CL_Vec4<Type>& vector) const {return CL_Vec4<Type>(vector.x * x, vector.y * y, vector.z * z, vector.w * w);}

	/// \brief * operator.
	CL_Vec4<Type> operator * (Type value) const {return CL_Vec4<Type>(value * x, value * y, value * z, value * w);}

	/// \brief /= operator.
	void operator /= (const CL_Vec4<Type>& vector) { x/= vector.x; y/= vector.y; z/= vector.z; w/= vector.w; }

	/// \brief /= operator.
	void operator /= ( Type value) { x/= value; y/= value; z/= value; w/= value; }

	/// \brief / operator.
	CL_Vec4<Type> operator / (const CL_Vec4<Type>& vector) const {return CL_Vec4<Type>(x / vector.x, y / vector.y, z / vector.z, w / vector.w);}

	/// \brief / operator.
	CL_Vec4<Type> operator / (Type value) const {return CL_Vec4<Type>(x / value, y / value, z / value, w / value);}

	/// \brief = operator.
	CL_Vec4<Type> &operator = (const CL_Vec4<Type>& vector) { x = vector.x; y = vector.y; z = vector.z; w = vector.w; return *this; }

	/// \brief == operator.
	bool operator == (const CL_Vec4<Type>& vector) const {return ((x == vector.x) && (y == vector.y) && (z == vector.z) && (w == vector.w));}

	/// \brief != operator.
	bool operator != (const CL_Vec4<Type>& vector) const {return ((x != vector.x) || (y != vector.y) || (z != vector.z) || (w != vector.w));}
/// \}
};

template<typename Type>
CL_Vec4<Type> operator * (const CL_Vec4<Type>& v, const CL_Mat4<Type>& matrix)
{
	return CL_Vec4<Type>(
		matrix[0*4+0]*v.x + matrix[0*4+1]*v.y + matrix[0*4+2]*v.z + matrix[0*4+3]*v.w,
		matrix[1*4+0]*v.x + matrix[1*4+1]*v.y + matrix[1*4+2]*v.z + matrix[1*4+3]*v.w,
		matrix[2*4+0]*v.x + matrix[2*4+1]*v.y + matrix[2*4+2]*v.z + matrix[2*4+3]*v.w,
		matrix[3*4+0]*v.x + matrix[3*4+1]*v.y + matrix[3*4+2]*v.z + matrix[3*4+3]*v.w);
}

template<typename Type>
CL_Vec4<Type> operator * (const CL_Mat4<Type>& matrix, const CL_Vec4<Type>& v)
{
	return CL_Vec4<Type>(
		matrix[0*4+0]*v.x + matrix[1*4+0]*v.y + matrix[2*4+0]*v.z + matrix[3*4+0]*v.w,
		matrix[0*4+1]*v.x + matrix[1*4+1]*v.y + matrix[2*4+1]*v.z + matrix[3*4+1]*v.w,
		matrix[0*4+2]*v.x + matrix[1*4+2]*v.y + matrix[2*4+2]*v.z + matrix[3*4+2]*v.w,
		matrix[0*4+3]*v.x + matrix[1*4+3]*v.y + matrix[2*4+3]*v.z + matrix[3*4+3]*v.w);
}

typedef CL_Vec4<unsigned char> CL_Vec4ub;
typedef CL_Vec4<char> CL_Vec4b;
typedef CL_Vec4<unsigned short> CL_Vec4us;
typedef CL_Vec4<short> CL_Vec4s;
typedef CL_Vec4<unsigned int> CL_Vec4ui;
typedef CL_Vec4<int> CL_Vec4i;
typedef CL_Vec4<float> CL_Vec4f;
typedef CL_Vec4<double> CL_Vec4d;

/// \}

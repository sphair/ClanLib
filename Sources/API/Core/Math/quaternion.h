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

#include "vec3.h"
#include "angle.h"

/// \brief Quaternion
///
/// These quaternion templates are defined for: float (CL_Quaternionf), double (CL_Quaterniond)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Quaternionx
{
public:
	/// \brief The real scalar part
	Type w;

	/// \brief The imaginary vector part
	union {	Type i; Type x; };
	union {	Type j; Type y; };
	union {	Type k; Type z; };

	CL_Quaternionx() : w(1), i(0), j(0), k(0) { }
	CL_Quaternionx(Type real, Type i, Type j, Type k) : w(real), i(i), j(j), k(k) { }
	CL_Quaternionx(Type real, const CL_Vec3<Type> &imag) : w(real), i(imag.x), j(imag.y), k(imag.z) { }
	CL_Quaternionx(const CL_Quaternionx<Type> &copy) : w(copy.w), i(copy.i), j(copy.j), k(copy.k) { }
	CL_Quaternionx(Type euler_x, Type euler_y, Type euler_z, CL_AngleUnit unit, CL_EulerOrder order);
	CL_Quaternionx(const CL_Vec3<Type> &euler, CL_AngleUnit unit, CL_EulerOrder order);
	CL_Quaternionx(const CL_Angle &euler_x, const CL_Angle &euler_y, const CL_Angle &euler_z, CL_EulerOrder order);

	static CL_Quaternionx<Type> axis_angle(const CL_Angle &angle, const CL_Vec3f &axis);
	static CL_Quaternionx<Type> multiply(const CL_Quaternionx<Type> &quaternion_1, const CL_Quaternionx<Type> &quaternion_2);


	/// \brief Linear Quaternion Interpolation
	///
	/// \param quaternion_initial = Source quaternion
	/// \param quaternion_final = Destination quaternion
	/// \param lerp_time = Time in the range of 0.0 to 1.0
	static CL_Quaternionx<Type> lerp(const CL_Quaternionx<Type> &quaternion_initial, const CL_Quaternionx<Type> &quaternion_final, Type lerp_time);

	/// \brief Spherical Quaternion Interpolation
	///
	/// \param quaternion_initial = Source quaternion
	/// \param quaternion_final = Destination quaternion
	/// \param slerp_time = Time in the range of 0.0 to 1.0
	static CL_Quaternionx<Type> slerp(const CL_Quaternionx<Type> &quaternion_initial, const CL_Quaternionx<Type> &quaternion_final, Type slerp_time);

/// \name Attributes
/// \{
public:
	/// \brief Convert the quaternion to a rotation matrix
	CL_Mat4<Type> to_matrix() const;

	/// \brief Get the quaternion magnitude
	Type magnitude() const;

/// \}
/// \name Operations
/// \{
public:
	void set(Type euler_x, Type euler_y, Type euler_z, CL_AngleUnit unit, CL_EulerOrder order);
	void set(const CL_Vec3<Type> &euler, CL_AngleUnit unit, CL_EulerOrder order);
	void set(const CL_Angle &euler_x, const CL_Angle &euler_y, const CL_Angle &euler_z, CL_EulerOrder order);

	/// \brief Multiply this quaternion by a specified matrix.
	///
	/// \param matrix = Matrix to multiply
	///
	/// \return reference to this object
	CL_Quaternionx<Type> &multiply(const CL_Mat4<Type> &matrix);	

	/// \brief Multiply this quaternion by another quaternion.
	///
	/// This multiplies the quaternion as follows: this = quaternion * this
	///
	/// \param quaternion = Quaternion to multiply
	///
	/// \return reference to this object
	CL_Quaternionx<Type> &multiply(const CL_Quaternionx<Type> &quaternion);

	CL_Quaternionx<Type> &rotate(const CL_Angle &angle, const CL_Vec3f &axis);

	CL_Quaternionx<Type> &rotate(const CL_Angle &euler_x, const CL_Angle &euler_y, const CL_Angle &euler_z, CL_EulerOrder order);

	/// \brief Normalizes this quaternion
	///
	/// \return reference to this object
	CL_Quaternionx<Type> &normalize();

	/// \brief Inverse this quaternion
	///
	/// This is the same as the conjugate of a quaternion
	///
	/// \return reference to this object
	CL_Quaternionx<Type> &inverse();

/// \}
/// \name Operators
/// \{
public:
	/// \brief Multiplication operator.
	CL_Quaternionx<Type> operator *(const CL_Quaternionx<Type> &mult) const { CL_Quaternionx<Type> result = mult; result.multiply(*this); return result; }

/// \}
};

/// \brief Quaternion - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Quaternionf : public CL_Quaternionx<float>
{
public:
	CL_Quaternionf() : CL_Quaternionx<float>() { }
	CL_Quaternionf(const CL_Quaternionx<float> &copy) : CL_Quaternionx<float>(copy) { }
	CL_Quaternionf(float real, float i, float j, float k) : CL_Quaternionx<float>(real, i, j, k) { }
	CL_Quaternionf(float real, const CL_Vec3<float> &imag) : CL_Quaternionx<float>(real, imag) { }
	CL_Quaternionf(float euler_x, float euler_y, float euler_z, CL_AngleUnit unit, CL_EulerOrder order) : CL_Quaternionx<float>(euler_x, euler_y, euler_z, unit, order) { }
	CL_Quaternionf(const CL_Vec3<float> &euler, CL_AngleUnit unit, CL_EulerOrder order) : CL_Quaternionx<float>(euler, unit, order) { }
	CL_Quaternionf(const CL_Angle &euler_x, const CL_Angle &euler_y, const CL_Angle &euler_z, CL_EulerOrder order) : CL_Quaternionx<float>(euler_x, euler_y, euler_z, order) { }

};

/// \brief Quaternion - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Quaterniond : public CL_Quaternionx<double>
{
public:
	CL_Quaterniond() : CL_Quaternionx<double>() { }
	CL_Quaterniond(const CL_Quaternionx<double> &copy) : CL_Quaternionx<double>(copy) { }
	CL_Quaterniond(double real, double i, double j, double k) : CL_Quaternionx<double>(real, i, j, k) { }
	CL_Quaterniond(double real, const CL_Vec3<double> &imag) : CL_Quaternionx<double>(real, imag) { }
	CL_Quaterniond(double euler_x, double euler_y, double euler_z, CL_AngleUnit unit, CL_EulerOrder order) : CL_Quaternionx<double>(euler_x, euler_y, euler_z, unit, order) { }
	CL_Quaterniond(const CL_Vec3<double> &euler, CL_AngleUnit unit, CL_EulerOrder order) : CL_Quaternionx<double>(euler, unit, order) { }
	CL_Quaterniond(const CL_Angle &euler_x, const CL_Angle &euler_y, const CL_Angle &euler_z, CL_EulerOrder order) : CL_Quaternionx<double>(euler_x, euler_y, euler_z, order) { }
};


/// \}

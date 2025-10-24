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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/Math/quaternion.h"
#include "API/Core/Math/mat4.h"

template<typename Type>
CL_Quaternionx<Type>::CL_Quaternionx(Type euler_x, Type euler_y, Type euler_z, CL_AngleUnit unit, CL_EulerOrder order)
{
	set(euler_x, euler_y, euler_z, unit, order);
}

template<typename Type>
CL_Quaternionx<Type>::CL_Quaternionx(const CL_Vec3<Type> &euler, CL_AngleUnit unit, CL_EulerOrder order)
{
	set(euler.x, euler.y, euler.z, unit, order);
}

template<typename Type>
CL_Quaternionx<Type>::CL_Quaternionx(const CL_Angle &euler_x, const CL_Angle &euler_y, const CL_Angle &euler_z, CL_EulerOrder order)
{
	set(euler_x, euler_y, euler_z, order);
}

template<typename Type>
void CL_Quaternionx<Type>::set(const CL_Vec3<Type> &euler, CL_AngleUnit unit, CL_EulerOrder order)
{
	set(euler.x, euler.y, euler.z, unit, order);
}

template<typename Type>
void CL_Quaternionx<Type>::set(const CL_Angle &euler_x, const CL_Angle &euler_y, const CL_Angle &euler_z, CL_EulerOrder order)
{
	set( euler_x.to_radians(), euler_y.to_radians(), euler_z.to_radians(), cl_radians, order );
}

template<typename Type>
void CL_Quaternionx<Type>::set(Type euler_x, Type euler_y, Type euler_z, CL_AngleUnit unit, CL_EulerOrder order)
{
	CL_Quaternionx<Type> q_x = CL_Quaternionx<Type>::axis_angle( CL_Angle(euler_x, unit), CL_Vec3f(1.0f, 0.0f, 0.0f));
	CL_Quaternionx<Type> q_y = CL_Quaternionx<Type>::axis_angle( CL_Angle(euler_y, unit), CL_Vec3f(0.0f, 1.0f, 0.0f));
	CL_Quaternionx<Type> q_z = CL_Quaternionx<Type>::axis_angle( CL_Angle(euler_z, unit), CL_Vec3f(0.0f, 0.0f, 1.0f));

	switch (order)
	{
		case cl_XYZ:
			*this = CL_Quaternionx<Type>::multiply(CL_Quaternionx<Type>::multiply(q_x, q_y), q_z);
			break;
		case cl_XZY:
			*this = CL_Quaternionx<Type>::multiply(CL_Quaternionx<Type>::multiply(q_x, q_z), q_y);
			break;
		case cl_YZX:
			*this = CL_Quaternionx<Type>::multiply(CL_Quaternionx<Type>::multiply(q_y, q_z), q_x);
			break;
		case cl_YXZ:
			*this = CL_Quaternionx<Type>::multiply(CL_Quaternionx<Type>::multiply(q_y, q_x), q_z);
			break;
		case cl_ZXY:
			*this = CL_Quaternionx<Type>::multiply(CL_Quaternionx<Type>::multiply(q_z, q_x), q_y);
			break;
		case cl_ZYX:
			*this = CL_Quaternionx<Type>::multiply(CL_Quaternionx<Type>::multiply(q_z, q_y), q_x);
			break;
		default:
			throw CL_Exception("Unknown euler order");
	}
}

template<typename Type>
CL_Quaternionx<Type> CL_Quaternionx<Type>::multiply(const CL_Quaternionx<Type> &quaternion_1, const CL_Quaternionx<Type> &quaternion_2)
{
	CL_Quaternionx<Type> quaternion_dest;
	quaternion_dest.x = quaternion_1.w*quaternion_2.x + quaternion_1.x*quaternion_2.w + quaternion_1.y*quaternion_2.z - quaternion_1.z*quaternion_2.y;
	quaternion_dest.y = quaternion_1.w*quaternion_2.y + quaternion_1.y*quaternion_2.w + quaternion_1.z*quaternion_2.x - quaternion_1.x*quaternion_2.z;
	quaternion_dest.z = quaternion_1.w*quaternion_2.z + quaternion_1.z*quaternion_2.w + quaternion_1.x*quaternion_2.y - quaternion_1.y*quaternion_2.x;
	quaternion_dest.w = quaternion_1.w*quaternion_2.w - quaternion_1.x*quaternion_2.x - quaternion_1.y*quaternion_2.y - quaternion_1.z*quaternion_2.z;
	return quaternion_dest;
}

template<typename Type>
CL_Quaternionx<Type> &CL_Quaternionx<Type>::multiply(const CL_Mat4<Type> &m)
{
	CL_Quaternionx<Type> result;

	result.i = m.matrix[ (4*0) + 0 ]*i + m.matrix[ (4*0) + 1 ]*j + m.matrix[ (4*0) + 2 ]*k + m.matrix[ (4*0) + 3 ]*w;
	result.j = m.matrix[ (4*1) + 0 ]*i + m.matrix[ (4*1) + 1 ]*j + m.matrix[ (4*1) + 2 ]*k + m.matrix[ (4*1) + 3 ]*w;
	result.k = m.matrix[ (4*2) + 0 ]*i + m.matrix[ (4*2) + 1 ]*j + m.matrix[ (4*2) + 2 ]*k + m.matrix[ (4*2) + 3 ]*w;
	result.w = m.matrix[ (4*3) + 0 ]*i + m.matrix[ (4*3) + 1 ]*j + m.matrix[ (4*3) + 2 ]*k + m.matrix[ (4*3) + 3 ]*w;

	*this = result;
	return *this;
}

template<typename Type>
CL_Mat4<Type> CL_Quaternionx<Type>::to_matrix() const
{
	CL_Mat4<Type> m = CL_Mat4<Type>::null();
	m.matrix[0*4+0] = 1 - 2*y*y - 2*z*z;
	m.matrix[1*4+0] = 2*x*y - 2*w*z;
	m.matrix[2*4+0] = 2*x*z + 2*w*y;
	m.matrix[0*4+1] = 2*x*y + 2*w*z;
	m.matrix[1*4+1] = 1 - 2*x*x - 2*z*z;
	m.matrix[2*4+1] = 2*y*z - 2*w*x;
	m.matrix[0*4+2] = 2*x*z - 2*w*y;
	m.matrix[1*4+2] = 2*y*z + 2*w*x;
	m.matrix[2*4+2] = 1 - 2*x*x - 2*y*y;
	m.matrix[3*4+3] = 1;
	return m;
}

template<typename Type>
CL_Quaternionx<Type> &CL_Quaternionx<Type>::normalize()
{
	Type magnitude = sqrt(x*x+y*y+z*z+w*w);
	if (magnitude != (Type) 0.0)
	{
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
		w /= magnitude;
	}
	else
	{
		x = y = z = w = (Type) 0.0;
	}
	return *this;
}

template<typename Type>
CL_Quaternionx<Type> CL_Quaternionx<Type>::axis_angle(const CL_Angle &angle, const CL_Vec3f &axis)
{
	Type length = axis.length();
	Type half_angle_radians = angle.to_radians() / (Type) 2.0;
	CL_Quaternionx<Type> quaternion;
	quaternion.w = cos(half_angle_radians);
	quaternion.x = axis.x * sin(half_angle_radians) / length;
	quaternion.y = axis.y * sin(half_angle_radians) / length;
	quaternion.z = axis.z * sin(half_angle_radians) / length;
	quaternion.normalize();
	return quaternion;
}

template<typename Type>
CL_Quaternionx<Type> &CL_Quaternionx<Type>::inverse()
{
	i = -i;
	j = -j;
	k = -k;
	return *this;
}

template<typename Type>
Type CL_Quaternionx<Type>::magnitude() const
{
	return sqrt(x*x+y*y+z*z+w*w);
}

template<typename Type>
CL_Quaternionx<Type> &CL_Quaternionx<Type>::multiply(const CL_Quaternionx<Type> &quaternion)
{
	*this = multiply(quaternion, *this);
	return *this;

}

template<typename Type>
CL_Quaternionx<Type> &CL_Quaternionx<Type>::rotate(const CL_Angle &angle, const CL_Vec3f &axis)
{
	return multiply(axis_angle(angle, axis));
}

template<typename Type>
CL_Quaternionx<Type> &CL_Quaternionx<Type>::rotate(const CL_Angle &euler_x, const CL_Angle &euler_y, const CL_Angle &euler_z, CL_EulerOrder order)
{
	CL_Quaternionx<Type> quaternion(euler_x, euler_y, euler_z, order);
	return multiply(quaternion);
}

template<typename Type>
CL_Quaternionx<Type> CL_Quaternionx<Type>::lerp(const CL_Quaternionx<Type> &quaternion_initial, const CL_Quaternionx<Type> &quaternion_final, Type lerp_time)
{
	CL_Quaternionx<Type> quaternion(
		quaternion_initial.w * (((Type) 1.0) - lerp_time) + quaternion_final.w * lerp_time,
		quaternion_initial.x * (((Type) 1.0) - lerp_time) + quaternion_final.x * lerp_time,
		quaternion_initial.y * (((Type) 1.0) - lerp_time) + quaternion_final.y * lerp_time,
		quaternion_initial.z * (((Type) 1.0) - lerp_time) + quaternion_final.z * lerp_time
	);
	quaternion.normalize();
	return quaternion;
}

template<typename Type>
CL_Quaternionx<Type> CL_Quaternionx<Type>::slerp(const CL_Quaternionx<Type> &quaternion_initial, const CL_Quaternionx<Type> &quaternion_final, Type slerp_time)
{
	Type q2[4];

	q2[0] = quaternion_final.x;
	q2[1] = quaternion_final.y;
	q2[2] = quaternion_final.z;
	q2[3] = quaternion_final.w;

	Type cos_theta = quaternion_initial.x * quaternion_final.x 
		+ quaternion_initial.y * quaternion_final.y 
		+ quaternion_initial.z * quaternion_final.z 
		+ quaternion_initial.w * quaternion_final.w;

	if (cos_theta < ((Type) 0.0))
	{
		q2[0] = -q2[0];
		q2[1] = -q2[1];
		q2[2] = -q2[2];
		q2[3] = -q2[3];
		cos_theta = -cos_theta;
	}

	Type beta = ((Type) 1.0) - slerp_time;

	if ( ((Type) 1.0) - cos_theta > ((Type) 0.001))
	{
		cos_theta = acos(cos_theta);
		Type sin_theta = 1.0f / sin(cos_theta);
		beta = sin(cos_theta * beta) * sin_theta;
		slerp_time = sin(cos_theta * slerp_time) * sin_theta;
	}

	CL_Quaternionx<Type> quaternion;
	quaternion.x = beta * quaternion_initial.x + slerp_time * q2[0];
	quaternion.y = beta * quaternion_initial.y + slerp_time * q2[1];
	quaternion.z = beta * quaternion_initial.z + slerp_time * q2[2];
	quaternion.w = beta * quaternion_initial.w + slerp_time * q2[3];
	return quaternion;
}

// Explicit instantiate the versions we use:

template class CL_Quaternionx<float>;
template class CL_Quaternionx<double>;


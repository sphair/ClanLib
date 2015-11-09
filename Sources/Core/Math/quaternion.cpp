/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/Math/quaternion.h"
#include "API/Core/Math/mat4.h"
#include <limits> // FLT_EPSILON
#include <cfloat> // FLT_EPSILON on linux

namespace clan
{
	template<typename Type>
	Quaternionx<Type>::Quaternionx(const Mat4<Type> &rotation_matrix)
	{
		Type size;

		// We assume that this is a pure rotation matrix without any scale or translation
		Type trace = rotation_matrix.matrix[0 * 4 + 0] + rotation_matrix.matrix[1 * 4 + 1] + rotation_matrix.matrix[2 * 4 + 2] + (Type)1;
		if (trace > (Type) 16.0*FLT_EPSILON)
		{
			size = sqrt(trace) * (Type)2;
			x = (rotation_matrix.matrix[1 * 4 + 2] - rotation_matrix.matrix[2 * 4 + 1]) / size;
			y = (rotation_matrix.matrix[2 * 4 + 0] - rotation_matrix.matrix[0 * 4 + 2]) / size;
			z = (rotation_matrix.matrix[0 * 4 + 1] - rotation_matrix.matrix[1 * 4 + 0]) / size;
			w = (Type) 0.25 * size;
		}
		else
		{
			// If the trace of the matrix is equal to zero then identify which major diagonal element has the greatest value.

			if (rotation_matrix.matrix[0 * 4 + 0] > rotation_matrix.matrix[1 * 4 + 1] && rotation_matrix.matrix[0 * 4 + 0] > rotation_matrix.matrix[2 * 4 + 2])
			{	// Column 0: 
				size = sqrt((Type) 1.0 + rotation_matrix.matrix[0 * 4 + 0] - rotation_matrix.matrix[1 * 4 + 1] - rotation_matrix.matrix[2 * 4 + 2]) * (Type)2;
				x = (Type) 0.25 * size;
				y = (rotation_matrix.matrix[0 * 4 + 1] + rotation_matrix.matrix[1 * 4 + 0]) / size;
				z = (rotation_matrix.matrix[2 * 4 + 0] + rotation_matrix.matrix[0 * 4 + 2]) / size;
				w = (rotation_matrix.matrix[1 * 4 + 2] - rotation_matrix.matrix[2 * 4 + 1]) / size;
			}
			else if (rotation_matrix.matrix[1 * 4 + 1] > rotation_matrix.matrix[2 * 4 + 2])
			{			// Column 1: 
				size = sqrt((Type) 1.0 + rotation_matrix.matrix[1 * 4 + 1] - rotation_matrix.matrix[0 * 4 + 0] - rotation_matrix.matrix[2 * 4 + 2]) * (Type)2;
				x = (rotation_matrix.matrix[0 * 4 + 1] + rotation_matrix.matrix[1 * 4 + 0]) / size;
				y = (Type) 0.25 * size;
				z = (rotation_matrix.matrix[1 * 4 + 2] + rotation_matrix.matrix[2 * 4 + 1]) / size;
				w = (rotation_matrix.matrix[2 * 4 + 0] - rotation_matrix.matrix[0 * 4 + 2]) / size;
			}
			else
			{						// Column 2:
				size = sqrt((Type) 1.0 + rotation_matrix.matrix[2 * 4 + 2] - rotation_matrix.matrix[0 * 4 + 0] - rotation_matrix.matrix[1 * 4 + 1]) * (Type)2;
				x = (rotation_matrix.matrix[2 * 4 + 0] + rotation_matrix.matrix[0 * 4 + 2]) / size;
				y = (rotation_matrix.matrix[1 * 4 + 2] + rotation_matrix.matrix[2 * 4 + 1]) / size;
				z = (Type) 0.25 * size;
				w = (rotation_matrix.matrix[0 * 4 + 1] - rotation_matrix.matrix[1 * 4 + 0]) / size;
			}
		}
	}

	template<typename Type>
	Quaternionx<Type> Quaternionx<Type>::rotation_between(Vec3<Type> v0, Vec3<Type> v1)
	{
		v0.normalize();
		v1.normalize();

		Type d = Vec3<Type>::dot(v0, v1);
		if (d >= 1.0f) // v0 and v1 is the same vector
		{
			return Quaternionx<Type>();
		}
		else if (d < Type(1e-6 - 1.0)) // v0 and v1 are parallel but pointing in opposite directions
		{
			// We can rotate around any axis - find one using the cross product:
			Vec3<Type> axis = Vec3<Type>::cross(Vec3<Type>(Type(1), Type(0), Type(0)), v0);
			if (Vec3<Type>::dot(axis, axis) < Type(1e-6)) // colinear
				axis = Vec3<Type>::cross(Vec3<Type>(Type(0), Type(1), Type(0)), v0);
			axis.normalize();
			return axis_angle(Angle(180.0f, angle_degrees), axis);
		}
		else
		{
			Type s = std::sqrt((1 + d) * 2);
			Type rcp_s = Type(1) / s;
			Vec3<Type> c = Vec3<Type>::cross(v0, v1);
			Quaternionx<Type> q(s * Type(0.5), c * rcp_s);
			q.normalize();
			return q;
		}
	}

	template<typename Type>
	Quaternionx<Type> Quaternionx<Type>::rotation_between(Vec4<Type> v0, Vec4<Type> v1)
	{
		return rotation_between(Vec3<Type>(v0.x, v0.y, v0.z), Vec3<Type>(v1.x, v1.y, v1.z));
	}

	template<typename Type>
	Quaternionx<Type>::Quaternionx(Type euler_x, Type euler_y, Type euler_z, AngleUnit unit, EulerOrder order)
	{
		set(euler_x, euler_y, euler_z, unit, order);
	}

	template<typename Type>
	Quaternionx<Type>::Quaternionx(const Vec3<Type> &euler, AngleUnit unit, EulerOrder order)
	{
		set(euler.x, euler.y, euler.z, unit, order);
	}

	template<typename Type>
	Quaternionx<Type>::Quaternionx(const Angle &euler_x, const Angle &euler_y, const Angle &euler_z, EulerOrder order)
	{
		set(euler_x, euler_y, euler_z, order);
	}

	template<typename Type>
	void Quaternionx<Type>::set(const Vec3<Type> &euler, AngleUnit unit, EulerOrder order)
	{
		set(euler.x, euler.y, euler.z, unit, order);
	}

	template<typename Type>
	void Quaternionx<Type>::set(const Angle &euler_x, const Angle &euler_y, const Angle &euler_z, EulerOrder order)
	{
		set(euler_x.to_radians(), euler_y.to_radians(), euler_z.to_radians(), angle_radians, order);
	}

	template<typename Type>
	void Quaternionx<Type>::set(Type euler_x, Type euler_y, Type euler_z, AngleUnit unit, EulerOrder order)
	{
		Quaternionx<Type> q_x = Quaternionx<Type>::axis_angle(Angle(euler_x, unit), Vec3f(1.0f, 0.0f, 0.0f));
		Quaternionx<Type> q_y = Quaternionx<Type>::axis_angle(Angle(euler_y, unit), Vec3f(0.0f, 1.0f, 0.0f));
		Quaternionx<Type> q_z = Quaternionx<Type>::axis_angle(Angle(euler_z, unit), Vec3f(0.0f, 0.0f, 1.0f));

		switch (order)
		{
		case order_XYZ:
			*this = Quaternionx<Type>::multiply(Quaternionx<Type>::multiply(q_x, q_y), q_z);
			break;
		case order_XZY:
			*this = Quaternionx<Type>::multiply(Quaternionx<Type>::multiply(q_x, q_z), q_y);
			break;
		case order_YZX:
			*this = Quaternionx<Type>::multiply(Quaternionx<Type>::multiply(q_y, q_z), q_x);
			break;
		case order_YXZ:
			*this = Quaternionx<Type>::multiply(Quaternionx<Type>::multiply(q_y, q_x), q_z);
			break;
		case order_ZXY:
			*this = Quaternionx<Type>::multiply(Quaternionx<Type>::multiply(q_z, q_x), q_y);
			break;
		case order_ZYX:
			*this = Quaternionx<Type>::multiply(Quaternionx<Type>::multiply(q_z, q_y), q_x);
			break;
		default:
			throw Exception("Unknown euler order");
		}
	}

	template<typename Type>
	Quaternionx<Type> Quaternionx<Type>::multiply(const Quaternionx<Type> &quaternion_1, const Quaternionx<Type> &quaternion_2)
	{
		Quaternionx<Type> quaternion_dest;
		quaternion_dest.x = quaternion_1.w*quaternion_2.x + quaternion_1.x*quaternion_2.w + quaternion_1.y*quaternion_2.z - quaternion_1.z*quaternion_2.y;
		quaternion_dest.y = quaternion_1.w*quaternion_2.y + quaternion_1.y*quaternion_2.w + quaternion_1.z*quaternion_2.x - quaternion_1.x*quaternion_2.z;
		quaternion_dest.z = quaternion_1.w*quaternion_2.z + quaternion_1.z*quaternion_2.w + quaternion_1.x*quaternion_2.y - quaternion_1.y*quaternion_2.x;
		quaternion_dest.w = quaternion_1.w*quaternion_2.w - quaternion_1.x*quaternion_2.x - quaternion_1.y*quaternion_2.y - quaternion_1.z*quaternion_2.z;
		return quaternion_dest;
	}

	template<typename Type>
	Quaternionx<Type> Quaternionx<Type>::operator *(const Mat4<Type> &m) const
	{
		Quaternionx<Type> result;

		result.i = m.matrix[(4 * 0) + 0] * i + m.matrix[(4 * 0) + 1] * j + m.matrix[(4 * 0) + 2] * k + m.matrix[(4 * 0) + 3] * w;
		result.j = m.matrix[(4 * 1) + 0] * i + m.matrix[(4 * 1) + 1] * j + m.matrix[(4 * 1) + 2] * k + m.matrix[(4 * 1) + 3] * w;
		result.k = m.matrix[(4 * 2) + 0] * i + m.matrix[(4 * 2) + 1] * j + m.matrix[(4 * 2) + 2] * k + m.matrix[(4 * 2) + 3] * w;
		result.w = m.matrix[(4 * 3) + 0] * i + m.matrix[(4 * 3) + 1] * j + m.matrix[(4 * 3) + 2] * k + m.matrix[(4 * 3) + 3] * w;

		return result;
	}

	template<typename Type>
	Mat4<Type> Quaternionx<Type>::to_matrix() const
	{
		Mat4<Type> m = Mat4<Type>::null();
		m.matrix[0 * 4 + 0] = 1 - 2 * y*y - 2 * z*z;
		m.matrix[1 * 4 + 0] = 2 * x*y - 2 * w*z;
		m.matrix[2 * 4 + 0] = 2 * x*z + 2 * w*y;
		m.matrix[0 * 4 + 1] = 2 * x*y + 2 * w*z;
		m.matrix[1 * 4 + 1] = 1 - 2 * x*x - 2 * z*z;
		m.matrix[2 * 4 + 1] = 2 * y*z - 2 * w*x;
		m.matrix[0 * 4 + 2] = 2 * x*z - 2 * w*y;
		m.matrix[1 * 4 + 2] = 2 * y*z + 2 * w*x;
		m.matrix[2 * 4 + 2] = 1 - 2 * x*x - 2 * y*y;
		m.matrix[3 * 4 + 3] = 1;
		return m;
	}

	template<typename Type>
	Quaternionx<Type> &Quaternionx<Type>::normalize()
	{
		Type magnitude = sqrt(x*x + y*y + z*z + w*w);
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
	Quaternionx<Type> Quaternionx<Type>::axis_angle(const Angle &angle, const Vec3f &axis)
	{
		Type length = axis.length();
		Type half_angle_radians = angle.to_radians() / (Type) 2.0;
		Quaternionx<Type> quaternion;
		quaternion.w = cos(half_angle_radians);
		quaternion.x = axis.x * sin(half_angle_radians) / length;
		quaternion.y = axis.y * sin(half_angle_radians) / length;
		quaternion.z = axis.z * sin(half_angle_radians) / length;
		quaternion.normalize();
		return quaternion;
	}

	template<typename Type>
	Quaternionx<Type> &Quaternionx<Type>::inverse()
	{
		i = -i;
		j = -j;
		k = -k;
		return *this;
	}

	template<typename Type>
	Type Quaternionx<Type>::magnitude() const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}

	template<typename Type>
	Quaternionx<Type> &Quaternionx<Type>::rotate(const Angle &angle, const Vec3f &axis)
	{
		*this = *this * axis_angle(angle, axis);
		return *this;
	}

	template<typename Type>
	Quaternionx<Type> &Quaternionx<Type>::rotate(const Angle &euler_x, const Angle &euler_y, const Angle &euler_z, EulerOrder order)
	{
		Quaternionx<Type> quaternion(euler_x, euler_y, euler_z, order);
		*this = *this * quaternion;
		return *this;
	}

	template<typename Type>
	Quaternionx<Type> Quaternionx<Type>::lerp(const Quaternionx<Type> &quaternion_initial, const Quaternionx<Type> &quaternion_final, Type lerp_time)
	{
		Quaternionx<Type> quaternion(
			quaternion_initial.w * (((Type) 1.0) - lerp_time) + quaternion_final.w * lerp_time,
			quaternion_initial.x * (((Type) 1.0) - lerp_time) + quaternion_final.x * lerp_time,
			quaternion_initial.y * (((Type) 1.0) - lerp_time) + quaternion_final.y * lerp_time,
			quaternion_initial.z * (((Type) 1.0) - lerp_time) + quaternion_final.z * lerp_time
			);
		quaternion.normalize();
		return quaternion;
	}

	template<typename Type>
	Quaternionx<Type> Quaternionx<Type>::slerp(const Quaternionx<Type> &quaternion_initial, const Quaternionx<Type> &quaternion_final, Type slerp_time)
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

		if (((Type) 1.0) - cos_theta > ((Type) 0.001))
		{
			cos_theta = acos(cos_theta);
			Type sin_theta = 1.0f / sin(cos_theta);
			beta = sin(cos_theta * beta) * sin_theta;
			slerp_time = sin(cos_theta * slerp_time) * sin_theta;
		}

		Quaternionx<Type> quaternion;
		quaternion.x = beta * quaternion_initial.x + slerp_time * q2[0];
		quaternion.y = beta * quaternion_initial.y + slerp_time * q2[1];
		quaternion.z = beta * quaternion_initial.z + slerp_time * q2[2];
		quaternion.w = beta * quaternion_initial.w + slerp_time * q2[3];
		return quaternion;
	}

	template<typename Type>
	Vec3<Type> Quaternionx<Type>::rotate_vector(const Vec3<Type> &v) const
	{
		Vec3<Type> q3(x, y, z);
		return v + Vec3<Type>::cross(q3, Vec3<Type>::cross(q3, v) + v * w) * ((Type) 2.0);
	}

	template<typename Type>
	Vec4<Type> Quaternionx<Type>::rotate_vector(const Vec4<Type> &v) const
	{
		Vec3<Type> rot = rotate_vector((Vec3<Type>)v);
		return Vec4<Type>(rot.x, rot.y, rot.z, v.w);
	}

	// Explicit instantiate the versions we use:

	template class Quaternionx<float>;
	template class Quaternionx<double>;
}

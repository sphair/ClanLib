/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/Math/vec4.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Math/quaternion.h"
#include <limits>

#ifndef CL_DISABLE_SSE2
//#include <mmintrin.h> /*Dont need it*/
#include <emmintrin.h>/*it's included here*/
#endif

#ifndef FLT_EPSILON
#include "float.h"
#endif

namespace clan
{
	// This function needs to be listed first because GCC is not very flexible handling templates

	template<>
	Mat4<float> Mat4<float>::operator *(const Mat4<float> &mult) const
	{
#if !defined CL_DISABLE_SSE2 && !defined __MINGW32__ //MinGW's version is flawed.
		Mat4<float> result;
		__m128 m1col0 = _mm_loadu_ps(matrix);
		__m128 m1col1 = _mm_loadu_ps(matrix + 4);
		__m128 m1col2 = _mm_loadu_ps(matrix + 8);
		__m128 m1col3 = _mm_loadu_ps(matrix + 12);
		_MM_TRANSPOSE4_PS(m1col0, m1col1, m1col2, m1col3);

		for (int cur_row = 0; cur_row < 4; cur_row++)
		{
			__m128 m2row = _mm_loadu_ps(mult.matrix + cur_row * 4);

			__m128 cell0 = _mm_mul_ps(m1col0, m2row);
			__m128 cell1 = _mm_mul_ps(m1col1, m2row);
			__m128 cell2 = _mm_mul_ps(m1col2, m2row);
			__m128 cell3 = _mm_mul_ps(m1col3, m2row);

			_MM_TRANSPOSE4_PS(cell0, cell1, cell2, cell3);
			__m128 row = _mm_add_ps(_mm_add_ps(_mm_add_ps(cell0, cell1), cell2), cell3);

			_mm_storeu_ps(result.matrix + cur_row * 4, row);
		}

		return result;
#else
		Mat4<float> result;
		for (int x=0; x<4; x++)
		{
			for (int y=0; y<4; y++)
			{
				result.matrix[x+y*4] =
					matrix[0*4 + x]*mult.matrix[y*4 + 0] +
					matrix[1*4 + x]*mult.matrix[y*4 + 1] +
					matrix[2*4 + x]*mult.matrix[y*4 + 2] +
					matrix[3*4 + x]*mult.matrix[y*4 + 3];
			}
		}
		return result;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////

	template<typename Type>
	Mat4<Type>::Mat4(const Mat3<Type> &copy)
	{
		matrix[0 * 4 + 0] = copy.matrix[0 * 3 + 0];
		matrix[0 * 4 + 1] = copy.matrix[0 * 3 + 1];
		matrix[0 * 4 + 2] = copy.matrix[0 * 3 + 2];
		matrix[0 * 4 + 3] = 0;
		matrix[1 * 4 + 0] = copy.matrix[1 * 3 + 0];
		matrix[1 * 4 + 1] = copy.matrix[1 * 3 + 1];
		matrix[1 * 4 + 2] = copy.matrix[1 * 3 + 2];
		matrix[1 * 4 + 3] = 0;
		matrix[2 * 4 + 0] = copy.matrix[2 * 3 + 0];
		matrix[2 * 4 + 1] = copy.matrix[2 * 3 + 1];
		matrix[2 * 4 + 2] = copy.matrix[2 * 3 + 2];
		matrix[2 * 4 + 3] = 0;
		matrix[3 * 4 + 0] = 0;
		matrix[3 * 4 + 1] = 0;
		matrix[3 * 4 + 2] = 0;
		matrix[3 * 4 + 3] = 1;
	}

	template<typename Type>
	Mat4<Type>::Mat4(const Mat2<Type> &copy)
	{
		matrix[0 * 4 + 0] = copy.matrix[0 * 2 + 0];
		matrix[0 * 4 + 1] = copy.matrix[0 * 2 + 1];
		matrix[0 * 4 + 2] = 0;
		matrix[0 * 4 + 3] = 0;
		matrix[1 * 4 + 0] = copy.matrix[1 * 2 + 0];
		matrix[1 * 4 + 1] = copy.matrix[1 * 2 + 1];
		matrix[1 * 4 + 2] = 0;
		matrix[1 * 4 + 3] = 0;
		matrix[2 * 4 + 0] = 0;
		matrix[2 * 4 + 1] = 0;
		matrix[2 * 4 + 2] = 1;
		matrix[2 * 4 + 3] = 0;
		matrix[3 * 4 + 0] = 0;
		matrix[3 * 4 + 1] = 0;
		matrix[3 * 4 + 2] = 0;
		matrix[3 * 4 + 3] = 1;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::frustum(Type left, Type right, Type bottom, Type top, Type z_near, Type z_far, Handedness handedness, ClipZRange clip_z)
	{
		Mat4<Type> frustum_matrix = null();
		Type  a = (right + left) / (right - left);
		Type  b = (top + bottom) / (top - bottom);
		Type  c = -(z_far + z_near) / (z_far - z_near);
		Type  d = -(2 * z_far * z_near) / (z_far - z_near);
		frustum_matrix.matrix[0 + 0 * 4] = (Type)(2 * z_near / (right - left));
		frustum_matrix.matrix[1 + 1 * 4] = (Type)(2 * z_near / (top - bottom));
		frustum_matrix.matrix[0 + 2 * 4] = (Type)a;
		frustum_matrix.matrix[1 + 2 * 4] = (Type)b;
		frustum_matrix.matrix[2 + 2 * 4] = (Type)c;
		frustum_matrix.matrix[2 + 3 * 4] = (Type)d;
		frustum_matrix.matrix[3 + 2 * 4] = (Type)-1;
		if (handedness == handed_left)
			frustum_matrix = frustum_matrix * Mat4<Type>::scale(1.0f, 1.0f, -1.0f);
		if (clip_z == clip_zero_positive_w)
		{
			Mat4<Type> scale_translate = identity();
			scale_translate[2 + 2 * 4] = (Type)0.5;
			scale_translate[2 + 3 * 4] = (Type)0.5;
			frustum_matrix = scale_translate * frustum_matrix;
		}
		return frustum_matrix;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::perspective(
		Type field_of_view_y_degrees,
		Type aspect,
		Type z_near,
		Type z_far,
		Handedness handedness,
		ClipZRange clip_z)
	{
		Mat4<Type> projection_matrix = null();
		Type field_of_view_y_rad = Type(field_of_view_y_degrees * PI / 180.0);
		Type f = Type(1.0 / tan(double(field_of_view_y_rad) / 2.0));
		projection_matrix.matrix[0 + 0 * 4] = (Type)(f / aspect);
		projection_matrix.matrix[1 + 1 * 4] = (Type)f;
		projection_matrix.matrix[2 + 2 * 4] = (Type)((z_far + z_near) / (z_near - z_far));
		projection_matrix.matrix[2 + 3 * 4] = (Type)((2 * z_far * z_near) / (z_near - z_far));
		projection_matrix.matrix[3 + 2 * 4] = (Type)-1;
		if (handedness == handed_left)
			projection_matrix = projection_matrix * Mat4<Type>::scale(1.0f, 1.0f, -1.0f);
		if (clip_z == clip_zero_positive_w)
		{
			Mat4<Type> scale_translate = identity();
			scale_translate[2 + 2 * 4] = (Type)0.5;
			scale_translate[2 + 3 * 4] = (Type)0.5;
			projection_matrix = scale_translate * projection_matrix;
		}
		return projection_matrix;
	}

	// For floats
	template<>
	Mat4f Mat4f::perspective(
		float field_of_view_y_degrees,
		float aspect,
		float z_near,
		float z_far,
		Handedness handedness,
		ClipZRange clip_z)
	{
		Mat4f projection_matrix = null();
		float field_of_view_y_rad = field_of_view_y_degrees * float(PI / 180.0f);
		float f = 1.0f / tan(field_of_view_y_rad / 2.0f);
		projection_matrix.matrix[0 + 0 * 4] = (f / aspect);
		projection_matrix.matrix[1 + 1 * 4] = f;
		projection_matrix.matrix[2 + 2 * 4] = ((z_far + z_near) / (z_near - z_far));
		projection_matrix.matrix[2 + 3 * 4] = ((2.0f * z_far * z_near) / (z_near - z_far));
		projection_matrix.matrix[3 + 2 * 4] = -1.0f;
		if (handedness == handed_left)
			projection_matrix = projection_matrix * Mat4f::scale(1.0f, 1.0f, -1.0f);
		if (clip_z == clip_zero_positive_w)
		{
			Mat4f scale_translate = identity();
			scale_translate[2 + 2 * 4] = 0.5f;
			scale_translate[2 + 3 * 4] = 0.5f;
			projection_matrix = scale_translate * projection_matrix;
		}
		return projection_matrix;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::ortho(Type left, Type right, Type bottom, Type top, Type z_near, Type z_far, Handedness handedness, ClipZRange clip_z)
	{
		Mat4<Type> ortho_matrix = null();
		Type tx = -(right + left) / (right - left);
		Type ty = -(top + bottom) / (top - bottom);
		Type tz = -(z_far + z_near) / (z_far - z_near);
		ortho_matrix.matrix[0 + 0 * 4] = (Type)(2 / (right - left));
		ortho_matrix.matrix[1 + 1 * 4] = (Type)(2 / (top - bottom));
		ortho_matrix.matrix[2 + 2 * 4] = (Type)(-2 / (z_far - z_near));
		ortho_matrix.matrix[0 + 3 * 4] = (Type)tx;
		ortho_matrix.matrix[1 + 3 * 4] = (Type)ty;
		ortho_matrix.matrix[2 + 3 * 4] = (Type)tz;
		ortho_matrix.matrix[3 + 3 * 4] = (Type)1;
		if (handedness == handed_left)
			ortho_matrix = ortho_matrix * Mat4<Type>::scale(1.0f, 1.0f, -1.0f);
		if (clip_z == clip_zero_positive_w)
		{
			Mat4<Type> scale_translate = identity();
			scale_translate[2 + 2 * 4] = (Type)0.5;
			scale_translate[2 + 3 * 4] = (Type)0.5;
			ortho_matrix = scale_translate * ortho_matrix;
		}
		return ortho_matrix;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::ortho_2d(Type left, Type right, Type bottom, Type top, Handedness handedness, ClipZRange clip_z)
	{
		return ortho(left, right, bottom, top, (Type)-1.0, (Type)1.0, handedness, clip_z);
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::rotate(const Angle &angle, Type x, Type y, Type z, bool normalize)
	{
		if (normalize)
		{
			Type len2 = x*x + y*y + z*z;
			if (len2 != (Type)1)
			{
				Type length = sqrt(len2);
				if (length > (Type)0)
				{
					x /= length;
					y /= length;
					z /= length;
				}
				else
				{
					x = (Type)0;
					y = (Type)0;
					z = (Type)0;
				}
			}
		}

		Mat4<Type> rotate_matrix = null();
		Type c = cos(angle.to_radians());
		Type s = sin(angle.to_radians());
		rotate_matrix.matrix[0 + 0 * 4] = (Type)(x*x*(1.0f - c) + c);
		rotate_matrix.matrix[0 + 1 * 4] = (Type)(x*y*(1.0f - c) - z*s);
		rotate_matrix.matrix[0 + 2 * 4] = (Type)(x*z*(1.0f - c) + y*s);
		rotate_matrix.matrix[1 + 0 * 4] = (Type)(y*x*(1.0f - c) + z*s);
		rotate_matrix.matrix[1 + 1 * 4] = (Type)(y*y*(1.0f - c) + c);
		rotate_matrix.matrix[1 + 2 * 4] = (Type)(y*z*(1.0f - c) - x*s);
		rotate_matrix.matrix[2 + 0 * 4] = (Type)(x*z*(1.0f - c) - y*s);
		rotate_matrix.matrix[2 + 1 * 4] = (Type)(y*z*(1.0f - c) + x*s);
		rotate_matrix.matrix[2 + 2 * 4] = (Type)(z*z*(1.0f - c) + c);
		rotate_matrix.matrix[3 + 3 * 4] = 1;
		return rotate_matrix;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::rotate(const Angle &angle_x, const Angle &angle_y, const Angle &angle_z, EulerOrder order)
	{
		return Mat4<Type>(Mat3<Type>::rotate(angle_x, angle_y, angle_z, order));
	}

	template<typename Type>
	Vec3<Type> Mat4<Type>::get_euler(EulerOrder order) const
	{
		Vec3<Type> angles;

		int pos_i;
		int pos_j;
		int pos_k;

		// Obtain the correct xyz rotation position from the original rotation order
		switch (order)
		{
		case order_XYZ:
			pos_i = 2; pos_j = 1; pos_k = 0; break;
		case order_XZY:
			pos_i = 1; pos_j = 2; pos_k = 0; break;
		case order_YZX:
			pos_i = 0; pos_j = 2; pos_k = 1; break;
		case order_YXZ:
			pos_i = 2; pos_j = 0; pos_k = 1; break;
		case order_ZXY:
			pos_i = 1; pos_j = 0; pos_k = 2; break;
		case order_ZYX:
			pos_i = 0; pos_j = 1; pos_k = 2; break;
		default:
			throw Exception("Unknown euler order");
		}


		Type cy = sqrt(matrix[(4 * pos_i) + pos_i] * matrix[(4 * pos_i) + pos_i] + matrix[(4 * pos_j) + pos_i] * matrix[(4 * pos_j) + pos_i]);
		if (cy > (Type) 16.0*FLT_EPSILON)
		{
			angles.x = atan2(matrix[(4 * pos_k) + pos_j], matrix[(4 * pos_k) + pos_k]);
			angles.y = atan2(-matrix[(4 * pos_k) + pos_i], cy);
			angles.z = atan2(matrix[(4 * pos_j) + pos_i], matrix[(4 * pos_i) + pos_i]);
		}
		else
		{
			angles.x = atan2(-matrix[(4 * pos_j) + pos_k], matrix[(4 * pos_j) + pos_j]);
			angles.y = atan2(-matrix[(4 * pos_k) + pos_i], cy);
			angles.z = 0;
		}

		// Swap the xyz value to the specified euler angle
		switch (order)
		{
		case order_XYZ:
			angles = Vec3<Type>(angles.z, angles.y, angles.x);
			break;
		case order_XZY:
			angles = Vec3<Type>(-angles.z, -angles.x, -angles.y);
			break;
		case order_YZX:
			angles = Vec3<Type>(angles.x, angles.z, angles.y);
			break;
		case order_YXZ:
			angles = Vec3<Type>(-angles.y, -angles.z, -angles.x);
			break;
		case order_ZXY:
			angles = Vec3<Type>(angles.y, angles.x, angles.z);
			break;
		case order_ZYX:
			angles = Vec3<Type>(-angles.x, -angles.y, -angles.z);
			break;
		}

		return angles;
	}

	template<typename Type>
	Vec3<Type> Mat4<Type>::get_transformed_point(const Vec3<Type> &vector) const
	{
		Vec3<Type> dest;

		dest.x = vector.x * matrix[0 + 0 * 4] +
			vector.y * matrix[0 + 1 * 4] +
			vector.z * matrix[0 + 2 * 4] +
			matrix[0 + 3 * 4];

		dest.y = vector.x * matrix[1 + 0 * 4] +
			vector.y * matrix[1 + 1 * 4] +
			vector.z * matrix[1 + 2 * 4] +
			matrix[1 + 3 * 4];

		dest.z = vector.x * matrix[2 + 0 * 4] +
			vector.y * matrix[2 + 1 * 4] +
			vector.z * matrix[2 + 2 * 4] +
			matrix[2 + 3 * 4];

		Type w = vector.x * matrix[3 + 0 * 4] +
			vector.y * matrix[3 + 1 * 4] +
			vector.z * matrix[3 + 2 * 4] +
			matrix[3 + 3 * 4];

		if (w != (Type) 0.0)
		{
			dest.x /= w;
			dest.y /= w;
			dest.z /= w;
		}

		return dest;
	}

	// For ints
	template<>
	Vec3<int> Mat4<int>::get_euler(EulerOrder order) const
	{
		throw Exception("Function not supported for ints");
	}

	// For ints
	template<>
	Mat4<int> Mat4<int>::rotate(const Angle &angle, int x, int y, int z, bool normalize)
	{
		if (normalize)
		{
			int len2 = x*x + y*y + z*z;
			if (len2 != (int)1)
			{
				int length = sqrt((float)len2);
				if (length > 0)
				{
					x /= length;
					y /= length;
					z /= length;
				}
				else
				{
					x = 0;
					y = 0;
					z = 0;
				}
			}
		}

		Mat4<int> rotate_matrix = null();
		float c = cos(angle.to_radians());
		float s = sin(angle.to_radians());
		rotate_matrix.matrix[0 + 0 * 4] = (int)floor((x*x*(1.0f - c) + c) + 0.5f);
		rotate_matrix.matrix[0 + 1 * 4] = (int)floor((x*y*(1.0f - c) - z*s) + 0.5f);
		rotate_matrix.matrix[0 + 2 * 4] = (int)floor((x*z*(1.0f - c) + y*s) + 0.5f);
		rotate_matrix.matrix[1 + 0 * 4] = (int)floor((y*x*(1.0f - c) + z*s) + 0.5f);
		rotate_matrix.matrix[1 + 1 * 4] = (int)floor((y*y*(1.0f - c) + c) + 0.5f);
		rotate_matrix.matrix[1 + 2 * 4] = (int)floor((y*z*(1.0f - c) - x*s) + 0.5f);
		rotate_matrix.matrix[2 + 0 * 4] = (int)floor((x*z*(1.0f - c) - y*s) + 0.5f);
		rotate_matrix.matrix[2 + 1 * 4] = (int)floor((y*z*(1.0f - c) + x*s) + 0.5f);
		rotate_matrix.matrix[2 + 2 * 4] = (int)floor((z*z*(1.0f - c) + c) + 0.5f);
		rotate_matrix.matrix[3 + 3 * 4] = 1;
		return rotate_matrix;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::scale(Type x, Type y, Type z)
	{
		Mat4<Type> scale_matrix = null();
		scale_matrix.matrix[0 + 0 * 4] = x;
		scale_matrix.matrix[1 + 1 * 4] = y;
		scale_matrix.matrix[2 + 2 * 4] = z;
		scale_matrix.matrix[3 + 3 * 4] = 1;
		return scale_matrix;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::translate(Type x, Type y, Type z)
	{
		Mat4<Type> translate_matrix = identity();
		translate_matrix.matrix[0 + 3 * 4] = x;
		translate_matrix.matrix[1 + 3 * 4] = y;
		translate_matrix.matrix[2 + 3 * 4] = z;
		return translate_matrix;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::look_at(
		Type eye_x, Type eye_y, Type eye_z,
		Type center_x, Type center_y, Type center_z,
		Type up_x, Type up_y, Type up_z)
	{
		Type f[3] = { center_x - eye_x, center_y - eye_y, center_z - eye_z };
		Type length_f = (Type)sqrt(double(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]));
		f[0] /= length_f;
		f[1] /= length_f;
		f[2] /= length_f;

		Type up[3] = { up_x, up_y, up_z };
		Type length_up = (Type)sqrt(double(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]));
		up[0] /= length_up;
		up[1] /= length_up;
		up[2] /= length_up;

		Vec4<Type> fv = Vec4<Type>(f[0], f[1], f[2], 0);
		Vec4<Type> s = Vec4<Type>::cross3(fv, Vec4<Type>(up[0], up[1], up[2], 0));
		Vec4<Type> u = Vec4<Type>::cross3(s, fv);

		Mat4<Type> m = null();
		m.matrix[0 + 0 * 4] = (Type)s.x;
		m.matrix[0 + 1 * 4] = (Type)s.y;
		m.matrix[0 + 2 * 4] = (Type)s.z;
		m.matrix[1 + 0 * 4] = (Type)u.x;
		m.matrix[1 + 1 * 4] = (Type)u.y;
		m.matrix[1 + 2 * 4] = (Type)u.z;
		m.matrix[2 + 0 * 4] = (Type)-f[0];
		m.matrix[2 + 1 * 4] = (Type)-f[1];
		m.matrix[2 + 2 * 4] = (Type)-f[2];
		m.matrix[3 + 3 * 4] = 1;
		m = m * translate((Type)-eye_x, (Type)-eye_y, (Type)-eye_z);
		return m;
	}

	// For floats
	template<>
	Mat4f Mat4f::look_at(
		float eye_x, float eye_y, float eye_z,
		float center_x, float center_y, float center_z,
		float up_x, float up_y, float up_z)
	{
		float f[3] = { center_x - eye_x, center_y - eye_y, center_z - eye_z };
		float length_f = sqrt(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
		f[0] /= length_f;
		f[1] /= length_f;
		f[2] /= length_f;

		float up[3] = { up_x, up_y, up_z };
		float length_up = sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
		up[0] /= length_up;
		up[1] /= length_up;
		up[2] /= length_up;

		Vec4f fv = Vec4f(f[0], f[1], f[2], 0);
		Vec4f s = Vec4f::cross3(fv, Vec4f(up[0], up[1], up[2], 0));
		Vec4f u = Vec4f::cross3(s, fv);

		Mat4f m = null();
		m.matrix[0 + 0 * 4] = s.x;
		m.matrix[0 + 1 * 4] = s.y;
		m.matrix[0 + 2 * 4] = s.z;
		m.matrix[1 + 0 * 4] = u.x;
		m.matrix[1 + 1 * 4] = u.y;
		m.matrix[1 + 2 * 4] = u.z;
		m.matrix[2 + 0 * 4] = -f[0];
		m.matrix[2 + 1 * 4] = -f[1];
		m.matrix[2 + 2 * 4] = -f[2];
		m.matrix[3 + 3 * 4] = 1;
		m = m * translate(-eye_x, -eye_y, -eye_z);
		return m;
	}

	template<typename Type>
	Mat4<Type> &Mat4<Type>::operator =(const Mat3<Type> &copy)
	{
		matrix[0 * 4 + 0] = copy.matrix[0 * 3 + 0];
		matrix[0 * 4 + 1] = copy.matrix[0 * 3 + 1];
		matrix[0 * 4 + 2] = copy.matrix[0 * 3 + 2];
		matrix[0 * 4 + 3] = 0;
		matrix[1 * 4 + 0] = copy.matrix[1 * 3 + 0];
		matrix[1 * 4 + 1] = copy.matrix[1 * 3 + 1];
		matrix[1 * 4 + 2] = copy.matrix[1 * 3 + 2];
		matrix[1 * 4 + 3] = 0;
		matrix[2 * 4 + 0] = copy.matrix[2 * 3 + 0];
		matrix[2 * 4 + 1] = copy.matrix[2 * 3 + 1];
		matrix[2 * 4 + 2] = copy.matrix[2 * 3 + 2];
		matrix[2 * 4 + 3] = 0;
		matrix[3 * 4 + 0] = 0;
		matrix[3 * 4 + 1] = 0;
		matrix[3 * 4 + 2] = 0;
		matrix[3 * 4 + 3] = 1;
		return *this;
	}

	template<typename Type>
	Mat4<Type> &Mat4<Type>::operator =(const Mat2<Type> &copy)
	{
		matrix[0 * 4 + 0] = copy.matrix[0 * 2 + 0];
		matrix[0 * 4 + 1] = copy.matrix[0 * 2 + 1];
		matrix[0 * 4 + 2] = 0;
		matrix[0 * 4 + 3] = 0;
		matrix[1 * 4 + 0] = copy.matrix[1 * 2 + 0];
		matrix[1 * 4 + 1] = copy.matrix[1 * 2 + 1];
		matrix[1 * 4 + 2] = 0;
		matrix[1 * 4 + 3] = 0;
		matrix[2 * 4 + 0] = 0;
		matrix[2 * 4 + 1] = 0;
		matrix[2 * 4 + 2] = 1;
		matrix[2 * 4 + 3] = 0;
		matrix[3 * 4 + 0] = 0;
		matrix[3 * 4 + 1] = 0;
		matrix[3 * 4 + 2] = 0;
		matrix[3 * 4 + 3] = 1;
		return *this;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::operator *(const Mat4<Type> &mult) const
	{
		Mat4<Type> result;
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				result.matrix[x + y * 4] =
					matrix[0 * 4 + x] * mult.matrix[y * 4 + 0] +
					matrix[1 * 4 + x] * mult.matrix[y * 4 + 1] +
					matrix[2 * 4 + x] * mult.matrix[y * 4 + 2] +
					matrix[3 * 4 + x] * mult.matrix[y * 4 + 3];
			}
		}
		return result;
	}

	template<typename Type>
	Mat4<Type> &Mat4<Type>::scale_self(Type x, Type y, Type z)
	{
		matrix[0 + 4 * 0] *= x;
		matrix[0 + 4 * 1] *= y;
		matrix[0 + 4 * 2] *= z;

		matrix[1 + 4 * 0] *= x;
		matrix[1 + 4 * 1] *= y;
		matrix[1 + 4 * 2] *= z;

		matrix[2 + 4 * 0] *= x;
		matrix[2 + 4 * 1] *= y;
		matrix[2 + 4 * 2] *= z;

		matrix[3 + 4 * 0] *= x;
		matrix[3 + 4 * 1] *= y;
		matrix[3 + 4 * 2] *= z;
		return *this;
	}

	template<>
	Mat4<float> &Mat4<float>::scale_self(float x, float y, float z)
	{
#if !defined CL_DISABLE_SSE2
		_mm_storeu_ps(matrix, _mm_mul_ps(_mm_loadu_ps(matrix), _mm_set_ps1(x)));
		_mm_storeu_ps(matrix + 4, _mm_mul_ps(_mm_loadu_ps(matrix + 4), _mm_set_ps1(y)));
		_mm_storeu_ps(matrix + 8, _mm_mul_ps(_mm_loadu_ps(matrix + 8), _mm_set_ps1(z)));
#else
		matrix[0+4*0] *= x;
		matrix[0+4*1] *= y;
		matrix[0+4*2] *= z;

		matrix[1+4*0] *= x;
		matrix[1+4*1] *= y;
		matrix[1+4*2] *= z;

		matrix[2+4*0] *= x;
		matrix[2+4*1] *= y;
		matrix[2+4*2] *= z;

		matrix[3+4*0] *= x;
		matrix[3+4*1] *= y;
		matrix[3+4*2] *= z;
#endif
		return *this;
	}

	template<typename Type>
	Mat4<Type> &Mat4<Type>::translate_self(Type x, Type y, Type z)
	{
		Type translate_value_1 = (matrix[0 + 4 * 0] * x) + (matrix[0 + 4 * 1] * y) + (matrix[0 + 4 * 2] * z) + matrix[0 + 4 * 3];
		Type translate_value_2 = (matrix[1 + 4 * 0] * x) + (matrix[1 + 4 * 1] * y) + (matrix[1 + 4 * 2] * z) + matrix[1 + 4 * 3];
		Type translate_value_3 = (matrix[2 + 4 * 0] * x) + (matrix[2 + 4 * 1] * y) + (matrix[2 + 4 * 2] * z) + matrix[2 + 4 * 3];
		Type translate_value_4 = (matrix[3 + 4 * 0] * x) + (matrix[3 + 4 * 1] * y) + (matrix[3 + 4 * 2] * z) + matrix[3 + 4 * 3];

		matrix[0 + 4 * 3] = translate_value_1;
		matrix[1 + 4 * 3] = translate_value_2;
		matrix[2 + 4 * 3] = translate_value_3;
		matrix[3 + 4 * 3] = translate_value_4;
		return *this;
	}

	template<>
	Mat4<float> &Mat4<float>::translate_self(float x, float y, float z)
	{
#if !defined CL_DISABLE_SSE2
		__m128 row0 = _mm_loadu_ps(matrix);
		__m128 row1 = _mm_loadu_ps(matrix + 4);
		__m128 row2 = _mm_loadu_ps(matrix + 8);
		__m128 row3 = _mm_loadu_ps(matrix + 12);

		row0 = _mm_mul_ps(row0, _mm_set1_ps(x));
		row1 = _mm_mul_ps(row1, _mm_set1_ps(y));
		row2 = _mm_mul_ps(row2, _mm_set1_ps(z));

		__m128 result = _mm_add_ps(_mm_add_ps(_mm_add_ps(row0, row1), row2), row3);
		_mm_storeu_ps(matrix + 12, result);
#else
		float translate_value_1 = (matrix[0+4*0] * x) + (matrix[0+4*1] * y) + (matrix[0+4*2] * z) + matrix[0+4*3];
		float translate_value_2 = (matrix[1+4*0] * x) + (matrix[1+4*1] * y) + (matrix[1+4*2] * z) + matrix[1+4*3];
		float translate_value_3 = (matrix[2+4*0] * x) + (matrix[2+4*1] * y) + (matrix[2+4*2] * z) + matrix[2+4*3];
		float translate_value_4 = (matrix[3+4*0] * x) + (matrix[3+4*1] * y) + (matrix[3+4*2] * z) + matrix[3+4*3];

		matrix[0+4*3] = translate_value_1;
		matrix[1+4*3] = translate_value_2;
		matrix[2+4*3] = translate_value_3;
		matrix[3+4*3] = translate_value_4;
#endif
		return *this;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::operator +(const Mat4<Type> &add_matrix) const
	{
		Mat4<Type> result;
		result.matrix[(0 * 4) + 0] = matrix[(0 * 4) + 0] + add_matrix.matrix[(0 * 4) + 0];
		result.matrix[(0 * 4) + 1] = matrix[(0 * 4) + 1] + add_matrix.matrix[(0 * 4) + 1];
		result.matrix[(0 * 4) + 2] = matrix[(0 * 4) + 2] + add_matrix.matrix[(0 * 4) + 2];
		result.matrix[(0 * 4) + 3] = matrix[(0 * 4) + 3] + add_matrix.matrix[(0 * 4) + 3];
		result.matrix[(1 * 4) + 0] = matrix[(1 * 4) + 0] + add_matrix.matrix[(1 * 4) + 0];
		result.matrix[(1 * 4) + 1] = matrix[(1 * 4) + 1] + add_matrix.matrix[(1 * 4) + 1];
		result.matrix[(1 * 4) + 2] = matrix[(1 * 4) + 2] + add_matrix.matrix[(1 * 4) + 2];
		result.matrix[(1 * 4) + 3] = matrix[(1 * 4) + 3] + add_matrix.matrix[(1 * 4) + 3];
		result.matrix[(2 * 4) + 0] = matrix[(2 * 4) + 0] + add_matrix.matrix[(2 * 4) + 0];
		result.matrix[(2 * 4) + 1] = matrix[(2 * 4) + 1] + add_matrix.matrix[(2 * 4) + 1];
		result.matrix[(2 * 4) + 2] = matrix[(2 * 4) + 2] + add_matrix.matrix[(2 * 4) + 2];
		result.matrix[(2 * 4) + 3] = matrix[(2 * 4) + 3] + add_matrix.matrix[(2 * 4) + 3];
		result.matrix[(3 * 4) + 0] = matrix[(3 * 4) + 0] + add_matrix.matrix[(3 * 4) + 0];
		result.matrix[(3 * 4) + 1] = matrix[(3 * 4) + 1] + add_matrix.matrix[(3 * 4) + 1];
		result.matrix[(3 * 4) + 2] = matrix[(3 * 4) + 2] + add_matrix.matrix[(3 * 4) + 2];
		result.matrix[(3 * 4) + 3] = matrix[(3 * 4) + 3] + add_matrix.matrix[(3 * 4) + 3];
		return result;
	}

	template<typename Type>
	Mat4<Type> Mat4<Type>::operator -(const Mat4<Type> &sub_matrix) const
	{
		Mat4<Type> result;
		result.matrix[(0 * 4) + 0] = matrix[(0 * 4) + 0] - sub_matrix.matrix[(0 * 4) + 0];
		result.matrix[(0 * 4) + 1] = matrix[(0 * 4) + 1] - sub_matrix.matrix[(0 * 4) + 1];
		result.matrix[(0 * 4) + 2] = matrix[(0 * 4) + 2] - sub_matrix.matrix[(0 * 4) + 2];
		result.matrix[(0 * 4) + 3] = matrix[(0 * 4) + 3] - sub_matrix.matrix[(0 * 4) + 3];
		result.matrix[(1 * 4) + 0] = matrix[(1 * 4) + 0] - sub_matrix.matrix[(1 * 4) + 0];
		result.matrix[(1 * 4) + 1] = matrix[(1 * 4) + 1] - sub_matrix.matrix[(1 * 4) + 1];
		result.matrix[(1 * 4) + 2] = matrix[(1 * 4) + 2] - sub_matrix.matrix[(1 * 4) + 2];
		result.matrix[(1 * 4) + 3] = matrix[(1 * 4) + 3] - sub_matrix.matrix[(1 * 4) + 3];
		result.matrix[(2 * 4) + 0] = matrix[(2 * 4) + 0] - sub_matrix.matrix[(2 * 4) + 0];
		result.matrix[(2 * 4) + 1] = matrix[(2 * 4) + 1] - sub_matrix.matrix[(2 * 4) + 1];
		result.matrix[(2 * 4) + 2] = matrix[(2 * 4) + 2] - sub_matrix.matrix[(2 * 4) + 2];
		result.matrix[(2 * 4) + 3] = matrix[(2 * 4) + 3] - sub_matrix.matrix[(2 * 4) + 3];
		result.matrix[(3 * 4) + 0] = matrix[(3 * 4) + 0] - sub_matrix.matrix[(3 * 4) + 0];
		result.matrix[(3 * 4) + 1] = matrix[(3 * 4) + 1] - sub_matrix.matrix[(3 * 4) + 1];
		result.matrix[(3 * 4) + 2] = matrix[(3 * 4) + 2] - sub_matrix.matrix[(3 * 4) + 2];
		result.matrix[(3 * 4) + 3] = matrix[(3 * 4) + 3] - sub_matrix.matrix[(3 * 4) + 3];
		return result;
	}

	template<typename Type>
	double Mat4<Type>::det() const
	{
		Mat3<Type> temp_matrix;
		Type a, b, c, d;
		Type value;

		temp_matrix = Mat3<Type>(
			matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2],
			matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		a = (Type)temp_matrix.det();

		temp_matrix = Mat3<Type>(
			matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2],
			matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		b = (Type)temp_matrix.det();

		temp_matrix = Mat3<Type>(
			matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		c = (Type)temp_matrix.det();

		temp_matrix = Mat3<Type>(
			matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2]);
		d = (Type)temp_matrix.det();

		value = matrix[0 * 4 + 0] * a;
		value -= matrix[0 * 4 + 1] * b;
		value += matrix[0 * 4 + 2] * c;
		value -= matrix[0 * 4 + 3] * d;

		return value;
	}

	template<typename Type>
	Mat4<Type> &Mat4<Type>::adjoint()
	{
		Mat4<Type> result;
		Mat3<Type> m;

		m = Mat3<Type>(
			matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2],
			matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[0 * 4 + 0] = (Type)m.det();

		m = Mat3<Type>(
			matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2],
			matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[1 * 4 + 0] = (Type)-m.det();

		m = Mat3<Type>(
			matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[2 * 4 + 0] = (Type)m.det();

		m = Mat3<Type>(
			matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2]);
		result.matrix[3 * 4 + 0] = (Type)-m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[0 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2],
			matrix[0 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[0 * 4 + 1] = (Type)-m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[0 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2],
			matrix[0 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[1 * 4 + 1] = (Type)m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[0 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[0 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[2 * 4 + 1] = (Type)-m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
			matrix[0 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
			matrix[0 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2]);
		result.matrix[3 * 4 + 1] = (Type)m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 1], matrix[1 * 4 + 1], matrix[3 * 4 + 1],
			matrix[0 * 4 + 2], matrix[1 * 4 + 2], matrix[3 * 4 + 2],
			matrix[0 * 4 + 3], matrix[1 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[0 * 4 + 2] = (Type)m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[1 * 4 + 0], matrix[3 * 4 + 0],
			matrix[0 * 4 + 2], matrix[1 * 4 + 2], matrix[3 * 4 + 2],
			matrix[0 * 4 + 3], matrix[1 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[1 * 4 + 2] = (Type)-m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[1 * 4 + 0], matrix[3 * 4 + 0],
			matrix[0 * 4 + 1], matrix[1 * 4 + 1], matrix[3 * 4 + 1],
			matrix[0 * 4 + 3], matrix[1 * 4 + 3], matrix[3 * 4 + 3]);
		result.matrix[2 * 4 + 2] = (Type)m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[1 * 4 + 0], matrix[3 * 4 + 0],
			matrix[0 * 4 + 1], matrix[1 * 4 + 1], matrix[3 * 4 + 1],
			matrix[0 * 4 + 2], matrix[1 * 4 + 2], matrix[3 * 4 + 2]);
		result.matrix[3 * 4 + 2] = (Type)-m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 1], matrix[1 * 4 + 1], matrix[2 * 4 + 1],
			matrix[0 * 4 + 2], matrix[1 * 4 + 2], matrix[2 * 4 + 2],
			matrix[0 * 4 + 3], matrix[1 * 4 + 3], matrix[2 * 4 + 3]);
		result.matrix[0 * 4 + 3] = (Type)-m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[1 * 4 + 0], matrix[2 * 4 + 0],
			matrix[0 * 4 + 2], matrix[1 * 4 + 2], matrix[2 * 4 + 2],
			matrix[0 * 4 + 3], matrix[1 * 4 + 3], matrix[2 * 4 + 3]);
		result.matrix[1 * 4 + 3] = (Type)m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[1 * 4 + 0], matrix[2 * 4 + 0],
			matrix[0 * 4 + 1], matrix[1 * 4 + 1], matrix[2 * 4 + 1],
			matrix[0 * 4 + 3], matrix[1 * 4 + 3], matrix[2 * 4 + 3]);
		result.matrix[2 * 4 + 3] = (Type)-m.det();

		m = Mat3<Type>(
			matrix[0 * 4 + 0], matrix[1 * 4 + 0], matrix[2 * 4 + 0],
			matrix[0 * 4 + 1], matrix[1 * 4 + 1], matrix[2 * 4 + 1],
			matrix[0 * 4 + 2], matrix[1 * 4 + 2], matrix[2 * 4 + 2]);
		result.matrix[3 * 4 + 3] = (Type)m.det();

		*this = result;
		return *this;
	}

	template<typename Type>
	Mat4<Type> &Mat4<Type>::inverse()
	{
		double d;

		d = det();

		// Inverse unknown when determinant is close to zero
		if (fabs(d) < 1e-15)
		{
			*this = null();
		}
		else
		{
			Mat4<Type> result = *this;
			result.adjoint();

			d = 1.0 / d;	// Inverse the determinant
			for (int i = 0; i < 16; i++)
			{
				result.matrix[i] = (Type)(result.matrix[i] * d);
			}

			*this = result;
		}
		return *this;
	}

	template<typename Type>
	Mat4<Type> &Mat4<Type>::transpose()
	{
		Type original[16];
		for (int cnt = 0; cnt < 16; cnt++)
			original[cnt] = matrix[cnt];

		matrix[0] = original[0];
		matrix[1] = original[4];
		matrix[2] = original[8];
		matrix[3] = original[12];
		matrix[4] = original[1];
		matrix[5] = original[5];
		matrix[6] = original[9];
		matrix[7] = original[13];
		matrix[8] = original[2];
		matrix[9] = original[6];
		matrix[10] = original[10];
		matrix[11] = original[14];
		matrix[12] = original[3];
		matrix[13] = original[7];
		matrix[14] = original[11];
		matrix[15] = original[15];

		return *this;
	}

	template<>
	Mat4<float> &Mat4<float>::transpose()
	{
#if !defined CL_DISABLE_SSE2
		__m128 row0 = _mm_loadu_ps(matrix);
		__m128 row1 = _mm_loadu_ps(matrix + 4);
		__m128 row2 = _mm_loadu_ps(matrix + 8);
		__m128 row3 = _mm_loadu_ps(matrix + 12);

		_MM_TRANSPOSE4_PS(row0, row1, row2, row3);

		_mm_storeu_ps(matrix, row0);
		_mm_storeu_ps(matrix + 4, row1);
		_mm_storeu_ps(matrix + 8, row2);
		_mm_storeu_ps(matrix + 12, row3);
#else
		float original[16];
		for (int cnt = 0; cnt<16; cnt++)
			original[cnt] = matrix[cnt];

		matrix[0] = original[0];
		matrix[1] = original[4];
		matrix[2] = original[8];
		matrix[3] = original[12];
		matrix[4] = original[1];
		matrix[5] = original[5];
		matrix[6] = original[9];
		matrix[7] = original[13];
		matrix[8] = original[2];
		matrix[9] = original[6];
		matrix[10] = original[10];
		matrix[11] = original[14];
		matrix[12] = original[3];
		matrix[13] = original[7];
		matrix[14] = original[11];
		matrix[15] = original[15];
#endif
		return *this;
	}

	template<typename Type>
	void Mat4<Type>::decompose(Vec3<Type> &out_position, Quaternionx<Type> &out_orientation, Vec3<Type> &out_scale) const
	{
		out_scale = Vec3<Type>(
			Vec3<Type>(matrix[0], matrix[1], matrix[2]).length(),
			Vec3<Type>(matrix[4], matrix[5], matrix[6]).length(),
			Vec3<Type>(matrix[8], matrix[9], matrix[10]).length());
		out_orientation = Quaternionx<Type>(Mat4<Type>::scale(Type(1) / out_scale) * (*this));
		out_position = Vec3<Type>(matrix[12], matrix[13], matrix[14]);
	}

	template<>
	void Mat4<int>::decompose(Vec3<int> &out_position, Quaternionx<int> &out_orientation, Vec3<int> &out_scale) const
	{
		throw Exception("Mat4::decompose not implemented for Mat4i");
	}

	// Explicit instantiate the versions we use:
	template class Mat4<int>;
	template class Mat4<float>;
	template class Mat4<double>;
}

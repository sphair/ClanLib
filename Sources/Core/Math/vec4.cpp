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
#include "API/Core/Math/vec4.h"
#include "API/Core/Math/angle.h"

namespace clan
{
	template<typename Type>
	Vec4<Type> &Vec4<Type>::normalize3()
	{
		Type f = length3();
		if (f != 0)
		{
			x /= f;
			y /= f;
			z /= f;
		}
		return *this;
	}

	template<typename Type>
	Vec4<Type> Vec4<Type>::normalize3(const Vec4<Type>& vector)
	{
		Vec4<Type> dest(vector);
		dest.normalize3();
		return dest;
	}

	template<typename Type>
	Vec4<Type> &Vec4<Type>::normalize4()
	{
		Type f = length4();
		if (f != 0)
		{
			x /= f;
			y /= f;
			z /= f;
			w /= f;
		}
		return *this;
	}

	template<typename Type>
	Vec4<Type> Vec4<Type>::normalize4(const Vec4<Type>& vector)
	{
		Vec4<Type> dest(vector);
		dest.normalize4();
		return dest;
	}

	template<typename Type>
	Angle Vec4<Type>::angle3(const Vec4<Type>& v) const
	{
		return Angle(acosf(float(dot3(v) / (length3()*v.length3()))), AngleUnit::radians);
	}

	// For floats
	template<>
	float Vec4f::distance3(const Vec4f &vector) const
	{
		float value_x, value_y, value_z;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		return sqrt(value_x*value_x + value_y*value_y + value_z*value_z);
	}

	// For doubles
	template<>
	double Vec4d::distance3(const Vec4d &vector) const
	{
		double value_x, value_y, value_z;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		return sqrt(value_x*value_x + value_y*value_y + value_z*value_z);
	}

	// For integers
	template<typename Type>
	Type Vec4<Type>::distance3(const Vec4<Type>& vector) const
	{
		float value_x, value_y, value_z;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		return (Type)floor(sqrt(value_x*value_x + value_y*value_y + value_z*value_z) + 0.5f);
	}

	// For integers
	template<typename Type>
	Type Vec4<Type>::distance4(const Vec4<Type>& vector) const
	{
		float value_x, value_y, value_z, value_w;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		value_w = w - vector.w;
		return (Type)floor(sqrt(value_x*value_x + value_y*value_y + value_z*value_z + value_w*value_w) + 0.5f);
	}

	// For double
	template<>
	double Vec4d::distance4(const Vec4d &vector) const
	{
		double value_x, value_y, value_z, value_w;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		value_w = w - vector.w;
		return sqrt(value_x*value_x + value_y*value_y + value_z*value_z + value_w*value_w);
	}

	// For float
	template<>
	float Vec4f::distance4(const Vec4f &vector) const
	{
		float value_x, value_y, value_z, value_w;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		value_w = w - vector.w;
		return sqrt(value_x*value_x + value_y*value_y + value_z*value_z + value_w*value_w);
	}

	template<typename Type>
	Vec4<Type> Vec4<Type>::cross3(const Vec4<Type>& v1, const Vec4<Type>& v2)
	{
		Vec4<Type> tmp = Vec4<Type>(v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x, 0);
		return tmp;
	}

	template<typename Type>
	Vec4<Type> &Vec4<Type>::cross3(const Vec4<Type>& v)
	{
		*this = cross3(*this, v);
		return *this;
	}

	// Same as glRotate[f/d](angle, a);
	template<typename Type>
	Vec4<Type> &Vec4<Type>::rotate3(const Angle &angle, const Vec4<Type>& a)
	{
		Vec4<Type> tmp = Vec4<Type>();

		float s = sin(angle.to_radians());
		float c = cos(angle.to_radians());

		tmp.x = (Type)(x*(a.x*a.x*(1 - c) + c) + y*(a.x*a.y*(1 - c) - a.z*s) + z*(a.x*a.z*(1 - c) + a.y*s));
		tmp.y = (Type)(x*(a.y*a.x*(1 - c) + a.z*s) + y*(a.y*a.y*(1 - c) + c) + z*(a.y*a.z*(1 - c) - a.x*s));
		tmp.z = (Type)(x*(a.x*a.z*(1 - c) - a.y*s) + y*(a.y*a.z*(1 - c) + a.x*s) + z*(a.z*a.z*(1 - c) + c));
		*this = tmp;
		return *this;
	}

	template<typename Type>
	Vec4<Type> Vec4<Type>::rotate3(const Vec4<Type>& vector, const Angle &angle, const Vec4<Type>& a)
	{
		Vec4<Type> dest(vector);
		dest.rotate3(angle, a);
		return dest;
	}

	template<typename Type>
	Vec4<Type> &Vec4<Type>::round()
	{
		x = (Type)floorf(x + 0.5f);
		y = (Type)floorf(y + 0.5f);
		z = (Type)floorf(z + 0.5f);
		w = (Type)floorf(w + 0.5f);
		return *this;
	}

	template<typename Type>
	Vec4<Type> Vec4<Type>::round(const Vec4<Type>& vector)
	{
		Vec4<Type> dest(vector);
		dest.round();
		return dest;
	}

	// Explicit instantiate the versions we use:
	template class Vec4<unsigned char>;
	template class Vec4<char>;
	template class Vec4<unsigned short>;
	template class Vec4<short>;
	template class Vec4<unsigned int>;
	template class Vec4<int>;
	template class Vec4<float>;
	template class Vec4<double>;
}

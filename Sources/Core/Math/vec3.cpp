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
#include "API/Core/Math/vec3.h"
#include "API/Core/Math/angle.h"

namespace clan
{
	template<typename Type>
	Angle Vec3<Type>::angle(const Vec3<Type>& v) const
	{
		return Angle(acosf(float(dot(v) / (length()*v.length()))), angle_radians);
	}

	template<typename Type>
	Angle Vec3<Type>::angle_normed(const Vec3<Type>& v) const
	{
		return Angle(acosf(float(dot(v))), angle_radians);
	}

	template<typename Type>
	Vec3<Type> Vec3<Type>::cross(const Vec3<Type>& v1, const Vec3<Type>& v2)
	{
		Vec3<Type> tmp = Vec3<Type>(v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
		return tmp;
	}

	template<typename Type>
	Vec3<Type> &Vec3<Type>::cross(const Vec3<Type>& v)
	{
		*this = cross(*this, v);
		return *this;
	}

	// For integers
	template<typename Type>
	Type Vec3<Type>::distance(const Vec3<Type>& vector) const
	{
		float value_x, value_y, value_z;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		return (Type)floor(sqrt(value_x*value_x + value_y*value_y + value_z*value_z) + 0.5f);
	}

	// For floats
	template<>
	float Vec3f::distance(const Vec3f &vector) const
	{
		float value_x, value_y, value_z;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		return sqrt(value_x*value_x + value_y*value_y + value_z*value_z);
	}

	// For doubles
	template<>
	double Vec3d::distance(const Vec3d &vector) const
	{
		double value_x, value_y, value_z;
		value_x = x - vector.x;
		value_y = y - vector.y;
		value_z = z - vector.z;
		return sqrt(value_x*value_x + value_y*value_y + value_z*value_z);
	}

	// Same as glRotate[f|d](angle, a);
	template<typename Type>
	Vec3<Type> &Vec3<Type>::rotate(const Angle &angle, const Vec3<Type>& a)
	{
		Vec3<Type> tmp = Vec3<Type>();

		float s = sin(angle.to_radians());
		float c = cos(angle.to_radians());

		tmp.x = (Type)(x*(a.x*a.x*(1 - c) + c) + y*(a.x*a.y*(1 - c) - a.z*s) + z*(a.x*a.z*(1 - c) + a.y*s));
		tmp.y = (Type)(x*(a.y*a.x*(1 - c) + a.z*s) + y*(a.y*a.y*(1 - c) + c) + z*(a.y*a.z*(1 - c) - a.x*s));
		tmp.z = (Type)(x*(a.x*a.z*(1 - c) - a.y*s) + y*(a.y*a.z*(1 - c) + a.x*s) + z*(a.z*a.z*(1 - c) + c));
		*this = tmp;
		return *this;
	}

	template<typename Type>
	Vec3<Type> Vec3<Type>::rotate(const Vec3<Type> &vector, const Angle &angle, const Vec3<Type>& a)
	{
		Vec3<Type> dest(vector);
		dest.rotate(angle, a);
		return dest;
	}

	template<typename Type>
	Vec3<Type> &Vec3<Type>::round()
	{
		x = (Type)floorf(x + 0.5f);
		y = (Type)floorf(y + 0.5f);
		z = (Type)floorf(z + 0.5f);
		return *this;
	}

	template<typename Type>
	Vec3<Type> Vec3<Type>::round(const Vec3<Type>& vector)
	{
		Vec3<Type> dest(vector);
		dest.round();
		return dest;
	}

	template<typename Type>
	Vec3<Type> Vec3<Type>::reflect(const Vec3<Type>& incident, const Vec3<Type>& normal)
	{
		return incident - normal.dot(incident) * normal * Type(2);
	}

	// Explicit instantiate the versions we use:
	template class Vec3<unsigned char>;
	template class Vec3<char>;
	template class Vec3<unsigned short>;
	template class Vec3<short>;
	template class Vec3<unsigned int>;
	template class Vec3<int>;
	template class Vec3<float>;
	template class Vec3<double>;
}

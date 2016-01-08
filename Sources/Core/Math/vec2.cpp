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
#include "API/Core/Math/vec2.h"
#include "API/Core/Math/point.h"

namespace clan
{
	template<typename Type>
	Angle Vec2<Type>::angle(const Vec2<Type>& v) const
	{
		return Angle((float)acosf(float(dot(v) / (length()*v.length()))), angle_radians);
	}

	template<>
	Angle Vec2<int>::angle(const Vec2<int>& v) const
	{
		float this_x = (float)x;
		float this_y = (float)y;
		float v_x = (float)v.x;
		float v_y = (float)v.y;

		float dot_v = this_x * v_x + this_y * v_y;
		float this_length = sqrt(this_x * this_x + this_y * this_y);
		float v_length = sqrt(v_x * v_x + v_y * v_y);
		return Angle(acosf(dot_v / (this_length * v_length)), angle_radians);
	}

	template<typename Type>
	Angle Vec2<Type>::angle_normed(const Vec2<Type>& v) const
	{
		return Angle((float)acosf(float(dot(v))), angle_radians);
	}

	template<>
	Angle Vec2<int>::angle_normed(const Vec2<int>& v) const
	{
		float this_x = (float)x;
		float this_y = (float)y;
		float v_x = (float)v.x;
		float v_y = (float)v.y;

		float dot_v = this_x * v_x + this_y * v_y;
		return Angle(acosf(dot_v), angle_radians);
	}

	template<typename Type>
	Angle Vec2<Type>::angle_line(const Vec2<Type>& v) const
	{
		return Angle(atan2f(v.y - y, v.x - x), angle_radians);
	}

	template<>
	Angle Vec2<int>::angle_line(const Vec2<int>& v) const
	{
		float this_x = (float)x;
		float this_y = (float)y;
		float v_x = (float)v.x;
		float v_y = (float)v.y;

		return Angle(atan2f(v_y - this_y, v_x - this_x), angle_radians);
	}

	template<typename Type>
	Vec2<Type> &Vec2<Type>::round()
	{
		x = (Type)floorf(x + 0.5f);
		y = (Type)floorf(y + 0.5f);
		return *this;
	}

	// For integers
	template<typename Type>
	Type Vec2<Type>::distance(const Vec2<Type>& vector) const
	{
		float value_x, value_y;
		value_x = x - vector.x;
		value_y = y - vector.y;
		return (Type)floor(sqrt(value_x*value_x + value_y*value_y) + 0.5f);
	}

	// For floats
	template<>
	float Vec2f::distance(const Vec2f &vector) const
	{
		float value_x, value_y;
		value_x = x - vector.x;
		value_y = y - vector.y;
		return sqrt(value_x*value_x + value_y*value_y);
	}

	// For doubles
	template<>
	double Vec2d::distance(const Vec2d &vector) const
	{
		double value_x, value_y;
		value_x = x - vector.x;
		value_y = y - vector.y;
		return sqrt(value_x*value_x + value_y*value_y);
	}

	template<typename Type>
	Vec2<Type> Vec2<Type>::round(const Vec2<Type>& vector)
	{
		Vec2<Type> dest(vector);
		dest.round();
		return dest;
	}

	template<typename Type>
	Vec2<Type> &Vec2<Type>::rotate(const Vec2<Type>& hotspot, const Angle &angle)
	{
		// Move the hotspot to 0,0
		Vec2<Type> r(x - hotspot.x, y - hotspot.y);

		float radians = angle.to_radians();
		float sin_angle = sinf(radians);
		float cos_angle = cosf(radians);

		float dest_x = (float)r.x * cos_angle - (float)r.y * sin_angle;
		float dest_y = (float)r.x * sin_angle + (float)r.y * cos_angle;

		x = round_value(dest_x + hotspot.x);
		y = round_value(dest_y + hotspot.y);
		return *this;
	}

	template<typename Type>
	Vec2<Type> Vec2<Type>::rotate(const Vec2<Type>& vector, const Vec2<Type>& hotspot, const Angle &angle)
	{
		Vec2<Type> dest(vector);
		dest.rotate(hotspot, angle);
		return dest;
	}

	template<typename Type>
	Type Vec2<Type>::round_value(float value) const { return (Type)value; }

	template<>
	int Vec2<int>::round_value(float value) const { return (int)floorf(value + 0.5f); }

	template<>
	unsigned int Vec2<unsigned int>::round_value(float value) const { return (unsigned int)floorf(value + 0.5f); }

	template<>
	short Vec2<short>::round_value(float value) const { return (short)floorf(value + 0.5f); }

	template<>
	unsigned short Vec2<unsigned short>::round_value(float value) const { return (unsigned short)floorf(value + 0.5f); }

	template<>
	char Vec2<char>::round_value(float value) const { return (char)floorf(value + 0.5f); }

	template<>
	unsigned char Vec2<unsigned char>::round_value(float value) const { return (unsigned char)floorf(value + 0.5f); }

	template<typename Type>
	Pointx<Type> Vec2<Type>::calc_origin(Origin origin, const Sizex<Type> &size)
	{
		switch (origin)
		{
		case origin_top_left:
		default:
			return Pointx<Type>(0, 0);
			break;
		case origin_top_center:
			return Pointx<Type>(size.width / 2, 0);
			break;
		case origin_top_right:
			return Pointx<Type>(size.width, 0);
			break;
		case origin_center_left:
			return Pointx<Type>(0, size.height / 2);
			break;
		case origin_center:
			return Pointx<Type>(size.width / 2, size.height / 2);
			break;
		case origin_center_right:
			return Pointx<Type>(size.width, size.height / 2);
			break;
		case origin_bottom_left:
			return Pointx<Type>(0, size.height);
			break;
		case origin_bottom_center:
			return Pointx<Type>(size.width / 2, size.height);
			break;
		case origin_bottom_right:
			return Pointx<Type>(size.width, size.height);
			break;
		}
	}

	// Explicit instantiate the versions we use:
	template class Vec2<unsigned char>;
	template class Vec2<char>;
	template class Vec2<unsigned short>;
	template class Vec2<short>;
	template class Vec2<unsigned int>;
	template class Vec2<int>;
	template class Vec2<float>;
	template class Vec2<double>;
}

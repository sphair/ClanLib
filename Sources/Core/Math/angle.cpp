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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{
	Angle::Angle() : value_rad(0)
	{
	}

	Angle::Angle(float value, AngleUnit unit)
	{
		if (unit == angle_radians)
		{
			value_rad = value;
		}
		else
		{
			value_rad = value * float(PI) / 180.0f;
		}
	}

	Angle Angle::from_radians(float value)
	{
		return Angle(value, angle_radians);
	}

	Angle Angle::from_degrees(float value)
	{
		return Angle(value, angle_degrees);
	}

	float Angle::to_degrees() const
	{
		return value_rad * 180.0f / float(PI);
	}

	float Angle::to_radians() const
	{
		return value_rad;
	}

	void Angle::set_degrees(float value_degrees)
	{
		value_rad = value_degrees * float(PI) / 180.0f;
	}

	void Angle::set_radians(float value_radians)
	{
		value_rad = value_radians;
	}

	Angle &Angle::normalize()
	{
		value_rad -= (2.0f * PI) * std::floor(value_rad / (2.0f * PI));
		return *this;
	}

	Angle &Angle::normalize_180()
	{
		normalize();
		if (value_rad > PI)
			value_rad -= PI * 2.0f;
		return *this;
	}

	void Angle::operator+=(const Angle &angle)
	{
		value_rad += angle.value_rad;
	}

	void Angle::operator-=(const Angle &angle)
	{
		value_rad -= angle.value_rad;
	}

	void Angle::operator*=(const Angle &angle)
	{
		value_rad *= angle.value_rad;
	}

	void Angle::operator/=(const Angle &angle)
	{
		value_rad /= angle.value_rad;
	}

	Angle Angle::operator+(const Angle &angle) const
	{
		return Angle(value_rad + angle.value_rad, angle_radians);
	}

	Angle Angle::operator-(const Angle &angle) const
	{
		return Angle(value_rad - angle.value_rad, angle_radians);
	}

	Angle Angle::operator*(const Angle &angle) const
	{
		return Angle(value_rad * angle.value_rad, angle_radians);
	}

	Angle Angle::operator*(float value) const
	{
		return Angle(value_rad * value, angle_radians);
	}

	Angle Angle::operator/(const Angle &angle) const
	{
		return Angle(value_rad / angle.value_rad, angle_radians);
	}

	Angle Angle::operator/(float value) const
	{
		return Angle(value_rad / value, angle_radians);
	}

	bool Angle::operator<(const Angle &angle) const
	{
		return value_rad < angle.value_rad;
	}

	bool Angle::operator>(const Angle &angle) const
	{
		return value_rad > angle.value_rad;
	}

	bool Angle::operator<=(const Angle &angle) const
	{
		return value_rad <= angle.value_rad;
	}

	bool Angle::operator>=(const Angle &angle) const
	{
		return value_rad >= angle.value_rad;
	}

	bool Angle::operator==(const Angle &angle) const
	{
		return value_rad == angle.value_rad;
	}

	bool Angle::operator!=(const Angle &angle) const
	{
		return value_rad != angle.value_rad;
	}
}

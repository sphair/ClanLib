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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Math/cl_math.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Angle Construction:

CL_Angle::CL_Angle() : value_rad(0)
{
}

CL_Angle::CL_Angle(float value, CL_AngleUnit unit)
{
	if (unit == cl_radians)
	{
		value_rad = value;
	}
	else
	{
		value_rad = value * float(CL_PI) / 180.0f;
	}
}

CL_Angle CL_Angle::from_radians(float value)
{
	return CL_Angle(value, cl_radians);
}

CL_Angle CL_Angle::from_degrees(float value)
{
	return CL_Angle(value, cl_degrees);
}


/////////////////////////////////////////////////////////////////////////////
// CL_Angle Attributes:

float CL_Angle::to_degrees() const
{
	return value_rad * 180.0f / float(CL_PI);
}

float CL_Angle::to_radians() const
{
	return value_rad;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Angle Operations:

void CL_Angle::set_degrees(float degrees)
{
	value_rad = degrees * float(CL_PI) / 180.0f;
}

void CL_Angle::set_radians(float radians)
{
	value_rad = radians;
}

CL_Angle &CL_Angle::normalize()
{
	value_rad = fmod(value_rad, CL_PI*2.0f);
	if (value_rad < 0.0f)
		value_rad += CL_PI*2.0f;
	return *this;
}

CL_Angle &CL_Angle::normalize_180()
{
	normalize();
	if (value_rad > CL_PI)
		value_rad -= CL_PI * 2.0f;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Angle Operators:

void CL_Angle::operator+=( const CL_Angle &angle )
{
	value_rad += angle.value_rad;
}

void CL_Angle::operator-=( const CL_Angle &angle )
{
	value_rad -= angle.value_rad;
}

void CL_Angle::operator*=( const CL_Angle &angle )
{
	value_rad *= angle.value_rad;
}

void CL_Angle::operator/=( const CL_Angle &angle )
{
	value_rad /= angle.value_rad;
}

CL_Angle CL_Angle::operator+( const CL_Angle &angle ) const
{
	return CL_Angle(value_rad + angle.value_rad, cl_radians);
}

CL_Angle CL_Angle::operator-( const CL_Angle &angle ) const
{
	return CL_Angle(value_rad - angle.value_rad, cl_radians);
}

CL_Angle CL_Angle::operator*( const CL_Angle &angle ) const
{
	return CL_Angle(value_rad * angle.value_rad, cl_radians);
}

CL_Angle CL_Angle::operator*( float value ) const
{
	return CL_Angle(value_rad * value, cl_radians);
}

CL_Angle CL_Angle::operator/( const CL_Angle &angle ) const
{
	return CL_Angle(value_rad / angle.value_rad, cl_radians);
}

CL_Angle CL_Angle::operator/( float value ) const
{
	return CL_Angle(value_rad / value, cl_radians);
}

bool CL_Angle::operator<( const CL_Angle &angle ) const
{
	return value_rad < angle.value_rad;
}

bool CL_Angle::operator>( const CL_Angle &angle ) const
{
	return value_rad > angle.value_rad;
}

bool CL_Angle::operator<=( const CL_Angle &angle ) const
{
	return value_rad <= angle.value_rad;
}

bool CL_Angle::operator>=( const CL_Angle &angle ) const
{
	return value_rad >= angle.value_rad;
}

bool CL_Angle::operator==( const CL_Angle &angle ) const
{
	return value_rad == angle.value_rad;
}

bool CL_Angle::operator!=( const CL_Angle &angle ) const
{
	return value_rad != angle.value_rad;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Angle Implementation:


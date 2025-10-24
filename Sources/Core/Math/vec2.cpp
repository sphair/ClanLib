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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/Math/vec2.h"
#include "API/Core/Math/point.h"

template<typename Type>
CL_Vec2<Type>::CL_Vec2(const CL_Pointx<int> &point) { x = (Type) point.x; y = (Type) point.y; }

template<typename Type>
CL_Vec2<Type>::CL_Vec2(const CL_Pointx<float> &point) { x = (Type) point.x; y = (Type) point.y; }

template<typename Type>
CL_Vec2<Type>::CL_Vec2(const CL_Pointx<double> &point) { x = (Type) point.x; y = (Type) point.y; }

// Add some rounding specializations

template<>
CL_Vec2<int>::CL_Vec2(const CL_Pointx<int> &point) { x = point.x ; y = point.y; }

template<>
CL_Vec2<int>::CL_Vec2(const CL_Pointx<float> &point) { x = (int) floor(point.x + 0.5f); y = (int) floor(point.y + 0.5f); }

template<>
CL_Vec2<int>::CL_Vec2(const CL_Pointx<double> &point) { x = (int) floor(point.x + 0.5f); y = (int) floor(point.y + 0.5); }

template<>
CL_Vec2<unsigned int>::CL_Vec2(const CL_Pointx<int> &point) { x = (unsigned int) point.x ; y = (unsigned int) point.y; }

template<>
CL_Vec2<unsigned int>::CL_Vec2(const CL_Pointx<float> &point) { x = (unsigned int) floor(point.x + 0.5f); y = (unsigned int) floor(point.y + 0.5f); }

template<>
CL_Vec2<unsigned int>::CL_Vec2(const CL_Pointx<double> &point) { x = (unsigned int) floor(point.x + 0.5f); y = (unsigned int) floor(point.y + 0.5); }

template<>
CL_Vec2<short>::CL_Vec2(const CL_Pointx<int> &point) { x = point.x ; y = point.y; }

template<>
CL_Vec2<short>::CL_Vec2(const CL_Pointx<float> &point) { x = (short) floor(point.x + 0.5f); y = (short) floor(point.y + 0.5f); }

template<>
CL_Vec2<short>::CL_Vec2(const CL_Pointx<double> &point) { x = (short) floor(point.x + 0.5f); y = (short) floor(point.y + 0.5); }

template<>
CL_Vec2<unsigned short>::CL_Vec2(const CL_Pointx<int> &point) { x = (unsigned short) point.x ; y = (unsigned short) point.y; }

template<>
CL_Vec2<unsigned short>::CL_Vec2(const CL_Pointx<float> &point) { x = (unsigned short) floor(point.x + 0.5f); y = (unsigned short) floor(point.y + 0.5f); }

template<>
CL_Vec2<unsigned short>::CL_Vec2(const CL_Pointx<double> &point) { x = (unsigned short) floor(point.x + 0.5f); y = (unsigned short) floor(point.y + 0.5); }

template<>
CL_Vec2<char>::CL_Vec2(const CL_Pointx<int> &point) { x = point.x ; y = point.y; }

template<>
CL_Vec2<char>::CL_Vec2(const CL_Pointx<float> &point) { x = (char) floor(point.x + 0.5f); y = (char) floor(point.y + 0.5f); }

template<>
CL_Vec2<char>::CL_Vec2(const CL_Pointx<double> &point) { x = (char) floor(point.x + 0.5f); y = (char) floor(point.y + 0.5); }

template<>
CL_Vec2<unsigned char>::CL_Vec2(const CL_Pointx<int> &point) { x = (unsigned char) point.x ; y = (unsigned char) point.y; }

template<>
CL_Vec2<unsigned char>::CL_Vec2(const CL_Pointx<float> &point) { x = (unsigned char) floor(point.x + 0.5f); y = (unsigned char) floor(point.y + 0.5f); }

template<>
CL_Vec2<unsigned char>::CL_Vec2(const CL_Pointx<double> &point) { x = (unsigned char) floor(point.x + 0.5f); y = (unsigned char) floor(point.y + 0.5); }

template<typename Type>
Type CL_Vec2<Type>::length() const {return (Type) floor(sqrt(float(x*x+y*y))+0.5f);}

template<>
double CL_Vec2<double>::length() const {return sqrt(x*x+y*y);}

template<>
float CL_Vec2<float>::length() const {return sqrt(x*x+y*y);}

template<typename Type>
CL_Vec2<Type> &CL_Vec2<Type>::normalize()
{
	Type f = length();
	if (f!=0)
	{
		x /= f;
		y /= f;
	}
	return *this;
}

template<typename Type>
CL_Vec2<Type> CL_Vec2<Type>::normalize(const CL_Vec2<Type>& vector)
{
	CL_Vec2<Type> dest(vector);
	dest.normalize();
	return dest;
}

template<typename Type>
CL_Angle CL_Vec2<Type>::angle(const CL_Vec2<Type>& v) const
{
	return CL_Angle((float)acosf(float(dot(v)/(length()*v.length()))), cl_radians);
}


template<typename Type>
CL_Vec2<Type> &CL_Vec2<Type>::round()
{
	x = (Type) floorf(x+0.5f);
	y = (Type) floorf(y+0.5f);
	return *this;
}

// For integers
template<typename Type>
Type CL_Vec2<Type>::distance(const CL_Vec2<Type>& vector) const
{
	float value_x, value_y;
	value_x = x - vector.x;
	value_y = y - vector.y;
	return (Type) floor(sqrt(value_x*value_x + value_y*value_y)+0.5f);
}

// For floats
template<>
float CL_Vec2f::distance(const CL_Vec2f &vector) const
{
	float value_x, value_y;
	value_x = x - vector.x;
	value_y = y - vector.y;
	return sqrt(value_x*value_x + value_y*value_y);
}

// For doubles
template<>
double CL_Vec2d::distance(const CL_Vec2d &vector) const
{
	double value_x, value_y;
	value_x = x - vector.x;
	value_y = y - vector.y;
	return sqrt(value_x*value_x + value_y*value_y);
}

template<typename Type>
CL_Vec2<Type> CL_Vec2<Type>::round(const CL_Vec2<Type>& vector)
{
	CL_Vec2<Type> dest(vector);
	dest.round();
	return dest;
}

template<typename Type>
CL_Vec2<Type> &CL_Vec2<Type>::rotate(const CL_Vec2<Type>& hotspot, const CL_Angle &angle)
{
	// Move the hotspot to 0,0
	CL_Vec2<Type> r(x - hotspot.x, y - hotspot.y);

	float radians = angle.to_radians();
	float sin_angle = sinf(radians);
	float cos_angle = cosf(radians);

	float dest_x = (float) r.x * cos_angle - (float) r.y * sin_angle;
	float dest_y = (float) r.x * sin_angle + (float) r.y * cos_angle;

	x = round_value(dest_x + hotspot.x);
	y = round_value(dest_y + hotspot.y);
	return *this;
}

template<typename Type>
CL_Vec2<Type> CL_Vec2<Type>::rotate(const CL_Vec2<Type>& vector, const CL_Vec2<Type>& hotspot, const CL_Angle &angle)
{
	CL_Vec2<Type> dest(vector);
	dest.rotate(hotspot, angle);
	return dest;
}

template<typename Type>
Type CL_Vec2<Type>::round_value(float value) const { return (Type) value; }

template<>
int CL_Vec2<int>::round_value(float value) const { return (int) floorf(value + 0.5f); }

template<>
unsigned int CL_Vec2<unsigned int>::round_value(float value) const { return (unsigned int) floorf(value + 0.5f); }

template<>
short CL_Vec2<short>::round_value(float value) const { return (short) floorf(value + 0.5f); }

template<>
unsigned short CL_Vec2<unsigned short>::round_value(float value) const { return (unsigned short) floorf(value + 0.5f); }

template<>
char CL_Vec2<char>::round_value(float value) const { return (char) floorf(value + 0.5f); }

template<>
unsigned char CL_Vec2<unsigned char>::round_value(float value) const { return (unsigned char) floorf(value + 0.5f); }

template<typename Type>
CL_Pointx<Type> CL_Vec2<Type>::calc_origin(CL_Origin origin, const CL_Sizex<Type> &size)
{
	switch(origin)
	{
	case origin_top_left:
	default:
		return CL_Pointx<Type>(0, 0);
		break;
	case origin_top_center:
		return CL_Pointx<Type>(size.width / 2, 0);
		break;
	case origin_top_right:
		return CL_Pointx<Type>(size.width, 0);
		break;
	case origin_center_left:
		return CL_Pointx<Type>(0, size.height / 2);
		break;
	case origin_center:
		return CL_Pointx<Type>(size.width / 2, size.height / 2);
		break;
	case origin_center_right:
		return CL_Pointx<Type>(size.width, size.height / 2);
		break;
	case origin_bottom_left:
		return CL_Pointx<Type>(0, size.height);
		break;
	case origin_bottom_center:
		return CL_Pointx<Type>(size.width / 2, size.height);
		break;
	case origin_bottom_right:
		return CL_Pointx<Type>(size.width, size.height);
		break;
	}
}

// Explicit instantiate the versions we use:
template class CL_Vec2<unsigned char>;
template class CL_Vec2<char>;
template class CL_Vec2<unsigned short>;
template class CL_Vec2<short>;
template class CL_Vec2<unsigned int>;
template class CL_Vec2<int>;
template class CL_Vec2<float>;
template class CL_Vec2<double>;

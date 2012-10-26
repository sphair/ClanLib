/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#pragma once

class PlaneEquation
{
public:
	PlaneEquation()
	: a(0.0), b(0.0), c(0.0), d(0.0)
	{
	}

	PlaneEquation(float a, float b, float c, float d)
	: a(a), b(b), c(c), d(d)
	{
	}

	float a, b, c, d;

	void normalize()
	{
		float length = sqrt(a*a+b*b+c*c);
		a /= length;
		b /= length;
		c /= length;
		d /= length;
	}

	float distance(const Vec3f &point) const
	{
		return a * point.x + b * point.y + c * point.z + d;
	}

	Vec3f get_normal() const
	{
		return Vec3f(a, b, c);
	}

	Vec3f get_origin() const
	{
		return Vec3f(-a*d, -b*d, -c*d);
	}

	static PlaneEquation left_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] + matrix[0+0*4],
			matrix[3+1*4] + matrix[0+1*4],
			matrix[3+2*4] + matrix[0+2*4],
			matrix[3+3*4] + matrix[0+3*4]);
	}

	static PlaneEquation right_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] - matrix[0+0*4],
			matrix[3+1*4] - matrix[0+1*4],
			matrix[3+2*4] - matrix[0+2*4],
			matrix[3+3*4] - matrix[0+3*4]);
	}

	static PlaneEquation top_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] - matrix[1+0*4],
			matrix[3+1*4] - matrix[1+1*4],
			matrix[3+2*4] - matrix[1+2*4],
			matrix[3+3*4] - matrix[1+3*4]);
	}

	static PlaneEquation bottom_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] + matrix[1+0*4],
			matrix[3+1*4] + matrix[1+1*4],
			matrix[3+2*4] + matrix[1+2*4],
			matrix[3+3*4] + matrix[1+3*4]);
	}

	static PlaneEquation near_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] + matrix[2+0*4],
			matrix[3+1*4] + matrix[2+1*4],
			matrix[3+2*4] + matrix[2+2*4],
			matrix[3+3*4] + matrix[2+3*4]);
	}

	static PlaneEquation far_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] - matrix[2+0*4],
			matrix[3+1*4] - matrix[2+1*4],
			matrix[3+2*4] - matrix[2+2*4],
			matrix[3+3*4] - matrix[2+3*4]);
	}
};

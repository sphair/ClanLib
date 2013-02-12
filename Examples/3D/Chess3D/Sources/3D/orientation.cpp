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
#include "precomp.h"
#include "orientation.h"

#ifdef WIN32
#define cl_sqrt sqrt
#else
#define cl_sqrt std::sqrt
#endif

/////////////////////////////////////////////////////////////////////////////
// Orientation Construction:

Orientation::Orientation()
{
}

Orientation Orientation::look_at(const Vec3f &dir, const Vec3f &up)
{
	return look_at(dir.x, dir.y, dir.z, up.x, up.y, up.z);
}

Orientation Orientation::look_at(float dir_x, float dir_y, float dir_z, float up_x, float up_y, float up_z)
{
	Vec3f f_from(dir_x, dir_y, dir_z); // z
	Vec3f up_from(up_x, up_y, up_z);
	f_from.normalize();
	up_from.normalize();
	Vec3f s_from = Vec3f::cross(f_from, up_from); // x
	Vec3f u_from = Vec3f::cross(s_from, f_from);  // y

	Vec3f f_to(0,0,1); // z
	Vec3f s_to(-1,0,0); // x
	Vec3f u_to(0,1,0); // y

	Vec3f cross_sum = Vec3f::cross(s_from, s_to) + Vec3f::cross(u_from, u_to) + Vec3f::cross(f_from, f_to);
	float dot_sum = Vec3f::dot(s_from, s_to) + Vec3f::dot(u_from, u_to) + Vec3f::dot(f_from, f_to);

	Orientation o;
	o.q.x = cross_sum.x;
	o.q.y = cross_sum.y;
	o.q.z = cross_sum.z;
	o.q.w = dot_sum + 1.0f;
	o.q.normalize();
	return o;
}

Orientation Orientation::look_at(const Vec3f &eye, const Vec3f &center, const Vec3f &up)
{
	return look_at(center - eye, up);
}

Orientation Orientation::look_at(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z)
{
	return look_at(center_x - eye_x, center_y - eye_y, center_z - eye_z, up_x, up_y, up_z);
}

Orientation Orientation::axis_angle(float angle, const Vec3f &axis)
{
	Orientation o;
	o.q = Quaternionf::axis_angle(Angle(angle, angle_degrees), axis);
	return o;
}

Orientation Orientation::euler_angles(float angle_x, float angle_y, float angle_z)
{
	Orientation o;
	o.q.set(angle_x, angle_y, angle_z, angle_degrees, order_XYZ);
	return o;
}

Orientation Orientation::quaternion(float x, float y, float z, float w)
{
	Orientation o;
	o.q.x = x;
	o.q.y = y;
	o.q.z = z;
	o.q.w = w;
	return o;
}

Orientation Orientation::quaternion(const Vec4f &quaternion)
{
	Orientation o;
	o.q.x = quaternion.x;
	o.q.y = quaternion.y;
	o.q.z = quaternion.z;
	o.q.w = quaternion.w;
	return o;
}

Orientation Orientation::multiply(const Orientation &o1, const Orientation &o2)
{
	Orientation o;
	o.q = Quaternionf::multiply(o1.q, o2.q);
	return o;
}

Orientation Orientation::interpolate(const Orientation &o1, const Orientation &o2, float t)
{
	Orientation o;
	o.q = Quaternionf::slerp(o1.q, o2.q, t);
	return o;
}

/////////////////////////////////////////////////////////////////////////////
// Orientation Attributes:

/////////////////////////////////////////////////////////////////////////////
// Orientation Operations:

Mat4f Orientation::to_matrix() const
{
	return q.to_matrix();
}

Orientation &Orientation::multiply(const Orientation &orientation)
{
	q.multiply(orientation.q);
	return *this;
}

Orientation &Orientation::rotate(float angle, const Vec3f &axis)
{
	q.rotate(Angle(angle, angle_degrees), axis);
	return *this;
}

Orientation &Orientation::rotate(float angle_x, float angle_y, float angle_z)
{
	q.rotate(Angle(angle_x, angle_degrees), Angle(angle_y, angle_degrees), Angle(angle_z, angle_degrees), order_XYZ);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Orientation Implementation:


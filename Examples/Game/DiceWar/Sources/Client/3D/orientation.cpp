
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
: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

Orientation Orientation::look_at(const CL_Vec3f &dir, const CL_Vec3f &up)
{
	return look_at(dir.x, dir.y, dir.z, up.x, up.y, up.z);
}

Orientation Orientation::look_at(float dir_x, float dir_y, float dir_z, float up_x, float up_y, float up_z)
{
	CL_Vec3f f_from(dir_x, dir_y, dir_z); // z
	CL_Vec3f up_from(up_x, up_y, up_z);
	f_from.normalize();
	up_from.normalize();
	CL_Vec3f s_from = CL_Vec3f::cross(f_from, up_from); // x
	CL_Vec3f u_from = CL_Vec3f::cross(s_from, f_from);  // y

	CL_Vec3f f_to(0,0,1); // z
	CL_Vec3f s_to(-1,0,0); // x
	CL_Vec3f u_to(0,1,0); // y

	CL_Vec3f cross_sum = CL_Vec3f::cross(s_from, s_to) + CL_Vec3f::cross(u_from, u_to) + CL_Vec3f::cross(f_from, f_to);
	float dot_sum = CL_Vec3f::dot(s_from, s_to) + CL_Vec3f::dot(u_from, u_to) + CL_Vec3f::dot(f_from, f_to);

	Orientation o;
	o.x = cross_sum.x;
	o.y = cross_sum.y;
	o.z = cross_sum.z;
	o.w = dot_sum + 1.0f;
	o.normalize();
	return o;
}

Orientation Orientation::look_at(const CL_Vec3f &eye, const CL_Vec3f &center, const CL_Vec3f &up)
{
	return look_at(center - eye, up);
}

Orientation Orientation::look_at(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z)
{
	return look_at(center_x - eye_x, center_y - eye_y, center_z - eye_z, up_x, up_y, up_z);
}

Orientation Orientation::axis_angle(float angle, const CL_Vec3f &axis)
{
	float length = axis.length();
	float half_angle_radians = angle * CL_PI / 360.0f;
	Orientation o;
	o.w = cos(half_angle_radians);
	o.x = axis.x * sin(half_angle_radians) / length;
	o.y = axis.y * sin(half_angle_radians) / length;
	o.z = axis.z * sin(half_angle_radians) / length;
	return o;
}

Orientation Orientation::euler_angles(float angle_x, float angle_y, float angle_z)
{
	float half_angle_x_radians = angle_x * CL_PI / 360.0f;
	float half_angle_y_radians = angle_y * CL_PI / 360.0f;
	float half_angle_z_radians = angle_z * CL_PI / 360.0f;

	Orientation rot_x;
	rot_x.x = cos(half_angle_x_radians);
	rot_x.y = sin(half_angle_x_radians);
	rot_x.z = 0;
	rot_x.w = 0;

	Orientation rot_y;
	rot_y.x = cos(half_angle_y_radians);
	rot_y.y = 0;
	rot_y.z = sin(half_angle_y_radians);
	rot_y.w = 0;

	Orientation rot_z;
	rot_z.x = cos(half_angle_z_radians);
	rot_z.y = 0;
	rot_z.z = 0;
	rot_z.w = sin(half_angle_z_radians);

	return multiply(multiply(rot_z, rot_y), rot_x);
}

Orientation Orientation::quaternion(float x, float y, float z, float w)
{
	Orientation o;
	o.x = x;
	o.y = y;
	o.z = z;
	o.w = w;
	return o;
}

Orientation Orientation::quaternion(const CL_Vec4f &quaternion)
{
	Orientation o;
	o.x = quaternion.x;
	o.y = quaternion.y;
	o.z = quaternion.z;
	o.w = quaternion.w;
	return o;
}

Orientation Orientation::multiply(const Orientation &o1, const Orientation &o2)
{
	Orientation o;
	o.x = o1.w*o2.x + o1.x*o2.w + o1.y*o2.z - o1.z*o2.y;
	o.y = o1.w*o2.y + o1.y*o2.w + o1.z*o2.x - o1.x*o2.z;
	o.z = o1.w*o2.z + o1.z*o2.w + o1.x*o2.y - o1.y*o2.x;
	o.w = o1.w*o2.w - o1.x*o2.x - o1.y*o2.y - o1.z*o2.z;
	return o;
}

Orientation Orientation::interpolate(const Orientation &o1, const Orientation &o2, float t)
{
	float cos_theta = 0.0f;
	float sin_theta = 0.0f;
	float beta = 0.0f;
	float q2[4];

	q2[0] = o2.x;
	q2[1] = o2.y;
	q2[2] = o2.z;
	q2[3] = o2.w;

	cos_theta = o1.x * o2.x + o1.y * o2.y + o1.z * o2.z + o1.w * o2.w;

	if (cos_theta < 0.0f)
	{
		q2[0] = -q2[0];
		q2[1] = -q2[1];
		q2[2] = -q2[2];
		q2[3] = -q2[3];
		cos_theta = -cos_theta;
	}

	beta = 1.0f - t;

	if (1.0f - cos_theta > 0.001f)
	{
		cos_theta = acos(cos_theta);
		sin_theta = 1.0f / sin(cos_theta);
		beta = sin(cos_theta * beta) * sin_theta;
		t = sin(cos_theta * t) * sin_theta;
	}

	Orientation o;
	o.x = beta * o1.x + t * q2[0];
	o.y = beta * o1.y + t * q2[1];
	o.z = beta * o1.z + t * q2[2];
	o.w = beta * o1.w + t * q2[3];
	return o;
}

/////////////////////////////////////////////////////////////////////////////
// Orientation Attributes:

/////////////////////////////////////////////////////////////////////////////
// Orientation Operations:

CL_Mat4f Orientation::to_matrix() const
{
	CL_Mat4f matrix = CL_Mat4f::null();
	matrix[0*4+0] = 1 - 2*y*y - 2*z*z;
	matrix[1*4+0] = 2*x*y - 2*w*z;
	matrix[2*4+0] = 2*x*z + 2*w*y;
	matrix[0*4+1] = 2*x*y + 2*w*z;
	matrix[1*4+1] = 1 - 2*x*x - 2*z*z;
	matrix[2*4+1] = 2*y*z - 2*w*x;
	matrix[0*4+2] = 2*x*z - 2*w*y;
	matrix[1*4+2] = 2*y*z + 2*w*x;
	matrix[2*4+2] = 1 - 2*x*x - 2*y*y;
	matrix[3*4+3] = 1;

	return matrix;
}

void Orientation::to_axis_angle(float &out_angle, float &out_vx, float &out_vy, float &out_vz) const
{
	float scale = sqrt(x*x + y*y + z*z);
	if (scale == 0.0f)
	{
		out_angle = acos(w) * 180.0f / CL_PI;
		out_vx = 0.0f;
		out_vy = 1.0f;
		out_vz = 0.0f;
		return;
	}
	out_angle = acos(w) * 180.0f / CL_PI;
	out_vx = x / scale;
	out_vy = y / scale;
	out_vz = z / scale;
}

Orientation &Orientation::multiply(const Orientation &orientation)
{
	*this = multiply(orientation, *this);
	return *this;
}

Orientation &Orientation::rotate(float angle, const CL_Vec3f &axis)
{
	return multiply(axis_angle(angle, axis));
}

Orientation &Orientation::rotate(float angle_x, float angle_y, float angle_z)
{
	return multiply(euler_angles(angle_x, angle_y, angle_z));
}

/////////////////////////////////////////////////////////////////////////////
// Orientation Implementation:

float Orientation::get_magnitude() const
{
	return sqrt(x*x+y*y+z*z+w*w);
}

Orientation &Orientation::normalize()
{
	float magnitude = get_magnitude();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
	w /= magnitude;
	return *this;
}


#pragma once

class Orientation
{
//! Construction:
public:
	Orientation();

	static Orientation look_at(const Vec3f &eye, const Vec3f &center, const Vec3f &up);
	static Orientation look_at(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z);
	static Orientation look_at(const Vec3f &dir, const Vec3f &up);
	static Orientation look_at(float dir_x, float dir_y, float dir_z, float up_x, float up_y, float up_z);
	static Orientation euler_angles(float angle_x, float angle_y, float angle_z);
	static Orientation axis_angle(float angle, const Vec3f &axis);
	static Orientation quaternion(float x, float y, float z, float w);
	static Orientation quaternion(const Vec4f &quaternion);
	static Orientation interpolate(const Orientation &orientation1, const Orientation &orientation2, float t);

//! Attributes:
public:
	float get_x() const { return x; }
	float get_y() const { return y; }
	float get_z() const { return z; }
	float get_w() const { return w; }
	Vec4f get_quaternion() const { return Vec4f(x, y, z, w); }

//! Operations:
public:
	Mat4f to_matrix() const;
	void to_axis_angle(float &out_angle, float &out_vx, float &out_vy, float &out_vz) const;

	static Orientation multiply(const Orientation &orientation1, const Orientation &orientation2);
	Orientation &multiply(const Orientation &orientation);

	Orientation &rotate(float angle, const Vec3f &axis);
	Orientation &rotate(float angle_x, float angle_y, float angle_z);

//! Implementation:
private:
	float get_magnitude() const;
	Orientation &normalize();

	float x, y, z, w;
};

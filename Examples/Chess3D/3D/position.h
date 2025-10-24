
#pragma once

class Orientation;

class Position
{
//! Construction:
public:
	Position();
	Position(float x, float y, float z);

//! Attributes:
public:
	void get_position(float &out_x, float &out_y, float &out_z);
	float get_x() const { return x; }
	float get_y() const { return y; }
	float get_z() const { return z; }

//! Operations:
public:
	CL_Mat4f to_matrix() const;
	void set_position(const float &x, const float &y, const float &z);
	void translate(const Orientation &orientation, float x, float y, float z);
	void set_x(float new_x) { x = new_x; }
	void set_y(float new_y) { y = new_y; }
	void set_z(float new_z) { z = new_z; }

//! Implementation:
private:
	float x, y, z;
};

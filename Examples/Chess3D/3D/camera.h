
#pragma once

#include "orientation.h"
#include "position.h"

class CL_GraphicContext;

class Camera
{
//! Construction:
public:
	Camera();

	Camera(
		float field_of_view,
		float xy_aspect,
		const Position &pos = Position(),
		const Orientation &orientation = Orientation());

//! Attributes:
public:
	float get_field_of_view() const { return field_of_view; }
	float get_xy_aspect() const { return xy_aspect; }
	const Position &get_position() const { return position; }
	Position &get_position() { return position; }
	const Orientation &get_orientation() const { return orientation; }
	Orientation &get_orientation() { return orientation; }
	CL_Mat4f get_projection(float z_near, float z_far) const;
	CL_Mat4f get_modelview() const;

//! Operations:
public:
	void set_field_of_view(float fov);
	void set_xy_aspect(float aspect);
	void set_position(const Position &position);
	void set_orientation(const Orientation &orientation);
	void setup_gc(CL_GraphicContext &gc, float z_near, float z_far);

//! Implementation:
private:
	float field_of_view;
	float xy_aspect;
	Position position;
	Orientation orientation;
};

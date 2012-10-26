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
#include "camera.h"

/////////////////////////////////////////////////////////////////////////////
// Camera Construction:

Camera::Camera()
{
}

Camera::Camera(
	float field_of_view,
	float xy_aspect,
	const Position &pos,
	const Orientation &orientation)
: field_of_view(field_of_view), xy_aspect(xy_aspect), position(pos),
  orientation(orientation)
{
}

/////////////////////////////////////////////////////////////////////////////
// Camera Attributes:

Mat4f Camera::get_projection(float z_near, float z_far) const
{
	return Mat4f::perspective(field_of_view, xy_aspect, z_near, z_far);
}

Mat4f Camera::get_modelview() const
{
	Mat4f matrix = orientation.to_matrix();

	float eye_x = position.get_x();
	float eye_y = position.get_y();
	float eye_z = position.get_z();

	float up_x = matrix[0*4+0]*0 + matrix[0*4+1]*1 + matrix[0*4+2]*0 + matrix[0*4+3];
	float up_y = matrix[1*4+0]*0 + matrix[1*4+1]*1 + matrix[1*4+2]*0 + matrix[1*4+3];
	float up_z = matrix[2*4+0]*0 + matrix[2*4+1]*1 + matrix[2*4+2]*0 + matrix[2*4+3];

	float dir_x = matrix[0*4+0]*0 + matrix[0*4+1]*0 + matrix[0*4+2]*1 + matrix[0*4+3];
	float dir_y = matrix[1*4+0]*0 + matrix[1*4+1]*0 + matrix[1*4+2]*1 + matrix[1*4+3];
	float dir_z = matrix[2*4+0]*0 + matrix[2*4+1]*0 + matrix[2*4+2]*1 + matrix[2*4+3];

	float center_x = eye_x+dir_x;
	float center_y = eye_y+dir_y;
	float center_z = eye_z+dir_z;

	return Mat4f::look_at(eye_x, eye_y, eye_z, center_x, center_y, center_z, up_x, up_y, up_z);
}

/////////////////////////////////////////////////////////////////////////////
// Camera Operations:

void Camera::set_field_of_view(float new_field_of_view)
{
	field_of_view = new_field_of_view;
}

void Camera::set_xy_aspect(float new_xy_aspect)
{
	xy_aspect = new_xy_aspect;
}

void Camera::set_position(const Position &new_position)
{
	position = new_position;
}

void Camera::set_orientation(const Orientation &new_orientation)
{
	orientation = new_orientation;
}

void Camera::setup_gc(GraphicContext &gc, float z_near, float z_far)
{
	gc.set_map_mode(cl_user_projection);
	gc.set_projection(get_projection(z_near, z_far));
	gc.set_modelview(get_modelview());
}

/////////////////////////////////////////////////////////////////////////////
// Camera Implementation:

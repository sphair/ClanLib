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

#include "orientation.h"
#include "position.h"

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
	Mat4f get_projection(float z_near, float z_far) const;
	Mat4f get_modelview() const;

//! Operations:
public:
	void set_field_of_view(float fov);
	void set_xy_aspect(float aspect);
	void set_position(const Position &position);
	void set_orientation(const Orientation &orientation);
	void setup_gc(GraphicContext &gc, float z_near, float z_far);

//! Implementation:
private:
	float field_of_view;
	float xy_aspect;
	Position position;
	Orientation orientation;
};

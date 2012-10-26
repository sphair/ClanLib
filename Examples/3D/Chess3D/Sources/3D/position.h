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
	Mat4f to_matrix() const;
	void set_position(const float &x, const float &y, const float &z);
	void translate(const Orientation &orientation, float x, float y, float z);
	void set_x(float new_x) { x = new_x; }
	void set_y(float new_y) { y = new_y; }
	void set_z(float new_z) { z = new_z; }

//! Implementation:
private:
	float x, y, z;
};

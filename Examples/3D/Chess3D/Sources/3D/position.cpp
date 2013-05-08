/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "position.h"
#include "orientation.h"

/////////////////////////////////////////////////////////////////////////////
// Position Construction:

Position::Position()
: x(0.0f), y(0.0f), z(0.0f)
{
}

Position::Position(float x, float y, float z)
: x(x), y(y), z(z)
{
}

/////////////////////////////////////////////////////////////////////////////
// Position Operations:

void Position::get_position(float &out_x, float &out_y, float &out_z)
{
	out_x = x;
	out_y = y;
	out_z = z;
}

/////////////////////////////////////////////////////////////////////////////
// Position Operations:

Mat4f Position::to_matrix() const
{
	return Mat4f::translate(x, y, z);
}

void Position::set_position(const float &new_x, const float &new_y, const float &new_z)
{
	x = new_x;
	y = new_y;
	z = new_z;
}

void Position::translate(const Orientation &orientation, float dx, float dy, float dz)
{
	Mat4f matrix = orientation.to_matrix();
	float tx = matrix[0*4+0]*dx + matrix[0*4+1]*dy + matrix[0*4+2]*dz + matrix[0*4+3];
	float ty = matrix[1*4+0]*dx + matrix[1*4+1]*dy + matrix[1*4+2]*dz + matrix[1*4+3];
	float tz = matrix[2*4+0]*dx + matrix[2*4+1]*dy + matrix[2*4+2]*dz + matrix[2*4+3];

	x += tx;
	y += ty;
	z += tz;
}

/////////////////////////////////////////////////////////////////////////////
// Position Implementation:

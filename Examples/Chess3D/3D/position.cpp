
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

CL_Mat4f Position::to_matrix() const
{
	return CL_Mat4f::translate(x, y, z);
}

void Position::set_position(const float &new_x, const float &new_y, const float &new_z)
{
	x = new_x;
	y = new_y;
	z = new_z;
}

void Position::translate(const Orientation &orientation, float dx, float dy, float dz)
{
	CL_Mat4f matrix = orientation.to_matrix();
	float tx = matrix[0*4+0]*dx + matrix[0*4+1]*dy + matrix[0*4+2]*dz + matrix[0*4+3];
	float ty = matrix[1*4+0]*dx + matrix[1*4+1]*dy + matrix[1*4+2]*dz + matrix[1*4+3];
	float tz = matrix[2*4+0]*dx + matrix[2*4+1]*dy + matrix[2*4+2]*dz + matrix[2*4+3];

	x += tx;
	y += ty;
	z += tz;
}

/////////////////////////////////////////////////////////////////////////////
// Position Implementation:

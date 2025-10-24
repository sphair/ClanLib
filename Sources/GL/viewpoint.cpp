/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/GL/viewpoint.h"
#include "API/GL/opengl_wrap.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CL_Viewpoint construction:

CL_Viewpoint::CL_Viewpoint() : pos(0,0,0), dir(0,0,1), up(0,1,0)
{
}

CL_Viewpoint::CL_Viewpoint(
	const CL_Vector &_pos,
	const CL_Vector &_dir,
	const CL_Vector &_up) : pos(_pos), dir(_dir), up(_up)
{
	dir.normalize();
	up.normalize();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Viewpoint attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Viewpoint operations:

void CL_Viewpoint::translate(float mov_x, float mov_y, float mov_z)
{
	pos.x += mov_x;
	pos.y += mov_y;
	pos.z += mov_z;
}

void CL_Viewpoint::set(float pos_x, float pos_y, float pos_z)
{
	pos.x = pos_x;
	pos.y = pos_y;
	pos.z = pos_z;
}

void CL_Viewpoint::translate_local(float mov_x, float mov_y, float mov_z)
{
	CL_Vector res = dir.cross(up);
	res.normalize();

	float delta_x = res.x*mov_x + up.x*mov_y + dir.x*mov_z;
	float delta_y = res.y*mov_x + up.y*mov_y + dir.y*mov_z;
	float delta_z = res.z*mov_x + up.z*mov_y + dir.z*mov_z;

	pos = pos+CL_Vector(delta_x, delta_y, delta_z, 0.0f);
}

void CL_Viewpoint::rotate_local(double angle, double x, double y, double z)
{
/*
	DESCRIPTION
	glRotate produces a rotation of angle degrees around the
	vector (x,y,z). The current matrix (see glMatrixMode) is
	multiplied by a rotation matrix with the product replacing
	the current matrix, as if glMultMatrix were called with the
	following matrix as its argument:

		( xx(1-c)+c    xy(1-c)-zs    xz(1-c)+ys   0  )
		|                                            |
		| yx(1-c)+zs   yy(1-c)+c     yz(1-c)-xs	  0  |
		| xz(1-c)-ys   yz(1-c)+xs    zz(1-c)+c	  0  |
		|                                            |
		(     0            0             0        1  )

	Where c = cos(angle), s = sine(angle), and ||( x,y,z )|| = 1
	(if not, the GL will normalize this vector).
*/
	CLdouble length = sqrt(x*x+y*y+z*z);
	x /= length;
	y /= length;
	z /= length;

	CLdouble c = cos(angle*3.14/180);
	CLdouble s = sin(angle*3.14/180);

	CLdouble matrix[4*4] =
	{
		x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0.0f,
		y*x*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s, 0.0f,
		x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// Rotate local dir vector:
	CLdouble matrix_dir[4*1] = { 0.0, 0.0, 1.0, 1.0 };
	CLdouble matrix_dir_result[4*1] =
	{
		matrix[0]*matrix_dir[0] + matrix[4]*matrix_dir[1] + matrix[8]*matrix_dir[2] + matrix[12]*matrix_dir[3],
		matrix[1]*matrix_dir[1] + matrix[5]*matrix_dir[1] + matrix[9]*matrix_dir[2] + matrix[13]*matrix_dir[3],
		matrix[2]*matrix_dir[1] + matrix[6]*matrix_dir[1] + matrix[10]*matrix_dir[2] + matrix[14]*matrix_dir[3],
		matrix[3]*matrix_dir[1] + matrix[7]*matrix_dir[1] + matrix[11]*matrix_dir[2] + matrix[15]*matrix_dir[3]
	};

	// Rotate local up vector:
	CLdouble matrix_up[4*1] = { 0.0, 1.0, 0.0, 1.0 };
	CLdouble matrix_up_result[4*1] =
	{
		matrix[0]*matrix_up[0] + matrix[4]*matrix_up[1] + matrix[8]*matrix_up[2] + matrix[12]*matrix_up[3],
		matrix[1]*matrix_up[1] + matrix[5]*matrix_up[1] + matrix[9]*matrix_up[2] + matrix[13]*matrix_up[3],
		matrix[2]*matrix_up[1] + matrix[6]*matrix_up[1] + matrix[10]*matrix_up[2] + matrix[14]*matrix_up[3],
		matrix[3]*matrix_up[1] + matrix[7]*matrix_up[1] + matrix[11]*matrix_up[2] + matrix[15]*matrix_up[3]
	};

	CL_Vector sideways = dir.cross(up);
	sideways.normalize();

	// Convert local vectors to world vectors:
	CL_Vector new_up(
		(float) (matrix_up_result[0]*sideways.x + matrix_up_result[1]*up.x + matrix_up_result[2]*dir.x),
		(float) (matrix_up_result[0]*sideways.y + matrix_up_result[1]*up.y + matrix_up_result[2]*dir.y),
		(float) (matrix_up_result[0]*sideways.z + matrix_up_result[1]*up.z + matrix_up_result[2]*dir.z));

	CL_Vector new_dir(
		(float) (matrix_dir_result[0]*sideways.x + matrix_dir_result[1]*up.x + matrix_dir_result[2]*dir.x),
		(float) (matrix_dir_result[0]*sideways.y + matrix_dir_result[1]*up.y + matrix_dir_result[2]*dir.y),
		(float) (matrix_dir_result[0]*sideways.z + matrix_dir_result[1]*up.z + matrix_dir_result[2]*dir.z));

	new_up.normalize();
	new_dir.normalize();

	up = new_up;
	dir = new_dir;
}

void CL_Viewpoint::enter_local() const
{
	// Move local model coordinates into place in the world:
	clMatrixMode(CL_MODELVIEW);
	clPushMatrix();

	CL_Vector f = -dir;
	CL_Vector up2 = -up;
	f.normalize();
	up2.normalize();
	CL_Vector s = f.cross(up2);
	CL_Vector u = s.cross(f);

	// Inverted gluLookAt coordinate switch matrix:
	CLfloat matrix[4*4] =
	{
		 s.x,  s.y,  s.z,  0.0f,
		 u.x,  u.y,  u.z,  0.0f,
		-f.x, -f.y, -f.z,  0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f
	};

	clTranslatef(pos.x, pos.y, pos.z);
	clMultMatrixf(matrix);
}

void CL_Viewpoint::leave_local() const
{
	// Restore model view matrix:
	clMatrixMode(CL_MODELVIEW);
	clPopMatrix();
/*
	// Render direction arrow for debugging:
	clDisable(CL_DEPTH_TEST);
	clDisable(CL_TEXTURE_2D);
	clBegin(CL_LINE_STRIP);
	clColor3f(1.0f,1.0f,1.0f);
	clVertex3f(pos.x+up.x, pos.y+up.y, pos.z+up.z);
	clVertex3f(pos.x+dir.x, pos.y+dir.y, pos.z+dir.z);
	clVertex3f(pos.x-up.x, pos.y-up.y, pos.z-up.z);
	clEnd();
	clEnable(CL_TEXTURE_2D);
	clEnable(CL_DEPTH_TEST);
*/
}

CL_Vector CL_Viewpoint::make_local_direction(const CL_Vector &v) const
{
	CLfloat matrix_v[4] = { v.x, v.y, v.z, v.w };

	CL_Vector f = -dir;
	CL_Vector up2 = -up;
	f.normalize();
	up2.normalize();
	CL_Vector s = f.cross(up2);
	CL_Vector u = s.cross(f);

	// gluLookAt coordinate switch matrix:
	CLfloat matrix[4*4] =
	{
		s.x, u.x, -f.x, 0.0f,
		s.y, u.y, -f.y, 0.0f,
		s.z, u.z, -f.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	CLfloat matrix_lv[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int r=0; r<4; r++)
	{
		for (int c=0; c<4; c++)
		{
			matrix_lv[r] += matrix[c*4+r]*matrix_v[c];
		}
	}

	return CL_Vector(matrix_lv[0], matrix_lv[1], matrix_lv[2], matrix_lv[3]);
}

void CL_Viewpoint::setup_modelview() const
{
	clMatrixMode(CL_MODELVIEW);
	clLoadIdentity();			
	gluLookAt(
		pos.x,          // position
		pos.y,
		pos.z,
		pos.x+dir.x,    // center
		pos.y+dir.y,
		pos.z+dir.z,
		up.x,           // up
		up.y,
		up.z);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Viewpoint implementation

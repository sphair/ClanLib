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

//! clanGL="Display 3D"
//! header=gl.h

#ifndef header_viewpoint
#define header_viewpoint

#ifdef CL_API_DLL
#ifdef CL_GL_EXPORT
#define CL_API_GL __declspec(dllexport)
#else
#define CL_API_GL __declspec(dllimport)
#endif
#else
#define CL_API_GL
#endif

#include "../Core/Math/cl_vector.h"

//: CL_Viewpoint object.
//- !group=GL/Display 3D!
//- !header=gl.h!
//- <p>A viewpoint represents a position and viewing direction in 3D.</p>
class CL_API_GL CL_Viewpoint
{
//! Construction:
public:
	//: Constructs a viewpoint.
	//param pos: Position of viewpoint.
	//param dir: Viewing direction of viewpoint.
	//param up: Up direction of viewpoint.
	CL_Viewpoint();

	CL_Viewpoint(
		const CL_Vector &pos,
		const CL_Vector &dir,
		const CL_Vector &up);

//! Attributes:
public:
	//: Position of viewpoint.
	CL_Vector pos;
	
	//: Viewing direction of viewpoint.
	CL_Vector dir;

	//: Up direction of viewpoint.
	CL_Vector up;

//! Operations:
public:
	//: Translates viewpoint position.
	void translate(float mov_x, float mov_y, float mov_z);

	//: Sets the viewpoint position.
	void set(float pos_x, float pos_y, float pos_z);

	//: Moves the viewpoint from its local point of view.
	//- <p>Moving in local coordinates means that it is moved according to its
	//- current viewing direction, where the z axis is in the facing direction,
	//- and the y axis is in the up direction.</p>
	void translate_local(float mov_x, float mov_y, float mov_z);

	//: Rotates the viewpoint from its local point view.
	//- <p>Rotating in local coordinates means that it is rotated according to its
	//- current viewing direction, where the z axis is in the facing direction,
	//- and the y axis is in the up direction.</p>
	void rotate_local(double angle, double rot_x, double rot_y, double rot_z);

	//: Setup OpenGL model view matrix to use local viewpoint coordinates.
	//- <p>Calling this function causes CL_Viewpoint to do the following:</p>
	//- <ul>
	//- <li>glMatrixMode(GL_MODELVIEW)</li>
	//- <li>glPushMatrix()</li>
	//- <li>glMultMatrix(world2local_transform)</li>
	//- </ul>
	//- <p>world2local_transform is a coordinate switch matrix, that convert local
	//- coordinates for a model to be positioned in the world at the viewpoint's
	//- position and viewing direction.</p>
	//- <p>It is required that leave_local() is to be called, when the local
	//- coordinate space model view is no longer needed.</p>
	void enter_local() const;

	//: Restores the OpenGL model view matrix prior to a call to enter_local().
	void leave_local() const;

	//: Converts a direction vector from world into local coordinates.
	CL_Vector make_local_direction(const CL_Vector &v) const;

	//: Sets up the model view matrix to look from this viewpoint.
	//- <p>Calling this function causes CL_Viewpoint to do the following:</p>
	//- <ul>
	//- <li>glMatrixMode(GL_MODELVIEW)</li>
	//- <li>glLoadIdentity()</li>
	//- <li>gluLookAt(pos, pos+dir, up)</li>
	//- </ul>
	void setup_modelview() const;
};

#endif

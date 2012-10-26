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
*/

#include "GL/precomp.h"
#include "API/Core/System/mutex.h"
#include "API/GL/setup_gl.h"
#include "API/GL/opengl_target.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SetupGL Construction:

static Mutex cl_opengl_mutex;

static int cl_opengl_refcount = 0;

static OpenGLTarget *cl_opengl_target = 0;

SetupGL::SetupGL()
{
	MutexSection mutex_lock(&cl_opengl_mutex);
	if (cl_opengl_refcount == 0)
		cl_opengl_target = new OpenGLTarget();
	cl_opengl_refcount++;
}

SetupGL::~SetupGL()
{
	MutexSection mutex_lock(&cl_opengl_mutex);
	cl_opengl_refcount--;
	if (cl_opengl_refcount == 0)
		delete cl_opengl_target;
}

void SetupGL::set_current()
{
	MutexSection mutex_lock(&cl_opengl_mutex);
	cl_opengl_target->set_current();
}

}

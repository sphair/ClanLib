/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "D3D9/precomp.h"
#include "API/D3D9/setup_d3d9.h"
#include "API/D3D9/d3d9_target.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SetupD3D9 Construction:

static CL_Mutex cl_d3d9_mutex;

static int cl_d3d9_refcount = 0;

static CL_D3D9Target *cl_d3d9_target = 0;

CL_SetupD3D9::CL_SetupD3D9()
{
	CL_MutexSection mutex_lock(&cl_d3d9_mutex);
	if (cl_d3d9_refcount == 0)
		cl_d3d9_target = new CL_D3D9Target();
	cl_d3d9_refcount++;
}

CL_SetupD3D9::~CL_SetupD3D9()
{
	CL_MutexSection mutex_lock(&cl_d3d9_mutex);
	cl_d3d9_refcount--;
	if (cl_d3d9_refcount == 0)
		delete cl_d3d9_target;
}

/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "SDL/precomp.h"
#include "API/SDL/setup_sdl.h"
#include "API/SDL/sdl_target.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SetupSDL Construction:

static CL_Mutex cl_sdl_mutex;

static int cl_sdl_refcount = 0;

static CL_SDLTarget *cl_sdl_target = 0;

CL_SetupSDL::CL_SetupSDL()
{
	CL_MutexSection mutex_lock(&cl_sdl_mutex);
	if (cl_sdl_refcount == 0)
		cl_sdl_target = new CL_SDLTarget();

	cl_sdl_refcount++;
}

CL_SetupSDL::~CL_SetupSDL()
{
	CL_MutexSection mutex_lock(&cl_sdl_mutex);
	cl_sdl_refcount--;
	if (cl_sdl_refcount == 0)
		delete cl_sdl_target;
}


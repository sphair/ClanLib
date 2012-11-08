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

#include "Compute/precomp.h"
#include "API/Core/System/mutex.h"
#include "API/Compute/setup_compute.h"
#include "opencl_bindings.h"

namespace clan
{

static Mutex cl_compute_mutex;
static int cl_compute_refcount = 0;

SetupCompute::SetupCompute()
{
	MutexSection mutex_lock(&cl_compute_mutex);
	if (cl_compute_refcount == 0)
	{
		OpenCLBindings::bind();
	}
	cl_compute_refcount++;
}

SetupCompute::~SetupCompute()
{
	MutexSection mutex_lock(&cl_compute_mutex);
	cl_compute_refcount--;
	if (cl_compute_refcount == 0)
	{
		// OpenCLBindings::unbind();
	}
}

}

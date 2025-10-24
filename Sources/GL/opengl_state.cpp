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
#include "API/GL/opengl_state.h"
#include "opengl_state_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLState construction:

CL_OpenGLState::CL_OpenGLState()
{
}

CL_OpenGLState::CL_OpenGLState(CL_GraphicContext *gc)
: impl(new CL_OpenGLState_Generic(gc))
{
}

CL_OpenGLState::CL_OpenGLState(CL_GraphicContext_OpenGL *gc_opengl)
: impl(new CL_OpenGLState_Generic(gc_opengl))
{
}

CL_OpenGLState::CL_OpenGLState(const CL_OpenGLState &copy) : impl(copy.impl)
{
}

CL_OpenGLState::~CL_OpenGLState()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLState attributes:

bool CL_OpenGLState::is_active() const
{
	return impl->is_active();
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLState operations:

void CL_OpenGLState::set_active()
{
	if (impl) impl->set_active();
}

void CL_OpenGLState::attach_data(CL_OpenGLStateData *data, bool delete_data)
{
	if (impl) impl->data_objects.push_back(std::pair<CL_OpenGLStateData *, bool>(data, delete_data));
}

void CL_OpenGLState::detach_data(CL_OpenGLStateData *data)
{
	if (impl == 0) return;

	std::list< std::pair<CL_OpenGLStateData *, bool> >::iterator it;
	for (it = impl->data_objects.begin(); it != impl->data_objects.end(); ++it)
	{
		if ((*it).first == data)
		{
			impl->data_objects.erase(it);
			return;
		}
	}
}

void CL_OpenGLState::setup_2d(bool upside_down)
{
	impl->setup_2d(upside_down);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLState implementation:

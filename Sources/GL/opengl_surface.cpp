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
#include "API/GL/opengl_surface.h"
#include "API/GL/opengl_wrap.h"
#include "surface_target_opengl.h"

#define gl_target ((CL_Surface_Target_OpenGL *) impl->target)

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLSurface construction:

CL_OpenGLSurface::CL_OpenGLSurface() : CL_Surface()
{
}
	
CL_OpenGLSurface::CL_OpenGLSurface(const CL_Surface &copy) : CL_Surface(copy)
{
}
	
CL_OpenGLSurface::CL_OpenGLSurface(
	CL_PixelBuffer provider,
	int flag) :
	CL_Surface(provider, flag)
{
}

CL_OpenGLSurface::CL_OpenGLSurface(
	const std::string &resource_id,
	CL_ResourceManager *manager) : CL_Surface(resource_id, manager)
{
}

CL_OpenGLSurface::CL_OpenGLSurface(
	const std::string &filename,
	int flag) : CL_Surface(filename, flag)
{
}

CL_OpenGLSurface::~CL_OpenGLSurface()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLSurface attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLSurface operations:

void CL_OpenGLSurface::bind()
{
	clBindTexture(CL_TEXTURE_2D, gl_target->handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLSurface implementation:

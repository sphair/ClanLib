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
**    Harry Storbacka
*/

#include "GL/precomp.h"
#include "API/Display/TargetProviders/render_buffer_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "opengl_render_buffer_provider.h"


CL_OpenGLRenderBufferProvider::CL_OpenGLRenderBufferProvider(CL_OpenGLGraphicContextProvider *gc_provider)
: gc_provider(gc_provider), handle(0)
{

}

CL_OpenGLRenderBufferProvider::~CL_OpenGLRenderBufferProvider()
{
	if (handle)
	{
		CL_OpenGL::set_active(gc_provider);
		clDeleteRenderbuffers(1, &handle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLRenderBufferProvider Attributes:

CLuint CL_OpenGLRenderBufferProvider::get_handle()
{
	return handle;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLRenderBufferProvider Operations:

void CL_OpenGLRenderBufferProvider::create(int width, int height, int format)
{
	CL_OpenGL::set_active(gc_provider);
	CLuint last_render_buffer = 0;
	clGetIntegerv(CL_RENDERBUFFER_BINDING, (CLint *) &last_render_buffer);

	clGenRenderbuffers(1, &handle);
	clBindRenderbuffer(CL_RENDERBUFFER, handle);
	clRenderbufferStorage(CL_RENDERBUFFER, format, width, height);

	clBindRenderbuffer(CL_RENDERBUFFER, last_render_buffer);
}

void CL_OpenGLRenderBufferProvider::destroy()
{
	delete this;
}


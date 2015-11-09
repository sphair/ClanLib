/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
*/

#include "GL/precomp.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "pbuffer_impl.h"
#include "../../GL1/gl1_graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "opengl_window_provider_wgl.h"

namespace clan
{
	PBuffer_GL1_Impl::PBuffer_GL1_Impl(GL1GraphicContextProvider *gc_provider) : gc_provider(gc_provider), pbuffer(0), pbuffer_context(0), pbuffer_dc(0)
	{
		if (!gc_provider)
			throw Exception("Unexpected provider");
	}

	PBuffer_GL1_Impl::~PBuffer_GL1_Impl()
	{
		reset();
	}

	void PBuffer_GL1_Impl::reset()
	{
		OpenGL::set_active(gc_provider);

		if (pbuffer_context) wglDeleteContext(pbuffer_context);
		if (pbuffer_dc) glWglReleasePbufferDCARB(pbuffer, pbuffer_dc);
		if (pbuffer) glWglDestroyPbufferARB(pbuffer);

		// Note: glWglReleasePbufferDCARB does not delete the GDI object associated with the pbuffer DC
		// DeleteDC must be called to prevent GDI object leaks
		if (pbuffer_dc) DeleteDC(pbuffer_dc);

		pbuffer_context = 0;
		pbuffer_dc = 0;
		pbuffer = 0;
		OpenGL::remove_active(this);
	}

	void PBuffer_GL1_Impl::create(OpenGLWindowProvider &window_provider, const Size &size)
	{
		reset();

		OpenGL::set_active(gc_provider);

		if (glWglCreatePbufferARB == 0)
		{
			throw Exception("WGL_ARB_pbuffer OpenGL extension not supported by this card");
		}

		int attribList[1] = { 0 };

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd 
			1,                              // version number
			// PFD_DRAW_TO_WINDOW |            // support window
			PFD_SUPPORT_OPENGL //|            // support OpenGL
			//PFD_DOUBLEBUFFER |              // double buffered
			//PFD_DEPTH_DONTCARE
			,             // do you care about a zbuffer?
			PFD_TYPE_RGBA,                  // RGBA type
			24,                             // 24-bit color depth
			0, 0, 0, 0, 0, 0,               // color bits ignored
			8,                              // alpha buffer
			0,                              // shift bit ignored
			0,                              // no accumulation buffer
			0, 0, 0, 0,                     // accum bits ignored
			0,                              // z-buffer
			0,                              // no stencil buffer
			0,                              // no auxiliary buffer
			PFD_MAIN_PLANE,                 // main layer
			0,                              // reserved
			0, 0, 0                         // layer masks ignored
		};

		int pixelformat = ChoosePixelFormat(wglGetCurrentDC(), &pfd);

		pbuffer = glWglCreatePbufferARB(
			wglGetCurrentDC(),
			pixelformat,
			size.width,
			size.height,
			attribList);
		pbuffer_dc = glWglGetPbufferDCARB(pbuffer);
		pbuffer_context = wglCreateContext(pbuffer_dc);

		HGLRC share_context = window_provider.get_share_context();
		if (share_context == 0)
			throw Exception("Shared OpenGL Context is not valid");

		wglShareLists(share_context, pbuffer_context);

		pbuffer_size = size;
	}

	void PBuffer_GL1_Impl::make_current() const
	{
		wglMakeCurrent(pbuffer_dc, pbuffer_context);
	}

	ProcAddress *PBuffer_GL1_Impl::get_proc_address(const std::string& function_name) const
	{
		return (void(*)())wglGetProcAddress(function_name.c_str());
	}

	void PBuffer_GL1_Impl::get_opengl_version(int &version_major, int &version_minor) const
	{
		gc_provider->get_opengl_version(version_major, version_minor);
	}

	void PBuffer_GL1_Impl::get_opengl_version(int &version_major, int &version_minor, int &version_release) const
	{
		gc_provider->get_opengl_version(version_major, version_minor, version_release);
	}
}

/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "../opengl1_wrap.h"
#include "../opengl1.h"
#include "pbuffer_impl.h"
#include "../gl1_graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_window_provider_wgl.h"
#include "API/Display/Window/input_context.h"

namespace clan
{

PBuffer_GL1_Impl::PBuffer_GL1_Impl(GL1GraphicContextProvider *gc_provider) : gc_provider(gc_provider)
, pbuffer(0), pbuffer_context(0), pbuffer_dc(0), pbuffer_gc_provider(0)
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
	if (pbuffer_gc_provider)
	{
		pbuffer_gc = GraphicContext();
		pbuffer_gc_provider = NULL;
	}
	GL1::set_active(gc_provider);

	if (pbuffer_context) wglDeleteContext(pbuffer_context);
	if (pbuffer_dc) cl1WglReleasePbufferDCARB(pbuffer, pbuffer_dc);
	if (pbuffer) cl1WglDestroyPbufferARB(pbuffer);

	// Note: cl1WglReleasePbufferDCARB does not delete the GDI object associated with the pbuffer DC
	// DeleteDC must be called to prevent GDI object leaks
	if (pbuffer_dc) DeleteDC(pbuffer_dc);

	pbuffer_context = 0;
	pbuffer_dc = 0;
	pbuffer = 0;
}

void PBuffer_GL1_Impl::create(GL1WindowProvider &window_provider, const Size &size)
{
	reset();

	GL1::set_active(gc_provider);

	if (cl1WglCreatePbufferARB == 0)
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

	pbuffer = cl1WglCreatePbufferARB(
		wglGetCurrentDC(),
		pixelformat,
		size.width,
		size.height,
		attribList);
	pbuffer_dc = cl1WglGetPbufferDCARB(pbuffer);
	pbuffer_context = wglCreateContext(pbuffer_dc);

	HGLRC share_context = window_provider.get_share_context();
	if (share_context == 0)
		throw Exception("Shared OpenGL Context is not valid");

	wglShareLists(share_context, pbuffer_context);

//FIXME	pbuffer_gc_provider = new GL1GraphicContextProvider(this);
//FIXME	pbuffer_gc = GraphicContext(pbuffer_gc_provider);
	pbuffer_size = size;
}

void PBuffer_GL1_Impl::set_active() const
{
	GL1::set_active(pbuffer_gc_provider);
}

InputContext PBuffer_GL1_Impl::get_ic()
{
	throw Exception("FrameBuffer does not contain InputContext");
}

PixelBuffer PBuffer_GL1_Impl::get_clipboard_image() const
{
	return PixelBuffer();
}

void PBuffer_GL1_Impl::make_current() const
{
	wglMakeCurrent(pbuffer_dc, pbuffer_context);
}

HWND PBuffer_GL1_Impl::get_hwnd() const
{
	return gc_provider->get_render_window().get_hwnd();
}

}

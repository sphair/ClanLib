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
**    Mark Page
*/

#include "GL1/precomp.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/GL1/opengl1.h"
#include "API/Display/TargetProviders/render_window_provider.h"
#include "pbuffer_impl.h"
#include "../gl1_graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_window_provider_wgl.h"

class CL_RenderWindowProvider_WGL_PBuffer : public CL_RenderWindowProvider
{
public:
	CL_RenderWindowProvider_WGL_PBuffer(HDC pbuffer_dc, HGLRC wgl_context, CL_Size pb_size)
		: pbuffer_dc(pbuffer_dc), wgl_context(wgl_context), size(pb_size)
	{
	}
	virtual ~CL_RenderWindowProvider_WGL_PBuffer()
	{
	}
	virtual int get_viewport_width() const
	{
		return size.width;
	}
	virtual int get_viewport_height() const
	{
		return size.height;
	}
	virtual void flip_buffers(int interval) const
	{
	}
	virtual void make_current() const
	{
		wglMakeCurrent(pbuffer_dc, wgl_context);
	}
	virtual const CL_RenderWindowProvider * new_worker_context() const
	{
		throw CL_Exception("Pixel buffer worker threads are not implemented");
	}

private:
	HDC pbuffer_dc;
	HGLRC wgl_context;
	CL_Size size;
};


CL_PBuffer_GL1_Impl::CL_PBuffer_GL1_Impl(CL_GL1GraphicContextProvider *gc_provider) : gc_provider(gc_provider)
, pbuffer(0), pbuffer_context(0), pbuffer_dc(0), pbuffer_gc_provider(0)
{
	CL_SharedGCData::add_disposable(this);

	if (!gc_provider)
		throw CL_Exception("Unexpected provider");

}

CL_PBuffer_GL1_Impl::~CL_PBuffer_GL1_Impl()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_PBuffer_GL1_Impl::on_dispose()
{
	std::vector<CL_GraphicContextProvider*> &opengl_contexts = CL_SharedGCData::get_gc_providers();
	if (!opengl_contexts.empty())
	{
		CL_GL1GraphicContextProvider *gc_provider = dynamic_cast<CL_GL1GraphicContextProvider*>(opengl_contexts[0]);
		if (gc_provider)
		{
			reset();
		}
	}
}

void CL_PBuffer_GL1_Impl::reset()
{
	if (pbuffer_gc_provider)
	{
		pbuffer_gc = CL_GraphicContext();
		pbuffer_gc_provider = NULL;
	}
	CL_GL1::set_active(gc_provider);

	if (pbuffer_context) wglDeleteContext(pbuffer_context);
	if (pbuffer_dc) cl1WglReleasePbufferDCARB(pbuffer, pbuffer_dc);
	if (pbuffer) cl1WglDestroyPbufferARB(pbuffer);

	pbuffer_context = 0;
	pbuffer_dc = 0;
	pbuffer = 0;
}

void CL_PBuffer_GL1_Impl::create(CL_GL1WindowProvider_WGL &window_provider, CL_Size &size)
{
	reset();

	CL_GL1::set_active(gc_provider);

	if (cl1WglCreatePbufferARB == 0)
	{
		throw CL_Exception("WGL_ARB_pbuffer OpenGL extension not supported by this card");
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
		throw CL_Exception("Shared OpenGL Context is not valid");

	wglShareLists(share_context, pbuffer_context);

	CL_RenderWindowProvider_WGL_PBuffer *render_window = new CL_RenderWindowProvider_WGL_PBuffer(pbuffer_dc, pbuffer_context, size);
	pbuffer_gc_provider = new CL_GL1GraphicContextProvider(render_window);
	pbuffer_gc = CL_GraphicContext(pbuffer_gc_provider);
}

void CL_PBuffer_GL1_Impl::set_active() const
{
	CL_GL1::set_active(pbuffer_gc_provider);
}

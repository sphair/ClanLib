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
#include "pbuffer_impl.h"
#include "../gl1_graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_window_provider_glx.h"

CL_RenderWindowProvider_GLX_PBuffer::CL_RenderWindowProvider_GLX_PBuffer(CL_PBuffer_GL1_Impl &pbuffer_impl, Display *display, GLXPbuffer pbuffer, GLXContext glx_context, CL_Size pb_size)
: pbuffer_impl(pbuffer_impl), display(display), pbuffer(pbuffer), glx_context(glx_context), size(pb_size)
{
}

CL_RenderWindowProvider_GLX_PBuffer::~CL_RenderWindowProvider_GLX_PBuffer()
{
}

int CL_RenderWindowProvider_GLX_PBuffer::get_viewport_width() const
{
	return size.width;
}

int CL_RenderWindowProvider_GLX_PBuffer::get_viewport_height() const
{
	return size.height;
}

void CL_RenderWindowProvider_GLX_PBuffer::flip_buffers(int interval) const
{
}

void CL_RenderWindowProvider_GLX_PBuffer::make_current() const
{
	pbuffer_impl.glx->glXMakeCurrent(display, pbuffer, glx_context);
}

CL_GL1ProcAddress *CL_RenderWindowProvider_GLX_PBuffer::get_proc_address(const CL_String8& function_name) const
{
	if (pbuffer_impl.glx->glXGetProcAddressARB)
		return pbuffer_impl.glx->glXGetProcAddressARB((GLubyte*)function_name.c_str());
	if (pbuffer_impl.glx->glXGetProcAddress)
		return pbuffer_impl.glx->glXGetProcAddress((GLubyte*)function_name.c_str());
	return NULL;
}

const CL_RenderWindowProvider * CL_RenderWindowProvider_GLX_PBuffer::new_worker_context() const
{
	throw CL_Exception("Pixel buffer worker threads are not implemented");
}

CL_PBuffer_GL1_Impl::CL_PBuffer_GL1_Impl(CL_GL1GraphicContextProvider *gc_provider) : gc_provider(gc_provider)
, pbuffer(0), pbuffer_context(0), disp(0), pbuffer_gc_provider(0), glx(NULL)
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

	if (glx)
	{
		if (pbuffer_context) glx->glXDestroyContext(disp, pbuffer_context);
		if (pbuffer) glx->glXDestroyPbuffer(disp, pbuffer);
	}

	pbuffer_context = 0;
	pbuffer = 0;
	glx = NULL;
}

void CL_PBuffer_GL1_Impl::create(CL_GL1WindowProvider_GLX &window_provider, CL_Size &size)
{
	reset();

	glx = &window_provider.glx;	// FIXME: What a hack!

	CL_GL1::set_active(gc_provider);

	disp = window_provider.get_display();
	if (disp == NULL)
	{
		throw CL_Exception("Cannot obtain GL1 display");
	}

	int scrnum;
	GLXFBConfig *fbconfig;
	XVisualInfo *visinfo;
	int nitems;

	int attrib[] = {
		GLX_DOUBLEBUFFER,  False,
		GLX_RED_SIZE,      4,
		GLX_GREEN_SIZE,    4,
		GLX_BLUE_SIZE,     4,
		GLX_ALPHA_SIZE,    4,
		GLX_DEPTH_SIZE,    0,
		GLX_RENDER_TYPE,   GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT | GLX_WINDOW_BIT,
		None
		};

	int pbufAttrib[] = {
		GLX_PBUFFER_WIDTH,   size.width,
		GLX_PBUFFER_HEIGHT,  size.height,
		GLX_LARGEST_PBUFFER, False,
		None
		};

	scrnum = DefaultScreen(disp );

	if ((glx->glXChooseFBConfig == NULL) ||  (glx->glXCreatePbuffer == NULL) || (glx->glXGetVisualFromFBConfig == NULL) ||  (glx->glXCreateContext == NULL))
	{
		throw CL_Exception("pbuffer support is not available (require glx 1.3)");
	}

	fbconfig = glx->glXChooseFBConfig(disp, scrnum, attrib, &nitems);

	if (fbconfig == NULL)
	{
		throw CL_Exception("Error: couldn't get fbconfig");
	}

	pbuffer = glx->glXCreatePbuffer(disp, fbconfig[0], pbufAttrib);
	if (!pbuffer)
	{
		throw CL_Exception("Error: couldn't create pbuffer");
	}

	visinfo = glx->glXGetVisualFromFBConfig(disp, fbconfig[0]);
	if (!visinfo)
	{
		throw CL_Exception("Error: couldn't get an RGBA, double-buffered visual");
	}

	GLXContext shared_context = window_provider.get_share_context();

	if (!shared_context)
	{
		throw CL_Exception("Error: cannot obtain the shared context");
	}

	pbuffer_context = glx->glXCreateContext( disp, visinfo, 
		shared_context, GL_TRUE );

	if (!pbuffer_context)
	{
		throw CL_Exception("Error: glXCreateContext failed");
	}

	XFree(fbconfig);
	XFree(visinfo);

	CL_RenderWindowProvider_GLX_PBuffer *render_window = new CL_RenderWindowProvider_GLX_PBuffer(*this, disp, pbuffer, pbuffer_context, size);
	pbuffer_gc_provider = new CL_GL1GraphicContextProvider(render_window);
	pbuffer_gc = CL_GraphicContext(pbuffer_gc_provider);
}

void CL_PBuffer_GL1_Impl::set_active() const
{
	CL_GL1::set_active(pbuffer_gc_provider);
}


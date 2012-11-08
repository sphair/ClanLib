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
**    Mark Page
*/

#include "GL1/precomp.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/GL1/opengl1.h"
#include "pbuffer_impl.h"
#include "../gl1_graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_window_provider_glx.h"

namespace clan
{

PBuffer_GL1_Impl::PBuffer_GL1_Impl(GL1GraphicContextProvider *gc_provider) : gc_provider(gc_provider)
, pbuffer(0), pbuffer_context(0), disp(0), pbuffer_gc_provider(0), glx(NULL)
{
	gc_provider->add_disposable(this);

	if (!gc_provider)
		throw Exception("Unexpected provider");

}

PBuffer_GL1_Impl::~PBuffer_GL1_Impl()
{
	dispose();
}

void PBuffer_GL1_Impl::on_dispose()
{
	reset();
	gc_provider->remove_disposable(this);
}

GL1ProcAddress *PBuffer_GL1_Impl::get_proc_address(const std::string& function_name) const
{
	if (glx->glXGetProcAddressARB)
		return glx->glXGetProcAddressARB((GLubyte*)function_name.c_str());
	if (glx->glXGetProcAddress)
		return glx->glXGetProcAddress((GLubyte*)function_name.c_str());
	return NULL;
}

void PBuffer_GL1_Impl::make_current() const
{
	glx->glXMakeCurrent(disp, pbuffer, pbuffer_context);
}

void PBuffer_GL1_Impl::reset()
{
	if (pbuffer_gc_provider)
	{
		pbuffer_gc = GraphicContext();
		pbuffer_gc_provider = NULL;
	}
	GL1::set_active(gc_provider);

	if (glx)
	{
		if (pbuffer_context) glx->glXDestroyContext(disp, pbuffer_context);
		if (pbuffer)
		{
			if (glx->glx_1_3)
			{
				glx->glXDestroyPbuffer(disp, pbuffer);
			}
			else
			{
				glx->glXDestroyPbufferSGIX(disp, pbuffer);
			}
		}
	}

	pbuffer_context = 0;
	pbuffer = 0;
	glx = NULL;
}

static bool cl1_ctxErrorOccurred = false;
static int cl1_ctxErrorHandler( ::Display *dpy, XErrorEvent *ev )
{
    cl1_ctxErrorOccurred = true;
    return 0;
}

void PBuffer_GL1_Impl::create(GL1WindowProvider_GLX &window_provider, Size &size)
{
	reset();

	glx = &window_provider.glx;	// FIXME: What a hack!

	GL1::set_active(gc_provider);

	disp = window_provider.get_display();
	if (disp == NULL)
	{
		throw Exception("Cannot obtain GL1 display");
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

	if ((glx->glXCreateContext == NULL))
	{
		throw Exception("internal error, cannot locate glxCreateContext");
	}

	if (glx->glx_1_3)
	{
		if ((glx->glXCreatePbuffer == NULL) || (glx->glXChooseFBConfig == NULL) || (glx->glXGetVisualFromFBConfig == NULL))
			throw Exception("glXCreatePbuffer is not available, even though you are running glx 1.3 or better.");

		fbconfig = glx->glXChooseFBConfig(disp, scrnum, attrib, &nitems);

		if (fbconfig == NULL)
		{
			throw Exception("Error: couldn't get fbconfig");
		}
		pbuffer = glx->glXCreatePbuffer(disp, fbconfig[0], pbufAttrib);

		if (!pbuffer)
			throw Exception("Error: couldn't create pbuffer");

		visinfo = glx->glXGetVisualFromFBConfig(disp, fbconfig[0]);
		if (!visinfo)
			throw Exception("Error: couldn't get an RGBA, double-buffered visual");
	}
	else
	{
		if ((glx->glXCreatePbufferSGIX == NULL) || (glx->glXChooseFBConfigSGIX == NULL) || (glx->glXGetVisualFromFBConfigSGIX == NULL))
			throw Exception("pbuffer support is not available. Try updating your graphic card driver.");
	
		fbconfig = glx->glXChooseFBConfigSGIX(disp, scrnum, attrib, &nitems);
		if (fbconfig == NULL)
		{
			throw Exception("Error: couldn't get fbconfig");
		}
		int pbufAttrib2[] = {
			GLX_LARGEST_PBUFFER, False,
			None
			};

		cl1_ctxErrorOccurred = false;
		int (*oldHandler)(::Display*, XErrorEvent*) = XSetErrorHandler(&cl1_ctxErrorHandler);

		pbuffer = glx->glXCreatePbufferSGIX(disp, fbconfig[0], size.width, size.height, pbufAttrib2);

		// Restore the original error handler
		XSetErrorHandler( oldHandler );		
		
		if (cl1_ctxErrorOccurred)
			throw Exception("Failed creating glx 1.2 framebuffer");

		if (!pbuffer)
			throw Exception("Error: couldn't create pbuffer");

		visinfo = glx->glXGetVisualFromFBConfigSGIX(disp, fbconfig[0]);	
		if (!visinfo)
			throw Exception("Error: couldn't get an RGBA, double-buffered visual");

	}

	GLXContext shared_context = window_provider.get_share_context();

	if (!shared_context)
	{
		throw Exception("Error: cannot obtain the shared context");
	}

	pbuffer_context = glx->glXCreateContext( disp, visinfo, shared_context, GL_TRUE );

	if (!pbuffer_context)
	{
		throw Exception("Error: glXCreateContext failed");
	}

	XFree(fbconfig);
	XFree(visinfo);

	pbuffer_gc_provider = new GL1GraphicContextProvider(this);
	pbuffer_gc = GraphicContext(pbuffer_gc_provider);
}

void PBuffer_GL1_Impl::set_active() const
{
	GL1::set_active(pbuffer_gc_provider);
}

}


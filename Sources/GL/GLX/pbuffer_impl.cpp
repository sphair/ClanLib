/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "../GL1/gl1_graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/Window/input_context.h"
#include "opengl_window_provider_glx.h"

namespace clan
{

PBuffer_GL1_Impl::PBuffer_GL1_Impl(GL1GraphicContextProvider *gc_provider) : gc_provider(gc_provider), window_provider(NULL), pbuffer(0), pbuffer_context(0)
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
	OpenGL::remove_active(this);

	if (window_provider)
	{
		if (pbuffer_context) window_provider->glx.glXDestroyContext(window_provider->get_display(), pbuffer_context);
		if (pbuffer)
		{
			if (window_provider->glx.glx_1_3)
			{
				glx->glXDestroyPbuffer(disp, pbuffer);
			}
			else
			{
				glx->glXDestroyPbufferSGIX(disp, pbuffer);
			}
		}
	}

	window_provider = NULL;
	pbuffer_context = 0;
	pbuffer = 0;
}

void PBuffer_GL1_Impl::create(OpenGLWindowProvider &gl_window_provider, const Size &size)
{
	reset();
	OpenGL::set_active(gc_provider);

	window_provider = &gl_window_provider;
	pbuffer_size = size;


	disp = gl_window_provider.get_display();
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

	if ((gl_window_provider.glx.glXCreateContext == NULL))
	{
		throw CL_Exception("internal error, cannot locate glxCreateContext");
	}

	if (gl_window_provider.glx.glx_1_3)
	{
		if ((gl_window_provider.glx.glXCreatePbuffer == NULL) || (gl_window_provider.glx.glXChooseFBConfig == NULL) || (gl_window_provider.glx.glXGetVisualFromFBConfig == NULL))
			throw CL_Exception("glXCreatePbuffer is not available, even though you are running glx 1.3 or better.");

		fbconfig = gl_window_provider.glx.glXChooseFBConfig(disp, scrnum, attrib, &nitems);

		if (fbconfig == NULL)
		{
			throw Exception("Error: couldn't get fbconfig");
		}
		pbuffer = gl_window_provider.glx.glXCreatePbuffer(disp, fbconfig[0], pbufAttrib);

		if (!pbuffer)
		{
			XFree(fbconfig);
			throw Exception("Error: couldn't create pbuffer");
		}

		visinfo = gl_window_provider.glx.glXGetVisualFromFBConfig(disp, fbconfig[0]);
		if (!visinfo)
		{
			XFree(fbconfig);
			throw Exception("Error: couldn't get an RGBA, double-buffered visual");
		}
	}
	else
	{
		if ((gl_window_provider.glx.glXCreatePbufferSGIX == NULL) || (gl_window_provider.glx.glXChooseFBConfigSGIX == NULL) || (gl_window_provider.glx.glXGetVisualFromFBConfigSGIX == NULL))
			throw Exception("pbuffer support is not available. Try updating your graphic card driver.");
	
		fbconfig = gl_window_provider.glx.glXChooseFBConfigSGIX(disp, scrnum, attrib, &nitems);
		if (fbconfig == NULL)
		{
			throw Exception("Error: couldn't get fbconfig");
		}
		int pbufAttrib2[] = {
			GLX_LARGEST_PBUFFER, False,
			None
			};

		cl1_ctxErrorOccurred = false;
		int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&cl1_ctxErrorHandler);

		pbuffer = gl_window_provider.glx.glXCreatePbufferSGIX(disp, fbconfig[0], size.width, size.height, pbufAttrib2);

		// Restore the original error handler
		XSetErrorHandler( oldHandler );		
		
		if (cl1_ctxErrorOccurred)
		{
			XFree(fbconfig);
			throw Exception("Failed creating glx 1.2 framebuffer");
		}

		if (!pbuffer)
		{
			XFree(fbconfig);
			throw Exception("Error: couldn't create pbuffer");
		}

		visinfo = gl_window_provider.glx.glXGetVisualFromFBConfigSGIX(disp, fbconfig[0]);	
		if (!visinfo)
		{
			XFree(fbconfig);
			throw Exception("Error: couldn't get an RGBA, double-buffered visual");
		}

	}

	GLXContext shared_context = gl_window_provider.get_share_context();

	if (!shared_context)
	{
		XFree(fbconfig);
		XFree(visinfo);
		throw Exception("Error: cannot obtain the shared context");
	}

	pbuffer_context = gl_window_provider.glx.glXCreateContext( disp, visinfo, shared_context, GL_TRUE );

	if (!pbuffer_context)
	{
		XFree(fbconfig);
		XFree(visinfo);
		throw Exception("Error: glXCreateContext failed");
	}

	XFree(fbconfig);
	XFree(visinfo);
}

void PBuffer_GL1_Impl::make_current() const
{
	pbuffer_impl.glx->glXMakeCurrent(display, pbuffer, glx_context);
}

void PBuffer_GL1_Impl::get_opengl_version(int &version_major, int &version_minor)
{
	gc_provider->get_opengl_version(version_major, version_minor);
}

void PBuffer_GL1_Impl::get_opengl_version(int &version_major, int &version_minor, int &version_release)
{
	gc_provider->get_opengl_version(version_major, version_minor, version_release);
}
	
ProcAddress *PBuffer_GL1_Impl::get_proc_address(const std::string& function_name) const
{
	if (pbuffer_impl.glx->glXGetProcAddressARB)
		return pbuffer_impl.glx->glXGetProcAddressARB((GLubyte*)function_name.c_str());
	if (pbuffer_impl.glx->glXGetProcAddress)
		return pbuffer_impl.glx->glXGetProcAddress((GLubyte*)function_name.c_str());
	return NULL;
}

}


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
**    Magnus Norddahl
**    Harry Storbacka
**    Mark Page
**    Kevin J Bluck
*/

#include "GL/precomp.h"
#include "opengl_window_provider_glx.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl_window_description.h"
#include "API/Core/Text/logger.h"
#include "Display/X11/cursor_provider_x11.h"
#include "../opengl_window_description_impl.h"
#include "../opengl_graphic_context_provider.h"
#include "../opengl_target_provider.h"
#include <cstdio>

#ifdef CL_USE_DLOPEN
#define CL_OPENGL_LIBRARY "libGL.so.1"
#include <dlfcn.h>
#endif
		
#ifdef CL_USE_DLOPEN
#define CL_LOAD_GLFUNC(x) dlsym(opengl_lib_handle, # x)
#else
#define CL_LOAD_GLFUNC(x) &x
#endif

CL_GL_RenderWindowProvider_GLX::CL_GL_RenderWindowProvider_GLX(CL_OpenGLWindowProvider_GLX & window, GLXContext glx_context, bool own_context)
	: window(window), glx_context(glx_context), own_context(own_context)
{
}

CL_GL_RenderWindowProvider_GLX::~CL_GL_RenderWindowProvider_GLX()
{
	if( glx_context && own_context)
	{
		window.glx.glXDestroyContext(window.get_display(), glx_context);
	}
}

int CL_GL_RenderWindowProvider_GLX::get_viewport_width() const
{
	return window.get_viewport().get_width();
}

int CL_GL_RenderWindowProvider_GLX::get_viewport_height() const
{
	return window.get_viewport().get_height();
}

void CL_GL_RenderWindowProvider_GLX::flip_buffers(int interval) const
{
	window.flip(interval);
}

void CL_GL_RenderWindowProvider_GLX::make_current() const
{
// *** Note, If glxMakeCurrent crashes KDE when using a nvidia graphics card, then
	// update the driver from nvidia.com ***
	window.glx.glXMakeCurrent(window.get_display(), window.get_window(), glx_context);
}

const CL_RenderWindowProvider * CL_GL_RenderWindowProvider_GLX::new_worker_context() const
{
	return new CL_GL_RenderWindowProvider_GLX(window, window.create_context(), true);
}

CL_ProcAddress *CL_GL_RenderWindowProvider_GLX::get_proc_address(const CL_String8& function_name) const
{
	if (window.glx.glXGetProcAddressARB)
		return window.glx.glXGetProcAddressARB((GLubyte*)function_name.c_str());
	if (window.glx.glXGetProcAddress)
		return window.glx.glXGetProcAddress((GLubyte*)function_name.c_str());
	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowProvider_GLX Construction:

CL_OpenGLWindowProvider_GLX::CL_OpenGLWindowProvider_GLX()
: x11_window(),
 opengl_context(0), opengl_visual_info(0), glXSwapIntervalSGI(NULL), glXSwapIntervalMESA(NULL), swap_interval(-1)
#ifdef CL_USE_DLOPEN
, opengl_lib_handle(NULL)
#endif
{
#ifdef CL_USE_DLOPEN
	// http://www.xfree86.org/4.8.0/DRI11.html -
	// "Do not close the library with dlclose() until after XCloseDisplay() has been called. When libGL.so initializes itself it registers several callbacks functions with Xlib. When XCloseDisplay() is called those callback functions are called. If libGL.so has already been unloaded with dlclose() this will cause a segmentation fault"
	// - Which it did - So we need x11_window to own the library (and close it)
	opengl_lib_handle = x11_window.dlopen(CL_OPENGL_LIBRARY, RTLD_NOW | RTLD_GLOBAL);
	if (!opengl_lib_handle)
	{
		throw CL_Exception(cl_format("Cannot open opengl library: %1", CL_OPENGL_LIBRARY));
	}
#endif
	glx.glXChooseVisual = (CL_GL_GLXFunctions::ptr_glXChooseVisual) CL_LOAD_GLFUNC(glXChooseVisual);
	glx.glXCopyContext = (CL_GL_GLXFunctions::ptr_glXCopyContext) CL_LOAD_GLFUNC(glXCopyContext);
	glx.glXCreateContext = (CL_GL_GLXFunctions::ptr_glXCreateContext) CL_LOAD_GLFUNC(glXCreateContext);
	glx.glXCreateGLXPixmap = (CL_GL_GLXFunctions::ptr_glXCreateGLXPixmap) CL_LOAD_GLFUNC(glXCreateGLXPixmap);
	glx.glXDestroyContext = (CL_GL_GLXFunctions::ptr_glXDestroyContext) CL_LOAD_GLFUNC(glXDestroyContext);
	glx.glXDestroyGLXPixmap = (CL_GL_GLXFunctions::ptr_glXDestroyGLXPixmap) CL_LOAD_GLFUNC(glXDestroyGLXPixmap);
	glx.glXGetConfig = (CL_GL_GLXFunctions::ptr_glXGetConfig) CL_LOAD_GLFUNC(glXGetConfig);
	glx.glXGetCurrentContext = (CL_GL_GLXFunctions::ptr_glXGetCurrentContext) CL_LOAD_GLFUNC(glXGetCurrentContext);
	glx.glXGetCurrentDrawable = (CL_GL_GLXFunctions::ptr_glXGetCurrentDrawable) CL_LOAD_GLFUNC(glXGetCurrentDrawable);
	glx.glXIsDirect = (CL_GL_GLXFunctions::ptr_glXIsDirect) CL_LOAD_GLFUNC(glXIsDirect);
	glx.glXMakeCurrent = (CL_GL_GLXFunctions::ptr_glXMakeCurrent) CL_LOAD_GLFUNC(glXMakeCurrent);
	glx.glXQueryExtension = (CL_GL_GLXFunctions::ptr_glXQueryExtension) CL_LOAD_GLFUNC(glXQueryExtension);
	glx.glXQueryVersion = (CL_GL_GLXFunctions::ptr_glXQueryVersion) CL_LOAD_GLFUNC(glXQueryVersion);
	glx.glXSwapBuffers = (CL_GL_GLXFunctions::ptr_glXSwapBuffers) CL_LOAD_GLFUNC(glXSwapBuffers);
	glx.glXUseXFont = (CL_GL_GLXFunctions::ptr_glXUseXFont) CL_LOAD_GLFUNC(glXUseXFont);
	glx.glXWaitGL = (CL_GL_GLXFunctions::ptr_glXWaitGL) CL_LOAD_GLFUNC(glXWaitGL);
	glx.glXWaitX = (CL_GL_GLXFunctions::ptr_glXWaitX) CL_LOAD_GLFUNC(glXWaitX);
	glx.glXGetClientString = (CL_GL_GLXFunctions::ptr_glXGetClientString) CL_LOAD_GLFUNC(glXGetClientString);
	glx.glXQueryServerString = (CL_GL_GLXFunctions::ptr_glXQueryServerString) CL_LOAD_GLFUNC(glXQueryServerString);
	glx.glXQueryExtensionsString = (CL_GL_GLXFunctions::ptr_glXQueryExtensionsString) CL_LOAD_GLFUNC(glXQueryExtensionsString);
	glx.glXGetCurrentDisplay = (CL_GL_GLXFunctions::ptr_glXGetCurrentDisplay) CL_LOAD_GLFUNC(glXGetCurrentDisplay);
	glx.glXChooseFBConfig = (CL_GL_GLXFunctions::ptr_glXChooseFBConfig) CL_LOAD_GLFUNC(glXChooseFBConfig);
	glx.glXCreateNewContext = (CL_GL_GLXFunctions::ptr_glXCreateNewContext) CL_LOAD_GLFUNC(glXCreateNewContext);
	glx.glXCreatePbuffer = (CL_GL_GLXFunctions::ptr_glXCreatePbuffer) CL_LOAD_GLFUNC(glXCreatePbuffer);
	glx.glXCreatePixmap = (CL_GL_GLXFunctions::ptr_glXCreatePixmap) CL_LOAD_GLFUNC(glXCreatePixmap);
	glx.glXCreateWindow = (CL_GL_GLXFunctions::ptr_glXCreateWindow) CL_LOAD_GLFUNC(glXCreateWindow);
	glx.glXDestroyPbuffer = (CL_GL_GLXFunctions::ptr_glXDestroyPbuffer) CL_LOAD_GLFUNC(glXDestroyPbuffer);
	glx.glXDestroyPixmap = (CL_GL_GLXFunctions::ptr_glXDestroyPixmap) CL_LOAD_GLFUNC(glXDestroyPixmap);
	glx.glXDestroyWindow = (CL_GL_GLXFunctions::ptr_glXDestroyWindow) CL_LOAD_GLFUNC(glXDestroyWindow);
	glx.glXGetCurrentReadDrawable = (CL_GL_GLXFunctions::ptr_glXGetCurrentReadDrawable) CL_LOAD_GLFUNC(glXGetCurrentReadDrawable);
	glx.glXGetFBConfigAttrib = (CL_GL_GLXFunctions::ptr_glXGetFBConfigAttrib) CL_LOAD_GLFUNC(glXGetFBConfigAttrib);
	glx.glXGetFBConfigs = (CL_GL_GLXFunctions::ptr_glXGetFBConfigs) CL_LOAD_GLFUNC(glXGetFBConfigs);
	glx.glXGetSelectedEvent = (CL_GL_GLXFunctions::ptr_glXGetSelectedEvent) CL_LOAD_GLFUNC(glXGetSelectedEvent);
	glx.glXGetVisualFromFBConfig = (CL_GL_GLXFunctions::ptr_glXGetVisualFromFBConfig) CL_LOAD_GLFUNC(glXGetVisualFromFBConfig);
	glx.glXMakeContextCurrent = (CL_GL_GLXFunctions::ptr_glXMakeContextCurrent) CL_LOAD_GLFUNC(glXMakeContextCurrent);
	glx.glXQueryContext = (CL_GL_GLXFunctions::ptr_glXQueryContext) CL_LOAD_GLFUNC(glXQueryContext);
	glx.glXQueryDrawable = (CL_GL_GLXFunctions::ptr_glXQueryDrawable) CL_LOAD_GLFUNC(glXQueryDrawable);
	glx.glXSelectEvent = (CL_GL_GLXFunctions::ptr_glXSelectEvent) CL_LOAD_GLFUNC(glXSelectEvent);

	glx.glXGetProcAddressARB = (CL_GL_GLXFunctions::ptr_glXGetProcAddressARB) CL_LOAD_GLFUNC(glXGetProcAddressARB);
	glx.glXGetProcAddress = (CL_GL_GLXFunctions::ptr_glXGetProcAddress) CL_LOAD_GLFUNC(glXGetProcAddress);

	if ( (glx.glXDestroyContext == NULL) ||
		(glx.glXMakeCurrent == NULL) ||
		(glx.glXGetCurrentContext == NULL) ||
		(glx.glXChooseVisual == NULL) ||
		(glx.glXIsDirect == NULL) ||
		(glx.glXGetConfig == NULL) ||
		(glx.glXQueryExtensionsString == NULL) ||
		(glx.glXQueryVersion == NULL) ||
		(glx.glXGetVisualFromFBConfig == NULL) ||
		(glx.glXCreateNewContext == NULL) ||
		(glx.glXCreateContext == NULL) )
	{
		throw CL_Exception("Cannot obtain required OpenGL GLX functions");
	}

	if ((glx.glXGetProcAddressARB == NULL) && (glx.glXGetProcAddress == NULL))
	{
		throw CL_Exception("Cannot obtain required OpenGL GLX functions");
	}

	x11_window.func_on_resized().set(this, &CL_OpenGLWindowProvider_GLX::on_window_resized);
}

CL_OpenGLWindowProvider_GLX::~CL_OpenGLWindowProvider_GLX()
{

	if (opengl_visual_info)
	{
		XFree(opengl_visual_info);
		opengl_visual_info = NULL;
	}

	if (opengl_context)
	{
		// save gc provider pointer so we can delete it later from shared list
		CL_GraphicContextProvider *destroyed_gc_provider = gc.get_provider();

		// Delete context from list of gc's that share textures.
		if (CL_SharedGCData::get_instance()) // Check that the cache hasn't been destroyed yet
		{
			std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
			if (gc_providers.size() == 1)
				CL_SharedGCData::dispose_objects();
			for (std::vector<CL_GraphicContextProvider*>::iterator it=gc_providers.begin(); it != gc_providers.end(); ++it)
			{
				if (destroyed_gc_provider == (*it))
				{
					gc_providers.erase(it);
					break;
				}
			}
		}

		// Destroy graphic context before the window is destroyed
		gc = CL_GraphicContext();

		// Delete the context

		Display *disp = x11_window.get_display();
		if (glx.glXGetCurrentContext() == opengl_context)
		{
			CL_OpenGL::set_active(NULL);
		}

		if (disp)
		{
			glx.glXDestroyContext(disp, opengl_context);
		}

		opengl_context = 0;
	}

}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowProvider_GLX Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowProvider_GLX Operations:

void CL_OpenGLWindowProvider_GLX::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc)
{
	site = new_site;
	bool create_provider_flag = false;

	x11_window.open_screen();
	Display *disp = x11_window.get_display();

	if (!opengl_context)
	{
		// FBConfigs were added in GLX version 1.3.
		int glx_major, glx_minor;
		if ( !glx.glXQueryVersion( disp, &glx_major, &glx_minor ) || 
			( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
		{
			glx_1_3 = false;
		}
		else
		{
			glx_1_3 = true;
		}

		create_provider_flag = true;

		if (glx_1_3)
		{
			create_glx_1_3(new_site, desc, disp);
		}
		else
		{
			create_glx_1_2(new_site, desc, disp);
		}

		if (!glx.glXIsDirect(disp, opengl_context))
			printf("No hardware acceleration available. I hope you got a really fast machine.\n");
	}

	x11_window.create(opengl_visual_info, site, desc);

	if (create_provider_flag)
	{
		gc = CL_GraphicContext(new CL_OpenGLGraphicContextProvider(new CL_GL_RenderWindowProvider_GLX(*this, opengl_context, false)));
		std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
		gc_providers.push_back(gc.get_provider());
	}

	setup_swap_interval_pointers();
	swap_interval = desc.get_swap_interval();
	if (swap_interval != -1)
	{
		if (glXSwapIntervalSGI)
		{
			glXSwapIntervalSGI(swap_interval);
		}
		else if (glXSwapIntervalMESA)
		{
			glXSwapIntervalMESA(swap_interval);
		}
	}
}

void CL_OpenGLWindowProvider_GLX::create_glx_1_3(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc, Display *disp)
{
	if (glx.glXChooseFBConfig == NULL)
		throw CL_Exception("Cannot find the glXChooseFBConfig function");

	// Setup OpenGL:
	int gl_attribs_single[] =
	{
		GLX_X_RENDERABLE, True,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DEPTH_SIZE, 16,
		GLX_STENCIL_SIZE, 8,
		GLX_BUFFER_SIZE, 24,
		None
	};

	CL_OpenGLWindowDescription gl_desc(desc);

	std::vector<int> gl_attribs;
	gl_attribs.reserve(64);

	gl_attribs.push_back(GLX_X_RENDERABLE);
	gl_attribs.push_back(True);
	gl_attribs.push_back(GLX_DRAWABLE_TYPE);
	gl_attribs.push_back(GLX_WINDOW_BIT);
	gl_attribs.push_back(GLX_RENDER_TYPE);
	gl_attribs.push_back(GLX_RGBA_BIT);
	gl_attribs.push_back(GLX_X_VISUAL_TYPE);
	gl_attribs.push_back(GLX_TRUE_COLOR);
	gl_attribs.push_back(GLX_RED_SIZE);
	gl_attribs.push_back(gl_desc.get_red_size());
	gl_attribs.push_back(GLX_GREEN_SIZE);
	gl_attribs.push_back(gl_desc.get_green_size());
	gl_attribs.push_back(GLX_BLUE_SIZE);
	gl_attribs.push_back(gl_desc.get_blue_size());
	gl_attribs.push_back(GLX_ALPHA_SIZE);
	gl_attribs.push_back(gl_desc.get_alpha_size());
	gl_attribs.push_back(GLX_DEPTH_SIZE);
	gl_attribs.push_back(gl_desc.get_depth_size());
	gl_attribs.push_back(GLX_STENCIL_SIZE);
	gl_attribs.push_back(gl_desc.get_stencil_size());
	gl_attribs.push_back(GLX_DOUBLEBUFFER);
	gl_attribs.push_back(gl_desc.get_doublebuffer() ? True : False);
	gl_attribs.push_back(GLX_STEREO);
	gl_attribs.push_back(gl_desc.get_stereo() ? True : False);
	gl_attribs.push_back(None);

	// get an appropriate visual
	int fb_count;
	GLXFBConfig *fbc = glx.glXChooseFBConfig( disp, DefaultScreen(disp), &gl_attribs[0], &fb_count );
	if (!fbc)
	{
		printf("Requested visual not supported by your OpenGL implementation. Falling back on singlebuffered Visual!\n");
		fbc = glx.glXChooseFBConfig( disp, DefaultScreen(disp), gl_attribs_single, &fb_count );
		if (!fbc)
			throw CL_Exception(" glxChooseFBConfig failed");
		fbconfig = fbc[0];
	}
	else
	{
		if (!glx.glXGetFBConfigAttrib)
			throw CL_Exception("Cannot find function glXGetFBConfigAttrib");

		int desired_config = 0;
		int max_sample_buffers = 0;
		int max_samples = 0;
		int required_samples = gl_desc.get_multisampling();
		// Find the best fitting multisampling option
		for (int i=0; i<fb_count; i++)
		{
			int samp_buf, samples;
			glx.glXGetFBConfigAttrib( disp, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
			glx.glXGetFBConfigAttrib( disp, fbc[i], GLX_SAMPLES       , &samples  );

			// Samples are most important, because they are variable
			if (max_samples < required_samples)
			{
				if (samples > max_samples)
				{
					max_samples = samples;
					desired_config = i;
				}
			}

			// Use the maximum sample buffer
			if (max_samples == samples)	// Only check if the sample is valid
			{
				if (max_sample_buffers < samp_buf)
				{
					max_sample_buffers = samp_buf;
					desired_config = i;
				}
			}
		}
		fbconfig = fbc[desired_config];
	}

	XFree(fbc);

	if (opengl_visual_info) XFree(opengl_visual_info);
	opengl_visual_info = glx.glXGetVisualFromFBConfig(disp, fbconfig);
	if (opengl_visual_info == NULL)
	{
		throw CL_Exception("glXGetVisualFromFBConfig failed");
	}

	// create a GLX context
	opengl_context = create_context();
}


void CL_OpenGLWindowProvider_GLX::create_glx_1_2(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc, Display *disp)
{
	// Setup OpenGL:
	int gl_attribs_single[] =
	{
		GLX_RGBA,
		GLX_DEPTH_SIZE, 16,
		GLX_STENCIL_SIZE, 8,
		GLX_BUFFER_SIZE, 24,
		None
	};

	int i = 0;

	CL_OpenGLWindowDescription gl_desc(desc);

	int gl_attribs[32];	// WARNING - create() assumes this is 32 in size

	// Note: gl_attribs[32] !!!!
	gl_attribs[i++] = GLX_RGBA;
	if( gl_desc.get_doublebuffer() ) gl_attribs[i++] = GLX_DOUBLEBUFFER;
	if( gl_desc.get_stereo() ) gl_attribs[i++] = GLX_STEREO;
	gl_attribs[i++] = GLX_BUFFER_SIZE;
	gl_attribs[i++] = gl_desc.get_buffer_size();
	gl_attribs[i++] = GLX_RED_SIZE; 
	gl_attribs[i++] = gl_desc.get_red_size();
	gl_attribs[i++] = GLX_GREEN_SIZE;
	gl_attribs[i++] = gl_desc.get_green_size();
	gl_attribs[i++] = GLX_BLUE_SIZE;
	gl_attribs[i++] = gl_desc.get_blue_size();
	gl_attribs[i++] = GLX_DEPTH_SIZE;
	gl_attribs[i++] = gl_desc.get_depth_size();
	gl_attribs[i++] = GLX_STENCIL_SIZE;
	gl_attribs[i++] = gl_desc.get_stencil_size();
	gl_attribs[i++] = None;

	// get an appropriate visual
	if (opengl_visual_info) XFree(opengl_visual_info);
	opengl_visual_info = glx.glXChooseVisual(disp, DefaultScreen(disp), gl_attribs);

	if (opengl_visual_info == NULL)
	{
		opengl_visual_info = glx.glXChooseVisual(disp, DefaultScreen(disp), gl_attribs_single);
		printf("Requested visual not supported by your OpenGL implementation. Falling back on singlebuffered Visual!\n");
		if (opengl_visual_info == NULL)
		{
			throw CL_Exception("glxChooseVisual failed");
		}
	}

	// create a GLX context
	opengl_context = create_context();

}

void CL_OpenGLWindowProvider_GLX::on_window_resized()
{
	if (gc.get_provider())
		((CL_OpenGLGraphicContextProvider *) gc.get_provider())->on_window_resized();
}

bool CL_OpenGLWindowProvider_GLX::is_glx_extension_supported(const char *ext_name)
{
	const char *ext_string = glx.glXQueryExtensionsString(x11_window.get_display(), opengl_visual_info->screen);
	if (ext_string)
	{
		const char *start;
		const char *where, *terminator;
		
		// Extension names should not have spaces.
		where = strchr(ext_name, ' ');
		if ( where || *ext_name == '\0' )
			return false;

		int ext_len = strlen(ext_name);
		
		// It takes a bit of care to be fool-proof about parsing the OpenGL extensions string. Don't be fooled by sub-strings, etc.
		for ( start = ext_string; ; )
		{
			where = strstr( start, ext_name );

			if ( !where )
				break;

			terminator = where + ext_len;

			if ( where == start || *(where - 1) == ' ' )
				if ( *terminator == ' ' || *terminator == '\0' )
					return true;

			start = terminator;
		}
	}
	return false;
}

void CL_OpenGLWindowProvider_GLX::setup_swap_interval_pointers()
{
	glXSwapIntervalSGI = (ptr_glXSwapIntervalSGI) CL_OpenGL::get_proc_address("glXSwapIntervalSGI");
	glXSwapIntervalMESA = (ptr_glXSwapIntervalMESA) CL_OpenGL::get_proc_address("glXSwapIntervalMESA");

	// See - http://dri.freedesktop.org/wiki/glXGetProcAddressNeverReturnsNULL ,get_proc_address() may return an invalid extension address
	if ( !is_glx_extension_supported("GLX_SGI_swap_control") )
	{
		glXSwapIntervalSGI = NULL;
	}

	if ( !is_glx_extension_supported("GLX_MESA_swap_control") )
	{
		glXSwapIntervalMESA = NULL;
	}
}

static bool cl_ctxErrorOccurred = false;
static int cl_ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
    cl_ctxErrorOccurred = true;
    return 0;
}

GLXContext CL_OpenGLWindowProvider_GLX::create_context()
{
	GLXContext shared_context = NULL;

	std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
	if (!gc_providers.empty())
	{
		CL_OpenGLGraphicContextProvider *gl_provider = dynamic_cast<CL_OpenGLGraphicContextProvider*>(gc_providers.back());
		if (gl_provider)
		{
			const CL_RenderWindowProvider *rwp = &gl_provider->get_render_window();
			const CL_GL_RenderWindowProvider_GLX *render_window_glx = dynamic_cast<const CL_GL_RenderWindowProvider_GLX*>(rwp);
			if (render_window_glx)
				shared_context = render_window_glx->get_context();
		}
	}

	GLXContext context;

	if (glx_1_3)
	{
		context = create_context_glx_1_3(shared_context);
	}
	else
	{
		context = create_context_glx_1_2(shared_context);
	}

	return context;
}

GLXContext CL_OpenGLWindowProvider_GLX::create_context_glx_1_3(GLXContext shared_context)
{
	GLXContext context;

	context = glx.glXCreateNewContext(x11_window.get_display(), fbconfig, GLX_RGBA_TYPE, shared_context, True);
	if(context == NULL)
		throw CL_Exception("glXCreateContext failed");
	
	int attribs[] = {
		0x2091, //GLX_CONTEXT_MAJOR_VERSION_ARB,
		3,
		0x2092, //GLX_CONTEXT_MINOR_VERSION_ARB,
		0,
		None
	};

	ptr_glXCreateContextAttribs glXCreateContextAttribs = NULL;

	if (is_glx_extension_supported("GLX_ARB_create_context"))
	{
		if (glx.glXGetProcAddressARB)
			glXCreateContextAttribs = (ptr_glXCreateContextAttribs) glx.glXGetProcAddressARB((GLubyte*) "glXCreateContextAttribsARB");
		if (glx.glXGetProcAddress)
			glXCreateContextAttribs = (ptr_glXCreateContextAttribs) glx.glXGetProcAddress((GLubyte*) "glXCreateContextAttribsARB");
	}


	if (glXCreateContextAttribs)
	{
		// Install an X error handler so the application won't exit if GL 3.0 context allocation fails.
		//
		// Note this error handler is global.  All display connections in all threads
		// of a process use the same error handler, so be sure to guard against other
		// threads issuing X commands while this code is running.
		cl_ctxErrorOccurred = false;
		int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&cl_ctxErrorHandler);
	  
		GLXContext context_gl3;
		context_gl3 = glXCreateContextAttribs(x11_window.get_display(), fbconfig, shared_context, True, attribs);
		
		// Restore the original error handler
		XSetErrorHandler( oldHandler );		
		
		if (cl_ctxErrorOccurred)
		{
			if (context_gl3)
			{
				glx.glXDestroyContext(x11_window.get_display(), context_gl3);
				context_gl3 = 0;
			}
		}
		
		if (context_gl3)
		{
			glx.glXDestroyContext(x11_window.get_display(), context);
			context = context_gl3;
		}
	}
	return context;
}

GLXContext CL_OpenGLWindowProvider_GLX::create_context_glx_1_2(GLXContext shared_context)
{
	GLXContext context;
	context = glx.glXCreateContext(x11_window.get_display(), opengl_visual_info, shared_context, GL_TRUE);
	if(context == NULL)
		throw CL_Exception("glXCreateContext failed");

	return context;
}

void CL_OpenGLWindowProvider_GLX::flip(int interval)
{
	CL_GraphicContext gc = get_gc();
	CL_OpenGL::set_active(gc);
	clFlush();

	if (interval != -1 && interval != swap_interval)
	{
		swap_interval = interval;
		if (glXSwapIntervalSGI)
		{
			glXSwapIntervalSGI(swap_interval);
		}
		else if (glXSwapIntervalMESA)
		{
			glXSwapIntervalMESA(swap_interval);
		}
	}

	glx.glXSwapBuffers(x11_window.get_display(), x11_window.get_window());
}

void CL_OpenGLWindowProvider_GLX::update(const CL_Rect &_rect)
{
	int width = get_viewport().get_width();
	int height = get_viewport().get_height();

	CL_Rect rect = _rect;
	if (rect.left < 0)
		rect.left = 0;
	if (rect.top < 0)
		rect.top = 0;
	if (rect.right > width)
		rect.right = width;
	if (rect.bottom > height)
		rect.bottom = height;
	if (rect.right <= rect.left || rect.bottom <= rect.top)
		return;

	CL_OpenGL::set_active(gc);
	clFlush();

	CLboolean isDoubleBuffered = CL_TRUE;
	clGetBooleanv(CL_DOUBLEBUFFER, &isDoubleBuffered);
	if (isDoubleBuffered)
	{
		CLint read_last_bound;
		CLint draw_last_bound;

		clGetIntegerv(CL_READ_FRAMEBUFFER_BINDING, &read_last_bound);
		clGetIntegerv(CL_DRAW_FRAMEBUFFER_BINDING, &draw_last_bound);

		clBindFramebuffer(CL_READ_FRAMEBUFFER, 0);
	    clBindFramebuffer(CL_DRAW_FRAMEBUFFER, 0);

		clReadBuffer(CL_BACK);
		clDrawBuffer(CL_FRONT);

		clBlitFramebuffer( 
			rect.left, height - rect.bottom,
			rect.right, height - rect.top,
			rect.left, height - rect.bottom,
			rect.right, height - rect.top,
			CL_COLOR_BUFFER_BIT, CL_LINEAR);

		clDrawBuffer(CL_BACK);
		clReadBuffer(CL_FRONT);

		if (read_last_bound)
			clBindFramebuffer(CL_READ_FRAMEBUFFER, read_last_bound);

		if (draw_last_bound)
			clBindFramebuffer(CL_DRAW_FRAMEBUFFER, draw_last_bound);

		clFlush();


	}
}


CL_CursorProvider *CL_OpenGLWindowProvider_GLX::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
	return new CL_CursorProvider_X11(sprite_description, hotspot);
}

void CL_OpenGLWindowProvider_GLX::set_cursor(CL_CursorProvider *cursor)
{
	x11_window.set_cursor(static_cast<CL_CursorProvider_X11 *>(cursor));
}

void CL_OpenGLWindowProvider_GLX::set_large_icon(const CL_PixelBuffer &image)
{
	x11_window.set_large_icon(image);
}

void CL_OpenGLWindowProvider_GLX::set_small_icon(const CL_PixelBuffer &image)
{
	x11_window.set_small_icon(image);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowProvider_GLX Implementation:



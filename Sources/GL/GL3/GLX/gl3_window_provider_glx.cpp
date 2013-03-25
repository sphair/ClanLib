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
**    Magnus Norddahl
**    Harry Storbacka
**    Mark Page
**    Kevin J Bluck
*/

#include "GL/precomp.h"
#include "gl3_window_provider_glx.h"
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
#include "Display/X11/display_message_queue_x11.h"
#include "GL/opengl_window_description_impl.h"
#include "GL/opengl_graphic_context_provider.h"
#include "GL/opengl_target_provider.h"
#include "GL/GL3/gl3_graphic_context_provider.h"
#include <cstdio>

#ifdef GL_USE_DLOPEN
#define GL_OPENGL_LIBRARY "libGL.so.1"
#include <dlfcn.h>
#endif
		
#ifdef GL_USE_DLOPEN
#define GL_LOAD_GLFUNC(x) dlsym(opengl_lib_handle, # x)
#else
#define GL_LOAD_GLFUNC(x) &x
#endif

#ifdef HAVE_X11_EXTENSIONS_XRENDER_H
#include <X11/extensions/Xrender.h>
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GL3WindowProvider Construction:

GL3WindowProvider::GL3WindowProvider(OpenGLWindowDescription &opengl_desc)
: x11_window(),
 opengl_context(0), opengl_visual_info(0), glXSwapIntervalSGI(NULL), glXSwapIntervalMESA(NULL), swap_interval(-1), opengl_desc(opengl_desc)
#ifdef GL_USE_DLOPEN
, opengl_lib_handle(NULL)
#endif
{
#ifdef GL_USE_DLOPEN
	// http://www.xfree86.org/4.8.0/DRI11.html -
	// "Do not close the library with dlclose() until after XCloseDisplay() has been called. When libGL.so initializes itself it registers several callbacks functions with Xlib. When XCloseDisplay() is called those callback functions are called. If libGL.so has already been unloaded with dlclose() this will cause a segmentation fault"
	// - Which it did - So we need x11_window to own the library (and close it)
	opengl_lib_handle = DisplayMessageQueue_X11::message_queue.dlopen_opengl(GL_OPENGL_LIBRARY, RTLD_NOW | RTLD_GLOBAL);
	if (!opengl_lib_handle)
	{
		throw Exception(string_format("Cannot open opengl library: %1", GL_OPENGL_LIBRARY));
	}
#endif
	glx.glXChooseVisual = (GL_GLXFunctions::ptr_glXChooseVisual) GL_LOAD_GLFUNC(glXChooseVisual);
	glx.glXCopyContext = (GL_GLXFunctions::ptr_glXCopyContext) GL_LOAD_GLFUNC(glXCopyContext);
	glx.glXCreateContext = (GL_GLXFunctions::ptr_glXCreateContext) GL_LOAD_GLFUNC(glXCreateContext);
	glx.glXCreateGLXPixmap = (GL_GLXFunctions::ptr_glXCreateGLXPixmap) GL_LOAD_GLFUNC(glXCreateGLXPixmap);
	glx.glXDestroyContext = (GL_GLXFunctions::ptr_glXDestroyContext) GL_LOAD_GLFUNC(glXDestroyContext);
	glx.glXDestroyGLXPixmap = (GL_GLXFunctions::ptr_glXDestroyGLXPixmap) GL_LOAD_GLFUNC(glXDestroyGLXPixmap);
	glx.glXGetConfig = (GL_GLXFunctions::ptr_glXGetConfig) GL_LOAD_GLFUNC(glXGetConfig);
	glx.glXGetCurrentContext = (GL_GLXFunctions::ptr_glXGetCurrentContext) GL_LOAD_GLFUNC(glXGetCurrentContext);
	glx.glXGetCurrentDrawable = (GL_GLXFunctions::ptr_glXGetCurrentDrawable) GL_LOAD_GLFUNC(glXGetCurrentDrawable);
	glx.glXIsDirect = (GL_GLXFunctions::ptr_glXIsDirect) GL_LOAD_GLFUNC(glXIsDirect);
	glx.glXMakeCurrent = (GL_GLXFunctions::ptr_glXMakeCurrent) GL_LOAD_GLFUNC(glXMakeCurrent);
	glx.glXQueryExtension = (GL_GLXFunctions::ptr_glXQueryExtension) GL_LOAD_GLFUNC(glXQueryExtension);
	glx.glXQueryVersion = (GL_GLXFunctions::ptr_glXQueryVersion) GL_LOAD_GLFUNC(glXQueryVersion);
	glx.glXSwapBuffers = (GL_GLXFunctions::ptr_glXSwapBuffers) GL_LOAD_GLFUNC(glXSwapBuffers);
	glx.glXUseXFont = (GL_GLXFunctions::ptr_glXUseXFont) GL_LOAD_GLFUNC(glXUseXFont);
	glx.glXWaitGL = (GL_GLXFunctions::ptr_glXWaitGL) GL_LOAD_GLFUNC(glXWaitGL);
	glx.glXWaitX = (GL_GLXFunctions::ptr_glXWaitX) GL_LOAD_GLFUNC(glXWaitX);
	glx.glXGetClientString = (GL_GLXFunctions::ptr_glXGetClientString) GL_LOAD_GLFUNC(glXGetClientString);
	glx.glXQueryServerString = (GL_GLXFunctions::ptr_glXQueryServerString) GL_LOAD_GLFUNC(glXQueryServerString);
	glx.glXQueryExtensionsString = (GL_GLXFunctions::ptr_glXQueryExtensionsString) GL_LOAD_GLFUNC(glXQueryExtensionsString);
	glx.glXGetCurrentDisplay = (GL_GLXFunctions::ptr_glXGetCurrentDisplay) GL_LOAD_GLFUNC(glXGetCurrentDisplay);
	glx.glXChooseFBConfig = (GL_GLXFunctions::ptr_glXChooseFBConfig) GL_LOAD_GLFUNC(glXChooseFBConfig);
	glx.glXCreateNewContext = (GL_GLXFunctions::ptr_glXCreateNewContext) GL_LOAD_GLFUNC(glXCreateNewContext);
	glx.glXCreatePbuffer = (GL_GLXFunctions::ptr_glXCreatePbuffer) GL_LOAD_GLFUNC(glXCreatePbuffer);
	glx.glXCreatePixmap = (GL_GLXFunctions::ptr_glXCreatePixmap) GL_LOAD_GLFUNC(glXCreatePixmap);
	glx.glXCreateWindow = (GL_GLXFunctions::ptr_glXCreateWindow) GL_LOAD_GLFUNC(glXCreateWindow);
	glx.glXDestroyPbuffer = (GL_GLXFunctions::ptr_glXDestroyPbuffer) GL_LOAD_GLFUNC(glXDestroyPbuffer);
	glx.glXDestroyPixmap = (GL_GLXFunctions::ptr_glXDestroyPixmap) GL_LOAD_GLFUNC(glXDestroyPixmap);
	glx.glXDestroyWindow = (GL_GLXFunctions::ptr_glXDestroyWindow) GL_LOAD_GLFUNC(glXDestroyWindow);
	glx.glXGetCurrentReadDrawable = (GL_GLXFunctions::ptr_glXGetCurrentReadDrawable) GL_LOAD_GLFUNC(glXGetCurrentReadDrawable);
	glx.glXGetFBConfigAttrib = (GL_GLXFunctions::ptr_glXGetFBConfigAttrib) GL_LOAD_GLFUNC(glXGetFBConfigAttrib);
	glx.glXGetFBConfigs = (GL_GLXFunctions::ptr_glXGetFBConfigs) GL_LOAD_GLFUNC(glXGetFBConfigs);
	glx.glXGetSelectedEvent = (GL_GLXFunctions::ptr_glXGetSelectedEvent) GL_LOAD_GLFUNC(glXGetSelectedEvent);
	glx.glXGetVisualFromFBConfig = (GL_GLXFunctions::ptr_glXGetVisualFromFBConfig) GL_LOAD_GLFUNC(glXGetVisualFromFBConfig);
	glx.glXMakeContextCurrent = (GL_GLXFunctions::ptr_glXMakeContextCurrent) GL_LOAD_GLFUNC(glXMakeContextCurrent);
	glx.glXQueryContext = (GL_GLXFunctions::ptr_glXQueryContext) GL_LOAD_GLFUNC(glXQueryContext);
	glx.glXQueryDrawable = (GL_GLXFunctions::ptr_glXQueryDrawable) GL_LOAD_GLFUNC(glXQueryDrawable);
	glx.glXSelectEvent = (GL_GLXFunctions::ptr_glXSelectEvent) GL_LOAD_GLFUNC(glXSelectEvent);

	glx.glXGetProcAddressARB = (GL_GLXFunctions::ptr_glXGetProcAddressARB) GL_LOAD_GLFUNC(glXGetProcAddressARB);
	glx.glXGetProcAddress = (GL_GLXFunctions::ptr_glXGetProcAddress) GL_LOAD_GLFUNC(glXGetProcAddress);

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
		throw Exception("Cannot obtain required OpenGL GLX functions");
	}

	if ((glx.glXGetProcAddressARB == NULL) && (glx.glXGetProcAddress == NULL))
	{
		throw Exception("Cannot obtain required OpenGL GLX functions");
	}

	x11_window.func_on_resized().set(this, &GL3WindowProvider::on_window_resized);
}

GL3WindowProvider::~GL3WindowProvider()
{

	if (opengl_visual_info)
	{
		XFree(opengl_visual_info);
		opengl_visual_info = NULL;
	}

	if (opengl_context)
	{
		if (!gc.is_null())
		{
			GL3GraphicContextProvider *gl_provider = dynamic_cast<GL3GraphicContextProvider*>(gc.get_provider());
			if (gl_provider)
				gl_provider->dispose();
		}
	
		// Delete the context

		::Display *disp = x11_window.get_display();
		if (glx.glXGetCurrentContext() == opengl_context)
		{
			OpenGL::set_active(NULL);
		}

		if (disp)
		{
			glx.glXDestroyContext(disp, opengl_context);
		}

		opengl_context = 0;
	}

}

/////////////////////////////////////////////////////////////////////////////
// GL3WindowProvider Attributes:


/////////////////////////////////////////////////////////////////////////////
// GL3WindowProvider Operations:


ProcAddress *GL3WindowProvider::get_proc_address(const std::string& function_name) const
{
	if (glx.glXGetProcAddressARB)
		return glx.glXGetProcAddressARB((GLubyte*)function_name.c_str());
	if (glx.glXGetProcAddress)
		return glx.glXGetProcAddress((GLubyte*)function_name.c_str());
	return NULL;
}

void GL3WindowProvider::make_current() const
{
	// *** Note, If glxMakeCurrent crashes KDE when using a nvidia graphics card, then
	// update the driver from nvidia.com ***
	glx.glXMakeCurrent(x11_window.get_display(), x11_window.get_window(), opengl_context);

}

void GL3WindowProvider::create(DisplayWindowSite *new_site, const DisplayWindowDescription &desc)
{
	site = new_site;
	bool create_provider_flag = false;

	::Display *disp = x11_window.get_display();

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
		gc = GraphicContext(new GL3GraphicContextProvider(this));
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

void GL3WindowProvider::create_glx_1_3(DisplayWindowSite *new_site, const DisplayWindowDescription &desc, ::Display *disp)
{
	if (glx.glXChooseFBConfig == NULL)
		throw Exception("Cannot find the glXChooseFBConfig function");

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

#ifdef HAVE_X11_EXTENSIONS_XRENDER_H
	bool use_layered = desc.is_layered();
	if (use_layered)
	{
		int render_event;
		int render_error;

		if (!XRenderQueryExtension(disp, &render_event, &render_error))
		{
			use_layered = false;
		}
		if (use_layered)
		{
			x11_window.func_on_clicked().set(this, &GL3WindowProvider::on_clicked);
		}
	}
#endif

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
	gl_attribs.push_back(4);
	gl_attribs.push_back(GLX_GREEN_SIZE);
	gl_attribs.push_back(4);
	gl_attribs.push_back(GLX_BLUE_SIZE);
	gl_attribs.push_back(4);
	gl_attribs.push_back(GLX_ALPHA_SIZE);
	gl_attribs.push_back(4);
	gl_attribs.push_back(GLX_DEPTH_SIZE);
	gl_attribs.push_back(desc.get_depth_size());
	gl_attribs.push_back(GLX_STENCIL_SIZE);
	gl_attribs.push_back(desc.get_stencil_size());
	gl_attribs.push_back(GLX_DOUBLEBUFFER);
	gl_attribs.push_back(True );
	gl_attribs.push_back(GLX_STEREO);
	gl_attribs.push_back(False);
	gl_attribs.push_back(None);

	// get an appropriate visual
	int fb_count;
	GLXFBConfig *fbc = glx.glXChooseFBConfig( disp, DefaultScreen(disp), &gl_attribs[0], &fb_count );
	if (!fbc)
	{
		printf("Requested visual not supported by your OpenGL implementation. Falling back on singlebuffered Visual!\n");
		fbc = glx.glXChooseFBConfig( disp, DefaultScreen(disp), gl_attribs_single, &fb_count );
		if (!fbc)
			throw Exception(" glxChooseFBConfig failed");
		fbconfig = fbc[0];
	}
	else
	{
		if (!glx.glXGetFBConfigAttrib)
			throw Exception("Cannot find function glXGetFBConfigAttrib");

		bool got_desired_config = false;
		int desired_config = 0;
		int max_sample_buffers = 0;
		int max_samples = 0;
		int required_samples = desc.get_multisampling();
		// Find the best fitting multisampling option
		for (int i=0; i<fb_count; i++)
		{
#ifdef HAVE_X11_EXTENSIONS_XRENDER_H
			if (use_layered)
			{
				// Only use visuals that contain an alpha mask.

				XVisualInfo* visual_info = glx.glXGetVisualFromFBConfig(disp, fbc[i]);
				if (!visual_info)
					continue;

				XRenderPictFormat *format = XRenderFindVisualFormat(disp, visual_info->visual);
				XFree(visual_info);
				if (!format)
					continue;
				if (format->direct.alphaMask < 255)
				{
					continue;
				}
				if (!got_desired_config)
				{
					desired_config = i;
					got_desired_config = true;
				}
			}
#endif


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
					got_desired_config = true;
				}
			}

			// Use the maximum sample buffer
			if (max_samples == samples)	// Only check if the sample is valid
			{
				if (max_sample_buffers < samp_buf)
				{
					max_sample_buffers = samp_buf;
					desired_config = i;
					got_desired_config = true;
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
		throw Exception("glXGetVisualFromFBConfig failed");
	}

	// create a GLX context
	opengl_context = create_context(desc);
}


void GL3WindowProvider::create_glx_1_2(DisplayWindowSite *new_site, const DisplayWindowDescription &desc, ::Display *disp)
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

	int gl_attribs[32];	// WARNING - create() assumes this is 32 in size

	// Note: gl_attribs[32] !!!!
	gl_attribs[i++] = GLX_RGBA;
	gl_attribs[i++] = GLX_DOUBLEBUFFER;
	gl_attribs[i++] = GLX_BUFFER_SIZE;
	gl_attribs[i++] = 24;
	gl_attribs[i++] = GLX_RED_SIZE; 
	gl_attribs[i++] = 4;
	gl_attribs[i++] = GLX_GREEN_SIZE;
	gl_attribs[i++] = 4;
	gl_attribs[i++] = GLX_BLUE_SIZE;
	gl_attribs[i++] = 4;
	gl_attribs[i++] = GLX_DEPTH_SIZE;
	gl_attribs[i++] = desc.get_depth_size();
	gl_attribs[i++] = GLX_STENCIL_SIZE;
	gl_attribs[i++] = desc.get_stencil_size();
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
			throw Exception("glxChooseVisual failed");
		}
	}

	// create a GLX context
	opengl_context = create_context(desc);

}

void GL3WindowProvider::on_window_resized()
{
	if (gc.get_provider())
		((GL3GraphicContextProvider *) gc.get_provider())->on_window_resized();
}

bool GL3WindowProvider::is_glx_extension_supported(const char *ext_name)
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

void GL3WindowProvider::setup_swap_interval_pointers()
{
	glXSwapIntervalSGI = (ptr_glXSwapIntervalSGI) OpenGL::get_proc_address("glXSwapIntervalSGI");
	glXSwapIntervalMESA = (ptr_glXSwapIntervalMESA) OpenGL::get_proc_address("glXSwapIntervalMESA");

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
static int cl_ctxErrorHandler( ::Display *dpy, XErrorEvent *ev )
{
    cl_ctxErrorOccurred = true;
    return 0;
}

GLXContext GL3WindowProvider::create_context(const DisplayWindowDescription &desc)
{
	GLXContext shared_context = NULL;

	std::unique_ptr<MutexSection> mutex_section;
	GraphicContextProvider* gc_providers = SharedGCData::get_provider(mutex_section);
	if (gc_providers)
	{
		GL3GraphicContextProvider *gl_provider = dynamic_cast<GL3GraphicContextProvider*>(gc_providers);
		if (gl_provider)
		{
			const DisplayWindowProvider *rwp = &gl_provider->get_render_window();
			const GL3WindowProvider *render_window_glx = dynamic_cast<const GL3WindowProvider*>(rwp);
			if (render_window_glx)
				shared_context = render_window_glx->opengl_context;
		}
	}

	GLXContext context;

	if (glx_1_3)
	{
		context = create_context_glx_1_3(desc, shared_context);
	}
	else
	{
		context = create_context_glx_1_2(desc, shared_context);
	}

	return context;
}

GLXContext GL3WindowProvider::create_context_glx_1_3_helper(GLXContext shared_context, int major_version, int minor_version, const DisplayWindowDescription &desc, ptr_glXCreateContextAttribs glXCreateContextAttribs)
{
	std::vector<int> int_attributes;

	int_attributes.push_back(0x2091);	// GLX_CONTEXT_MAJOR_VERSION_ARB
	int_attributes.push_back(major_version);
	int_attributes.push_back(0x2092);	// GLX_CONTEXT_MINOR_VERSION_ARB
	int_attributes.push_back(minor_version);

	// Layer plane does not exist on GLX - http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
	//int_attributes.push_back(0x2093);	// GLX_CONTEXT_LAYER_PLANE_ARB
	//int_attributes.push_back(opengl_desc.get_layer_plane());

	int_attributes.push_back(0x2094);	// GLX_CONTEXT_FLAGS_ARB
	int flags = 0;
	if (opengl_desc.get_debug())
		flags |= 0x1;	// GLX_CONTEXT_DEBUG_BIT_ARB
	if (opengl_desc.get_forward_compatible())	// GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
		flags |= 0x2;	
	int_attributes.push_back(flags);

	int_attributes.push_back(0x9126);	// GLX_CONTEXT_PROFILE_MASK_ARB
	flags = 0;
	if (opengl_desc.get_core_profile())
		flags |= 0x1;	// GLX_CONTEXT_CORE_PROFILE_BIT_ARB
	if (opengl_desc.get_compatibility_profile())	// GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
		flags |= 0x2;	
	int_attributes.push_back(flags);

	int_attributes.push_back(None);
	
	cl_ctxErrorOccurred = false;

	GLXContext context_gl3 = glXCreateContextAttribs(x11_window.get_display(), fbconfig, shared_context, True, &int_attributes[0]);

	if (cl_ctxErrorOccurred)
	{
		if (context_gl3)
		{
			glx.glXDestroyContext(x11_window.get_display(), context_gl3);
			context_gl3 = 0;
		}
	}
	return context_gl3;
}

GLXContext GL3WindowProvider::create_context_glx_1_3(const DisplayWindowDescription &desc, GLXContext shared_context)
{
	GLXContext context;

	context = glx.glXCreateNewContext(x11_window.get_display(), fbconfig, GLX_RGBA_TYPE, shared_context, True);
	if(context == NULL)
		throw Exception("glXCreateContext failed");
	
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
		int (*oldHandler)(::Display*, XErrorEvent*) = XSetErrorHandler(&cl_ctxErrorHandler);
	  
		GLXContext context_gl3 = 0;

		int gl_major = opengl_desc.get_version_major();
		int gl_minor = opengl_desc.get_version_minor();
		if (opengl_desc.get_allow_lower_versions() == false)
		{
			context_gl3 = create_context_glx_1_3_helper(shared_context, gl_major, gl_minor, desc, glXCreateContextAttribs);
	
			if (!context_gl3)
				throw Exception(string_format("This application requires OpenGL %1.%2 or above. Try updating your drivers, or upgrade to a newer graphics card.",  gl_major, gl_minor));
		}
		else
		{
			static const char opengl_version_list[] = 
			{
				// Clanlib supported version pairs
				4,3,
				4,2,
				4,1,
				4,0,
				3,3,
				3,2,
				3,1,
				3,0,
				0,0,	// End of list
			};

			const char *opengl_version_list_ptr = opengl_version_list;
			do
			{
				int major = *(opengl_version_list_ptr++);
				if (major == 0)
					break;
					
				int minor = *(opengl_version_list_ptr++);

				// Find the appropriate version in the list
				if (major > gl_major)
					continue;

				if (major == gl_major)
				{
					if (minor > gl_minor)
						continue;	
				}

				context_gl3 = create_context_glx_1_3_helper(shared_context, gl_major, gl_minor, desc, glXCreateContextAttribs);

			}while(!context_gl3);
		}
		
		// Restore the original error handler
		XSetErrorHandler( oldHandler );		
		
		if (context_gl3)
		{
			glx.glXDestroyContext(x11_window.get_display(), context);
			context = context_gl3;
		}
	}
	return context;
}

GLXContext GL3WindowProvider::create_context_glx_1_2(const DisplayWindowDescription &desc, GLXContext shared_context)
{
	if (opengl_desc.get_allow_lower_versions() == false)
		throw Exception("GLX 1.2 does not support opengl version selection.");

	GLXContext context;
	context = glx.glXCreateContext(x11_window.get_display(), opengl_visual_info, shared_context, GL_TRUE);
	if(context == NULL)
		throw Exception("glXCreateContext failed");

	return context;
}

void GL3WindowProvider::flip(int interval)
{
	GraphicContext gc = get_gc();
	OpenGL::set_active(gc);
	glFlush();

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

void GL3WindowProvider::update(const Rect &_rect)
{
	int width = get_viewport().get_width();
	int height = get_viewport().get_height();

	Rect rect = _rect;
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

	OpenGL::set_active(gc);
	glFlush();

	GLboolean isDoubleBuffered = GL_TRUE;
	glGetBooleanv(GL_DOUBLEBUFFER, &isDoubleBuffered);
	if (isDoubleBuffered)
	{
		GLint read_last_bound;
		GLint draw_last_bound;

		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_last_bound);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_last_bound);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glReadBuffer(GL_BACK);
		glDrawBuffer(GL_FRONT);

		glBlitFramebuffer( 
			rect.left, height - rect.bottom,
			rect.right, height - rect.top,
			rect.left, height - rect.bottom,
			rect.right, height - rect.top,
			GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glDrawBuffer(GL_BACK);
		glReadBuffer(GL_FRONT);

		if (read_last_bound)
			glBindFramebuffer(GL_READ_FRAMEBUFFER, read_last_bound);

		if (draw_last_bound)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_last_bound);

		glFlush();


	}
}


CursorProvider *GL3WindowProvider::create_cursor(const CursorDescription &cursor_description, const Point &hotspot)
{
	return new CursorProvider_X11(cursor_description, hotspot);
}

void GL3WindowProvider::set_cursor(CursorProvider *cursor)
{
	x11_window.set_cursor(static_cast<CursorProvider_X11 *>(cursor));
}

void GL3WindowProvider::set_large_icon(const PixelBuffer &image)
{
	x11_window.set_large_icon(image);
}

void GL3WindowProvider::set_small_icon(const PixelBuffer &image)
{
	x11_window.set_small_icon(image);
}

void GL3WindowProvider::enable_alpha_channel(const Rect &blur_rect)
{
	// Implement me, if possible
}

void GL3WindowProvider::extend_frame_into_client_area(int height)
{
	// Implement me, if possible
}

/////////////////////////////////////////////////////////////////////////////
// GL3WindowProvider Implementation:


bool GL3WindowProvider::on_clicked(XButtonEvent &event)
{
	if (event.button != 1)	// Left mouse button
		return true;

	int height = get_viewport().get_height();

	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_FRONT);

	Rect rect = Rect(event.x,event.y, Size(1,1));

	PixelBuffer pixelbuffer(rect.get_width(), rect.get_height(), tf_rgba8);
	glReadPixels(
		rect.left, height - rect.bottom,
		rect.right - rect.left, rect.bottom - rect.top,
		GL_RGBA,
		GL_UNSIGNED_INT_8_8_8_8,
		pixelbuffer.get_data());

	const ubyte32 *xptr = (const ubyte32 *) (pixelbuffer.get_data());
	if (((*xptr) & 0xFF) < 10)
	{

		XLowerWindow(x11_window.get_display(), x11_window.get_window());
//		XWindowChanges wc;
//		wc.stack_mode = Below;
//		XConfigureWindow(x11_window.get_display(), x11_window.get_window(), CWStackMode, &wc);
		return false;
	}
	return true;
}

}




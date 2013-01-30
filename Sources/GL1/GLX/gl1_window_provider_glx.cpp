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

#include "GL1/precomp.h"
#include "gl1_window_provider_glx.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "GL1/opengl1.h"
#include "GL1/opengl1_wrap.h"
#include "API/GL1/opengl1_window_description.h"
#include "API/Core/Text/logger.h"
#include "Display/X11/cursor_provider_x11.h"
#include "Display/X11/display_message_queue_x11.h"
#include "../gl1_window_description_impl.h"
#include "../gl1_graphic_context_provider.h"
#include "../gl1_target_provider.h"
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GL1WindowProvider_GLX Construction:

GL1WindowProvider_GLX::GL1WindowProvider_GLX()
: x11_window(),
 opengl_context(0), opengl_visual_info(0), glXSwapIntervalSGI(NULL), glXSwapIntervalMESA(NULL), swap_interval(-1)
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
	glx.glXChooseVisual = (GL1_GLXFunctions::ptr_glXChooseVisual) GL_LOAD_GLFUNC(glXChooseVisual);
	glx.glXCopyContext = (GL1_GLXFunctions::ptr_glXCopyContext) GL_LOAD_GLFUNC(glXCopyContext);
	glx.glXCreateContext = (GL1_GLXFunctions::ptr_glXCreateContext) GL_LOAD_GLFUNC(glXCreateContext);
	glx.glXCreateGLXPixmap = (GL1_GLXFunctions::ptr_glXCreateGLXPixmap) GL_LOAD_GLFUNC(glXCreateGLXPixmap);
	glx.glXDestroyContext = (GL1_GLXFunctions::ptr_glXDestroyContext) GL_LOAD_GLFUNC(glXDestroyContext);
	glx.glXDestroyGLXPixmap = (GL1_GLXFunctions::ptr_glXDestroyGLXPixmap) GL_LOAD_GLFUNC(glXDestroyGLXPixmap);
	glx.glXGetConfig = (GL1_GLXFunctions::ptr_glXGetConfig) GL_LOAD_GLFUNC(glXGetConfig);
	glx.glXGetCurrentContext = (GL1_GLXFunctions::ptr_glXGetCurrentContext) GL_LOAD_GLFUNC(glXGetCurrentContext);
	glx.glXGetCurrentDrawable = (GL1_GLXFunctions::ptr_glXGetCurrentDrawable) GL_LOAD_GLFUNC(glXGetCurrentDrawable);
	glx.glXIsDirect = (GL1_GLXFunctions::ptr_glXIsDirect) GL_LOAD_GLFUNC(glXIsDirect);
	glx.glXMakeCurrent = (GL1_GLXFunctions::ptr_glXMakeCurrent) GL_LOAD_GLFUNC(glXMakeCurrent);
	glx.glXQueryExtension = (GL1_GLXFunctions::ptr_glXQueryExtension) GL_LOAD_GLFUNC(glXQueryExtension);
	glx.glXQueryVersion = (GL1_GLXFunctions::ptr_glXQueryVersion) GL_LOAD_GLFUNC(glXQueryVersion);
	glx.glXSwapBuffers = (GL1_GLXFunctions::ptr_glXSwapBuffers) GL_LOAD_GLFUNC(glXSwapBuffers);
	glx.glXUseXFont = (GL1_GLXFunctions::ptr_glXUseXFont) GL_LOAD_GLFUNC(glXUseXFont);
	glx.glXWaitGL = (GL1_GLXFunctions::ptr_glXWaitGL) GL_LOAD_GLFUNC(glXWaitGL);
	glx.glXWaitX = (GL1_GLXFunctions::ptr_glXWaitX) GL_LOAD_GLFUNC(glXWaitX);
	glx.glXGetClientString = (GL1_GLXFunctions::ptr_glXGetClientString) GL_LOAD_GLFUNC(glXGetClientString);
	glx.glXQueryServerString = (GL1_GLXFunctions::ptr_glXQueryServerString) GL_LOAD_GLFUNC(glXQueryServerString);
	glx.glXQueryExtensionsString = (GL1_GLXFunctions::ptr_glXQueryExtensionsString) GL_LOAD_GLFUNC(glXQueryExtensionsString);
	glx.glXGetCurrentDisplay = (GL1_GLXFunctions::ptr_glXGetCurrentDisplay) GL_LOAD_GLFUNC(glXGetCurrentDisplay);
	glx.glXChooseFBConfig = (GL1_GLXFunctions::ptr_glXChooseFBConfig) GL_LOAD_GLFUNC(glXChooseFBConfig);
	glx.glXCreateNewContext = (GL1_GLXFunctions::ptr_glXCreateNewContext) GL_LOAD_GLFUNC(glXCreateNewContext);
	glx.glXCreatePbuffer = (GL1_GLXFunctions::ptr_glXCreatePbuffer) GL_LOAD_GLFUNC(glXCreatePbuffer);
	glx.glXCreatePixmap = (GL1_GLXFunctions::ptr_glXCreatePixmap) GL_LOAD_GLFUNC(glXCreatePixmap);
	glx.glXCreateWindow = (GL1_GLXFunctions::ptr_glXCreateWindow) GL_LOAD_GLFUNC(glXCreateWindow);
	glx.glXDestroyPbuffer = (GL1_GLXFunctions::ptr_glXDestroyPbuffer) GL_LOAD_GLFUNC(glXDestroyPbuffer);
	glx.glXDestroyPixmap = (GL1_GLXFunctions::ptr_glXDestroyPixmap) GL_LOAD_GLFUNC(glXDestroyPixmap);
	glx.glXDestroyWindow = (GL1_GLXFunctions::ptr_glXDestroyWindow) GL_LOAD_GLFUNC(glXDestroyWindow);
	glx.glXGetCurrentReadDrawable = (GL1_GLXFunctions::ptr_glXGetCurrentReadDrawable) GL_LOAD_GLFUNC(glXGetCurrentReadDrawable);
	glx.glXGetFBConfigAttrib = (GL1_GLXFunctions::ptr_glXGetFBConfigAttrib) GL_LOAD_GLFUNC(glXGetFBConfigAttrib);
	glx.glXGetFBConfigs = (GL1_GLXFunctions::ptr_glXGetFBConfigs) GL_LOAD_GLFUNC(glXGetFBConfigs);
	glx.glXGetSelectedEvent = (GL1_GLXFunctions::ptr_glXGetSelectedEvent) GL_LOAD_GLFUNC(glXGetSelectedEvent);
	glx.glXGetVisualFromFBConfig = (GL1_GLXFunctions::ptr_glXGetVisualFromFBConfig) GL_LOAD_GLFUNC(glXGetVisualFromFBConfig);
	glx.glXMakeContextCurrent = (GL1_GLXFunctions::ptr_glXMakeContextCurrent) GL_LOAD_GLFUNC(glXMakeContextCurrent);
	glx.glXQueryContext = (GL1_GLXFunctions::ptr_glXQueryContext) GL_LOAD_GLFUNC(glXQueryContext);
	glx.glXQueryDrawable = (GL1_GLXFunctions::ptr_glXQueryDrawable) GL_LOAD_GLFUNC(glXQueryDrawable);
	glx.glXSelectEvent = (GL1_GLXFunctions::ptr_glXSelectEvent) GL_LOAD_GLFUNC(glXSelectEvent);

	glx.glXGetProcAddressARB = (GL1_GLXFunctions::ptr_glXGetProcAddressARB) GL_LOAD_GLFUNC(glXGetProcAddressARB);
	glx.glXGetProcAddress = (GL1_GLXFunctions::ptr_glXGetProcAddress) GL_LOAD_GLFUNC(glXGetProcAddress);

	glx.glXCreatePbufferSGIX = NULL;	// Setup later
	glx.glXDestroyPbufferSGIX = NULL;	// Setup later
	glx.glXChooseFBConfigSGIX = NULL;	// Setup later
	glx.glXGetVisualFromFBConfigSGIX = NULL;	// Setup later

	if ( (glx.glXDestroyContext == NULL) ||
		(glx.glXMakeCurrent == NULL) ||
		(glx.glXGetCurrentContext == NULL) ||
		(glx.glXChooseVisual == NULL) ||
		(glx.glXIsDirect == NULL) ||
		(glx.glXGetConfig == NULL) ||
		(glx.glXQueryExtensionsString == NULL) ||
		(glx.glXCreateContext == NULL) )
	{
		throw Exception("Cannot obtain required OpenGL GLX functions");
	}

	if ((glx.glXGetProcAddressARB == NULL) && (glx.glXGetProcAddress == NULL))
	{
		throw Exception("Cannot obtain required OpenGL GLX functions");
	}

	x11_window.func_on_resized().set(this, &GL1WindowProvider_GLX::on_window_resized);
}

GL1WindowProvider_GLX::~GL1WindowProvider_GLX()
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
			GL1GraphicContextProvider *gl_provider = dynamic_cast<GL1GraphicContextProvider*>(gc.get_provider());
			if (gl_provider)
				gl_provider->dispose();
		}

		// Delete the context

		::Display *disp = x11_window.get_display();
		if (glx.glXGetCurrentContext() == opengl_context)
		{
			GL1::set_active(NULL);
		}

		if (disp)
		{
			glx.glXDestroyContext(disp, opengl_context);
		}

		opengl_context = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// GL1WindowProvider_GLX Attributes:


/////////////////////////////////////////////////////////////////////////////
// GL1WindowProvider_GLX Operations:

GL1ProcAddress *GL1WindowProvider_GLX::get_proc_address(const std::string& function_name) const
{
	if (glx.glXGetProcAddressARB)
		return glx.glXGetProcAddressARB((GLubyte*)function_name.c_str());
	if (glx.glXGetProcAddress)
		return glx.glXGetProcAddress((GLubyte*)function_name.c_str());
	return NULL;
}

void GL1WindowProvider_GLX::make_current() const
{
	// *** Note, If glxMakeCurrent crashes KDE when using a nvidia graphics card, then
	// update the driver from nvidia.com ***
	glx.glXMakeCurrent(x11_window.get_display(), x11_window.get_window(), opengl_context);

}

void GL1WindowProvider_GLX::create(DisplayWindowSite *new_site, const DisplayWindowDescription &desc)
{
	site = new_site;
	bool create_provider_flag = false;

	::Display *disp = x11_window.get_display();

	if (!opengl_context)
	{

		int glx_major, glx_minor;
		if ( !glx.glXQueryVersion( disp, &glx_major, &glx_minor ) || 
			( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
		{
			glx.glx_1_3 = false;
		}
		else
		{
			glx.glx_1_3 = true;
		}

		create_provider_flag = true;
		// Setup OpenGL:
		int gl_attribs_single[] =
		{
			GLX_RGBA,
			GLX_DEPTH_SIZE, 16,
			GLX_STENCIL_SIZE, 8,
			GLX_BUFFER_SIZE, 24,
			None
		};

		int gl_attribs[32];
		int i = 0;

		GL1WindowDescription gl_desc(desc);
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
				throw Exception("glxChooseVisual failed");
			}
		}

		// create a GLX context
		opengl_context = create_context();

		if (!glx.glXIsDirect(disp, opengl_context))
			printf("No hardware acceleration available. I hope you got a really fast machine.\n");
	}

	x11_window.create(opengl_visual_info, site, desc);

	if (create_provider_flag)
	{
		gc = GraphicContext(new GL1GraphicContextProvider(this));
	}

	setup_extension_pointers();
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

void GL1WindowProvider_GLX::on_window_resized()
{
	if (gc.get_provider())
		((GL1GraphicContextProvider *) gc.get_provider())->on_window_resized();
}

bool GL1WindowProvider_GLX::is_glx_extension_supported(const char *ext_name)
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

void GL1WindowProvider_GLX::setup_extension_pointers()
{
	glXSwapIntervalSGI = (ptr_glXSwapIntervalSGI) GL1::get_proc_address("glXSwapIntervalSGI");
	glXSwapIntervalMESA = (ptr_glXSwapIntervalMESA) GL1::get_proc_address("glXSwapIntervalMESA");

	// See - http://dri.freedesktop.org/wiki/glXGetProcAddressNeverReturnsNULL ,get_proc_address() may return an invalid extension address
	if ( !is_glx_extension_supported("GLX_SGI_swap_control") )
	{
		glXSwapIntervalSGI = NULL;
	}

	if ( !is_glx_extension_supported("GLX_MESA_swap_control") )
	{
		glXSwapIntervalMESA = NULL;
	}


	glx.glXCreatePbufferSGIX = (GL1_GLXFunctions::ptr_glXCreatePbufferSGIX) GL1::get_proc_address("glXCreateGLXPbufferSGIX");
	glx.glXDestroyPbufferSGIX = (GL1_GLXFunctions::ptr_glXDestroyPbuffer) GL1::get_proc_address("glXDestroyGLXPbufferSGIX");
	glx.glXChooseFBConfigSGIX = (GL1_GLXFunctions::ptr_glXChooseFBConfig) GL1::get_proc_address("glXChooseFBConfigSGIX");
	glx.glXGetVisualFromFBConfigSGIX = (GL1_GLXFunctions::ptr_glXGetVisualFromFBConfig) GL1::get_proc_address("glXGetVisualFromFBConfigSGIX");
	if ( !is_glx_extension_supported("GLX_SGIX_pbuffer") )
	{
		glx.glXCreatePbufferSGIX = NULL;
		glx.glXDestroyPbufferSGIX = NULL;
	}
	if ( !is_glx_extension_supported("GLX_SGIX_fbconfig") )
	{
		glx.glXChooseFBConfigSGIX = NULL;
		glx.glXGetVisualFromFBConfigSGIX = NULL;
	}
}

GLXContext GL1WindowProvider_GLX::get_share_context()
{
	GLXContext shared_context = NULL;

	std::unique_ptr<MutexSection> mutex_section;
	GraphicContextProvider* gc_providers = SharedGCData::get_provider(mutex_section);
	if (gc_providers)
	{
		GL1GraphicContextProvider *gl_provider = dynamic_cast<GL1GraphicContextProvider*>(gc_providers);
		if (gl_provider)
		{
			const DisplayWindowProvider *rwp = &gl_provider->get_render_window();
			const GL1WindowProvider_GLX *render_window_glx = dynamic_cast<const GL1WindowProvider_GLX*>(rwp);
			if (render_window_glx)
				shared_context = render_window_glx->opengl_context;
		}
	}
	return shared_context;
}

GLXContext GL1WindowProvider_GLX::create_context()
{
	GLXContext shared_context = get_share_context();

	GLXContext context;
	context = glx.glXCreateContext(x11_window.get_display(), opengl_visual_info, shared_context, GL_TRUE);
	if(context == NULL)
		throw Exception("glXCreateContext failed");

	return context;
}

void GL1WindowProvider_GLX::flip(int interval)
{
	GraphicContext gc = get_gc();
	GL1::set_active(gc);

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

void GL1WindowProvider_GLX::update(const Rect &_rect)
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

	GL1::set_active(gc);

	GLint old_viewport[4], old_matrix_mode;
	GLdouble old_matrix_projection[16], old_matrix_modelview[16];
	cl1GetIntegerv(GL_VIEWPORT, old_viewport);
	cl1GetIntegerv(GL_MATRIX_MODE, &old_matrix_mode);
	cl1GetDoublev(GL_PROJECTION_MATRIX, old_matrix_projection);
	cl1GetDoublev(GL_MODELVIEW_MATRIX, old_matrix_modelview);

	cl1Viewport(0, 0, width, height);
	cl1MatrixMode(GL_PROJECTION);
	cl1LoadIdentity();
	cl1MultMatrixf(Mat4f::ortho_2d(0.0, width, 0.0, height, handed_right, clip_negative_positive_w));
	cl1MatrixMode(GL_MODELVIEW);
	cl1LoadIdentity();

	GLboolean isDoubleBuffered = GL_TRUE;
	cl1GetBooleanv(GL_DOUBLEBUFFER, &isDoubleBuffered);
	if (isDoubleBuffered)
	{
		cl1ReadBuffer(GL_BACK);
		cl1DrawBuffer(GL_FRONT);

		cl1RasterPos2i(rect.left, height - rect.bottom);

		cl1PixelZoom(1.0f, 1.0f);

		cl1CopyPixels(	rect.left, height - rect.bottom,
				rect.right - rect.left, rect.bottom - rect.top,
				GL_COLOR);

		cl1DrawBuffer(GL_BACK);
		cl1Flush();
	}

	cl1Viewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
	cl1MatrixMode(GL_PROJECTION);
	cl1LoadMatrixd(old_matrix_projection);
	cl1MatrixMode(GL_MODELVIEW);
	cl1LoadMatrixd(old_matrix_modelview);
	cl1MatrixMode(old_matrix_mode);
}


CursorProvider *GL1WindowProvider_GLX::create_cursor(const CursorDescription &cursor_description, const Point &hotspot)
{
	return new CursorProvider_X11(cursor_description, hotspot);
}

void GL1WindowProvider_GLX::set_cursor(CursorProvider *cursor)
{
	x11_window.set_cursor(static_cast<CursorProvider_X11 *>(cursor));
}

void GL1WindowProvider_GLX::set_large_icon(const PixelBuffer &image)
{
	x11_window.set_large_icon(image);
}

void GL1WindowProvider_GLX::set_small_icon(const PixelBuffer &image)
{
	x11_window.set_small_icon(image);
}

void GL1WindowProvider_GLX::enable_alpha_channel(const Rect &blur_rect)
{
	// Implement me, if possible
}

void GL1WindowProvider_GLX::extend_frame_into_client_area(int height)
{
	// Implement me, if possible
}

/////////////////////////////////////////////////////////////////////////////
// GL1WindowProvider_GLX Implementation:

}


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
**    Mark Page
**    Kevin J Bluck
*/

#include "GL1/precomp.h"
#include "gl1_window_provider_glx.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/display_window_message.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/TargetProviders/render_window_provider.h"
#include "API/GL1/opengl1.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/GL1/opengl1_window_description.h"
#include "API/Core/Text/logger.h"
#include "Display/X11/cursor_provider_x11.h"
#include "../gl1_window_description_impl.h"
#include "../gl1_graphic_context_provider.h"
#include "../gl1_target_provider.h"
#include <cstdio>
#include "pbuffer_impl.h"

CL_RenderWindowProvider_GLX::CL_RenderWindowProvider_GLX(CL_GL1WindowProvider_GLX & window, GLXContext glx_context)
	: window(window), glx_context(glx_context)
{
}

CL_RenderWindowProvider_GLX::~CL_RenderWindowProvider_GLX()
{
}

int CL_RenderWindowProvider_GLX::get_viewport_width() const
{
	return window.get_viewport().get_width();
}

int CL_RenderWindowProvider_GLX::get_viewport_height() const
{
	return window.get_viewport().get_height();
}

void CL_RenderWindowProvider_GLX::flip_buffers(int interval) const
{
	window.flip(interval);
}

void CL_RenderWindowProvider_GLX::make_current() const
{
	// *** Note, If glxMakeCurrent crashes KDE when using a nvidia graphics card, then
	// update the driver from nvidia.com ***
	glXMakeCurrent(window.get_display(), window.get_window(),
	               glx_context ? glx_context : window.get_opengl_context());
}

const CL_RenderWindowProvider * CL_RenderWindowProvider_GLX::new_worker_context() const
{
	return new CL_RenderWindowProvider_GLX(window, window.create_context());
}

CL_GL1WindowProvider_GLX &CL_RenderWindowProvider_GLX::get_window()
{
	return window;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_GLX Construction:

CL_GL1WindowProvider_GLX::CL_GL1WindowProvider_GLX()
: x11_window(&CL_GL1TargetProvider::message_queue),
 opengl_context(0), opengl_visual_info(0), glXSwapIntervalSGI(NULL), glXSwapIntervalMESA(NULL), last_set_interval(-1)
{
	x11_window.func_on_resized().set(this, &CL_GL1WindowProvider_GLX::on_window_resized);
}

CL_GL1WindowProvider_GLX::~CL_GL1WindowProvider_GLX()
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
		if (glXGetCurrentContext() == opengl_context)
		{
			CL_GL1::set_active(NULL);
		}

		if (disp)
		{
			glXDestroyContext(disp, opengl_context);
		}

		opengl_context = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_GLX Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_GLX Operations:

void CL_GL1WindowProvider_GLX::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc)
{
	site = new_site;
	bool create_provider_flag = false;

	x11_window.open_screen();
	Display *disp = x11_window.get_display();

	if (!opengl_context)
	{
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

		CL_GL1WindowDescription gl_desc(desc);
//		if( gl_desc )	
//		{
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
/*		}
		else
		{
			gl_attribs[i++] = GLX_RGBA;
			gl_attribs[i++] = GLX_DOUBLEBUFFER;
			gl_attribs[i++] = GLX_DEPTH_SIZE;
			gl_attribs[i++] = 16;
			gl_attribs[i++] = GLX_STENCIL_SIZE;
			gl_attribs[i++] = 8;
			gl_attribs[i++] = GLX_BUFFER_SIZE;
			gl_attribs[i++] = 24;
			gl_attribs[i++] = None;
		}*/

		// get an appropriate visual
		if (opengl_visual_info) XFree(opengl_visual_info);
		opengl_visual_info = glXChooseVisual(disp, DefaultScreen(disp), gl_attribs);
	
		if (opengl_visual_info == NULL)
		{
			opengl_visual_info = glXChooseVisual(disp, DefaultScreen(disp), gl_attribs_single);
			printf("Requested visual not supported by your OpenGL implementation. Falling back on singlebuffered Visual!\n");
			if (opengl_visual_info == NULL)
			{
				throw CL_Exception("glxChooseVisual failed");
			}
		}

		// create a GLX context
		opengl_context = create_context();

		if (!glXIsDirect(disp, opengl_context))
			printf("No hardware acceleration available. I hope you got a really fast machine.\n");

		last_set_interval = -1;

	}

	int screen_bpp = 0;
	glXGetConfig(disp, opengl_visual_info, GLX_BUFFER_SIZE, &screen_bpp);

	x11_window.create(opengl_visual_info, screen_bpp, site, desc);

	if (create_provider_flag)
	{
		gc = CL_GraphicContext(new CL_GL1GraphicContextProvider(new CL_RenderWindowProvider_GLX(*this, opengl_context)));
		std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
		gc_providers.push_back(gc.get_provider());
		setup_swap_interval_pointers();
	}
}

void CL_GL1WindowProvider_GLX::on_window_resized()
{
	if (gc.get_provider())
		((CL_GL1GraphicContextProvider *) gc.get_provider())->on_window_resized();
}

bool CL_GL1WindowProvider_GLX::is_glx_extension_supported(const char *ext_name)
{
	const char *ext_string = glXQueryExtensionsString(x11_window.get_display(), opengl_visual_info->screen);
	if (ext_string)
	{
		if (strstr(ext_string, ext_name))
		{
			return true;
		}
	}
	return false;
}

void CL_GL1WindowProvider_GLX::setup_swap_interval_pointers()
{
	glXSwapIntervalSGI = (ptr_glXSwapIntervalSGI) CL_GL1::get_proc_address("glXSwapIntervalSGI");
	glXSwapIntervalMESA = (ptr_glXSwapIntervalMESA) CL_GL1::get_proc_address("glXSwapIntervalMESA");

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

GLXContext CL_GL1WindowProvider_GLX::get_share_context()
{
	GLXContext shared_context = NULL;

	std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
	if (!gc_providers.empty())
	{
		CL_GL1GraphicContextProvider *gl_provider = dynamic_cast<CL_GL1GraphicContextProvider*>(gc_providers.back());
		if (gl_provider)
		{
			const CL_RenderWindowProvider *rwp = &gl_provider->get_render_window();
			const CL_RenderWindowProvider_GLX *render_window_glx = dynamic_cast<const CL_RenderWindowProvider_GLX*>(rwp);
			if (render_window_glx)
				shared_context = render_window_glx->get_context();
		}
	}
	return shared_context;
}

GLXContext CL_GL1WindowProvider_GLX::create_context()
{
	GLXContext shared_context = get_share_context();

	GLXContext context;
	context = glXCreateContext(x11_window.get_display(), opengl_visual_info, shared_context, GL_TRUE);
	if(context == NULL)
		throw CL_Exception("glXCreateContext failed");

	return context;
}

void CL_GL1WindowProvider_GLX::flip(int interval)
{
	CL_GraphicContext gc = get_gc();
	CL_GL1::set_active(gc);

	if (interval != -1)
	{
		if (last_set_interval != interval)
		{
			last_set_interval = interval;
			if (glXSwapIntervalSGI)
			{
				glXSwapIntervalSGI(interval);
			}else if (glXSwapIntervalMESA)
			{
				glXSwapIntervalMESA(interval);
			}
		}
	}

	glXSwapBuffers(x11_window.get_display(), x11_window.get_window());
}

void CL_GL1WindowProvider_GLX::update(const CL_Rect &_rect)
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

	CL_GL1::set_active(gc);

	CLint old_viewport[4], old_matrix_mode;
	CLdouble old_matrix_projection[16], old_matrix_modelview[16];
	cl1GetIntegerv(CL_VIEWPORT, old_viewport);
	cl1GetIntegerv(CL_MATRIX_MODE, &old_matrix_mode);
	cl1GetDoublev(CL_PROJECTION_MATRIX, old_matrix_projection);
	cl1GetDoublev(CL_MODELVIEW_MATRIX, old_matrix_modelview);

	cl1Viewport(0, 0, width, height);
	cl1MatrixMode(CL_PROJECTION);
	cl1LoadIdentity();
	cl1MultMatrixf(CL_Mat4f::ortho_2d(0.0, width, 0.0, height));
	cl1MatrixMode(CL_MODELVIEW);
	cl1LoadIdentity();
	cl1Translated(cl_pixelcenter_constant, cl_pixelcenter_constant, 0.0);

	CLboolean isDoubleBuffered = CL_TRUE;
	cl1GetBooleanv(CL_DOUBLEBUFFER, &isDoubleBuffered);
	if (isDoubleBuffered)
	{
		cl1ReadBuffer(CL_BACK);
		cl1DrawBuffer(CL_FRONT);

		cl1RasterPos2i(rect.left, height - rect.bottom);

		cl1PixelZoom(1.0f, 1.0f);

		cl1CopyPixels(	rect.left, height - rect.bottom,
				rect.right - rect.left, rect.bottom - rect.top,
				CL_COLOR);

		cl1DrawBuffer(CL_BACK);
		cl1Flush();
	}

	cl1Viewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
	cl1MatrixMode(CL_PROJECTION);
	cl1LoadMatrixd(old_matrix_projection);
	cl1MatrixMode(CL_MODELVIEW);
	cl1LoadMatrixd(old_matrix_modelview);
	cl1MatrixMode(old_matrix_mode);
}


CL_CursorProvider *CL_GL1WindowProvider_GLX::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
	return new CL_CursorProvider_X11(sprite_description, hotspot);
}

void CL_GL1WindowProvider_GLX::set_cursor(CL_CursorProvider *cursor)
{
	x11_window.set_cursor(static_cast<CL_CursorProvider_X11 *>(cursor));
}

CL_PBuffer_GL1 CL_GL1WindowProvider_GLX::create_pbuffer(CL_GL1GraphicContextProvider *gc_provider, CL_Size size)
{
	CL_PBuffer_GL1 pbuffer(gc_provider);

	pbuffer.create(*this, size);

	return pbuffer;
}

void CL_GL1WindowProvider_GLX::set_large_icon(const CL_PixelBuffer &image)
{
	x11_window.set_large_icon(image);
}

void CL_GL1WindowProvider_GLX::set_small_icon(const CL_PixelBuffer &image)
{
	x11_window.set_small_icon(image);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_GLX Implementation:



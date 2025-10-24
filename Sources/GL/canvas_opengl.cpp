/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "canvas_opengl.h"
#include "API/Core/System/error.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Display/graphic_context.h"
#include "API/GL/opengl_state.h"
#include "API/GL/opengl_wrap.h"
#ifdef WIN32
#include "WGL/display_window_opengl.h"
#else
#ifdef __APPLE__
#include "AGL/display_window_opengl.h"
#else
#include "GLX/display_window_opengl.h"
#endif
#endif
#include "graphic_context_opengl.h"
#include "surface_target_opengl.h"
#include <cstring>
#ifndef WIN32
#include <cstdio>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Canvas_OpenGL construction:

CL_Canvas_OpenGL::CL_Canvas_OpenGL()
{
	CL_OpenGLState state(CL_Display::get_current_window()->get_gc());
	state.set_active();

	selected_surface = 0;
	gc = 0;
	pbuffer = 0;
	pbuffer_context = 0;

#ifdef WIN32
	pbuffer_dc = 0;

	if (wglCreatePbufferARB == 0)
	{
		wglCreatePbufferARB = (ptr_wglCreatePbufferARB) wglGetProcAddress("wglCreatePbufferARB");
		wglCreatePbufferARB = (ptr_wglCreatePbufferARB) wglGetProcAddress("wglCreatePbufferARB");
		wglGetPbufferDCARB = (ptr_wglGetPbufferDCARB) wglGetProcAddress("wglGetPbufferDCARB");
		wglReleasePbufferDCARB = (ptr_wglReleasePbufferDCARB) wglGetProcAddress("wglReleasePbufferDCARB");
		wglDestroyPbufferARB = (ptr_wglDestroyPbufferARB) wglGetProcAddress("wglDestroyPbufferARB");
		wglQueryPbufferARB = (ptr_wglQueryPbufferARB) wglGetProcAddress("wglQueryPbufferARB");
	}
#else
#ifndef __APPLE__
	if (glXCreateGLXPbufferSGIX == 0)
	{
		glXCreateGLXPbufferSGIX = (ptr_glXCreateGLXPbufferSGIX) CL_OpenGL::get_proc_address("glXCreateGLXPbufferSGIX");
		glXDestroyGLXPbufferSGIX = (ptr_glXDestroyGLXPbufferSGIX) CL_OpenGL::get_proc_address("glXDestroyGLXPbufferSGIX");
		glXQueryGLXPbufferSGIX = (ptr_glXQueryGLXPbufferSGIX) CL_OpenGL::get_proc_address("glXQueryGLXPbufferSGIX");
		glXSelectEventSGIX = (ptr_glXSelectEventSGIX) CL_OpenGL::get_proc_address("glXSelectEventSGIX");
		glXGetSelectedEventSGIX = (ptr_glXGetSelectedEventSGIX) CL_OpenGL::get_proc_address("glXGetSelectedEventSGIX");

		glXChooseFBConfigSGIX = (ptr_glXChooseFBConfigSGIX) CL_OpenGL::get_proc_address("glXChooseFBConfigSGIX");
		glXCreateContextWithConfigSGIX = (ptr_glXCreateContextWithConfigSGIX) CL_OpenGL::get_proc_address("glXCreateContextWithConfigSGIX");
	}
#endif
#endif

	pbuffer_changed = false;
	texture_changed = true;
}

CL_Canvas_OpenGL::~CL_Canvas_OpenGL()
{
	if (selected_surface)
	{
		sync_texture();
		selected_surface->selected_canvas = 0;
	}
	delete gc;

	CL_OpenGLState state(CL_Display::get_current_window()->get_gc());
	state.set_active();
#ifdef WIN32
	if (pbuffer_context) wglDeleteContext(pbuffer_context);
	if (pbuffer_dc) wglReleasePbufferDCARB(pbuffer, pbuffer_dc);
	if (pbuffer) wglDestroyPbufferARB(pbuffer);
#else
#ifdef __APPLE__
	if (pbuffer) aglDestroyPBuffer(pbuffer);
	if (pbuffer_context) aglDestroyContext(pbuffer_context);
#else
	if (pbuffer_context) glXDestroyContext(CL_DisplayWindow_OpenGL::get_display(), pbuffer_context);
	if (pbuffer) glXDestroyGLXPbufferSGIX(CL_DisplayWindow_OpenGL::get_display(), pbuffer);
#endif
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_Canvas_OpenGL attributes:

CL_GraphicContext *CL_Canvas_OpenGL::get_gc()
{
	if (selected_surface == 0) return 0;

	if (gc == 0)
	{
		// Create working pbuffer:
#ifdef WIN32
		if (wglCreatePbufferARB == 0)
		{
			throw CL_Error("WGL_ARB_pbuffer OpenGL extension not supported by this card");
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
			8,                              // no alpha buffer
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

//		int pixelformat = GetPixelFormat(wglGetCurrentDC());

		pbuffer = wglCreatePbufferARB(
			wglGetCurrentDC(),
			pixelformat,
			selected_surface->surface_size.width,
			selected_surface->surface_size.height,
			attribList);
		pbuffer_dc = wglGetPbufferDCARB(pbuffer);
		pbuffer_context = wglCreateContext(pbuffer_dc);

		if (!CL_DisplayWindow_OpenGL::opengl_contexts.empty())
		{
			wglShareLists(CL_DisplayWindow_OpenGL::opengl_contexts.back(), pbuffer_context);
		}
#else
#ifdef __APPLE__
		GLint attrib[] = { AGL_RGBA, AGL_NO_RECOVERY, AGL_DEPTH_SIZE, 24, AGL_CLOSEST_POLICY, AGL_NONE };
		AGLPixelFormat pixelformat = aglChoosePixelFormat(NULL, 0, attrib);
		if (AGL_NO_ERROR != aglGetError())
			throw CL_Error("Could not choose canvas pixel format: " + std::string((const char*)aglErrorString(aglGetError())));

		pbuffer_context = aglCreateContext(pixelformat, CL_DisplayWindow_OpenGL::share_context);
		if (AGL_NO_ERROR != aglGetError())
			throw CL_Error("Could not create canvas gl context: " + std::string((const char*)aglErrorString(aglGetError())));

		if (GL_FALSE==aglCreatePBuffer(
				selected_surface->surface_size.width,
				selected_surface->surface_size.height,
				GL_TEXTURE_RECTANGLE_EXT, //GL_TEXTURE_2D,
				GL_RGBA,
				0,
				&pbuffer)
			) throw CL_Error("Could not create canvas PBuffer: " + std::string((const char*)aglErrorString(aglGetError())));

		aglSetPBuffer(pbuffer_context, pbuffer, 0, 0, aglGetVirtualScreen(CL_DisplayWindow_OpenGL::share_context));
		if (AGL_NO_ERROR != aglGetError())
			throw CL_Error("Could not set up canvas PBuffer: " + std::string((const char*)aglErrorString(aglGetError())));
#else
   int scrnum;
   GLXFBConfig *fbconfig;
   XVisualInfo *visinfo;
   int nitems;

   int attrib[] = {
      GLX_DOUBLEBUFFER,  False,
      GLX_RED_SIZE,      1,
      GLX_GREEN_SIZE,    1,
      GLX_BLUE_SIZE,     1,
      GLX_DEPTH_SIZE,    1,
      GLX_RENDER_TYPE,   GLX_RGBA_BIT,
      GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT | GLX_WINDOW_BIT,
      None
   };
   int pbufAttrib[] = {
     GLX_PBUFFER_WIDTH,   selected_surface->surface_size.width,
     GLX_PBUFFER_HEIGHT,  selected_surface->surface_size.height,
     GLX_LARGEST_PBUFFER, False,
     None
   };


   scrnum   = DefaultScreen( CL_DisplayWindow_OpenGL::get_display() );

   fbconfig = glXChooseFBConfig(CL_DisplayWindow_OpenGL::get_display(),
                                scrnum,
                                attrib,
                                &nitems);
   if (NULL == fbconfig) {
      fprintf(stderr,"Error: couldn't get fbconfig\n");
      exit(1);
   }

   pbuffer = glXCreatePbuffer(CL_DisplayWindow_OpenGL::get_display(), fbconfig[0], pbufAttrib);

   visinfo = glXGetVisualFromFBConfig(CL_DisplayWindow_OpenGL::get_display(), fbconfig[0]);
   if (!visinfo) {
      fprintf(stderr, "Error: couldn't get an RGBA, double-buffered visual\n");
      exit(1);
   }

   pbuffer_context = glXCreateContext( CL_DisplayWindow_OpenGL::get_display(), visinfo, 
                                       dynamic_cast<CL_GraphicContext_OpenGL*>(CL_Display::get_current_window()->get_gc()->impl)->get_context(), GL_TRUE );
   if (!pbuffer_context) {
      fprintf(stderr, "Error: glXCreateContext failed\n");
      exit(1);
   }

   XFree(fbconfig);
   XFree(visinfo);

#endif
#endif

		// Create GC interface for it:
		gc = new CL_GraphicContext(new CL_GraphicContext_OpenGL(this));
		gc->set_cliprect(CL_Rect(0, 0, selected_surface->surface_size.width, selected_surface->surface_size.height));

		// Download surface to pbuffer:
		pbuffer_changed = false;
		texture_changed = true;
		sync_pbuffer();
	}

	return gc;
}

CL_PixelBuffer CL_Canvas_OpenGL::get_pixeldata(const CL_Rect &area)
{
	if (selected_surface)
		return selected_surface->get_pixeldata ();
	else
		return CL_PixelBuffer();
}

int CL_Canvas_OpenGL::get_width() const
{
	if (selected_surface) return selected_surface->get_width();
	return 0;
}
	
int CL_Canvas_OpenGL::get_height() const
{
	if (selected_surface) return selected_surface->get_height();
	return 0;
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Canvas_OpenGL operations:

void CL_Canvas_OpenGL::set_pixeldata(const CL_Point &dest, const CL_Rect &src, const CL_PixelBuffer &data)
{
}

void CL_Canvas_OpenGL::select_surface(CL_Surface_Generic *surface)
{
	if (selected_surface)
	{
		sync_texture();
		selected_surface->selected_canvas = 0;
	}
	if (surface == 0)
	{
		selected_surface = 0;
		pbuffer_changed = false;
		texture_changed = false;
	}
	else
	{
		selected_surface = (CL_Surface_Target_OpenGL *) surface->target;
		selected_surface->selected_canvas = this;
		pbuffer_changed = false;
		texture_changed = true;
		// alloc_pbuffer();
	}
}
	
void CL_Canvas_OpenGL::sync_surface()
{
	sync_texture();
}

void CL_Canvas_OpenGL::sync_pbuffer()
{
	if (texture_changed && gc != 0)
	{
		CL_OpenGLState state(gc);
		state.set_active();

		// Setup down/up 2d projection matrix:
		clMatrixMode(CL_PROJECTION);
		clLoadIdentity();

		CLdouble viewport[4];
		clGetDoublev(CL_VIEWPORT, viewport);

		CLdouble width = viewport[2];
		CLdouble height = viewport[3];
		gluOrtho2D(0.0, width, 0.0, height);

		clMatrixMode(CL_MODELVIEW);
		clLoadIdentity();

		// Copy texture to pbuffer:
		clEnable(CL_TEXTURE_2D);
		clBindTexture(CL_TEXTURE_2D, selected_surface->handle);
		clMatrixMode(CL_TEXTURE);
		CLdouble scale_matrix[16];
		memset(scale_matrix, 0, sizeof(CLdouble)*16);
		scale_matrix[0] = 1.0f/float(selected_surface->texture_size.width);
		scale_matrix[5] = 1.0f/float(selected_surface->texture_size.height);
		scale_matrix[10] = 1.0f;
		scale_matrix[15] = 1.0f;
		clLoadMatrixd(scale_matrix);
		clMatrixMode(CL_MODELVIEW);

		clBegin(CL_QUADS);
		clColor3f(1.0f, 1.0f, 1.0f);
		clTexCoord2f(float(selected_surface->surface_size.width), 0.0f);
		clVertex2f(float(selected_surface->surface_size.width), 0.0f);
		clTexCoord2f(float(selected_surface->surface_size.width), float(selected_surface->surface_size.height));
		clVertex2f(float(selected_surface->surface_size.width), float(selected_surface->surface_size.height));
		clTexCoord2f(0.0f, float(selected_surface->surface_size.height));
		clVertex2f(0.0f, float(selected_surface->surface_size.height));
		clTexCoord2f(0.0f, 0.0f);
		clVertex2f(0.0f, 0.0f);
		clEnd();
#ifdef __APPLE__
		clFlush();
#endif
	}
	texture_changed = false;
}

void CL_Canvas_OpenGL::sync_texture()
{
	if (pbuffer_changed && gc != 0)
	{
		CL_OpenGLState state(gc);
		state.set_active();

		// Copy pbuffer to texture:
		clEnable(CL_TEXTURE_2D);
		clBindTexture(CL_TEXTURE_2D, selected_surface->handle);
		clCopyTexSubImage2D(
			CL_TEXTURE_2D,
			0,
			0, 0,
			0, 0,
			selected_surface->surface_size.width, selected_surface->surface_size.height);
#ifdef __APPLE__
		clFlush();
#endif
	}
	pbuffer_changed = false;
}

void CL_Canvas_OpenGL::set_pbuffer_modified()
{
	pbuffer_changed = true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Canvas_OpenGL implementation:

#ifdef WIN32
CL_Canvas_OpenGL::ptr_wglCreatePbufferARB CL_Canvas_OpenGL::wglCreatePbufferARB = 0;

CL_Canvas_OpenGL::ptr_wglGetPbufferDCARB CL_Canvas_OpenGL::wglGetPbufferDCARB = 0;

CL_Canvas_OpenGL::ptr_wglReleasePbufferDCARB CL_Canvas_OpenGL::wglReleasePbufferDCARB = 0;

CL_Canvas_OpenGL::ptr_wglDestroyPbufferARB CL_Canvas_OpenGL::wglDestroyPbufferARB = 0;

CL_Canvas_OpenGL::ptr_wglQueryPbufferARB CL_Canvas_OpenGL::wglQueryPbufferARB = 0;
#else
#ifndef __APPLE__
CL_Canvas_OpenGL::ptr_glXCreateGLXPbufferSGIX CL_Canvas_OpenGL::glXCreateGLXPbufferSGIX = 0;

CL_Canvas_OpenGL::ptr_glXDestroyGLXPbufferSGIX CL_Canvas_OpenGL::glXDestroyGLXPbufferSGIX = 0;

CL_Canvas_OpenGL::ptr_glXQueryGLXPbufferSGIX CL_Canvas_OpenGL::glXQueryGLXPbufferSGIX = 0;

CL_Canvas_OpenGL::ptr_glXSelectEventSGIX CL_Canvas_OpenGL::glXSelectEventSGIX = 0;

CL_Canvas_OpenGL::ptr_glXGetSelectedEventSGIX CL_Canvas_OpenGL::glXGetSelectedEventSGIX = 0;

CL_Canvas_OpenGL::ptr_glXChooseFBConfigSGIX CL_Canvas_OpenGL::glXChooseFBConfigSGIX = 0;
	
CL_Canvas_OpenGL::ptr_glXCreateContextWithConfigSGIX CL_Canvas_OpenGL::glXCreateContextWithConfigSGIX = 0;
#endif
#endif

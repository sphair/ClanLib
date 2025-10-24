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

#ifndef header_canvas_opengl
#define header_canvas_opengl

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "Display/canvas_generic.h"

#ifdef WIN32
	#ifndef WGL_DRAW_TO_PBUFFER_ARB
		#define WGL_DRAW_TO_PBUFFER_ARB     0x202D
		#define WGL_MAX_PBUFFER_PIXELS_ARB  0x202E
		#define WGL_MAX_PBUFFER_WIDTH_ARB   0x202F
		#define WGL_MAX_PBUFFER_HEIGHT_ARB  0x2030
		#define WGL_PBUFFER_LARGEST_ARB     0x2033
		#define WGL_PBUFFER_WIDTH_ARB       0x2034
		#define WGL_PBUFFER_HEIGHT_ARB      0x2035
		#define WGL_PBUFFER_LOST_ARB        0x2036

	    DECLARE_HANDLE(HPBUFFERARB);
	#endif
#else
	#ifdef __APPLE__
		#include "AGL/display_window_opengl.h"
		#include <AGL/agl.h>
	#else
		#include "GLX/display_window_opengl.h"
		#ifndef GLX_MAX_PBUFFER_WIDTH_SGIX
			#define GLX_MAX_PBUFFER_WIDTH_SGIX  0x8016
			#define GLX_MAX_PBUFFER_HEIGHT_SGIX 0x8017
			#define GLX_MAX_PBUFFER_PIXELS_SGIX 0x8018
			#define GLX_PBUFFER_BIT_SGIX        0x00000004
			#define GLX_WIDTH_SGIX              0x801D
			#define GLX_HEIGHT_SGIX             0x801E
			#define GLX_EVENT_MASK_SGIX         0x801F
			#define GLX_DAMAGED_SGIX            0x8020
			#define GLX_SAVED_SGIX              0x8021
		#endif
		#ifndef GLX_DRAWABLE_TYPE_SGIX
			#define GLX_DRAWABLE_TYPE_SGIX      0x8010
			#define GLX_RENDER_TYPE_SGIX        0x8011
			#define GLX_X_RENDERABLE_SGIX       0x8012
			typedef __GLXFBConfigRec *  GLXFBConfigSGIX;
		#endif
	#endif
#endif

class CL_Surface_Target_OpenGL;

class CL_Canvas_OpenGL : public CL_Canvas_Generic
{
//! Construction:
public:
	CL_Canvas_OpenGL();

	virtual ~CL_Canvas_OpenGL();

//! Attributes:
public:
	virtual CL_GraphicContext *get_gc();

	virtual CL_PixelBuffer get_pixeldata(const CL_Rect &area);

	virtual int get_width() const;
	
	virtual int get_height() const;
	
#ifdef WIN32
	HPBUFFERARB get_pbuffer() { return pbuffer; }

	HGLRC get_pbuffer_context() { return pbuffer_context; }

	HDC get_pbuffer_dc() { return pbuffer_dc; }
#else
#ifdef __APPLE__
	AGLPbuffer get_pbuffer() { return pbuffer; }
	
	AGLContext get_pbuffer_context() { return pbuffer_context; }
#else
	GLXPbuffer get_pbuffer() { return pbuffer; }

	GLXContext get_pbuffer_context() { return pbuffer_context; }
#endif
#endif

//! Operations:
public:
	virtual void set_pixeldata(const CL_Point &dest, const CL_Rect &src, const CL_PixelBuffer &data);

	virtual void select_surface(CL_Surface_Generic *surface);
	
	virtual void sync_surface();

	void sync_pbuffer();

	void sync_texture();

	void set_pbuffer_modified();

//! Implementation:
private:
	CL_Surface_Target_OpenGL *selected_surface;

	CL_GraphicContext *gc;

	//: OpenGL ARB PBuffer sync flags:
	bool pbuffer_changed, texture_changed;

#ifdef WIN32
	HPBUFFERARB pbuffer;

	HGLRC pbuffer_context;

	HDC pbuffer_dc;
#else
#ifdef __APPLE__
	AGLPbuffer pbuffer;
	AGLContext pbuffer_context;
#else
	GLXPbuffer pbuffer;

	GLXContext pbuffer_context;
#endif
#endif

	// OpenGL pbuffer binding evilness:
#ifdef WIN32
	typedef HPBUFFERARB (APIENTRY *ptr_wglCreatePbufferARB)(
		HDC hDC,
		int iPixelFormat,
		int iWidth,
		int iHeight,
		const int *piAttribList);

	typedef HDC (APIENTRY *ptr_wglGetPbufferDCARB)(
		HPBUFFERARB hPbuffer);

	typedef int (APIENTRY *ptr_wglReleasePbufferDCARB)(
		HPBUFFERARB hPbuffer,
		HDC hDC);

	typedef BOOL (APIENTRY *ptr_wglDestroyPbufferARB)(
		HPBUFFERARB hPbuffer);

	typedef BOOL (APIENTRY *ptr_wglQueryPbufferARB)(
		HPBUFFERARB hPbuffer,
		int iAttribute,
		int *piValue);

	static ptr_wglCreatePbufferARB wglCreatePbufferARB;

	static ptr_wglGetPbufferDCARB wglGetPbufferDCARB;

	static ptr_wglReleasePbufferDCARB wglReleasePbufferDCARB;

	static ptr_wglDestroyPbufferARB wglDestroyPbufferARB;

	static ptr_wglQueryPbufferARB wglQueryPbufferARB;
#else
#ifndef __APPLE__
	typedef GLXPbuffer (*ptr_glXCreateGLXPbufferSGIX)(
		Display *dpy,
		GLXFBConfig config,
		unsigned int width,
		unsigned int height,
		int *attrib_list);

	typedef void (*ptr_glXDestroyGLXPbufferSGIX)(
		Display *dpy,
		GLXPbuffer pbuf);

	typedef void (*ptr_glXQueryGLXPbufferSGIX)(
		Display *dpy,
		GLXPbuffer pbuf,
		int attribute,
		unsigned int *value);

	typedef void (*ptr_glXSelectEventSGIX)(
		Display *dpy,
		GLXDrawable drawable,
		unsigned long mask);

	typedef void (*ptr_glXGetSelectedEventSGIX)(
		Display *dpy,
		GLXDrawable drawable,
		unsigned long *mask);

	typedef GLXFBConfigSGIX *(*ptr_glXChooseFBConfigSGIX)(
		Display *dpy,
		int screen,
		const int *attrib_list,
		int *nelements);

	typedef GLXContext (*ptr_glXCreateContextWithConfigSGIX)(
		Display *dpy,
		GLXFBConfig config,
		int render_type,
		GLXContext share_list,
		Bool direct);

	static ptr_glXCreateGLXPbufferSGIX glXCreateGLXPbufferSGIX;

	static ptr_glXDestroyGLXPbufferSGIX glXDestroyGLXPbufferSGIX;

	static ptr_glXQueryGLXPbufferSGIX glXQueryGLXPbufferSGIX;

	static ptr_glXSelectEventSGIX glXSelectEventSGIX;

	static ptr_glXGetSelectedEventSGIX glXGetSelectedEventSGIX;
	
	static ptr_glXChooseFBConfigSGIX glXChooseFBConfigSGIX;
	
	static ptr_glXCreateContextWithConfigSGIX glXCreateContextWithConfigSGIX;
#endif
#endif
};

#endif

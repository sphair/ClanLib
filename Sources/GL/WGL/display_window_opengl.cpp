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
#include "display_window_opengl.h"
#include "GL/graphic_context_opengl.h"
#include "GL/gl_state_checker.h"
#include "API/Display/display_window_description.h"
#include "API/Core/System/error.h"
#include "API/GL/opengl_state.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl_window_description.h"
#include "GL/opengl_window_description_generic.h"

#ifndef AC_SRC_ALPHA
#define AC_SRC_ALPHA 1
#endif

#ifndef ULW_OPAQUE
#define ULW_OPAQUE 0
#endif

#ifndef ULW_COLORKEY
#define ULW_COLORKEY 1
#endif

#ifndef ULW_ALPHA
#define ULW_ALPHA 2
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_DisplayWindow_OpenGL::CL_DisplayWindow_OpenGL()
{
	layered_dc = 0;
	layered_bitmap = 0;
	context = 0;
	hdc = 0;
	gc = CL_GraphicContext(new CL_GraphicContext_OpenGL(this));
	user32_dll = 0;
}

CL_DisplayWindow_OpenGL::~CL_DisplayWindow_OpenGL()
{
	destroy_window();
	if (layered_bitmap)
	{
		SelectObject(layered_dc, layered_orig_bitmap);
		DeleteObject(layered_bitmap);
	}
	if (layered_dc) DeleteDC(layered_dc);
	if (user32_dll) FreeLibrary(user32_dll);
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_GraphicContext *CL_DisplayWindow_OpenGL::get_gc()
{
	return &gc;
}

CL_PixelBuffer CL_DisplayWindow_OpenGL::get_buffer(int i) const
{
	if (i == 0) 
	{
		return gc.get_pixeldata(CL_Rect(0, 0, get_width(), get_height()), CL_FRONT);
	}
	else
	{
		return gc.get_pixeldata(CL_Rect(0, 0, get_width(), get_height()), CL_BACK);
	}
}

int CL_DisplayWindow_OpenGL::get_buffer_count() const
{
	return 2;
}

HDC CL_DisplayWindow_OpenGL::get_dc()
{
	if (layered_dc) return layered_dc;
	return hdc;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_DisplayWindow_OpenGL::create_window(const CL_DisplayWindowDescription &desc)
{
	if (desc.is_fullscreen())
	{
		set_videomode(desc.get_size().width, desc.get_size().height, desc.get_bpp(), desc.get_refresh_rate());
	}
	else
	{
		restore_videomode();
	}

	CL_DisplayWindow_Win32::create_window(desc);

	if (context) return; // OpenGL already setup for this window

	// Setup OpenGL:

	hdc = GetDC(get_hwnd());
	
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;

	const CL_OpenGLWindowDescription_Generic *gldesc = dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(desc.impl.get());
	if (gldesc)
	{
		pfd.iPixelType = (gldesc->rgba) ? PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
		if (gldesc->doublebuffer) pfd.dwFlags |= PFD_DOUBLEBUFFER;
		if (gldesc->stereo) pfd.dwFlags |= PFD_STEREO;
		pfd.cColorBits = gldesc->buffer_size;
		pfd.iLayerType = gldesc->level; // MSDN docs say this one is ignored! -- mbn 12. jan 2004
		pfd.cAuxBuffers = gldesc->aux_buffers;
		pfd.cRedBits = gldesc->red_size;
		pfd.cGreenBits = gldesc->green_size;
		pfd.cBlueBits = gldesc->blue_size;
		pfd.cAlphaBits = gldesc->alpha_size;
		pfd.cDepthBits = gldesc->depth_size;
		pfd.cStencilBits = gldesc->stencil_size;
		pfd.cAccumRedBits = gldesc->accum_red_size;
		pfd.cAccumGreenBits = gldesc->accum_green_size;
		pfd.cAccumBlueBits = gldesc->accum_blue_size;
		pfd.cAccumAlphaBits = gldesc->accum_alpha_size;
		// gldesc dont have an accum bits, so trying to calculate it:
		pfd.cAccumBits = (pfd.cAccumRedBits + pfd.cAccumGreenBits + pfd.cAccumBlueBits + pfd.cAccumAlphaBits + 7) / 8;
		pfd.cAccumBits *= 8;
		
	}
	else
	{
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.dwFlags |= PFD_DOUBLEBUFFER;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.cStencilBits = 8;
	}

	if (desc.is_layered())
	{
		pfd.cAlphaBits = 8;
		pfd.dwFlags |= PFD_DRAW_TO_BITMAP | PFD_DOUBLEBUFFER_DONTCARE;
	}

	if (desc.is_layered())
	{
		if (user32_dll == 0)
		{
			user32_dll = LoadLibrary("user32.dll");
			if (user32_dll == 0) throw CL_Error("Unable to load user32.dll!");

			ptr_UpdateLayeredWindow = (PtrUpdateLayeredWindow) GetProcAddress(user32_dll, "UpdateLayeredWindow");
			if (ptr_UpdateLayeredWindow == 0)
			{
				FreeLibrary(user32_dll);
				user32_dll = 0;
				throw CL_Error("Layered windows require Windows 2000 or Windows XP!");
			}
		}

		layered_dc = CreateCompatibleDC(hdc);
		cl_assert(layered_dc != 0);

		layered_bitmap = CreateCompatibleBitmap(hdc, desc.get_size().width, desc.get_size().height);
		cl_assert(layered_bitmap != 0);
		layered_orig_bitmap = (HBITMAP) SelectObject(layered_dc, layered_bitmap);
	}

	int pixelformat = ChoosePixelFormat(get_dc(), &pfd);
	SetPixelFormat(get_dc(), pixelformat, &pfd);

	context = wglCreateContext(get_dc());
	cl_assert(context != NULL);

	if (!opengl_contexts.empty()) wglShareLists(opengl_contexts.back(), context);
	opengl_contexts.push_back(context);

	ShowWindow(get_hwnd(), SW_SHOW);
	UpdateWindow(get_hwnd());
}

void CL_DisplayWindow_OpenGL::destroy_window()
{
	if (context)
	{
		wglMakeCurrent(0, 0);
		wglDeleteContext(context);
		opengl_contexts.remove(context);
		context = 0;
	}

	if (layered_bitmap)
	{
		SelectObject(layered_dc, layered_orig_bitmap);
		DeleteObject(layered_bitmap);

		layered_bitmap = 0;
	}

	if (layered_dc) DeleteDC(layered_dc);
	layered_dc = 0;

	if (is_fullscreen()) restore_videomode();
	CL_DisplayWindow_Win32::destroy_window();
}

bool CL_DisplayWindow_OpenGL::received_event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		if (context)
		{
			wglMakeCurrent(0, 0);
			wglDeleteContext(context);
			opengl_contexts.remove(context);
			context = 0;
		}
		break;
	}

	return CL_DisplayWindow_Win32::received_event(uMsg, wParam, lParam);
}

void CL_DisplayWindow_OpenGL::flip(int interval)
{
	CL_GLStateChecker::from_gc(get_gc())->flush();
	CL_DisplayWindow_Win32::flip(interval);

	if (interval != -1)
	{
		typedef BOOL (APIENTRY *ptr_wglSwapIntervalEXT)(int interval);
		ptr_wglSwapIntervalEXT wglSwapIntervalEXT = (ptr_wglSwapIntervalEXT) wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapIntervalEXT)
		{
			wglSwapIntervalEXT(interval);
		}
	}

	BOOL retval = SwapBuffers(get_dc());

	repaint_layered();
}

void CL_DisplayWindow_OpenGL::update(const CL_Rect &rect)
{
	CL_OpenGLState state(get_gc());
	state.set_active();
	state.setup_2d(true);

	CLboolean isDoubleBuffered = CL_TRUE;
	clGetBooleanv(CL_DOUBLEBUFFER, &isDoubleBuffered);
	if (isDoubleBuffered)
	{
		clReadBuffer(CL_BACK);
		CLfloat raster_pos[4];

		clGetFloatv(CL_CURRENT_RASTER_POSITION, raster_pos);

		clDrawBuffer(CL_FRONT);
		clRasterPos3f((CLfloat)rect.left, (CLfloat)(get_height()-rect.bottom), raster_pos[2]);

		clCopyPixels(rect.left, get_height()-rect.bottom, rect.get_width(), rect.get_height(), CL_COLOR);

		clFlush();

		// Return to drawing to the back buffer
		clDrawBuffer(CL_BACK);
	}

	repaint_layered();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_DisplayWindow_OpenGL::repaint_layered()
{
	if (layered_dc == 0) return; // Not a layered window.
	if (ptr_UpdateLayeredWindow == 0) return; // Not supported on this platform.

	POINT layer_src_position;
	layer_src_position.x = 0;
	layer_src_position.y = 0;

	POINT layer_dest_position;
	layer_dest_position.x = get_layer_position().left;
	layer_dest_position.y = get_layer_position().top;

	SIZE layer_size;
	layer_size.cx = get_width();
	layer_size.cy = get_height();

	BLENDFUNCTION blendfunc;
	blendfunc.BlendOp = AC_SRC_OVER;
	blendfunc.BlendFlags = 0;
	blendfunc.SourceConstantAlpha = 255;
	blendfunc.AlphaFormat = AC_SRC_ALPHA;

	ptr_UpdateLayeredWindow(
		get_hwnd(),
		hdc,
		&layer_dest_position,
		&layer_size,
		layered_dc,
		&layer_src_position,
		RGB(0,0,0),
		&blendfunc,
		ULW_COLORKEY/*ULW_ALPHA*/);
}

std::list<HGLRC> CL_DisplayWindow_OpenGL::opengl_contexts;

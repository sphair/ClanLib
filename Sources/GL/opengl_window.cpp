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
#include "API/GL/opengl_window.h"
#include "API/Display/display_window_description.h"

#ifdef WIN32
#include "WGL/display_window_opengl.h"
#else
#ifdef __APPLE__
#include "AGL/display_window_opengl.h"
#else
#include "GLX/display_window_opengl.h"
#endif
#endif

#define gl_impl ((CL_DisplayWindow_OpenGL *) impl)

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindow construction:

CL_OpenGLWindow::CL_OpenGLWindow() : CL_DisplayWindow()
{
}

CL_OpenGLWindow::CL_OpenGLWindow(const CL_OpenGLWindow &copy) : CL_DisplayWindow(copy)
{
}

CL_OpenGLWindow::CL_OpenGLWindow(
	const std::string &title,
	int width,
	int height,
	bool start_fullscreen,
	bool allow_resize,
	int flipping_buffers)
: CL_DisplayWindow(new CL_DisplayWindow_OpenGL)
{
	CL_DisplayWindowDescription desc;
	desc.set_title(title);
	desc.set_size(CL_Size(width, height));
	desc.set_fullscreen(start_fullscreen);
	desc.set_allow_resize(allow_resize);
	desc.set_flipping_buffers(flipping_buffers);

	gl_impl->create_window(desc);

	// Automatically map first created display window as 'CL_Display current'.
	if (CL_DisplayWindow_Generic::current == 0) CL_DisplayWindow_Generic::current = this;

	gl_impl->sig_resize(get_width(), get_height());
}

CL_OpenGLWindow::~CL_OpenGLWindow()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindow attributes:

#ifdef WIN32

HGLRC CL_OpenGLWindow::get_context()
{
	return gl_impl->get_context();
}

HDC CL_OpenGLWindow::get_dc()
{
	return gl_impl->get_dc();
}

HWND CL_OpenGLWindow::get_wnd()
{
	return gl_impl->get_hwnd();
}
#else
#ifndef __APPLE__
Display *CL_OpenGLWindow::get_wnd()
{
	return gl_impl->get_hwnd();
}
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindow operations:

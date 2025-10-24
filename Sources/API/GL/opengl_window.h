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

//! clanGL="Display 2D"
//! header=gl.h

#ifndef header_opengl_window
#define header_opengl_window

#ifdef CL_API_DLL
#ifdef CL_GL_EXPORT
#define CL_API_GL __declspec(dllexport)
#else
#define CL_API_GL __declspec(dllimport)
#endif
#else
#define CL_API_GL
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Display/display_window.h"

#ifdef WIN32
#include <windows.h>
#else
#ifndef __APPLE__
#include <X11/Xlib.h>
#endif
#endif

//: OpenGL top level window.
//- !group=GL/Display 2D!
//- !header=gl.h!
class CL_API_GL CL_OpenGLWindow : public CL_DisplayWindow
{
//! Construction:
public:
	//: Constructs an opengl window.
	//param title: Titlebar text.
	//param width, height: Size of window, in pixels.
	//param start_fullscreen: If true, window will go full screen.
	//param allow_resize: If true, window will have resize grabbers.
	//param flipping_buffer: Amount of buffers in flipping system.
	CL_OpenGLWindow();

	CL_OpenGLWindow(const CL_OpenGLWindow &copy);

	CL_OpenGLWindow(
		const std::string &title,
		int width,
		int height,
		bool start_fullscreen = false,
		bool allow_resize = false,
		int flipping_buffers = 2);

	~CL_OpenGLWindow();

//! Attributes:
public:
#ifdef WIN32
	//: Returns the Win32 OpenGL resource context.
	HGLRC get_context();

	//: Returns the Win32 device context handle.
	HDC get_dc();

	//: Returns the Win32 window handle.
	HWND get_wnd();
#else
#ifndef __APPLE__
	//: Retuens the X11 display handle.
	Display *get_wnd();
#endif
#endif

//! Operations:
public:

//! Implementation:
private:
};

#endif

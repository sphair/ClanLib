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

#ifndef header_display_window_opengl
#define header_display_window_opengl

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "Display/Win32/display_window_win32.h"
#include "API/Display/graphic_context.h"
#include "API/Display/pixel_buffer.h"

#include <GL/glu.h>
#include <stack>

class CL_DisplayWindow_OpenGL : public CL_DisplayWindow_Win32
{
//! Construction:
public:
	CL_DisplayWindow_OpenGL();

	virtual ~CL_DisplayWindow_OpenGL();

//! Attributes:
public:
	//: Returns the gc of this window.
	virtual CL_GraphicContext *get_gc();

	//: Returns pixel buffer of frame buffer.
	virtual CL_PixelBuffer get_buffer(int i) const;

	//: Returns the number of frame buffers.
	virtual int get_buffer_count() const;

	//: OpenGL rendering context for this window.
	HGLRC get_context() { return context; }

	//: Device context for this window.
	HDC get_dc();

	static std::list<HGLRC> opengl_contexts;

//! Operations:
public:
	//: Called when a window is to be created.
	virtual void create_window(const CL_DisplayWindowDescription &desc);

	//: Called when a window is destroyed.
	virtual void destroy_window();

	//: Called when window receives a windows event.
	virtual bool received_event(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//: Flip opengl buffers.
	virtual void flip(int interval);

	//: Copy a region of the backbuffer to the frontbuffer.
	virtual void update(const CL_Rect &rect);

//! Implementation:
private:
	//: Repaint layered window.
	void repaint_layered();

	//: OpenGL compatible gc.
	CL_GraphicContext gc;

	//: OpenGL rendering context for this window.
	HGLRC context;

	//: Device context for this window.
	HDC hdc;

	//: Device context to layered bitmap.
	HDC layered_dc;

	//: Layered bitmap.
	HBITMAP layered_bitmap;

	//: Original bitmap selected into the layered DC.
	HBITMAP layered_orig_bitmap;

	//: Layered window, user32.dll dll handle.
	HMODULE user32_dll;

	//: Layered window, function pointer for UpdateLayeredWindow.
	typedef BOOL (WINAPI *PtrUpdateLayeredWindow)(
	  HWND, HDC, POINT *, SIZE *, HDC, POINT *, COLORREF, BLENDFUNCTION *, DWORD);

	PtrUpdateLayeredWindow ptr_UpdateLayeredWindow;
};

#endif


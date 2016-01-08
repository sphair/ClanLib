/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

#ifndef WIN32

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/XInput.h>

#endif

Rect get_window_frame_size()
{
#ifdef WIN32
	RECT border_rect;
	border_rect.left = 200;
	border_rect.right = 200 + 200;
	border_rect.top = 200;
	border_rect.bottom = 200 + 200;
	AdjustWindowRectEx(&border_rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX, FALSE, 0);
	Rect border_size(border_rect.left - 200, border_rect.top - 200, border_rect.right - 400, border_rect.bottom - 400);

#else

	Display *d;
	Window w;
	d=XOpenDisplay(NULL);
	if(d==NULL)
	{
		throw Exception("Cannot open display");
	}
	int s=DefaultScreen(d);
	const int win_xpos = 128;
	const int win_ypos = 128;
	const int win_width = 128;
	const int win_height = 128;
	w=XCreateSimpleWindow(d, RootWindow(d, s), win_xpos, win_ypos, win_width, win_height, 0, BlackPixel(d, s), WhitePixel(d, s));
	XSelectInput(d, w, 
		ExposureMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		StructureNotifyMask |
		PropertyChangeMask |
		PointerMotionMask |
		EnterWindowMask |
		LeaveWindowMask |
		KeymapStateMask |
		FocusChangeMask);

	// setup size hints:
	XSizeHints size_hints;
	memset(&size_hints, 0, sizeof(XSizeHints));
	size_hints.x = win_xpos;
	size_hints.y = win_ypos;
	size_hints.width       = win_width;
	size_hints.height      = win_height;
	size_hints.base_width  = win_width;
	size_hints.base_height = win_height;
	size_hints.min_width   = size_hints.width;
	size_hints.min_height  = size_hints.height;
	size_hints.max_width   = size_hints.width;
	size_hints.max_height  = size_hints.height;
	size_hints.flags       = PSize|PBaseSize;
	size_hints.flags |= PMinSize | PMaxSize | PPosition;		// These flags are important
	XSetWMNormalHints(d, w, &size_hints);

	XMapWindow(d, w);

	// Wait for mapped
	XEvent event;
	do {
		XMaskEvent(d, StructureNotifyMask, &event);
	}while ( (event.type != MapNotify) || (event.xmap.event != w) );

	int xpos;
	int ypos;
	unsigned int width;
	unsigned int height;
	Window *children_ptr;
	unsigned int num_child;
	Window temp_window;
	XWindowAttributes attr;

	XGetWindowAttributes(d, w, &attr);

	xpos = attr.x;
	ypos = attr.y;
	width = attr.width;
	height = attr.height;

	// Search all parent windows .... there MUST be an easier may

	Window current_window = w;
	while(true)
	{
		children_ptr = NULL;
		XQueryTree(d, current_window, &temp_window, &current_window, &children_ptr, &num_child);
		if (children_ptr)
			XFree(children_ptr);

		if (!current_window) break;

		XGetWindowAttributes(d, current_window, &attr);
		xpos += attr.x;
		ypos += attr.y;
	}

	XDestroyWindow(d, w);
	XCloseDisplay(d);

	xpos -= win_xpos;
	ypos -= win_ypos;

	Rect border_size = Rect(-xpos, -ypos, 0, 0);

#endif
	return border_size;
}

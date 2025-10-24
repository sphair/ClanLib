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
*/

#include "Display/precomp.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/Text/logger.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/keys.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/event_provider.h"
#include "x11_window.h"
#include "input_device_provider_x11keyboard.h"
#include "input_device_provider_x11mouse.h"
#ifdef HAVE_LINUX_JOYSTICK_H
#include "input_device_provider_linuxjoystick.h"
#endif
#include "display_message_queue_x11.h"
#include <X11/Xatom.h>
#include <cstdio>
#include "../Window/input_context_impl.h"

#include <dlfcn.h>

CL_X11Window::CL_X11Window()
: window(0), window_last_focus(0), cmap(0), allow_resize(false), bpp(0), fullscreen(false),
  disp(0), system_cursor(0), hidden_cursor(0), cursor_bitmap(0), 
  site(0), clipboard(this), dlopen_lib_handle(NULL)
{
	keyboard = CL_InputDevice(new CL_InputDeviceProvider_X11Keyboard(this));
	mouse = CL_InputDevice(new CL_InputDeviceProvider_X11Mouse(this));

	CL_DisplayMessageQueue_X11::message_queue.add_client(this);
}

CL_X11Window::~CL_X11Window()
{
	CL_DisplayMessageQueue_X11::message_queue.remove_client(this);
	CL_DisplayMessageQueue_X11::message_queue.set_mouse_capture(this, false);

	if (!ic.impl.is_null())
		ic.impl->dispose();

	get_keyboard()->dispose();
	get_mouse()->dispose();

	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();

	close_window();

	if (disp)
	{
		XCloseDisplay(disp);
		disp = 0;
	}

	// This MUST be called after XCloseDisplay - It is used for http://www.xfree86.org/4.8.0/DRI11.html
	if (dlopen_lib_handle)
	{
		dlclose(dlopen_lib_handle);
	}

}

void *CL_X11Window::dlopen(const char *filename, int flag)
{
	if (dlopen_lib_handle)
		throw CL_Exception(cl_text("CL_X11Window::dlopen called twice - This is currently not supported, and is probably a bug!"));
	dlopen_lib_handle = ::dlopen(filename, flag);
	return dlopen_lib_handle;
}


CL_Rect CL_X11Window::get_geometry() const
{
	CL_Rect scr_pos = get_screen_position();
	if (window_has_caption)
	{
		return CL_Rect(	scr_pos.left + frame_size.left, scr_pos.top + frame_size.top, 
				scr_pos.right + frame_size.right, scr_pos.bottom + frame_size.bottom);
	}else
	{
		return scr_pos;
	}
}

CL_Rect CL_X11Window::get_viewport() const
{
	Window new_window;
	int x, y;
	unsigned int width, height;
	unsigned int temp, border_width;
	XGetGeometry(disp, window, &new_window, &x, &y, &width, &height, &border_width, &temp);
	return CL_Rect(0, 0, width, height);
}


bool CL_X11Window::has_focus() const
{
	Window focus_return;
	int revert_to_return;
	XGetInputFocus(disp, &focus_return, &revert_to_return);
	if (focus_return == window) return true;
	return false;
}

bool CL_X11Window::is_minimized() const
{
	XWindowAttributes attr;

	XGetWindowAttributes(disp, window, &attr);
	if (attr.map_state == IsViewable) return false;
	return true;
}

bool CL_X11Window::is_maximized() const
{
	XWindowAttributes attr;

	XGetWindowAttributes(disp, window, &attr);
	if (attr.map_state == IsViewable) return false;
	return true;
}


bool CL_X11Window::is_visible() const
{
	XWindowAttributes attr;

	XGetWindowAttributes(disp, window, &attr);
	if (attr.map_state == IsViewable) return false;
	return true;
}

void CL_X11Window::open_screen()
{
	if (disp == 0)
	{
		disp = XOpenDisplay(0);
		if (disp == 0)
			throw CL_Exception("Could not open X11 display!");
	}
}

void CL_X11Window::create(XVisualInfo *visual, int screen_bpp, CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &description)
{
	site = new_site;

	get_window_frame_size();	// Cache the window frame size

	open_screen();

	create_new_window(visual, screen_bpp, description);
}


CL_Point CL_X11Window::client_to_screen(const CL_Point &client)
{
	CL_Rect view = get_geometry();

	if (window_has_caption)
	{
		return CL_Point(view.left + client.x - frame_size.left, view.top + client.y - frame_size.top);
	}else
	{
		return CL_Point(view.left + client.x, view.top + client.y);
	}
}

CL_Point CL_X11Window::screen_to_client(const CL_Point &screen)
{
	CL_Rect view = get_geometry();
	if (window_has_caption)
	{
		return CL_Point(screen.x - view.left + frame_size.left, screen.y - view.top +  frame_size.top);
	}else
	{
		return CL_Point(screen.x - view.left, screen.y - view.top);
	}
}

void CL_X11Window::show_system_cursor()
{
	XDefineCursor(disp, window, system_cursor);
}

void CL_X11Window::set_cursor(CL_StandardCursor type)
{
	//TODO:
}

void CL_X11Window::hide_system_cursor()
{
	XDefineCursor(disp, window, hidden_cursor);
}

void CL_X11Window::set_title(const CL_StringRef &new_title)
{
	XSetStandardProperties(disp, window, new_title.c_str(), new_title.c_str(), None, NULL, 0, NULL);
}

void CL_X11Window::set_position(const CL_Rect &pos, bool client_area)
{
	int result;

	// Clear hints - to allow repositioning
	XSizeHints *size_hints = XAllocSizeHints();
	if (size_hints)
	{
		long user_hints;
		XGetWMNormalHints(disp, window, size_hints, &user_hints);
		size_hints->flags &= ~( PSize|PBaseSize | PMinSize | PMaxSize | PPosition);
		XSetWMNormalHints(disp, window, size_hints);
	}

	clear_structurenotify_events();

	int width = pos.get_width();
	int height = pos.get_height();
	int xpos = pos.left;
	int ypos = pos.top;

	if ( (client_area) )
	{
		if (window_has_caption)
		{
			xpos += frame_size.left;
			ypos += frame_size.top;
		}
		result = XMoveResizeWindow(disp, window, xpos, ypos, width, height);
	}
	else
	{
		width =  width + frame_size.left;
		height = height + frame_size.top;
		result = XMoveResizeWindow(disp, window, xpos, ypos, width, height);
	}
	
	if (! ( (result == BadValue) || (result == BadWindow) ) )
	{
		XEvent event;
		// Wait to resize
		do {
			XMaskEvent(disp, StructureNotifyMask, &event);
		}while ( (event.type != ConfigureNotify) || (event.xconfigure.event != window) );
	}

	// Restore hints
	if (size_hints)
	{
		size_hints->x = xpos;
		size_hints->y = ypos;
		size_hints->width       = width;
		size_hints->height      = height;
		size_hints->base_width  = width;
		size_hints->base_height = height;
		size_hints->min_width   = width;
		size_hints->min_height  = height;
		size_hints->max_width   = width;
		size_hints->max_height  = height;
		size_hints->flags       |= PMinSize | PMaxSize | PPosition;

		if (!resize_enabled)
		{
			size_hints->flags |= PMinSize | PMaxSize;
		}

		XSetWMNormalHints(disp, window, size_hints);
		XFree(size_hints);
	}
}

void CL_X11Window::set_size(int width, int height, bool client_area)
{
	int result;

	if (!resize_enabled)	// If resize has been disabled, we have to temporary enable it
	{
		XSizeHints *size_hints = XAllocSizeHints();
		if (size_hints)
		{
			long user_hints;
			XGetWMNormalHints(disp, window, size_hints, &user_hints);
			size_hints->flags &= ~(PMinSize | PMaxSize);
			XSetWMNormalHints(disp, window, size_hints);
			XFree(size_hints);
		}
	}
	clear_structurenotify_events();

	if ( (client_area) || (!window_has_caption) )
	{
		//TODO: Resize does not work on windows without a caption - May need to temporary enable it ... this is silly though!
		result = XResizeWindow(disp, window, width, height);
	}
	else
	{
		width =  width + frame_size.left;
		height = height + frame_size.top;
		result = XResizeWindow(disp, window, width, height);
	}
	
	if (! ( (result == BadValue) || (result == BadWindow) ) )
	{
		XEvent event;
		// Wait to resize
		do {
			XMaskEvent(disp, StructureNotifyMask, &event);
		}while ( (event.type != ConfigureNotify) || (event.xconfigure.event != window) );
	}

	if (!resize_enabled)	// If resize has been disabled, do disable x11 window resize
	{
		XSizeHints *size_hints = XAllocSizeHints();
		if (size_hints)
		{
			long user_hints;
			XGetWMNormalHints(disp, window, size_hints, &user_hints);
			size_hints->width       = width;
			size_hints->height      = height;
			size_hints->base_width  = width;
			size_hints->base_height = height;
			size_hints->min_width   = width;
			size_hints->min_height  = height;
			size_hints->max_width   = width;
			size_hints->max_height  = height;
			size_hints->flags       |= PMinSize | PMaxSize;
			XSetWMNormalHints(disp, window, size_hints);
			XFree(size_hints);
		}
	}

}

void CL_X11Window::set_enabled(bool enable)
{
	if (enable)
	{
		if (!has_focus())
			XSetInputFocus(disp, window, RevertToNone, CurrentTime);
	}else
	{
		if (has_focus())
			XSetInputFocus(disp, window_last_focus, window_last_revert_return, CurrentTime);
	}
}

void CL_X11Window::minimize()
{
	XIconifyWindow(disp, window, current_screen);
}

void CL_X11Window::wait_mapped()
{
	XEvent event;
	do {
		XMaskEvent(disp, StructureNotifyMask, &event);
	}while ( (event.type != MapNotify) || (event.xmap.event != window) );

}

void CL_X11Window::restore()
{
	XMapWindow(disp, window);
	wait_mapped();
}

void CL_X11Window::maximize()
{
//TODO:	ShowWindow(hwnd, SW_MAXIMIZE);
}

void CL_X11Window::show(bool activate)
{
	XMapWindow(disp, window);
	wait_mapped();
	clear_structurenotify_events();
	if (activate) set_enabled(true);
}

void CL_X11Window::hide()
{
	set_enabled(false);
	XUnmapWindow(disp, window);
}

void CL_X11Window::bring_to_front()
{
	XRaiseWindow(disp, window);
}

void CL_X11Window::capture_mouse(bool capture)
{
	CL_DisplayMessageQueue_X11::message_queue.set_mouse_capture(this, capture);
}

void CL_X11Window::clear_structurenotify_events()
{
	// Ennsure event pool is empty
	XEvent event;
	while( XCheckMaskEvent(disp, StructureNotifyMask, &event));

}

CL_Rect CL_X11Window::get_screen_position() const
{
	int xpos;
	int ypos;
	unsigned int width;
	unsigned int height;
	Window *children_ptr;
	unsigned int num_child;
	Window temp_window;
	XWindowAttributes attr;

	XGetWindowAttributes(disp, window, &attr);

	xpos = attr.x;
	ypos = attr.y;
	width = attr.width;
	height = attr.height;

	// Search all parent windows .... there MUST be an easier may
	Window current_window = window;
	while(true)
	{
		children_ptr = NULL;
		XQueryTree(disp, current_window, &temp_window, &current_window, &children_ptr, &num_child);
		if (children_ptr)
			XFree(children_ptr);

		if (!current_window) break;

		XGetWindowAttributes(disp, current_window, &attr);
		xpos += attr.x;
		ypos += attr.y;
	}
	return (CL_Rect(xpos, ypos, width+xpos, height+ypos));
}

void CL_X11Window::close_window()
{
	current_window_events.clear();

	bool focus = false;

	if(window)
	{
		focus = has_focus();
		XDestroyWindow(disp, window);
		window = 0;
	}

	if (system_cursor)
	{
		XFreeCursor(disp, system_cursor);
		system_cursor = 0;
	}

	if (hidden_cursor)
	{
		XFreeCursor(disp, hidden_cursor);
		hidden_cursor = 0;
	}

	if (cursor_bitmap)
	{
		XFreePixmap(disp, cursor_bitmap);
		cursor_bitmap = 0;
	}

	if (cmap)
	{
		XFreeColormap(disp, cmap);
		cmap = 0;
	}

	if (focus && window_last_focus)
	{
//TODO: How to return the focus to the previous window - Checking that it is still valid ?
//		XSetInputFocus(disp, window_last_focus, window_last_revert_return, CurrentTime);
	}
}

void CL_X11Window::create_new_window(XVisualInfo *visual, int screen_bpp, const CL_DisplayWindowDescription &desc)
{
	close_window();	// Close the window if already opened (maybe it should be modified instead of recreated?)

	current_screen = visual->screen;
	
	CL_SocketMessage_X11 screen_connection;
	screen_connection.type = CL_EventProvider::type_fd_read;
	screen_connection.handle = ConnectionNumber(disp);
	current_window_events.push_back(screen_connection);

	window_has_caption = desc.has_caption();

	XGetInputFocus(disp, &window_last_focus, &window_last_revert_return);

	bpp = screen_bpp;

	// create a color map
	cmap = XCreateColormap( disp, RootWindow(disp,  current_screen), visual->visual, AllocNone);

	memset(&attributes, 0, sizeof(attributes));

	attributes.colormap = cmap;
	attributes.border_pixel = 0;
	attributes.override_redirect = False;

	// create a window in window mode
	attributes.event_mask =
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
		FocusChangeMask;

	int win_x = desc.get_position().left;
	int win_y = desc.get_position().top;
	int win_width = desc.get_size().width;
	int win_height = desc.get_size().height;

	// Check for valid width and height
	if ( win_width <=0 ) 
	{
		win_width = 128; 
	}

	if ( win_height <=0 ) 
	{
		win_height = 128; 
	}

	bool client_area = desc.get_position_client_area();

	if (window_has_caption)
	{
		if (!client_area)
		{

			// The API for desc.get_size() includes the window frame, but X11 expects it excluded
			win_width += frame_size.left;
			win_height += frame_size.top;
		}
	}

	if (client_area)
	{
		if (win_x >=0)
		{
			win_x -= frame_size.left;
			if (win_x < 0)
				win_x = 0;
		}
		if (win_y >=0)
		{
			win_y -= frame_size.left;
			if (win_y < 0)
				win_y = 0;
		}
	}

	Window parent = RootWindow(disp, current_screen);


	if (!desc.get_owner().is_null())
	{
		// This is not required. When enabled (with a parent window) popup windows would be created, with an incorrect style
		//parent = provider_glx->get_window();

		CL_Rect rect = desc.get_owner().get_geometry();
		if (win_x == -1 && win_y == -1)
		{
			win_x = rect.get_width()/2 - win_width/2;
			win_y = rect.get_height()/2 - win_height/2;
		}
		win_x += rect.left;
		win_y += rect.top;
	}

	if (desc.is_fullscreen())
	{
		win_x = 0;
		win_y = 0;

	}

	if (win_x == -1 && win_y == -1)
	{
		win_x = DisplayWidth(disp, current_screen)/2 - win_width/2;
		win_y = DisplayHeight(disp, current_screen)/2 - win_height/2;
	}

	window = XCreateWindow(disp, parent,
		win_x, win_y, win_width, win_height, 0,  visual->depth,
		InputOutput,  visual->visual, CWBorderPixel | CWColormap | CWOverrideRedirect | CWEventMask, &attributes);

	if (window == 0)
		throw CL_Exception(cl_text("Unable to create the X11 window"));

	clipboard.setup();

	// set title of window:
	set_title(desc.get_title());

	// Remove window caption
	if (!window_has_caption)
	{
		Atom WM_HINTS;

		// Motif and Gnome
		WM_HINTS = XInternAtom(disp, "_MOTIF_WM_HINTS", True);
		if ( WM_HINTS != None )
		{
			// From Xm/MwmUtil.h
			struct {
				long flags;
				long functions;
				long decorations;
				long input_mode;
				long status;
			} MWMHints = { 2, 0, 0, 0, 0 };

			XChangeProperty(disp, window,
				WM_HINTS, WM_HINTS, 32,
				PropModeReplace,
				(unsigned char *)&MWMHints,
				sizeof(MWMHints)/sizeof(long));
		}
		// KDM
		WM_HINTS = XInternAtom(disp, "KWM_WIN_DECORATION", True);
		if ( WM_HINTS != None ) {
			long KWMHints = 0;

			XChangeProperty(disp, window,
				WM_HINTS, WM_HINTS, 32,
				PropModeReplace,
				(unsigned char *)&KWMHints,
				sizeof(KWMHints)/sizeof(long));
		}

		// Other
		WM_HINTS = XInternAtom(disp, "_WIN_HINTS", True);
		if ( WM_HINTS != None ) {
			long OtherHints = 0;
			XChangeProperty(disp, window,
				WM_HINTS, WM_HINTS, 32,
				PropModeReplace,
				(unsigned char *)&OtherHints,
				sizeof(OtherHints)/sizeof(long));
		}

	}

	// setup size hints:
	XSizeHints *size_hints = XAllocSizeHints();
	if (size_hints == NULL)
	{
		throw CL_Exception(cl_text("Cannot allocate X11 XSizeHints structure"));
	}

	size_hints->x = win_x;
	size_hints->y = win_y;
	size_hints->width       = win_width;
	size_hints->height      = win_height;
	size_hints->base_width  = win_width;
	size_hints->base_height = win_height;
	size_hints->min_width   = win_width;
	size_hints->min_height  = win_height;
	size_hints->max_width   = win_width;
	size_hints->max_height  = win_height;
	size_hints->flags       = PSize|PBaseSize|PPosition;

	resize_enabled = desc.get_allow_resize() || desc.is_fullscreen(); // Fs needs resizable window

	if (!resize_enabled)
	{
		size_hints->flags |= PMinSize | PMaxSize;
	}
	XSetWMNormalHints(disp, window, size_hints);
	
	XFree(size_hints);

	// handle wm_delete_events if in windowed mode:
	Atom wm_delete = XInternAtom(disp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(disp, window, &wm_delete, 1);

	// Create input devices for window:
	ic.clear();
	ic.add_keyboard(keyboard);
	ic.add_mouse(mouse);

	// Go looking for joysticks:
	setup_joysticks();

	// Complete setting up the window
	system_cursor = XCreateFontCursor(disp, XC_left_ptr);
	char data[64]; // 8x8
	memset(data, 0, 64);

	XColor black_color;
	memset(&black_color, 0, sizeof(black_color));

	cursor_bitmap = XCreateBitmapFromData(disp, window, data, 8, 8);
	hidden_cursor = XCreatePixmapCursor(disp, cursor_bitmap, cursor_bitmap, &black_color, &black_color, 0,0 );

	// make window visible:
	if (desc.is_visible())
	{
		show(true);
		set_enabled(true);
	}

	if (desc.is_fullscreen()) set_fullscreen();

	XSync(disp, True);

}

void CL_X11Window::get_message(CL_X11Window *mouse_capture_window)
{
	XEvent event;
	CL_Rect *rect;

	ic.poll(false);		// Check input devices

	// Dispatch all Xlib events
	if (get_xevent(event))
	{
		switch(event.type)
		{
			//Resize or Move
			case ConfigureNotify:
			{
				int bw = event.xconfigure.border_width;
				CL_Rect rect( 
					event.xconfigure.x + bw,
					event.xconfigure.y + bw,
					event.xconfigure.x + bw + event.xconfigure.width,
					event.xconfigure.y + bw + event.xconfigure.height
				);
				CL_Rect rect_saved = rect;
				int width = rect.right - rect.left;
				int height = rect.bottom - rect.top;

				// Move window
				if ( (rect_saved.left != rect.left) || (rect_saved.right != rect.right) )
				{
					XMoveWindow(disp, window, rect.left-bw, rect.top-bw);
				}

				// Resize Window
				if ( (width != event.xconfigure.width) || (height != event.xconfigure.height) )
				{
					if (site && !site->func_window_resize->is_null())
					{
						site->func_window_resize->invoke(rect);	// Excluding borders
					}
					XResizeWindow(disp, window, width, height);
				}

				if (!callback_on_resized.is_null())
					callback_on_resized.invoke();

				if (site)
				{
					site->sig_resize->invoke(width, height);	// Excluding borders
				}
			
				break;
			}
			case ClientMessage:

				if (site) site->sig_window_close->invoke();
				break;
			case Expose:
				// Repaint notification
				// Could be more efficient if we checked ahead for other
				// repaint notifications
				rect = new CL_Rect(event.xexpose.x, event.xexpose.y,
					event.xexpose.x + event.xexpose.width, event.xexpose.y + event.xexpose.height);
				if (site) site->sig_paint->invoke(*rect);
				delete rect;
				break;
			case FocusIn:
				if (site)
					site->sig_got_focus->invoke();

				break;
			case FocusOut:
				if (site)
				{
					if (!has_focus())	// For an unknown reason, FocusOut is called when clicking on title bar of window
					{
						site->sig_lost_focus->invoke();
					}
				}
				break;
			case KeyRelease:
			case KeyPress:
				if (get_keyboard())
					get_keyboard()->received_keyboard_input(event.xkey);
				break;
			//case KeymapNotify:
			//	break;
			case ButtonPress:
			case ButtonRelease:
				if (mouse_capture_window->get_mouse())
				{
					if (this != mouse_capture_window)
					{
						CL_Rect this_scr = get_screen_position();
						CL_Rect capture_scr = mouse_capture_window->get_screen_position();
	
						event.xbutton.x += this_scr.left - capture_scr.left;
						event.xbutton.y += this_scr.top - capture_scr.top;
					}
					
					mouse_capture_window->get_mouse()->received_mouse_input(event.xbutton);
				}
				break;
			case MotionNotify:
				if (mouse_capture_window->get_mouse())
				{

					if (this != mouse_capture_window)
					{
						CL_Rect this_scr = get_screen_position();
						CL_Rect capture_scr = mouse_capture_window->get_screen_position();
	
						event.xmotion.x += this_scr.left - capture_scr.left;
						event.xmotion.y += this_scr.top - capture_scr.top;
					}
					
					mouse_capture_window->get_mouse()->received_mouse_move(event.xmotion);
				}
				break;

			case SelectionClear:	// New clipboard selection owner
				clipboard.event_selection_clear(event.xselectionclear);
				break;
			case SelectionNotify:
				clipboard.event_selection_notify();
				break;
			case SelectionRequest:	// Clipboard requests
				clipboard.event_selection_request(event.xselectionrequest);
				break;

			//case PropertyNotify:
			//	break;		

		default:
			break;
		}
	}
}

// This is called for each window by CL_DisplayMessageQueue_X11 to check for messages
bool CL_X11Window::has_messages()
{
	bool message_flag = false;

	if (XPending(disp) > 0)
	{
		message_flag = true;
	}

	if (ic.poll(true))
	{
		message_flag = true;
	}

	return message_flag;
}

void CL_X11Window::setup_joysticks()
{
	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();
	joysticks.clear();

#ifdef HAVE_LINUX_JOYSTICK_H

	char pathname[128];
	char joydev[128];

	if (access("/dev/input/", R_OK | X_OK) == 0)
	{
		strcpy(joydev, "/dev/input/js%d");
	} 
	else
	{
		strcpy(joydev, "/dev/js%d");
	}

	const int max_joysticks = 16;
	for(int i = 0; i < max_joysticks; ++i)
	{
		snprintf(pathname, sizeof(pathname), joydev, i);
		
		if (access(pathname, R_OK) == 0)
		{
			try
			{
				CL_InputDeviceProvider_LinuxJoystick *joystick_provider = new CL_InputDeviceProvider_LinuxJoystick(this, cl_text(pathname));
				CL_InputDevice device(joystick_provider);
				joysticks.push_back(device);
				ic.add_joystick(device);

				CL_SocketMessage_X11 joystick_connection;
				joystick_connection.type = CL_EventProvider::type_fd_read;
				joystick_connection.handle = joystick_provider->get_fd();
				current_window_events.push_back(joystick_connection);

			} 
			catch (CL_Exception error)
			{
				cl_log_event("debug", "Joystick Error: %1", error.message);
			}
		}
	}
#endif
}

void CL_X11Window::set_fullscreen()
{
	// Set fullscreen mode: http://standards.freedesktop.org/wm-spec/wm-spec-1.3.html
	XEvent xev = { 0 };
	xev.xclient.type = ClientMessage;
	xev.xclient.send_event = True;
	xev.xclient.message_type = XInternAtom(disp, "_NET_WM_STATE", False);
	xev.xclient.window = window;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = 1;
	xev.xclient.data.l[1] = XInternAtom(disp, "_NET_WM_STATE_FULLSCREEN", False);
	// Send the event and set fullscreen to true if successful
	fullscreen = XSendEvent(disp, DefaultRootWindow(disp), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}


void CL_X11Window::set_clipboard_text(const CL_StringRef &text)
{
	clipboard.set_clipboard_text(text);
}

void CL_X11Window::set_clipboard_image(const CL_PixelBuffer &buf)
{
	throw CL_Exception(cl_text("Todo: CL_X11Window::set_clipboard_image"));
}

CL_String CL_X11Window::get_clipboard_text() const
{
	return clipboard.get_clipboard_text();
}

CL_PixelBuffer CL_X11Window::get_clipboard_image() const
{
	throw CL_Exception(cl_text("Todo: CL_X11Window::get_clipboard_image"));

	return CL_PixelBuffer();
}

bool CL_X11Window::is_clipboard_text_available() const
{
	return clipboard.is_clipboard_text_available();
}

bool CL_X11Window::is_clipboard_image_available() const
{
	return clipboard.is_clipboard_image_available();
}

void CL_X11Window::set_cursor(CL_CursorProvider_X11 *cursor)
{
	//TODO:
}

CL_Rect CL_X11Window::frame_size(0,0,0,0);
bool CL_X11Window::frame_size_set = false;

CL_Rect CL_X11Window::get_window_frame_size()
{
	if (frame_size_set)
	{
		return frame_size;
	}
#ifdef FIXME_THIS_LOOKS_TERRIBLE
	Display *d;
	Window w;
	d=XOpenDisplay(NULL);
	if(d==NULL)
	{
		throw CL_Exception(cl_text("Cannot open display"));
	}
	int s=DefaultScreen(d);

	// Try 2 attempts at different positions to ensure that an accurate reading is made
	// FIXME: This looks terrible, the temporary window is shown!
	for (int attempt_cnt = 0; attempt_cnt < 2; attempt_cnt++)
	{
		int win_xpos = 128 + attempt_cnt*32;
		int win_ypos = 128 + attempt_cnt*32;
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
		XSizeHints *size_hints = XAllocSizeHints();
		if (size_hints)
		{
			size_hints->x = win_xpos;
			size_hints->y = win_ypos;
			size_hints->width       = win_width;
			size_hints->height      = win_height;
			size_hints->base_width  = win_width;
			size_hints->base_height = win_height;
			size_hints->min_width   = win_width;
			size_hints->min_height  = win_height;
			size_hints->max_width   = win_width;
			size_hints->max_height  = win_height;
			size_hints->flags       = PSize|PBaseSize|PPosition;
			XSetWMNormalHints(d, w, size_hints);
			XFree(size_hints);
		}

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

		xpos -= win_xpos;
		ypos -= win_ypos;

		if (attempt_cnt)
		{
			// Ensure the frame size is the same as different positions
			if ((frame_size.left != -xpos) || (frame_size.top != -ypos))
			{
				// Set to a zero frame size
				frame_size = CL_Rect(0, 0, 0, 0);
				break;
			}
		}else
		{
			frame_size = CL_Rect(-xpos, -ypos, 0, 0);
		}

	}

	XCloseDisplay(d);
#endif
	frame_size_set = true;

	return frame_size;
}

// Note, This function does not search the cache, this is intentional, as the event should never be in the cache when this function is used
bool CL_X11Window::get_xevent( XEvent &event, int event_type, bool block ) const
{
	if (XCheckTypedWindowEvent(disp, window, event_type, &event))
	{
		return true;
	}else
	{
		return false;
	}

}

bool CL_X11Window::get_xevent( XEvent &event ) const
{
	if  (XPending(disp) > 0)
	{
		XNextEvent(disp, &event);
		return true;
	}
	return false;
}

void CL_X11Window::request_repaint( const CL_Rect &cl_rect )
{
	XEvent event;
	event.xexpose.type = Expose;
	event.xexpose.serial = 0;
	event.xexpose.send_event = True;
	event.xexpose.display = disp;
	event.xexpose.window = window;
	event.xexpose.x = cl_rect.left;
	event.xexpose.y = cl_rect.top;
	event.xexpose.width = cl_rect.get_width();
	event.xexpose.height = cl_rect.get_height();
	event.xexpose.count = 0;

	XSendEvent( disp, window, False, 0, &event );
}

void CL_X11Window::set_minimum_size( int width, int height, bool client_area)
{
	//TODO: Implement for X11
//	if (client_area)
//		throw CL_Exception(cl_text("Congratulations! You just got assigned the task of adding support for client area in CL_Win32Window::set_minimum_size(...)."));
//
//	this->minimum_size = CL_Size(width,height);
}

void CL_X11Window::set_maximum_size( int width, int height, bool client_area)
{
	//TODO: Implement for X11
//	if (client_area)
//		throw CL_Exception(cl_text("Congratulations! You just got assigned the task of adding support for client area in CL_Win32Window::set_maximum_size(...)."));
//
//	this->maximum_size = CL_Size(width,height);
}

void CL_X11Window::get_keyboard_modifiers(bool &key_shift, bool &key_alt, bool &key_ctrl) const
{
	if (!get_keyboard())
	{
		key_shift = false;
		key_alt = false;
		key_ctrl = false;
		return;
	}
	return get_keyboard()->get_keyboard_modifiers(key_shift, key_alt, key_ctrl);
}

CL_Point CL_X11Window::get_mouse_position() const
{
	if (!get_mouse())
	{
		return CL_Point();
	}
	return get_mouse()->get_position();
}

// Important: Use XFree() on the returned pointer (if not NULL)
unsigned char *CL_X11Window::get_property(Window use_window, Atom prop, unsigned long *number_items_ptr, int *actual_format_ptr, Atom *actual_type_ptr) const
{
	unsigned long bytes_after;
	int read_bytes = 1024;
	unsigned char *read_data = NULL;
	do
	{
		if(read_data != NULL)
		{
			XFree(read_data);
		}
		int result = XGetWindowProperty(disp, use_window, prop, 0, read_bytes, False, AnyPropertyType, actual_type_ptr, actual_format_ptr, number_items_ptr, &bytes_after, &read_data);
		if (result != Success)
		{
			*number_items_ptr = 0;
			*actual_format_ptr = 0;
			*actual_type_ptr = None;
			return NULL;
		}
		read_bytes += bytes_after;
	}while(bytes_after != 0);
	return read_data;
}

const std::vector<CL_SocketMessage_X11> &CL_X11Window::get_window_socket_messages() const
{
	return current_window_events;
}

void CL_X11Window::set_large_icon(const CL_PixelBuffer &image)
{

}

void CL_X11Window::set_small_icon(const CL_PixelBuffer &image)
{

}

CL_InputDeviceProvider_X11Keyboard *CL_X11Window::get_keyboard() const
{
	return static_cast<CL_InputDeviceProvider_X11Keyboard *>(keyboard.get_provider());
}

CL_InputDeviceProvider_X11Mouse *CL_X11Window::get_mouse() const
{
	return static_cast<CL_InputDeviceProvider_X11Mouse *>(mouse.get_provider());
}



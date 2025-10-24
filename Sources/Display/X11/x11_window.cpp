/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include <X11/XKBlib.h>
#include <dlfcn.h>

#define _NET_WM_STATE_REMOVE  0
#define _NET_WM_STATE_ADD     1
#define _NET_WM_STATE_TOGGLE  2

CL_X11Window::CL_X11Window()
: window(0), cmap(0), minimized(false), maximized(false), restore_to_maximized(false), fullscreen(false),
  disp(0), system_cursor(0), hidden_cursor(0), cursor_bitmap(0), 
  site(0), clipboard(this), dlopen_lib_handle(NULL), size_hints(NULL),
  wm_protocols(None), wm_delete_window(None), wm_state(None), net_wm_state(None),
  net_wm_state_maximized_vert(None), net_wm_state_maximized_horz(None),
  net_wm_state_hidden(None), net_wm_state_fullscreen(None)
{
	last_repaint_rect.reserve(32);
	keyboard = CL_InputDevice(new CL_InputDeviceProvider_X11Keyboard(this));
	mouse = CL_InputDevice(new CL_InputDeviceProvider_X11Mouse(this));

	CL_DisplayMessageQueue_X11::message_queue.add_client(this);
}

CL_X11Window::~CL_X11Window()
{
	CL_DisplayMessageQueue_X11::message_queue.remove_client(this);
	CL_DisplayMessageQueue_X11::message_queue.set_mouse_capture(this, false);

	if (ic.impl)
		ic.impl->dispose();

	get_keyboard()->dispose();
	get_mouse()->dispose();

	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();

	close_window();

	if (size_hints)
		XFree(size_hints);

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
		throw CL_Exception("CL_X11Window::dlopen called twice - This is currently not supported, and is probably a bug!");
	dlopen_lib_handle = ::dlopen(filename, flag);
	return dlopen_lib_handle;
}


CL_Rect CL_X11Window::get_geometry() const
{
	return get_screen_position();
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
	unsigned long number_items;
	int actual_format;
	Atom actual_type;

	// first check _NET_WM_STATE property if supported
	if (net_wm_state != None)
		return check_net_wm_state(net_wm_state_hidden);

	// now check WM_STATE property
	if (wm_state != None)
	{
		unsigned char *data = get_property(window, wm_state, &number_items, &actual_format, &actual_type);
		if (data != NULL)
		{
			long state = *(long *)data;
			XFree(data);
			return state == IconicState;
		}
	}

	return false;
}

bool CL_X11Window::is_maximized() const
{
	return check_net_wm_state(net_wm_state_maximized_vert, net_wm_state_maximized_horz);
}

bool CL_X11Window::is_visible() const
{
	XWindowAttributes attr;

	XGetWindowAttributes(disp, window, &attr);
	if (attr.map_state == IsViewable) return false;
	return true;
}

CL_Size CL_X11Window::get_minimum_size(bool client_area) const
{
	if (!client_area)
		return minimum_size;
	else
		throw CL_Exception("CL_X11Window::get_minimum_size not implemented for client_area");
}

CL_Size CL_X11Window::get_maximum_size(bool client_area) const
{
	if (!client_area)
		return maximum_size;
	else
		throw CL_Exception("CL_X11Window::get_maximum_size not implemented for client_area");
}

CL_String CL_X11Window::get_title() const
{
	return window_title;
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

void CL_X11Window::create(XVisualInfo *visual, CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &description)
{
	site = new_site;

	open_screen();

	create_new_window(visual, description);
}


CL_Point CL_X11Window::client_to_screen(const CL_Point &client)
{
	CL_Rect view = get_geometry();
	return CL_Point(view.left + client.x, view.top + client.y);
}

CL_Point CL_X11Window::screen_to_client(const CL_Point &screen)
{
	CL_Rect view = get_geometry();
	return CL_Point(screen.x - view.left, screen.y - view.top);
}

void CL_X11Window::show_system_cursor()
{
	XDefineCursor(disp, window, system_cursor);
}

void CL_X11Window::set_cursor(CL_StandardCursor type)
{
	if (system_cursor)
	{
		XFreeCursor(disp, system_cursor);
		system_cursor = 0;
	}
	unsigned int index = XC_left_ptr;
	switch (type)
	{
		case cl_cursor_arrow:
			index = XC_left_ptr;
			break;
		case cl_cursor_appstarting:
			index = XC_watch;
			break;
		case cl_cursor_cross:
			index = XC_cross;
			break;
		case cl_cursor_hand:	
			index = XC_hand2;
			break;
		case cl_cursor_ibeam:
			index = XC_xterm;
			break;
		case cl_cursor_size_all:	
			index = XC_fleur;
			break;
		case cl_cursor_size_ns:
			index = XC_double_arrow;
			break;
		case cl_cursor_size_we:
			index = XC_sb_h_double_arrow;
			break;
		case cl_cursor_uparrow:
			index = XC_sb_up_arrow;
			break;
		case cl_cursor_wait:
			index = XC_watch;
			break;
		case cl_cursor_no:
			index = XC_X_cursor;
			break;
		case cl_cursor_size_nesw:
		case cl_cursor_size_nwse:
		default:
			break;
	}
	system_cursor = XCreateFontCursor(disp, index);
	XDefineCursor(disp, window, system_cursor);
}

void CL_X11Window::hide_system_cursor()
{
	XDefineCursor(disp, window, hidden_cursor);
}

void CL_X11Window::set_title(const CL_StringRef &new_title)
{
	window_title = new_title;
	XSetStandardProperties(disp, window, new_title.c_str(), new_title.c_str(), None, NULL, 0, NULL);
}

void CL_X11Window::set_position(const CL_Rect &pos, bool client_area)
{
	int result;

	if (!resize_enabled)	// If resize has been disabled, we have to temporary enable it
	{
		long user_hints;
		XGetWMNormalHints(disp, window, size_hints, &user_hints);
		size_hints->flags &= ~(PMinSize | PMaxSize);
		XSetWMNormalHints(disp, window, size_hints);
	}

	clear_structurenotify_events();

	result = XMoveResizeWindow(disp, window, pos.left, pos.top, pos.get_width(), pos.get_height());
	
	if (! ( (result == BadValue) || (result == BadWindow) ) )
	{
		XEvent event;
		// Wait to resize
		do {
			XMaskEvent(disp, StructureNotifyMask, &event);
		}while ( (event.type != ConfigureNotify) || (event.xconfigure.event != window) );
		XPutBackEvent(disp, &event);
	}

	if (!resize_enabled)	// resize has been temporary enabled
	{
		size_hints->min_width   = pos.get_width();
		size_hints->min_height  = pos.get_height();
		size_hints->max_width   = pos.get_width();
		size_hints->max_height  = pos.get_height();
		size_hints->flags |= PMinSize | PMaxSize;
		XSetWMNormalHints(disp, window, size_hints);
	}

}

void CL_X11Window::set_size(int width, int height, bool client_area)
{
	int result;

	if (!resize_enabled)	// If resize has been disabled, we have to temporary enable it
	{
		long user_hints;
		XGetWMNormalHints(disp, window, size_hints, &user_hints);
		size_hints->flags &= ~(PMinSize | PMaxSize);
		XSetWMNormalHints(disp, window, size_hints);
	}
	clear_structurenotify_events();

	result = XResizeWindow(disp, window, width, height);
	
	if (! ( (result == BadValue) || (result == BadWindow) ) )
	{
		XEvent event;
		// Wait to resize
		do {
			XMaskEvent(disp, StructureNotifyMask, &event);
		}while ( (event.type != ConfigureNotify) || (event.xconfigure.event != window) );
		XPutBackEvent(disp, &event);
	}

	if (!resize_enabled)	// resize has been temporary enabled
	{
		size_hints->min_width   = width;
		size_hints->min_height  = height;
		size_hints->max_width   = width;
		size_hints->max_height  = height;
		size_hints->flags |= PMinSize | PMaxSize;
		XSetWMNormalHints(disp, window, size_hints);
	}

}

void CL_X11Window::set_enabled(bool enable)
{
	// Window's version of set_enabled() calls EnableWindow() which tells the windows API that the window can have input focus if desired. 
	// If you do require it for linux, changing the masks: KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask might work
}

void CL_X11Window::minimize()
{
	if (!is_minimized())
		restore_to_maximized = is_maximized();
	XIconifyWindow(disp, window, current_screen);
}

void CL_X11Window::wait_mapped()
{
	XWindowAttributes attr;
	XGetWindowAttributes(disp, window, &attr);
	if (attr.map_state != IsUnmapped)
		return;

	XEvent event;
	do {
		XMaskEvent(disp, StructureNotifyMask, &event);
	}while ( (event.type != MapNotify) || (event.xmap.event != window) );
}

void CL_X11Window::restore()
{
	if (is_minimized())
	{
		if (restore_to_maximized)
		{
			maximize();
		}
		else
		{
			XMapWindow(disp, window);
			wait_mapped();
		}
	}
	else if (is_maximized())
	{
		modify_net_wm_state(false, net_wm_state_maximized_vert, net_wm_state_maximized_horz);
	}
}

void CL_X11Window::maximize()
{
	modify_net_wm_state(true, net_wm_state_maximized_vert, net_wm_state_maximized_horz);
	XMapWindow(disp, window);
	wait_mapped();
}

void CL_X11Window::show(bool activate)
{
	XMapWindow(disp, window);
	wait_mapped();
	clear_structurenotify_events();
	if (activate) set_enabled(true);

	// Force the window is updated
	CL_Rect window_rect = get_viewport();
	set_size(window_rect.get_width(), window_rect.get_height(), true);
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

bool CL_X11Window::check_net_wm_state(Atom atom1, Atom atom2) const
{
	unsigned long number_items;
	int actual_format;
	Atom actual_type;

	// search for atom1 or atom2 in _NET_WM_STATE array
	if (net_wm_state != None)
	{
		unsigned char *data = get_property(window, net_wm_state, &number_items, &actual_format, &actual_type);
		if (data != NULL)
		{
			bool find = false;
			for (unsigned i = 0; i < number_items; ++i)
			{
				long atom = ((long *)data)[i];
				if (atom != None && (atom == atom1 || atom == atom2))
				{
					find = true;
					break;
				}
			}
			XFree(data);
			return find;
		}
	}

	return false;
}

bool CL_X11Window::modify_net_wm_state(bool add, Atom atom1, Atom atom2)
{
	// change _NET_WM_STATE property, see: http://standards.freedesktop.org/wm-spec/wm-spec-latest.html
	if (net_wm_state != None)
	{
		XEvent xev;
		memset(&xev, 0, sizeof(xev));
		xev.xclient.type = ClientMessage;
		xev.xclient.window = window;
		xev.xclient.message_type = net_wm_state;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
		xev.xclient.data.l[1] = atom1;
		xev.xclient.data.l[2] = atom2;
		return XSendEvent(disp, DefaultRootWindow(disp), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev) != 0;
	}

	return false;
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

}

void CL_X11Window::create_new_window(XVisualInfo *visual, const CL_DisplayWindowDescription &desc)
{
	close_window();	// Close the window if already opened (maybe it should be modified instead of recreated?)

	current_screen = visual->screen;
	
	CL_SocketMessage_X11 screen_connection;
	screen_connection.type = CL_EventProvider::type_fd_read;
	screen_connection.handle = ConnectionNumber(disp);
	current_window_events.push_back(screen_connection);

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

	Window parent = RootWindow(disp, current_screen);

	if (!desc.get_owner().is_null())
	{
		// This is not required. When enabled (with a parent window) popup windows would be created, with an incorrect style
		//parent = desc.get_owner().get_window();

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
		int disp_width = DisplayWidth(disp, current_screen);
		int disp_height = DisplayHeight(disp, current_screen);
		win_x = (disp_width - win_width)/2;
		win_y = (disp_height - win_height)/2;

		// It is very annoying to specify new windows centered when using 2 screens spanned
		if (disp_width > 2000)	// A guess that 2 monitors are used
		{
			if (win_x > 1024)
			{
				win_x = (1024 - win_width)/2;
			}
		}
	}

	window = XCreateWindow(disp, parent,
		win_x, win_y, win_width, win_height, 0,  visual->depth,
		InputOutput,  visual->visual, CWBorderPixel | CWColormap | CWOverrideRedirect | CWEventMask, &attributes);

	if (window == 0)
		throw CL_Exception("Unable to create the X11 window");

	if (!desc.get_owner().is_null())
	{
		CL_DisplayWindow owner = desc.get_owner();
		XSetTransientForHint(disp, window, owner.get_window());
	}

	clipboard.setup();

	// set title of window:
	set_title(desc.get_title());


	// Motif and Gnome
	Atom WM_HINTS;
	WM_HINTS = XInternAtom(disp, "_MOTIF_WM_HINTS", True);
	if ( WM_HINTS != None )
	{

#ifndef MwmHintsDecoration
#define MwmHintsDecorations (1 << 1)
#define MwmDecorAll (1 << 0)
#define MwmDecorBorder (1 << 1)
#define MwmDecorHandle (1 << 2)
#define MwmDecorTitle (1 << 3)
#endif

		int decor;
		if (desc.has_caption())
		{
			if (!desc.has_border())
			{
				decor = MwmDecorTitle;
			}
			else	decor = -1;
		}
		else
		{
			if (desc.has_border())
			{
				decor = MwmDecorBorder;
			}
			else	decor = 0;
		}

		if (decor >=0)	// Decor specified
		{
			// From Xm/MwmUtil.h
			typedef struct {
				long flags;
				long functions;
				long decorations;
				long input_mode;
				long status;
			} MWMHints;
			MWMHints hints = { MwmHintsDecorations, 0, decor, 0, 0 };

			XChangeProperty(disp, window,
			WM_HINTS, WM_HINTS, 32,
			PropModeReplace,
			(unsigned char *)&hints,
			sizeof(MWMHints)/sizeof(long));
		}
	}


	// Remove window caption
	if (!desc.has_caption())
	{

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

	resize_enabled = desc.get_allow_resize() || desc.is_fullscreen(); // Fs needs resizable window

	if (resize_enabled)
	{
		minimum_size = CL_Size(32, 32);
		maximum_size = CL_Size(0, 0);	// No maximum size by default
	}
	else
	{
		minimum_size = CL_Size(win_width, win_height);
		maximum_size = CL_Size(win_width, win_height);
	}

	// setup size hints:
	if (!size_hints)
	{
		size_hints = XAllocSizeHints();
		if (size_hints == NULL)
		{
			throw CL_Exception("Cannot allocate X11 XSizeHints structure");
		}
	}

	size_hints->x = win_x;
	size_hints->y = win_y;
	size_hints->width       = win_width;
	size_hints->height      = win_height;
	size_hints->base_width  = win_width;
	size_hints->base_height = win_height;
	size_hints->min_width   = minimum_size.width;
	size_hints->min_height  = minimum_size.height;
	size_hints->max_width   = maximum_size.width;
	size_hints->max_height  = maximum_size.height;
	size_hints->flags       = PSize|PBaseSize|PPosition|PMinSize;

	if (!resize_enabled)
	{
		size_hints->flags |= PMaxSize;
	}
	XSetWMNormalHints(disp, window, size_hints);

	// retrieve some useful atoms
	wm_protocols = XInternAtom(disp, "WM_PROTOCOLS", True);
	wm_delete_window = XInternAtom(disp, "WM_DELETE_WINDOW", True);
	wm_state = XInternAtom(disp, "WM_STATE", True);
	net_wm_state = XInternAtom(disp, "_NET_WM_STATE", True);
	net_wm_state_maximized_vert = XInternAtom(disp, "_NET_WM_STATE_MAXIMIZED_VERT", True);
	net_wm_state_maximized_horz = XInternAtom(disp, "_NET_WM_STATE_MAXIMIZED_HORZ", True);
	net_wm_state_hidden = XInternAtom(disp, "_NET_WM_STATE_HIDDEN", True);
	net_wm_state_fullscreen = XInternAtom(disp, "_NET_WM_STATE_FULLSCREEN", True);

	// subscribe to window manager events
	XSetWMProtocols(disp, window, &wm_delete_window, 1);

	// Create input devices for window:
	ic.clear();
	ic.add_keyboard(keyboard);
	ic.add_mouse(mouse);

	// Make auto repeat keys detectable
	Bool supports_detectable_autorepeat;
	XkbSetDetectableAutoRepeat(disp,1,&supports_detectable_autorepeat);

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
		show(false);
	}

	if (desc.is_fullscreen()) set_fullscreen();

	minimized = is_minimized();
	maximized = is_maximized();
	restore_to_maximized = maximized;

	XSync(disp, True);
}

void CL_X11Window::get_message(CL_X11Window *mouse_capture_window)
{
	XEvent event;
	std::vector<CL_Rect> exposed_rects;
	CL_Rect largest_exposed_rect;
	CL_Rect rect;

	ic.poll(false);		// Check input devices
	// Dispatch all Xlib events
	while (get_xevent(event))
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

				if (site)
				{
					site->sig_window_moved->invoke();
					if (!site->func_window_resize->is_null())
					{
						site->func_window_resize->invoke(rect);	// Excluding borders
					}

					if (!callback_on_resized.is_null())
						callback_on_resized.invoke();

					site->sig_resize->invoke(rect.get_width(), rect.get_height());	// Excluding borders
				}
	
				break;
			}
			case ClientMessage:
				// handle window manager messages
				if (event.xclient.message_type == wm_protocols)
				{
					if (event.xclient.data.l[0] == wm_delete_window)
					{
						if (site)
							site->sig_window_close->invoke();
					}
				}
				break;
			case Expose:
				// Repaint notification
				if (!site)
					break;

				if (exposed_rects.empty())	// First call, reverse some additional memory as required
				{
					unsigned int num_exposed = event.xexpose.count;
					if (num_exposed > 1024)		// Have an upper limit if something strange is going on
						num_exposed = 1024;
					if (num_exposed < 8)		// Allocate a min of 8 expose_rects
						num_exposed = 8;
					exposed_rects.reserve(num_exposed);
				}

				rect = CL_Rect(event.xexpose.x, event.xexpose.y, 
					event.xexpose.x + event.xexpose.width, event.xexpose.y + event.xexpose.height);

				exposed_rects.push_back(rect);

				// For optimisation later on, calculate the largest exposed rect
				if ((largest_exposed_rect.get_width() * largest_exposed_rect.get_height()) < (rect.get_width() * rect.get_height()))
				{
					largest_exposed_rect = rect;
				}

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
			case PropertyNotify:
				if (net_wm_state != None)
				{
					if (event.xproperty.atom == net_wm_state && event.xproperty.state == PropertyNewValue)
					{
						if (is_minimized())
						{
							if (!minimized && site != NULL)
								site->sig_window_minimized->invoke();
							minimized = true;
							maximized = false;
						}
						else if (is_maximized())
						{
							if (!maximized && site != NULL)
								site->sig_window_maximized->invoke();
							if (minimized && site != NULL)
							{
								// generate resize events for minimized -> maximized transition
								CL_Rect rect = get_geometry();
								site->sig_window_moved->invoke();
								if (!site->func_window_resize->is_null())
									site->func_window_resize->invoke(rect);
								if (!callback_on_resized.is_null())
									callback_on_resized.invoke();
								site->sig_resize->invoke(rect.get_width(), rect.get_height());
							}
							minimized = false;
							maximized = true;
						}
						else
						{
							if ((minimized || maximized) && site != NULL)
								site->sig_window_restored->invoke();
							minimized = false;
							maximized = false;
						}
					}
				}
				else
				{
					if (event.xproperty.atom == wm_state && event.xproperty.state == PropertyNewValue)
					{
						if (is_minimized())
						{
							if (!minimized && site != NULL)
								site->sig_window_minimized->invoke();
							minimized = true;
						}
						else
						{
							if (minimized && site != NULL)
								site->sig_window_restored->invoke();
							minimized = false;
						}
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
		default:
			break;
		}
	}

	// Send any exposure events, unless they have already been sent
	unsigned int max = exposed_rects.size();

	if (max==1)	// Simple case, a single rect
	{
		site->sig_paint->invoke(largest_exposed_rect);
	}
	else if (max > 8)
	{
		// TODO: Why is this condition significantly faster?
		CL_Rect window_rect = get_viewport();
		site->sig_paint->invoke(window_rect);
	}
	else if (max > 1)
	{
		// Send the largest rect first
		site->sig_paint->invoke(largest_exposed_rect);
		for (unsigned int cnt=0; cnt < max; cnt++)
		{
			CL_Rect &rect = exposed_rects[cnt];

			// Rect is the larged rect or is inside the largest rect
			if (largest_exposed_rect.is_inside(rect))
			{
				continue;
			}

			// Search for later larger rects that contain this rect
			bool inner_flag = false;
			for (unsigned int inner_cnt=cnt+1; inner_cnt < max; inner_cnt++)
			{
				if (exposed_rects[inner_cnt].is_inside(rect))
				{
					inner_flag = true;
					break;
				}
			}

			if (!inner_flag)
			{
				site->sig_paint->invoke(rect);
			}
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
				CL_InputDeviceProvider_LinuxJoystick *joystick_provider = new CL_InputDeviceProvider_LinuxJoystick(this, pathname);
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
	fullscreen = modify_net_wm_state(true, net_wm_state_fullscreen);
}

void CL_X11Window::set_clipboard_text(const CL_StringRef &text)
{
	clipboard.set_clipboard_text(text);
}

void CL_X11Window::set_clipboard_image(const CL_PixelBuffer &buf)
{
	throw CL_Exception("Todo: CL_X11Window::set_clipboard_image");
}

CL_String CL_X11Window::get_clipboard_text() const
{
	return clipboard.get_clipboard_text();
}

CL_PixelBuffer CL_X11Window::get_clipboard_image() const
{
	throw CL_Exception("Todo: CL_X11Window::get_clipboard_image");

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

	CL_Rect rect = cl_rect;
	CL_Rect window_rect = get_viewport();

	rect.clip(window_rect);

	// Validate rect size (if outside clipping region)
	if ((rect.get_width() <= 0) || (rect.get_height() <= 0) )
		return;

	// Search the repaint list
	unsigned int max = last_repaint_rect.size();
	for (unsigned int cnt=0; cnt < max; cnt++)
	{
		// Ensure request for exposure has not already been made
		if (last_repaint_rect[cnt].is_inside(rect))
		{
			return;
		}

		// Does this esposure completely contain one already been made
		if (rect.is_inside(last_repaint_rect[cnt]))
		{
			last_repaint_rect[cnt] = rect;

			// We can also flag all others as free space to optimise further, but get_messages() indirectly does this (and probably faster)
			return;
		}

	}
	// Message sent in process_queued_events() because expose events need to contain a counter
	last_repaint_rect.push_back(rect);

}

void CL_X11Window::process_queued_events()
{
	unsigned int max = last_repaint_rect.size();
	for (unsigned int cnt=0; cnt < max; cnt++)
	{
		CL_Rect &rect = last_repaint_rect[cnt];

		XEvent event;
		event.xexpose.type = Expose;
		event.xexpose.serial = 0;
		event.xexpose.send_event = True;
		event.xexpose.display = disp;
		event.xexpose.window = window;
		event.xexpose.x = rect.left;
		event.xexpose.y = rect.top;	
		event.xexpose.width = rect.get_width();
		event.xexpose.height = rect.get_height();
		event.xexpose.count = (max -1) - cnt;
		XSendEvent( disp, window, False, 0, &event );
	}
	last_repaint_rect.clear();
}

void CL_X11Window::set_minimum_size( int width, int height, bool client_area)
{
	minimum_size = CL_Size(width,height);

	long user_hints;
	XGetWMNormalHints(disp, window, size_hints, &user_hints);
	size_hints->min_width   = minimum_size.width;
	size_hints->min_height  = minimum_size.height;
	size_hints->flags |= PMinSize;
	XSetWMNormalHints(disp, window, size_hints);
}

void CL_X11Window::set_maximum_size( int width, int height, bool client_area)
{
	maximum_size = CL_Size(width,height);

	long user_hints;
	XGetWMNormalHints(disp, window, size_hints, &user_hints);
	size_hints->max_width   = maximum_size.width;
	size_hints->max_height  = maximum_size.height;
	size_hints->flags |= PMaxSize;
	XSetWMNormalHints(disp, window, size_hints);
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


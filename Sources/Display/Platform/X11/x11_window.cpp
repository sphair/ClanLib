/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "x11_window.h"
#include "input_device_provider_x11keyboard.h"
#include "input_device_provider_x11mouse.h"
#ifdef HAVE_LINUX_JOYSTICK_H
#include "input_device_provider_linuxjoystick.h"
#endif
#include "display_message_queue_x11.h"
#include <X11/Xatom.h>
#include <cstdio>
#include "../../Window/input_context_impl.h"
#include <X11/XKBlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include "../../setup_display.h"

#ifndef MWM_HINTS_FUNCTIONS
/* bit definitions for MwmHints.flags */
#define MWM_HINTS_FUNCTIONS	(1L << 0)
#define MWM_HINTS_DECORATIONS	(1L << 1)
#define MWM_HINTS_INPUT_MODE	(1L << 2)
#define MWM_HINTS_STATUS	(1L << 3)
#endif

#ifndef MWM_FUNC_ALL
/* bit definitions for MwmHints.functions */
#define MWM_FUNC_ALL		(1L << 0)
#define MWM_FUNC_RESIZE		(1L << 1)
#define MWM_FUNC_MOVE		(1L << 2)
#define MWM_FUNC_MINIMIZE	(1L << 3)
#define MWM_FUNC_MAXIMIZE	(1L << 4)
#define MWM_FUNC_CLOSE		(1L << 5)
#endif

#ifndef MWM_DECOR_ALL
/* bit definitions for MwmHints.decorations */
#define MWM_DECOR_ALL		(1L << 0)
#define MWM_DECOR_BORDER	(1L << 1)
#define MWM_DECOR_RESIZEH	(1L << 2)
#define MWM_DECOR_TITLE		(1L << 3)
#define MWM_DECOR_MENU		(1L << 4)
#define MWM_DECOR_MINIMIZE	(1L << 5)
#define MWM_DECOR_MAXIMIZE	(1L << 6)
#endif

#ifndef _NET_WM_STATE_REMOVE
#define _NET_WM_STATE_REMOVE  0
#define _NET_WM_STATE_ADD     1
#define _NET_WM_STATE_TOGGLE  2
#endif

#ifndef KDE_noDecoration
#define KDE_noDecoration              0
#define KDE_normalDecoration          1
#define KDE_tinyDecoration            2
#endif

namespace clan
{

// From Xm/MwmUtil.h
typedef struct
{
	// These were "int", but only work at "long". Is this okay on a 32bit platform?
	long flags;
	long functions;
	long decorations;
	long input_mode;
	long status;
} ClanLib_MotifWmHints;

X11Window::X11Window()
: color_map(0), system_cursor(0), hidden_cursor(0), cursor_bitmap(0), size_hints(nullptr),
  minimized(false), maximized(false), restore_to_maximized(false), fullscreen(false),
  wm_protocols(None), wm_delete_window(None), wm_state(None), motif_wm_hints(None), net_wm_state(None), net_wm_state_maximized_vert(None),
  net_wm_state_maximized_horz(None), net_wm_state_hidden(None), net_wm_state_fullscreen(None), kwm_win_decoration(None), win_hints(None),
  net_wm_ping(None), net_frame_extents(None),
  is_window_mapped(false),
  site(nullptr), clipboard(this),
  always_send_window_position_changed_event(false), always_send_window_size_changed_event(false)

{
	handle.display = SetupDisplay::get_message_queue()->get_display();

	last_repaint_rect.reserve(32);
	keyboard = InputDevice(new InputDeviceProvider_X11Keyboard(this));
	mouse = InputDevice(new InputDeviceProvider_X11Mouse(this));

	SetupDisplay::get_message_queue()->add_client(this);
}

X11Window::~X11Window()
{
	SetupDisplay::get_message_queue()->remove_client(this);
	SetupDisplay::get_message_queue()->set_mouse_capture(this, false);

	ic.dispose();

	get_keyboard()->dispose();
	get_mouse()->dispose();

	for (auto & elem : joysticks)
		elem.get_provider()->dispose();

	close_window();
}

void X11Window::create(XVisualInfo *visual, DisplayWindowSite *new_site, const DisplayWindowDescription &desc)
{
	site = new_site;

	close_window();		// Reset all variables

	current_screen = visual->screen;

	int disp_width_px = XDisplayWidth(handle.display, current_screen);
	int disp_height_px = XDisplayHeight(handle.display, current_screen);
	int disp_width_mm = XDisplayWidthMM(handle.display, current_screen);

	// Get DPI of screen or use 96.0f if Xlib doesn't have a value.
	ppi = (disp_width_mm < 24) ? 96.0f : (25.4f * static_cast<float>(disp_width_px) / static_cast<float>(disp_width_mm));

	// Update pixel ratio.
	set_pixel_ratio(pixel_ratio);

	color_map = XCreateColormap(handle.display, RootWindow(handle.display, visual->screen), visual->visual, AllocNone);

	XSetWindowAttributes attr;
	memset(&attr, 0, sizeof(attr));

	attr.border_pixel = 0;
	attr.override_redirect = False;
	attr.colormap = color_map;
	attr.event_mask =
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		EnterWindowMask |
		LeaveWindowMask |
		PointerMotionMask |
		KeymapStateMask |
		ExposureMask |
		StructureNotifyMask |
		FocusChangeMask |
		PropertyChangeMask;

	// retrieve some useful atoms
	motif_wm_hints = XInternAtom(handle.display, "_MOTIF_WM_HINTS", True);
	wm_protocols = XInternAtom(handle.display, "WM_PROTOCOLS", True);
	wm_delete_window = XInternAtom(handle.display, "WM_DELETE_WINDOW", True);
	net_wm_ping = XInternAtom(handle.display, "_NET_WM_PING", True);
	wm_state = XInternAtom(handle.display, "WM_STATE", True);
	net_wm_state = XInternAtom(handle.display, "_NET_WM_STATE", True);
	net_wm_state_maximized_vert = XInternAtom(handle.display, "_NET_WM_STATE_MAXIMIZED_VERT", True);
	net_wm_state_maximized_horz = XInternAtom(handle.display, "_NET_WM_STATE_MAXIMIZED_HORZ", True);
	net_wm_state_hidden = XInternAtom(handle.display, "_NET_WM_STATE_HIDDEN", True);
	net_wm_state_fullscreen = XInternAtom(handle.display, "_NET_WM_STATE_FULLSCREEN", True);
	net_frame_extents = XInternAtom(handle.display, "_NET_FRAME_EXTENTS", True);
	kwm_win_decoration = XInternAtom(handle.display, "KWM_WIN_DECORATION", True);
	win_hints = XInternAtom(handle.display, "_WIN_HINTS", True);
	Atom net_wm_window_type = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE", True);
	Atom net_wm_window_type_desktop = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_DESKTOP", True);
	Atom net_wm_window_type_dock = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_DOCK", True);
	Atom net_wm_window_type_toolbar = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_TOOLBAR", True);
	Atom net_wm_window_type_menu = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_MENU", True);
	Atom net_wm_window_type_utility = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_UTILITY", True);
	Atom net_wm_window_type_splash = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_SPLASH", True);
	Atom net_wm_window_type_dialog = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_DIALOG", True);
	Atom net_wm_window_type_dropdown_menu = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", True);
	Atom net_wm_window_type_popup_menu = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_POPUP_MENU", True);
	Atom net_wm_window_type_tooltip = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_TOOLTIP", True);
	Atom net_wm_window_type_notification = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_NOTIFICATION", True);
	Atom net_wm_window_type_combo = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_COMBO", True);
	Atom net_wm_window_type_dnd = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_DND", True);
	Atom net_wm_window_type_normal = XInternAtom(handle.display, "_NET_WM_WINDOW_TYPE_NORMAL", True);
	Atom kde_net_wm_window_type_override = XInternAtom(handle.display, "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE", True);

	size_hints = XAllocSizeHints();
	if (!size_hints)
		throw Exception("Cannot allocate X11 XSizeHints structure");

	system_cursor = XCreateFontCursor(handle.display, XC_left_ptr); // This is allowed to fail

	int win_x = desc.get_position().left * pixel_ratio;
	int win_y = desc.get_position().top * pixel_ratio;
	int win_width = desc.get_size().width * pixel_ratio;
	int win_height = desc.get_size().height * pixel_ratio;

	// Check for valid width and height
	if (win_width <= 0)
	{
		throw Exception("Invalid window width.");
		// win_width = 128;
	}

	if (win_height <= 0)
	{
		throw Exception("Invalid window height.");
		// win_height = 128;
	}

	if (desc.is_fullscreen())
	{
		win_x = 0;
		win_y = 0;
	}

	if (win_x == -1 && win_y == -1)
	{
		win_x = (disp_width_px - win_width)/2 - 1;
		win_y = (disp_height_px - win_height)/2 - 1;
	}

	handle.window = XCreateWindow(
			handle.display, RootWindow(handle.display, current_screen),
			win_x, win_y, win_width, win_height, 0, visual->depth,
			InputOutput, visual->visual,
			CWBorderPixel | CWColormap | CWOverrideRedirect | CWEventMask,
			&attr
			);

	if (!handle.window)
		throw Exception("Unable to create the X11 window");

	if (!desc.get_owner().is_null())
	{
		DisplayWindow owner = desc.get_owner();
		XSetTransientForHint(handle.display, handle.window, owner.get_handle().window);
	}

	// Setup the hidden cursor (Maybe this should be done only once when required)
	char data[64]; // 8x8
	memset(data, 0, 64);

	XColor black_color;
	memset(&black_color, 0, sizeof(black_color));

	cursor_bitmap = XCreateBitmapFromData(handle.display, handle.window, data, 8, 8);
	hidden_cursor = XCreatePixmapCursor(handle.display, cursor_bitmap, cursor_bitmap, &black_color, &black_color, 0,0 );

	// set title of window:
	set_title(desc.get_title());

	// Inform the window manager who we are
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) > -1)
	{
		hostname[sizeof(hostname)-1] = 0;
		long pid = getpid();
		if (pid > 0)
		{
			Atom atom = XInternAtom(handle.display, "_NET_WM_PID", False);
			if (atom)
			{
				long pid = getpid();
				XChangeProperty(handle.display, handle.window, atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &pid, 1);
			}

			atom = XInternAtom(handle.display, "WM_CLIENT_MACHINE", False);
			if (atom)
			{
				long pid = getpid();
				XChangeProperty(handle.display, handle.window, atom, XA_STRING, 8, PropModeReplace, (unsigned char *) hostname, strlen(hostname));
			}
		}
	}

	// Retrieve the frame size (emulating microsoft windows)
	bool window_is_frameless = false;
	bool window_has_thin_frame = false;
	if (!desc.has_caption())
	{
		if (desc.get_allow_resize())
		{
			window_has_thin_frame = true;
		}

		if (desc.is_popup())
		{
			window_is_frameless = true;
		}
	}

	if (net_wm_window_type != None)
	{
		Atom decor;
		if (desc.is_dialog())
		{
			decor = net_wm_window_type_dialog;
		}
		else if (window_is_frameless)
		{
			decor = net_wm_window_type_dropdown_menu;
		}
		else
		{
			decor = net_wm_window_type_normal;
		}

		if (decor)	// Ensure the atom exists
		{
			XChangeProperty(handle.display, handle.window, net_wm_window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&decor, 1);
		}

		if (window_is_frameless)	// Attempt frameless windows for KDE 2 and 3, if dropdown menus are not supported
		{
			if (kde_net_wm_window_type_override)
				XChangeProperty(handle.display, handle.window, net_wm_window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&kde_net_wm_window_type_override, 1);
		}
	}

	// Do not use an else clause here, because on the net_wm_window_type may not turn off the border
	if ( motif_wm_hints != None )
	{
		long decor = 0;
		if (!window_is_frameless)
		{
			if (window_has_thin_frame)
			{
				decor |= MWM_DECOR_BORDER;
			}
			else
			{
				if (desc.get_allow_resize())
					decor |= MWM_DECOR_RESIZEH;

				if (desc.has_caption())
					decor |= MWM_DECOR_TITLE | MWM_DECOR_MENU;

				if (desc.has_maximize_button())
					decor |= MWM_DECOR_MAXIMIZE;

				if (desc.has_minimize_button())
					decor |= MWM_DECOR_MINIMIZE;
			}
		}

		ClanLib_MotifWmHints hints = { MWM_HINTS_DECORATIONS, 0, decor, 0, 0 };
		XChangeProperty(handle.display, handle.window, motif_wm_hints, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&hints, 5);

	}
	else if (kwm_win_decoration != None)
	{
		long decor;
		if (window_is_frameless)
		{
			decor = KDE_noDecoration;
		}
		else
		{
			decor = window_has_thin_frame ? KDE_tinyDecoration : -1;
		}

		if (decor >= 0)
		{
			XChangeProperty(handle.display, handle.window, kwm_win_decoration, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&decor, 1);
		}
	}
	else if (win_hints)
	{
		long decor;
		if (window_is_frameless)
		{
			decor = 0;
		}
		else
		{
			decor = window_has_thin_frame ? 0 : -1;
		}

		if (decor >= 0)
		{
			XChangeProperty(handle.display, handle.window, win_hints, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&decor, 1);
		}
	}

	resize_enabled = desc.get_allow_resize() || desc.is_fullscreen(); // Fs needs resizable window

	if (resize_enabled)
	{
		minimum_size = Size(32, 32);
		maximum_size = Size(0, 0);	// No maximum size by default
	}
	else
	{
		minimum_size = Size(win_width, win_height);
		maximum_size = Size(win_width, win_height);
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
	size_hints->width_inc   = 1;
	size_hints->height_inc  = 1;
	size_hints->win_gravity = NorthWestGravity;
	size_hints->flags       = PSize|PBaseSize|PPosition|PMinSize|PResizeInc|PWinGravity;

	if (!resize_enabled)
	{
		size_hints->flags |= PMaxSize;
	}
	XSetWMNormalHints(handle.display, handle.window, size_hints);

	// subscribe to window manager events
	XSetWMProtocols(handle.display, handle.window, &wm_delete_window, 1);

	// Make auto repeat keys detectable
	Bool supports_detectable_autorepeat;
	XkbSetDetectableAutoRepeat(handle.display,1,&supports_detectable_autorepeat);

	fullscreen = desc.is_fullscreen() && net_wm_state && net_wm_state_fullscreen;
	if (fullscreen)
	{
		Atom atom1 = net_wm_state_fullscreen;
		XChangeProperty(handle.display, handle.window, net_wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&atom1, 1);
	}

	// Guess the initial position (until the window is mapped)
	requested_size_contains_frame = !desc.get_position_client_area();
	requested_current_window_client_area = Rect(win_x, win_y, Size(win_width, win_height));
	current_window_client_area = requested_current_window_client_area;

	// Set window visibility
	if (desc.is_visible())
	{
		show(false);
	}
	minimized = is_minimized();
	maximized = is_maximized();
	restore_to_maximized = maximized;

	// Setup the clipboard
	clipboard.setup();

	// Create input devices for window:
	ic.clear();
	ic.add_keyboard(keyboard);
	ic.add_mouse(mouse);

	// Go looking for joysticks:
	setup_joysticks();
}

void X11Window::calculate_window_frame_size()
{
	if (!net_frame_extents)
		return;

	unsigned long number_items;
	int actual_format;
	Atom actual_type;

	// _NET_FRAME_EXTENTS, left, right, top, bottom, CARDINAL[4]/32
	unsigned char *data = get_property(handle.window, net_frame_extents, &number_items, &actual_format, &actual_type);
	if (data != nullptr)
	{
		if (number_items >= 4)
		{
			frame_size_left = ((long *)data)[0];
			frame_size_right = ((long *)data)[1];
			frame_size_top = ((long *)data)[2];
			frame_size_bottom = ((long *)data)[3];
		}
		XFree(data);
	}
}

void X11Window::close_window()
{
	current_window_events.clear();

	if (handle.window)
	{
		XDestroyWindow(handle.display, handle.window);
		handle.window = 0;
	}

	if (system_cursor)
	{
		XFreeCursor(handle.display, system_cursor);
		system_cursor = 0;
	}

	if (hidden_cursor)
	{
		XFreeCursor(handle.display, hidden_cursor);
		hidden_cursor = 0;
	}

	if (cursor_bitmap)
	{
		XFreePixmap(handle.display, cursor_bitmap);
		cursor_bitmap = 0;
	}

	if (color_map)
	{
		XFreeColormap(handle.display, color_map);
		color_map = 0;
	}

	if (size_hints)
	{
		XFree(size_hints);
		size_hints = nullptr;
	}

	wm_protocols = None;
	wm_delete_window = None;
	wm_state = None;
	net_wm_ping = None;
	motif_wm_hints = None;
	net_wm_state = None;
	net_wm_state_maximized_vert = None;
	net_wm_state_maximized_horz = None;
	net_wm_state_hidden = None;
	net_wm_state_fullscreen = None;
	net_frame_extents = None;
	kwm_win_decoration = None;
	win_hints = None;

	frame_size_left = 0;
	frame_size_right = 0;
	frame_size_top = 0;
	frame_size_bottom = 0;
	frame_size_calculated = false;
}

Rect X11Window::get_geometry() const
{
	Rect rect = requested_current_window_client_area;
	rect.left -=frame_size_left;
	rect.top -=frame_size_top;
	rect.right += frame_size_right;
	rect.bottom += frame_size_bottom;
	return rect;
}

Rect X11Window::get_viewport() const
{
	return Rect(0, 0, requested_current_window_client_area.get_size());
}

bool X11Window::has_focus() const
{
	Window focus_return;
	int revert_to_return;
	XGetInputFocus(handle.display, &focus_return, &revert_to_return);
	if (focus_return == handle.window) return true;
	return false;
}

bool X11Window::is_minimized() const
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
		unsigned char *data = get_property(handle.window, wm_state, &number_items, &actual_format, &actual_type);
		if (data != nullptr)
		{
			long state = *(long *)data;
			XFree(data);
			return state == IconicState;
		}
	}

	return false;
}

bool X11Window::is_maximized() const
{
	return check_net_wm_state(net_wm_state_maximized_vert, net_wm_state_maximized_horz);
}

bool X11Window::is_visible() const
{
	XWindowAttributes attr;
	XGetWindowAttributes(handle.display, handle.window, &attr);
	if (attr.map_state == IsViewable) return false;
	return true;
}

Size X11Window::get_minimum_size(bool client_area) const
{
	if (!client_area)
		return minimum_size;
	else
		throw Exception("X11Window::get_minimum_size() not implemented for client_area");
}

Size X11Window::get_maximum_size(bool client_area) const
{
	if (!client_area)
		return maximum_size;
	else
		throw Exception("X11Window::get_maximum_size() not implemented for client_area");
}

std::string X11Window::get_title() const
{
	return window_title;
}

Point X11Window::client_to_screen(const Point &client)
{
	Rect view = requested_current_window_client_area;
	return Point(view.left + client.x, view.top + client.y);
}

Point X11Window::screen_to_client(const Point &screen)
{
	Rect view = requested_current_window_client_area;
	return Point(screen.x - view.left, screen.y - view.top);
}

void X11Window::show_system_cursor()
{
	XDefineCursor(handle.display, handle.window, system_cursor);
}

void X11Window::set_cursor(StandardCursor type)
{
	if (system_cursor)
	{
		XFreeCursor(handle.display, system_cursor);
		system_cursor = 0;
	}
	unsigned int index = XC_left_ptr;
	switch (type)
	{
		case StandardCursor::arrow:
			index = XC_left_ptr;
			break;
		case StandardCursor::appstarting:
			index = XC_watch;
			break;
		case StandardCursor::cross:
			index = XC_cross;
			break;
		case StandardCursor::hand:
			index = XC_hand2;
			break;
		case StandardCursor::ibeam:
			index = XC_xterm;
			break;
		case StandardCursor::size_all:
			index = XC_fleur;
			break;
		case StandardCursor::size_ns:
			index = XC_double_arrow;
			break;
		case StandardCursor::size_we:
			index = XC_sb_h_double_arrow;
			break;
		case StandardCursor::uparrow:
			index = XC_sb_up_arrow;
			break;
		case StandardCursor::wait:
			index = XC_watch;
			break;
		case StandardCursor::no:
			index = XC_X_cursor;
			break;
		case StandardCursor::size_nesw:
		case StandardCursor::size_nwse:
		default:
			break;
	}
	system_cursor = XCreateFontCursor(handle.display, index);
	XDefineCursor(handle.display, handle.window, system_cursor);
}

void X11Window::hide_system_cursor()
{
	XDefineCursor(handle.display, handle.window, hidden_cursor);
}

void X11Window::set_title(const std::string &new_title)
{
	window_title = new_title;
	XSetStandardProperties(handle.display, handle.window, new_title.c_str(), new_title.c_str(), None, nullptr, 0, nullptr);
}

void X11Window::set_position(const Rect &pos, bool client_area)
{
	always_send_window_position_changed_event = true;
	always_send_window_size_changed_event = true;

	if (!frame_size_calculated)	// If the frame size has not yet been calculated, we delay setting the window position until later (when mapped)
	{
		requested_current_window_client_area = pos;
		requested_size_contains_frame = !client_area;
		return;
	}
	if (!resize_enabled)	// If resize has been disabled, we have to temporary enable it
	{
		long user_hints;
		XGetWMNormalHints(handle.display, handle.window, size_hints, &user_hints);
		size_hints->flags &= ~(PMinSize | PMaxSize);
		XSetWMNormalHints(handle.display, handle.window, size_hints);
	}

	int width = pos.get_width();
	int height = pos.get_height();

	if (client_area)
	{
		requested_current_window_client_area = pos;
		XMoveResizeWindow(handle.display, handle.window, pos.left - frame_size_left, pos.top - frame_size_top, width, height);
	}
	else
	{
		width = width - frame_size_left - frame_size_right;
		height = height - frame_size_top - frame_size_bottom;
		requested_current_window_client_area = Rect(pos.left + frame_size_left, pos.top + frame_size_top, Size(width, height));
		XMoveResizeWindow(handle.display, handle.window, pos.left, pos.top, width, height);
	}

	if (!resize_enabled)	// resize has been temporary enabled
	{
		size_hints->min_width   = width;
		size_hints->min_height  = height;
		size_hints->max_width   = width;
		size_hints->max_height  = height;
		size_hints->flags |= PMinSize | PMaxSize;
		XSetWMNormalHints(handle.display, handle.window, size_hints);
	}
}

void X11Window::set_size(int width, int height, bool client_area)
{
	always_send_window_size_changed_event = true;

	if (!frame_size_calculated)	// If the frame size has not yet been calculated, we delay setting the window position until later (when mapped)
	{
		requested_current_window_client_area = Rect(requested_current_window_client_area.left, requested_current_window_client_area.top, Size(width, height));
		requested_size_contains_frame = !client_area;
		return;
	}

	if (!resize_enabled)	// If resize has been disabled, we have to temporary enable it
	{
		long user_hints;
		XGetWMNormalHints(handle.display, handle.window, size_hints, &user_hints);
		size_hints->flags &= ~(PMinSize | PMaxSize);
		XSetWMNormalHints(handle.display, handle.window, size_hints);
	}

	if (client_area)
	{
		requested_current_window_client_area = Rect(requested_current_window_client_area.left, requested_current_window_client_area.top, Size(width, height));

		XResizeWindow(handle.display, handle.window, width, height);
	}
	else
	{
		width = width - frame_size_left - frame_size_right;
		height = height - frame_size_top - frame_size_bottom;
		if (width < 1)
			width = 1;
		if (height < 1)
			height = 1;

		requested_current_window_client_area = Rect(requested_current_window_client_area.left, requested_current_window_client_area.top, Size(width, height));

		XResizeWindow(handle.display, handle.window, width, height);
	}

	if (!resize_enabled)	// resize has been temporary enabled
	{
		size_hints->min_width   = width;
		size_hints->min_height  = height;
		size_hints->max_width   = width;
		size_hints->max_height  = height;
		size_hints->flags |= PMinSize | PMaxSize;
		XSetWMNormalHints(handle.display, handle.window, size_hints);
	}

}

void X11Window::set_enabled(bool enable)
{
	XSetWindowAttributes attr;
	attr.event_mask =
		( enable
			? KeyPressMask
			| KeyReleaseMask
			| ButtonPressMask
			| ButtonReleaseMask
			| PointerMotionMask
			: 0
		)
		| EnterWindowMask
		| LeaveWindowMask
		| KeymapStateMask
		| ExposureMask
		| StructureNotifyMask
		| FocusChangeMask
		| PropertyChangeMask;

	XChangeWindowAttributes(handle.display, handle.window, CWEventMask, &attr);
}

void X11Window::minimize()
{
	if (!is_minimized())
		restore_to_maximized = is_maximized();
	XIconifyWindow(handle.display, handle.window, current_screen);
}

void X11Window::restore()
{
	if (is_minimized())
	{
		if (restore_to_maximized)
		{
			maximize();
		}
		else
		{
			map_window();
		}
	}
	else if (is_maximized())
	{
		modify_net_wm_state(false, net_wm_state_maximized_vert, net_wm_state_maximized_horz);
	}
}

void X11Window::map_window()
{
	clear_structurenotify_events();

	if (!is_window_mapped)
	{
		int result = XMapWindow(handle.display, handle.window);
		if ( (result == BadValue) || (result == BadWindow) )
		{
			throw Exception("Failed to map window");
		}
		else
		{
			XEvent event;
			do {
				XWindowEvent(handle.display, handle.window, StructureNotifyMask, &event);
			}while ( event.type != MapNotify );

			is_window_mapped = true;

			if (fullscreen)
				XSetInputFocus(handle.display, handle.window, RevertToParent, CurrentTime);
		}

		if (!frame_size_calculated)
		{
			frame_size_calculated = true;
			calculate_window_frame_size();

			if (!fullscreen)
			{
				// Now we know the frame size, nudge the window into the exact position
				Rect frame_size = requested_current_window_client_area;	// Must copy as set_position() changes this variable, causing all sorts of problems
				set_position(frame_size, !requested_size_contains_frame);
			}
			current_window_client_area = get_screen_position();
			requested_current_window_client_area = current_window_client_area;
		}

		always_send_window_position_changed_event = true;
		always_send_window_size_changed_event = true;
	}
}

void X11Window::unmap_window()
{
	clear_structurenotify_events();
	if (is_window_mapped)
	{
		int result = XUnmapWindow(handle.display, handle.window);
		if ( (result == BadValue) || (result == BadWindow) )
		{
			throw Exception("Failed to unmap window");
		}
		else
		{
			XEvent event;
			do {
				XWindowEvent(handle.display, handle.window, StructureNotifyMask, &event);
			}while (event.type != UnmapNotify);

			is_window_mapped = false;
		}
	}
}

void X11Window::maximize()
{
	modify_net_wm_state(true, net_wm_state_maximized_vert, net_wm_state_maximized_horz);
}

void X11Window::show(bool activate)
{
	map_window();
	if (activate) set_enabled(true);
}

void X11Window::hide()
{
	set_enabled(false);
	unmap_window();
}

void X11Window::bring_to_front()
{
	XRaiseWindow(handle.display, handle.window);
}

void X11Window::capture_mouse(bool capture)
{
	SetupDisplay::get_message_queue()->set_mouse_capture(this, capture);
}

void X11Window::clear_structurenotify_events()
{
	XEvent event;
	while( XCheckMaskEvent(handle.display, StructureNotifyMask, &event));

}

bool X11Window::check_net_wm_state(Atom atom1, Atom atom2) const
{
	unsigned long number_items;
	int actual_format;
	Atom actual_type;

	// search for atom1 or atom2 in _NET_WM_STATE array
	if (net_wm_state != None)
	{
		unsigned char *data = get_property(handle.window, net_wm_state, &number_items, &actual_format, &actual_type);
		if (data != nullptr)
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

bool X11Window::modify_net_wm_state(bool add, Atom atom1, Atom atom2)
{
	// change _NET_WM_STATE property, see: http://standards.freedesktop.org/wm-spec/wm-spec-latest.html
	if (net_wm_state != None)
	{
		XEvent xev;
		memset(&xev, 0, sizeof(xev));
		xev.xclient.type = ClientMessage;
		xev.xclient.window = handle.window;
		xev.xclient.message_type = net_wm_state;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
		xev.xclient.data.l[1] = atom1;
		xev.xclient.data.l[2] = atom2;
		xev.xclient.data.l[3] = None;
		return XSendEvent(handle.display, DefaultRootWindow(handle.display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev) != 0;
	}

	return false;
}

Rect X11Window::get_screen_position() const
{
	int xpos;
	int ypos;
	unsigned int width;
	unsigned int height;
	Window *children_ptr;
	unsigned int num_child;
	Window temp_window;
	XWindowAttributes attr;

	XLockDisplay(handle.display);

	XGetWindowAttributes(handle.display, handle.window, &attr);

	xpos = attr.x;
	ypos = attr.y;
	width = attr.width;
	height = attr.height;

	Window current_window = handle.window;
	while(true)
	{
		children_ptr = nullptr;
		XQueryTree(handle.display, current_window, &temp_window, &current_window, &children_ptr, &num_child);
		if (children_ptr)
			XFree(children_ptr);

		if (!current_window) break;

		XGetWindowAttributes(handle.display, current_window, &attr);
		xpos += attr.x;
		ypos += attr.y;
	}
	XUnlockDisplay(handle.display);

	return (Rect(xpos, ypos, width+xpos, height+ypos));

}

//Rect X11Window::get_client_position() const
//{
//	Window root_window;
//	int x, y;
//	unsigned int width, height;
//	unsigned int depth, border_width;
//	XGetGeometry(handle.display, handle.window, &root_window, &x, &y, &width, &height, &border_width, &depth);
//
//	Window child_return;
//	int newx, newy;
//	XTranslateCoordinates(handle.display, handle.window, root_window, x, y, &newx, &newy, &child_return);
//
//	return Rect(newx, newy, Size(width, height));
//}

void X11Window::process_window_resize(const Rect &new_rect)
{
	Rect rect = current_window_client_area;
	requested_current_window_client_area = current_window_client_area = new_rect;

	if (site)
	{
		if ( (rect.left != current_window_client_area.left) || (rect.top != current_window_client_area.top) || always_send_window_position_changed_event )
		{
			(site->sig_window_moved)();
		}

		if ( (rect.get_width() != current_window_client_area.get_width()) || (rect.get_height() != current_window_client_area.get_height()) || always_send_window_size_changed_event )
		{
			Rectf rectf = rect;
			rectf.left   /= pixel_ratio,
			rectf.top    /= pixel_ratio,
			rectf.right  /= pixel_ratio,
			rectf.bottom /= pixel_ratio;

			if (site->func_window_resize)
				(site->func_window_resize)(rectf);

			if (callback_on_resized)
				callback_on_resized();

			(site->sig_resize)(rectf.get_width(), rectf.get_height());
			rect = rectf;
		}
	}
	always_send_window_position_changed_event = false;

}

void X11Window::process_message(XEvent &event, X11Window *mouse_capture_window)
{

	Rect rect;

	bool process_input_context = false;
	switch(event.type)
	{
		//Resize or Move
		case ConfigureNotify:
		{
			// From http://tronche.com/gui/x/icccm/sec-4.html
			// (A client will receive a synthetic ConfigureNotify event that describes the (unchanged) geometry of the window)
			// (The client will not receive a real ConfigureNotify event because no change has actually taken place.)

			if (event.xany.send_event != 0)
			{
				int bw = event.xconfigure.border_width;
				rect = Rect(
					event.xconfigure.x + bw,
					event.xconfigure.y + bw,
					event.xconfigure.x + bw + event.xconfigure.width,
					event.xconfigure.y + bw + event.xconfigure.height
				);
			}
			else
			{
				rect = get_screen_position();
			}
			process_window_resize(rect);

			break;
		}
		case ClientMessage:
			// handle window manager messages
			if (wm_protocols)
			{
				if (event.xclient.message_type == wm_protocols)
				{
					if (wm_delete_window)
					{
						if (event.xclient.data.l[0] == wm_delete_window)
						{
							if (site)
								(site->sig_window_close)();
						}
					}
					if (net_wm_ping)
					{
						if (event.xclient.data.l[0] == net_wm_ping)
						{
							XSendEvent( handle.display, RootWindow(handle.display, current_screen), False, SubstructureNotifyMask | SubstructureRedirectMask, &event );
						}
					}
				}
			}
			break;
		case Expose:
			// Repaint notification

			if (always_send_window_position_changed_event)	// Sometimes ConfigureNotify notifications are not sent, so make sure here that we are valid
			{
				rect = get_screen_position();
				process_window_resize(rect);
			}

			if (!site)
				break;

			if (exposed_rects.empty())	// First call, reserve some additional memory as required
			{
				unsigned int num_exposed = event.xexpose.count;
				exposed_rects.reserve(max_allowable_expose_events);
			}

			if (exposed_rects.size() < max_allowable_expose_events)
			{
				rect = Rect(event.xexpose.x, event.xexpose.y,
					event.xexpose.x + event.xexpose.width, event.xexpose.y + event.xexpose.height);

				exposed_rects.push_back(rect);

				// For optimisation later on, calculate the largest exposed rect
				if ((largest_exposed_rect.get_width() * largest_exposed_rect.get_height()) < (rect.get_width() * rect.get_height()))
				{
					largest_exposed_rect = rect;
				}
			}
			break;
		case FocusIn:
			if (site)
				(site->sig_got_focus)();

			break;
		case FocusOut:
			if (site)
			{
				if (!has_focus())	// For an unknown reason, FocusOut is called when clicking on title bar of window
				{
					(site->sig_lost_focus)();
				}
			}
			break;
		case PropertyNotify:
			if (!site)
				break;

			if (net_wm_state != None)
			{
				if (event.xproperty.atom == net_wm_state && event.xproperty.state == PropertyNewValue)
				{
					if (is_minimized())
					{
						if (!minimized && site != nullptr)
							(site->sig_window_minimized)();
						minimized = true;
						maximized = false;
					}
					else if (is_maximized())
					{
						if (!maximized && site != nullptr)
							(site->sig_window_maximized)();
						if (minimized && site != nullptr)
						{
							// generate resize events for minimized -> maximized transition
							Rectf rectf = get_geometry();
							rectf.left   /= pixel_ratio,
							rectf.top    /= pixel_ratio,
							rectf.right  /= pixel_ratio,
							rectf.bottom /= pixel_ratio;

							(site->sig_window_moved)();
							if (site->func_window_resize)
							{
								(site->func_window_resize)(rectf);
								rect = rectf;
							}
							if (callback_on_resized)
								callback_on_resized();
							(site->sig_resize)(rectf.get_width(), rectf.get_height());
						}
						minimized = false;
						maximized = true;
					}
					else
					{
						if ((minimized || maximized) && site != nullptr)
							(site->sig_window_restored)();
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
						if (!minimized && site != nullptr)
							(site->sig_window_minimized)();
						minimized = true;
					}
					else
					{
						if (minimized && site != nullptr)
							(site->sig_window_restored)();
						minimized = false;
					}
				}
			}
			break;
		case KeyRelease:
		case KeyPress:
			if (get_keyboard())
			{
				get_keyboard()->received_keyboard_input(event.xkey);
				process_input_context = true;
			}
			break;
		case ButtonPress:
		case ButtonRelease:
			if (mouse_capture_window->get_mouse() && event.xany.send_event==0)
			{
				if (callback_on_clicked)
				{
					// This callback is required for GL layered windows
					if (!callback_on_clicked(event.xbutton))
						break;
				}

				// Adjust to what clanlib client expects
				event.xmotion.x = event.xmotion.x_root - requested_current_window_client_area.left;
				event.xmotion.y = event.xmotion.y_root - requested_current_window_client_area.top;

				if (this != mouse_capture_window)
				{
					Rect this_scr = current_window_client_area;
					Rect capture_scr = mouse_capture_window->current_window_client_area;

					event.xbutton.x += this_scr.left - capture_scr.left;
					event.xbutton.y += this_scr.top - capture_scr.top;
				}

				mouse_capture_window->get_mouse()->received_mouse_input(event.xbutton);
				process_input_context = true;
			}
			break;
		case MotionNotify:
			if (mouse_capture_window->get_mouse() && event.xany.send_event==0)
			{
				// Adjust to what clanlib client expects
				event.xmotion.x = event.xmotion.x_root - requested_current_window_client_area.left;
				event.xmotion.y = event.xmotion.y_root - requested_current_window_client_area.top;
				if (this != mouse_capture_window)
				{
					Rect this_scr = current_window_client_area;
					Rect capture_scr = mouse_capture_window->current_window_client_area;

					event.xmotion.x += this_scr.left - capture_scr.left;
					event.xmotion.y += this_scr.top - capture_scr.top;
				}

				mouse_capture_window->get_mouse()->received_mouse_move(event.xmotion);
				process_input_context = true;
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

	if (process_input_context)
	{
		// Immediately dispatch any messages queued (to ensure any later event is adjusted for window geometry or cursor changes)
		ic.process_messages();
		if (ic.is_disposed())
			return;		// Disposed, thefore "this" is invalid, must exit now
	}

}
void X11Window::process_message_complete()
{
	process_window_sockets(false);		// Check input devices

	// Send any exposure events, unless they have already been sent
	unsigned int max = exposed_rects.size();

	if (max==1)	// Simple case, a single rect
	{
		(site->sig_paint)(largest_exposed_rect);
	}
	else if (max >= max_allowable_expose_events)
	{
		Rect window_rect = get_viewport();
		(site->sig_paint)(window_rect);
	}
	else if (max > 1)
	{
		// Send the largest rect first
		(site->sig_paint)(largest_exposed_rect);
		for (unsigned int cnt=0; cnt < max; cnt++)
		{
			Rect &rect = exposed_rects[cnt];

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
				(site->sig_paint)(rect);
			}
		}
	}
	exposed_rects.clear();
	largest_exposed_rect = Rect();
}

void X11Window::setup_joysticks()
{
	for (auto & elem : joysticks)
		elem.get_provider()->dispose();
	joysticks.clear();

#ifdef HAVE_LINUX_JOYSTICK_H

	std::string joydev;
	if (access("/dev/input/", R_OK | X_OK) == 0)
		joydev = "/dev/input/js%1";
	else
		joydev = "/dev/js%1";

	const int max_joysticks = 16;
	for(int i = 0; i < max_joysticks; ++i)
	{
		std::string pathname = string_format(joydev, i);

		if (access(pathname.c_str(), R_OK) == 0)
		{
			try
			{
				auto joystick_provider = new InputDeviceProvider_LinuxJoystick(this, pathname);
				InputDevice device(joystick_provider);
				joysticks.push_back(device);
				ic.add_joystick(device);

				current_window_events.push_back(joystick_provider->get_fd());

			}
			catch (Exception error)
			{
				log_event("debug", "Joystick Error: %1", error.message);
			}
		}
	}
#endif
}

//void X11Window::set_fullscreen()
//{
//	modify_net_wm_state(true, net_wm_state_fullscreen);
//}

void X11Window::set_clipboard_text(const std::string &text)
{
	clipboard.set_clipboard_text(text);
}

void X11Window::set_clipboard_image(const PixelBuffer &buf)
{
	throw Exception("Todo: X11Window::set_clipboard_image");
}

std::string X11Window::get_clipboard_text() const
{
	return clipboard.get_clipboard_text();
}

PixelBuffer X11Window::get_clipboard_image() const
{
	throw Exception("Todo: X11Window::get_clipboard_image");
}

bool X11Window::is_clipboard_text_available() const
{
	return clipboard.is_clipboard_text_available();
}

bool X11Window::is_clipboard_image_available() const
{
	return clipboard.is_clipboard_image_available();
}

void X11Window::set_cursor(CursorProvider_X11 *cursor)
{
	//TODO:
}

void X11Window::request_repaint( const Rect &cl_rect )
{

	Rect rect = cl_rect;
	Rect window_rect = get_viewport();

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

void X11Window::process_queued_events()
{
	unsigned int max = last_repaint_rect.size();
	for (unsigned int cnt=0; cnt < max; cnt++)
	{
		Rect &rect = last_repaint_rect[cnt];

		XEvent event;
		event.xexpose.type = Expose;
		event.xexpose.serial = 0;
		event.xexpose.send_event = True;
		event.xexpose.display = handle.display;
		event.xexpose.window = handle.window;
		event.xexpose.x = rect.left;
		event.xexpose.y = rect.top;
		event.xexpose.width = rect.get_width();
		event.xexpose.height = rect.get_height();
		event.xexpose.count = (max -1) - cnt;
		XSendEvent( handle.display, handle.window, False, 0, &event );
	}
	last_repaint_rect.clear();
}

void X11Window::set_minimum_size(int width, int height, bool client_area)
{
	minimum_size = Size(width,height);

	long user_hints;
	XGetWMNormalHints(handle.display, handle.window, size_hints, &user_hints);
	size_hints->min_width   = minimum_size.width;
	size_hints->min_height  = minimum_size.height;
	size_hints->flags |= PMinSize;
	XSetWMNormalHints(handle.display, handle.window, size_hints);
}

void X11Window::set_maximum_size(int width, int height, bool client_area)
{
	maximum_size = Size(width,height);

	long user_hints;
	XGetWMNormalHints(handle.display, handle.window, size_hints, &user_hints);
	size_hints->max_width   = maximum_size.width;
	size_hints->max_height  = maximum_size.height;
	size_hints->flags |= PMaxSize;
	XSetWMNormalHints(handle.display, handle.window, size_hints);
}

void X11Window::set_pixel_ratio(float ratio)
{
	pixel_ratio = ratio;

	// Pixel ratio is not set; calculate closest pixel ratio.
	if (pixel_ratio == 0.0f)
	{
		int s = std::round(ppi / 16.0f);
		/**/ if (s <= 6)  // <=  96 PPI; old tech; use 1:1 ratio.
		{
			pixel_ratio = 1.0f;
		}
		else if (s >= 12) // >= 192 PPI; new tech; use 1:1 ratio to avoid sub-pixeling.
		{
			pixel_ratio = static_cast<float>(s / 6);
		}
		else // 96 ~ 192 PPI; modern; use one-sixth steps
		{
			pixel_ratio = static_cast<float>(s) / 6.0f;
		}
	}
	// TODO Adjust everything related to pixel ratio.
}

void X11Window::get_keyboard_modifiers(bool &key_shift, bool &key_alt, bool &key_ctrl) const
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

Point X11Window::get_mouse_position() const
{
	if (!get_mouse())
	{
		return Point();
	}
	return get_mouse()->get_device_position();
}

// Important: Use XFree() on the returned pointer (if not NULL)
unsigned char *X11Window::get_property(Window use_window, Atom prop, unsigned long *number_items_ptr, int *actual_format_ptr, Atom *actual_type_ptr) const
{
	unsigned long bytes_after;
	int read_bytes = 1024;
	unsigned char *read_data = nullptr;
	do
	{
		if(read_data != nullptr)
		{
			XFree(read_data);
		}
		int result = XGetWindowProperty(handle.display, use_window, prop, 0, read_bytes, False, AnyPropertyType, actual_type_ptr, actual_format_ptr, number_items_ptr, &bytes_after, &read_data);
		if (result != Success)
		{
			*number_items_ptr = 0;
			*actual_format_ptr = 0;
			*actual_type_ptr = None;
			return nullptr;
		}
		read_bytes += bytes_after;
	}while(bytes_after != 0);
	return read_data;
}

const std::vector<int> &X11Window::get_window_socket_messages() const
{
	return current_window_events;
}

void X11Window::set_large_icon(const PixelBuffer &image)
{
	unsigned int size = (image.get_width() * image.get_height()) + 2; // header is 2 ints
	unsigned long* data = (unsigned long*)malloc(size * sizeof(unsigned long));

	// set header
	data[0] = image.get_width();
	data[1] = image.get_height();

	// icon data is expected as ARGB
	PixelBuffer transformed_image = image.to_format(tf_bgra8);

	// on 64bit systems, the destination buffer is 64 bit per pixel
	// thus, we have to copy each pixel individually (no memcpy)
	for (int y = 0; y < image.get_height(); ++y) {
		const uint32_t* src = (const uint32_t*)transformed_image.get_line(y);
		unsigned long* dst = &data[2 + (y * image.get_width())];
		for (int x = 0; x < image.get_width(); ++x) {
			dst[x] = src[x];
		}
	}

	// set icon geometry
	unsigned long* geom = (unsigned long*)malloc(4 * sizeof(unsigned long));
	geom[0] = geom[1] = 0; // x, y
	geom[2] = image.get_width();
	geom[3] = image.get_height();

	Atom propertyGeom = XInternAtom(handle.display, "_NET_WM_ICON_GEOMETRY", 0);
	XChangeProperty(handle.display, handle.window, propertyGeom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)geom, 4);

	// set icon data
	Atom property = XInternAtom(handle.display, "_NET_WM_ICON", 0);
	XChangeProperty(handle.display, handle.window, property, XA_CARDINAL, 32, PropModeReplace,
		(unsigned char*)data, size);

}

void X11Window::set_small_icon(const PixelBuffer &image)
{
	set_large_icon(image);
}

InputDeviceProvider_X11Keyboard *X11Window::get_keyboard() const
{
	return static_cast<InputDeviceProvider_X11Keyboard *>(keyboard.get_provider());
}

InputDeviceProvider_X11Mouse *X11Window::get_mouse() const
{
	return static_cast<InputDeviceProvider_X11Mouse *>(mouse.get_provider());
}

bool X11Window::process_window_sockets(bool peek_only)
{
	bool message = false;
	for (auto & elem : joysticks)
	{
		InputDeviceProvider_LinuxJoystick *joystick_provider = dynamic_cast<InputDeviceProvider_LinuxJoystick *>(elem.get_provider());
		if (joystick_provider)
			message = joystick_provider->poll(peek_only);
	}
	return message;
}

}


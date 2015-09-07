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
**    Chu Chin Kuan
*/

#include "Display/precomp.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/System/system.h"
#include "API/Core/Text/logger.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"

#include <algorithm>
#include <cstdio>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include <dlfcn.h>
#include <unistd.h>

#include "x11_window.h"
#include "input_device_provider_x11keyboard.h"
#include "input_device_provider_x11mouse.h"
#ifdef HAVE_LINUX_JOYSTICK_H
#include "input_device_provider_linuxjoystick.h"
#endif
#include "display_message_queue_x11.h"
#include "../../Window/input_context_impl.h"
#include "../../setup_display.h"

#ifndef _NET_WM_STATE_REMOVE
#define _NET_WM_STATE_REMOVE  0
#define _NET_WM_STATE_ADD     1
#define _NET_WM_STATE_TOGGLE  2
#endif

constexpr int _ResizeMinimumSize_ = 8;

namespace clan
{

X11Window::X11Window()
: handle(), color_map(0), system_cursor(0), hidden_cursor(0), cursor_bitmap(0), size_hints(nullptr),
  minimized(false), maximized(false), restore_to_maximized(false), fullscreen(false),
  is_window_mapped(false),
  site(nullptr), clipboard(this)
{
	handle.display = SetupDisplay::get_message_queue()->get_display();
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

	// Reset all variables
	close_window();

	handle.screen = visual->screen;
	atoms = X11Atoms(handle.display);

	int disp_width_px = XDisplayWidth(handle.display, handle.screen);
	int disp_height_px = XDisplayHeight(handle.display, handle.screen);
	int disp_width_mm = XDisplayWidthMM(handle.display, handle.screen);

	// Get DPI of screen or use 96.0f if Xlib doesn't have a value.
	ppi = (disp_width_mm < 24) ? 96.0f : (25.4f * static_cast<float>(disp_width_px) / static_cast<float>(disp_width_mm));

	// Update pixel ratio.
	set_pixel_ratio(pixel_ratio);

	// Get X11 root window.
	auto _root_window = RootWindow(handle.display, handle.screen);

	// Get and validate initial window position and size.
	int win_x = desc.get_position().left * pixel_ratio;
	int win_y = desc.get_position().top * pixel_ratio;
	int win_width = desc.get_size().width * pixel_ratio;
	int win_height = desc.get_size().height * pixel_ratio;

	if (win_width <= 0)
		throw Exception("Invalid window width.");

	if (win_height <= 0)
		throw Exception("Invalid window height.");

	// Set values if fullscreen requested.
	if (desc.is_fullscreen())
	{
		win_x = 0;
		win_y = 0;
		win_width = disp_width_px;
		win_height = disp_height_px;
	}

	// Center window if position supplied is (-1, -1)
	if (win_x == -1 && win_y == -1)
	{
		win_x = (disp_width_px - win_width)/2 - 1;
		win_y = (disp_height_px - win_height)/2 - 1;
	}

	// Set minimum and maximum size
	this->resize_allowed = desc.get_allow_resize() || desc.is_fullscreen(); // Fullscreen mode needs a resizable window.
	if (resize_allowed)
	{
		minimum_size = Size(_ResizeMinimumSize_, _ResizeMinimumSize_);
		maximum_size = Size(0, 0); // No maximum size by default.
	}
	else
	{
		minimum_size = Size(win_width, win_height);
		maximum_size = Size(win_width, win_height);
	}

	// Setup X11 size hints.
	this->size_hints = XAllocSizeHints();
	if (size_hints == NULL)
		throw Exception("Failed to allocate X11 XSizeHints structure.");

	size_hints->flags       = PMinSize | (resize_allowed ? 0 : PMaxSize);
	size_hints->flags      |= PResizeInc | PBaseSize | PWinGravity;
	// x, y, width, height are obsolete.
	size_hints->min_width   = minimum_size.width;
	size_hints->min_height  = minimum_size.height;
	size_hints->max_width   = maximum_size.width;
	size_hints->max_height  = maximum_size.height;
	size_hints->width_inc   = 1;
	size_hints->height_inc  = 1;
	size_hints->base_width  = win_width;
	size_hints->base_height = win_height;
	size_hints->win_gravity = NorthWestGravity;

	// Setup X11 colormap.
	//
	// The X.Org XServer implementation used on most systems requires that
	// a color-map be set for the window. Additionally, windows with a
	// different color-depth than its parent must have the border-pixel flag
	// set when creating them. Failure to do either will cause XCreateWindow()
	// to result in a BadMatch error.
	//
	// Source: stackoverflow.com/questions/3645632
	color_map = XCreateColormap(handle.display, _root_window, visual->visual, AllocNone);

	// Static popups are unresizable captionless popup windows.
	// These windows should not be decorated.
	bool is_static_popup = desc.is_popup() && !desc.has_caption() && !desc.get_allow_resize();

	// Tell X11 to perserve graphical content under small popup windows to avoid redraws.
	bool save_under = desc.is_popup() && ( (win_width * win_height) < (256 * 256 * pixel_ratio * pixel_ratio) );

	// Setup window attributes.
	XSetWindowAttributes attr = XSetWindowAttributes {
		.background_pixmap  = None, /* default */
		.background_pixel   =  0ul, /* default: undefined */
		.border_pixmap      = CopyFromParent, /* default */
		.border_pixel       =  0ul, /* see color_map details above */
		.bit_gravity        = ForgetGravity, /* default */
		.win_gravity        = NorthWestGravity, /* default */
		.backing_store      = NotUseful, /* default */
		.backing_planes     = -1ul, /* default */
		.backing_pixel      =  0ul, /* default */
		.save_under         = save_under ? True : False,
		.event_mask         = KeyPressMask
		                    | KeyReleaseMask
		                    | ButtonPressMask
		                    | ButtonReleaseMask
		                    | EnterWindowMask
		                    | LeaveWindowMask
		                    | PointerMotionMask
		                    | KeymapStateMask
		                    | ExposureMask
		                    // | VisibilityChangeMask
		                    | StructureNotifyMask
		                    | FocusChangeMask
		                    | PropertyChangeMask ,
		.do_not_propagate_mask  = NoEventMask, /* default */
		.override_redirect      = is_static_popup ? True : False,
		.colormap               = color_map, /* see color_map details above */
		.cursor                 = None /* default; Let X11 handle the cursor for now. */
	};

	this->system_cursor = XCreateFontCursor(handle.display, XC_left_ptr); // This is allowed to fail

	log_event("debug", "clan::X11Window::create(): Creating window...");
	log_event("debug", "    x%1 y%2 w%3 h%4 b%5 d%6", win_x, win_y, win_width, win_height, border_width, visual->depth);
	log_event("debug", "    a.su%1, a.od%2", save_under, is_static_popup);

	// Create window
	handle.window = XCreateWindow(
			handle.display, _root_window,
			win_x, win_y, win_width, win_height, border_width,
			visual->depth, InputOutput, visual->visual,
			CWBorderPixel | CWOverrideRedirect | CWSaveUnder | CWEventMask | CWColormap,
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
	hidden_cursor = XCreatePixmapCursor(handle.display, cursor_bitmap, cursor_bitmap, &black_color, &black_color, 0,0);

	// Set title of window:
	set_title(desc.get_title());

	{   // Inform the window manager who we are, so it can kill us if we're not good for its universe.
		Atom atom;
		int32_t pid = getpid();
		if (pid > 0)
		{
			atom = atoms.get_atom(handle.display, "_NET_WM_PID", False);
			XChangeProperty(handle.display, handle.window, atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &pid, 1);
		}

		char hostname[256];
		if (gethostname(hostname, sizeof(hostname)) > -1)
		{
			hostname[255] = 0;
			atom = atoms.get_atom(handle.display, "WM_CLIENT_MACHINE", False);
			XChangeProperty(handle.display, handle.window, atom, XA_STRING, 8, PropModeReplace, (unsigned char *) hostname, strlen(hostname));
		}
	}

	// Set-up window type/styling.
	// TODO Support more window types, broaden ClanLib window type support, etc.
	if (atoms["_NET_WM_WINDOW_TYPE"] != None)
	{
		Atom type = None;
		std::string name;

		if (desc.is_dialog())
		{
			name = "_NET_WM_WINDOW_TYPE_DIALOG";
			type = atoms[name];
		}
		else if (desc.is_popup())
		{
			if (is_static_popup)
			{
				name = "_NET_WM_WINDOW_TYPE_TOOLTIP";
				type = atoms[name];
			}
			else if (desc.has_caption()) // A pop-up with title bar -> utility
			{
				name = "_NET_WM_WINDOW_TYPE_UTILITY";
				type = atoms[name];
			} // else, a pop-up without a title bar -> popup-menu, combo, dropdown, tooltip, ...

			if (type == None) { name = "_NET_WM_WINDOW_TYPE_POPUP_MENU"; type = atoms[name]; }
			if (type == None) { name = "_NET_WM_WINDOW_TYPE_COMBO"; type = atoms[name]; }
			if (type == None) { name = "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU"; type = atoms[name]; }
		} // else if (desc.is_normal())

		// Fallback to normal window type if WM doesn't support what we want.
		if (type == None) { name = "_NET_WM_WINDOW_TYPE_NORMAL"; type = atoms[name]; }

		if (type != None) // Ensure selected type exists.
		{
			XChangeProperty(handle.display, handle.window, atoms["_NET_WM_WINDOW_TYPE"], XA_ATOM, 32, PropModeReplace, (unsigned char *)&type, 1);
			log_event("debug", "clan::X11Window::create(): Creating window of type '%1'.", name);
		}
		else
		{
			log_event("debug", "clan::X11Window::create(): Failed to find a suitable window type.");
		}
	}
	else
	{
		log_event("debug", "clan::X11Window::create(): _NET_WM_WINDOW_TYPE does not exist.");
	}

	// Set size hints
	XSetWMNormalHints(handle.display, handle.window, size_hints);

	{	// Subscribe to WM events.
		Atom protocol = atoms["WM_DELETE_WINDOW"];
		Status result = XSetWMProtocols(handle.display, handle.window, &protocol, 1);
		if (result == 0)
			log_event("debug", "clan::X11Window::create(): Failed to set WM_PROTOCOLS.");
	}

	{	// Make auto-repeat keys detectable.
		Bool supports_detectable_autorepeat;
		XkbSetDetectableAutoRepeat(handle.display, True, &supports_detectable_autorepeat);
	}

	{	// Make window full-screen if requested.
		if (atoms["_NET_WM_STATE"] == None && atoms["_NET_WM_STATE_FULLSCREEN"])
		{
			fullscreen = false;
			log_event("debug", "clan::X11Window: Fullscreen not supported by WM.");
		}
		else
		{
			fullscreen = desc.is_fullscreen();
		}

		if (fullscreen)
		{
			Atom state = atoms["_NET_WM_STATE_FULLSCREEN"];
			XChangeProperty(handle.display, handle.window, atoms["_NET_WM_STATE"], XA_ATOM, 32, PropModeReplace, (unsigned char *)&state, 1);
		}
	}

	update_frame_extents();

	client_area = desc.get_position_client_area() // supplied position is at ? client area : window area;
		? Rect::xywh(win_x, win_y, win_width, win_height)
		: Rect::xywh(win_x + frame_extents.left, win_y + frame_extents.right, win_width, win_height)
		;

	// Set window visibility
	if (desc.is_visible())
	{
		show(false);
	}

	// Setup the clipboard
	clipboard.setup();

	// Create input devices for window:
	ic.clear();
	ic.add_keyboard(keyboard);
	ic.add_mouse(mouse);

	// Go looking for joysticks:
	setup_joysticks();
}

void X11Window::update_frame_extents()
{
	frame_extents = Rect { border_width, border_width, border_width, border_width };

	if (atoms["_NET_FRAME_EXTENTS"] == None)
		return;

	// Request frame extents from WM.
	if (atoms["_NET_REQUEST_FRAME_EXTENTS"] != None)
	{
		XEvent event;
		memset(&event, 0, sizeof(event));

		event.type = ClientMessage;
		event.xclient.window = handle.window;
		event.xclient.format = 32;
		event.xclient.message_type = atoms["_NET_REQUEST_FRAME_EXTENTS"];

		XSendEvent(handle.display, RootWindow(handle.display, handle.screen), False, SubstructureNotifyMask | SubstructureRedirectMask, &event);

		int timer = 10;
		while(true)
		{
			if (timer < 0)
			{
				log_event("debug", "clan::X11Window: Your window manager has a broken _NET_REQUEST_FRAME_EXTENTS implementation.");
				break;
			}

			if (XCheckMaskEvent(handle.display, PropertyNotify, &event))
			{
				break;
			}

			clan::System::sleep(5);
			timer--;
		}
	}

	unsigned long  item_count;
	// _NET_FRAME_EXTENTS, left, right, top, bottom, CARDINAL[4]/32
	unsigned char *data = atoms.get_property(handle.window, "_NET_FRAME_EXTENTS", item_count);
	if (data == NULL)
		return;

	if (item_count >= 4)
	{
		long *cardinal = (long *)data;
		frame_extents.left   = cardinal[0];
		frame_extents.right  = cardinal[1];
		frame_extents.top    = cardinal[2];
		frame_extents.bottom = cardinal[3];
	}

	XFree(data);
}

void X11Window::close_window()
{
	current_window_events.clear();

	if (handle.window)
	{
		XDestroyWindow(handle.display, handle.window);
		handle.window = 0;
		handle.screen = -1;
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

	atoms.clear();
}

Rect X11Window::get_geometry() const
{
	Rect geom = client_area;
	geom.expand(frame_extents.left, frame_extents.top, frame_extents.right, frame_extents.bottom);
	return geom;
}

Rect X11Window::get_viewport() const
{
	return Rect { 0, 0, client_area.get_size() };
}

bool X11Window::has_focus() const
{
	Window focus_window;
	int    focus_state;
	XGetInputFocus(handle.display, &focus_window, &focus_state);
	return (focus_window == handle.window);
}

bool X11Window::is_minimized() const
{
	if (!is_window_mapped)
		log_event("debug", "clan::X11Window::is_minimized(): Window is unmapped.");

	// Check FreeDeskop specified _NET_WM_STATE first.
	if (atoms["_NET_WM_STATE"] != None)
	{
		if (atoms["_NET_WM_STATE_HIDDEN"] != None)
		{
			auto ret = atoms.check_net_wm_state(handle.window, { "_NET_WM_STATE_HIDDEN" } );
			return ret.front();
		}
		else
			log_event("debug", "clan::X11Window::is_minimized(): _NET_WM_STATE_HIDDEN not provided by WM.");
	}
	else
		log_event("debug", "clan::X11Window::is_minimized(): _NET_WM_STATE not provided by WM.");

	// If not available, check legacy WM_STATE property
	if (atoms["WM_STATE"] != None)
	{
		unsigned long  item_count;
		unsigned char *data = atoms.get_property(handle.window, "WM_STATE", item_count);
		if (data != NULL)
		{
			long state = *(long *)data;
			XFree(data);
			return state == IconicState;
		}
		else
			log_event("debug", "clan::X11Window::is_minimized(): Failed to query WM_STATE.");
	}
	else
		log_event("debug", "clan::X11Window::is_minimized(): WM_STATE not provided by WM.");

	return false;
}

bool X11Window::is_maximized() const
{
	if (!is_window_mapped)
		log_event("debug", "clan::X11Window::is_minimized(): Window is unmapped.");

	auto ret = atoms.check_net_wm_state(handle.window, { "_NET_WM_STATE_MAXIMIZED_HORZ", "_NET_WM_STATE_MAXIMIZED_VERT" });
	if (ret[0] != ret[1])
		log_event("debug", "clan::X11Window::is_maximized(): Window is only maximized on the %1 side.", ret[0] ? "horizontal" : "vertical");

	return ret[0] && ret[1];
}

bool X11Window::is_visible() const
{
	XWindowAttributes attr;
	XGetWindowAttributes(handle.display, handle.window, &attr);
	if (attr.map_state == IsViewable) return false;
	return true;
}

Size X11Window::get_minimum_size(bool for_client_area) const
{
	if (for_client_area)
		return minimum_size; // TODO This used to throw exceptions
	else
		return Size{
			minimum_size.width + frame_extents.left + frame_extents.right,
			minimum_size.height + frame_extents.top + frame_extents.bottom
		};
}

Size X11Window::get_maximum_size(bool for_client_area) const
{
	if (for_client_area)
		return maximum_size; // TODO This used to throw exceptions
	else
		return Size{
			maximum_size.width + frame_extents.left + frame_extents.right,
			maximum_size.height + frame_extents.top + frame_extents.bottom
		};
}

Point X11Window::client_to_screen(const Point &client)
{
	return Point{ client_area.left + client.x, client_area.top + client.y };
}

Point X11Window::screen_to_client(const Point &screen)
{
	return Point{ screen.x - client_area.left, screen.y - client_area.top };
}

void X11Window::show_system_cursor()
{
	XDefineCursor(handle.display, handle.window, system_cursor);
}

void X11Window::hide_system_cursor()
{
	XDefineCursor(handle.display, handle.window, hidden_cursor);
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

void X11Window::set_title(const std::string &new_title)
{
	window_title = new_title;
	XSetStandardProperties(handle.display, handle.window, new_title.c_str(), new_title.c_str(), None, nullptr, 0, nullptr);
}

void X11Window::set_position(const Rect &pos, bool pos_is_client_area)
{
	update_frame_extents();

	if (!resize_allowed) // If resize has been disabled, we have to temporary enable it
	{
		long user_hints;
		XGetWMNormalHints(handle.display, handle.window, size_hints, &user_hints);
		size_hints->flags &= ~(PMinSize | PMaxSize);
		XSetWMNormalHints(handle.display, handle.window, size_hints);
	}

	int width = pos.get_width();
	int height = pos.get_height();

	if (pos_is_client_area)
	{
		client_area = pos;
		XMoveResizeWindow(handle.display, handle.window, pos.left - frame_extents.left, pos.top - frame_extents.top, width, height);
	}
	else
	{
		width = width - frame_extents.left - frame_extents.right;
		height = height - frame_extents.top - frame_extents.bottom;
		client_area = Rect::xywh(pos.left + frame_extents.left, pos.top + frame_extents.top, width, height);
		XMoveResizeWindow(handle.display, handle.window, pos.left, pos.top, width, height);
	}

	if (!resize_allowed) // resize has been temporary enabled
	{
		size_hints->min_width   = width;
		size_hints->min_height  = height;
		size_hints->max_width   = width;
		size_hints->max_height  = height;
		size_hints->flags |= PMinSize | PMaxSize;
		XSetWMNormalHints(handle.display, handle.window, size_hints);
	}
}

void X11Window::set_size(int width, int height, bool size_is_client_area)
{
	update_frame_extents();

	if (!resize_allowed) // If resize has been disabled, we have to temporary enable it
	{
		long user_hints;
		XGetWMNormalHints(handle.display, handle.window, size_hints, &user_hints);
		size_hints->flags &= ~(PMinSize | PMaxSize);
		XSetWMNormalHints(handle.display, handle.window, size_hints);
	}

	if (!size_is_client_area)
	{
		width  = std::max(_ResizeMinimumSize_, width  - frame_extents.left - frame_extents.right);
		height = std::max(_ResizeMinimumSize_, height - frame_extents.top - frame_extents.bottom);
	}

	client_area = Rect::xywh(client_area.left, client_area.top, width, height);
	XResizeWindow(handle.display, handle.window, width, height);

	if (!resize_allowed) // resize has been temporary enabled
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
	if (!is_window_mapped)
		log_event("debug", "clan::X11Window::minimize(): Window is not yet mapped.");

	if (!is_minimized())
	{
		log_event("debug", "clan::X11Window::minimize(): Minimizing.");
		restore_to_maximized = is_maximized();
		XIconifyWindow(handle.display, handle.window, handle.screen);
		minimized = true;
	}
	else
	{
		log_event("debug", "clan::X11Window::minimize(): Window already minimized.");
	}
}

void X11Window::restore()
{
	if (!is_window_mapped)
		log_event("debug", "clan::X11Window::restore(): Window is not yet mapped.");

	if (is_minimized())
	{
		if (restore_to_maximized)
		{
			log_event("debug", "clan::X11Window::restore(): Restoring to maximized window.");
			maximize();
		}
		else
		{
			log_event("debug", "clan::X11Window::restore(): Restoring minimized window.");
			map_window();
			maximized = false;
		}
	}
	else if (is_maximized())
	{
		log_event("debug", "clan::X11Window::restore(): Restoring window size.");
		atoms.modify_net_wm_state(handle.window, _NET_WM_STATE_REMOVE, "_NET_WM_STATE_MAXIMIZED_HORZ", "_NET_WM_STATE_MAXIMIZED_VERT");
		maximized = false;
	}
}

void X11Window::map_window()
{
	// Clear exisitng StructureNofify events.
	XEvent event;
	while(XCheckMaskEvent(handle.display, StructureNotifyMask, &event));

	if (is_window_mapped)
	{
		if (!is_minimized())
		{
			log_event("debug", "clan::X11Window::map_window(): Window already mapped.");
			return;
		}
		else
			log_event("debug", "clan::X11Window::map_window(): Mapping minimized window.");
	}

	log_event("debug", "clan::X11Window::map_window(): Mapping window...");

	int result = XMapWindow(handle.display, handle.window);
	if ((result == BadValue) || (result == BadWindow))
		throw Exception("Failed to map window.");

	XFlush(handle.display);

	do {
		XWindowEvent(handle.display, handle.window, StructureNotifyMask, &event);
	} while (event.type != MapNotify);
	XFlush(handle.display);

	is_window_mapped = true;

	if (fullscreen)
	{
		XSetInputFocus(handle.display, handle.window, RevertToParent, CurrentTime);
		XFlush(handle.display);
	}
	else
	{
		update_frame_extents();
		set_position(client_area, true);
	}
}

void X11Window::unmap_window()
{
	// Clear exisitng StructureNofify events.
	XEvent event;
	while(XCheckMaskEvent(handle.display, StructureNotifyMask, &event));

	if (!is_window_mapped)
	{
		log_event("debug", "clan::X11Window::unmap_window(): Window already unmapped.");
		return;
	}

	log_event("debug", "clan::X11Window::map_window(): Unmapping window...");

	int result = XUnmapWindow(handle.display, handle.window);
	if ( (result == BadValue) || (result == BadWindow) )
		throw Exception("Failed to unmap window.");

	XFlush(handle.display);

	do {
		XWindowEvent(handle.display, handle.window, StructureNotifyMask, &event);
	} while (event.type != UnmapNotify);
	XFlush(handle.display);

	is_window_mapped = false;
}

void X11Window::maximize()
{
	log_event("debug", "clan::X11Window::maximize(): Maximizing window.");
	atoms.modify_net_wm_state(handle.window, _NET_WM_STATE_ADD, "_NET_WM_STATE_MAXIMIZED_HORZ", "_NET_WM_STATE_MAXIMIZED_VERT");
	maximized = true;
}

void X11Window::show(bool activate)
{
	log_event("debug", "clan::X11Window::show(): Mapping window.");
	map_window();
	if (activate)
		set_enabled(true);
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

Rect X11Window::get_screen_position() const
{
	XLockDisplay(handle.display);

	Window  root;
	Window  parent;
	Window  child;
	Window* children;
	unsigned int children_count;

	// Get parent Window
	XQueryTree(handle.display, handle.window, &root, &parent, &children, &children_count);
	XFree(children);

	// Get this window's attributes.
	XWindowAttributes attr;
	XGetWindowAttributes(handle.display, handle.window, &attr);

	int xpos;
	int ypos;
	// Get parent window's translation coordinates to root window.
	XTranslateCoordinates(handle.display, parent, root, attr.x, attr.y, &xpos, &ypos, &child);

	return Rect::xywh(xpos, ypos, attr.width, attr.height);
}

void X11Window::process_window_resize(const Rect &new_rect)
{
	Rect old_client_area = client_area;
	client_area = new_rect;

	if (site)
	{
		if (old_client_area.left != client_area.left || old_client_area.top != client_area.top)
		{
			(site->sig_window_moved)();
		}

		if (old_client_area.get_width() != client_area.get_width() || old_client_area.get_height() != client_area.get_height())
		{
			Rectf rectf = client_area;
			rectf.left   /= pixel_ratio;
			rectf.top    /= pixel_ratio;
			rectf.right  /= pixel_ratio;
			rectf.bottom /= pixel_ratio;

			if (callback_on_resized)
				callback_on_resized(); // OpenGLWindowProvider::on_window_resized

			(site->sig_resize)(rectf.get_width(), rectf.get_height()); // TopLevelWindow_Impl::on_resize

			if (site->func_window_resize)
				(site->func_window_resize)(rectf); // TopLevelWindow_Impl::on_paint
		}
	}
}

void X11Window::process_message(XEvent &event, X11Window *mouse_capture_window)
{
	switch(event.type)
	{
		case ConfigureNotify:
		{	// Resize or Move
			Rect new_client_area = (event.xany.send_event == 0)
				? get_screen_position()
				: Rect::xywh(
						event.xconfigure.x + event.xconfigure.border_width,
						event.xconfigure.y + event.xconfigure.border_width,
						event.xconfigure.width, event.xconfigure.height
						);

			process_window_resize(new_client_area);
			break;
		}
		case ClientMessage:
		{	// handle window manager messages
			Atom WM_PROTOCOLS = atoms["WM_PROTOCOLS"];
			if (WM_PROTOCOLS == None)
			{
				log_event("debug", "clan::X11Window::process_message: WM_PROTOCOLS not supported by WM.");
				break;
			}
			else if (event.xclient.message_type == WM_PROTOCOLS)
			{
				unsigned long protocol = event.xclient.data.l[0];
				if (protocol == None)
				{
					log_event("debug", "clan::X11Window::process_message: WM_PROTOCOLS event protocol supplied is None.");
					break;
				}

				Atom WM_DELETE_WINDOW = atoms["WM_DELETE_WINDOW"];
				Atom _NET_WM_PING = atoms["_NET_WM_PING"];

				if (protocol == WM_DELETE_WINDOW && site)
				{
					(site->sig_window_close)();
				}
				else if (protocol == _NET_WM_PING)
				{
					XSendEvent(handle.display, RootWindow(handle.display, handle.screen), False, SubstructureNotifyMask | SubstructureRedirectMask, &event);
				}
			}
			break;
		}
		case Expose:
		{	// Window exposure. Immediate repaint.
			if (!site)
				break;
			
			Rect new_geometry = Rect::xywh(event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height);
			(site->sig_paint)(new_geometry);
			break;
		}
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
		{	// Iconify, Maximized, ...
			if (!site)
				break;

			Atom _NET_WM_STATE = atoms["_NET_WM_STATE"];
			Atom WM_STATE = atoms["WM_STATE"]; // legacy.

			if (_NET_WM_STATE != None && event.xproperty.atom == _NET_WM_STATE && event.xproperty.state == PropertyNewValue)
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
						rectf.left   /= pixel_ratio;
						rectf.top    /= pixel_ratio;
						rectf.right  /= pixel_ratio;
						rectf.bottom /= pixel_ratio;

						(site->sig_window_moved)();
						if (site->func_window_resize)
							(site->func_window_resize)(rectf);

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
			else if (WM_STATE != None && event.xproperty.atom == WM_STATE && event.xproperty.state == PropertyNewValue)
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
			break;
		}
		case KeyRelease:
		case KeyPress:
			if (get_keyboard())
			{
				get_keyboard()->received_keyboard_input(event.xkey);
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

				if (this != mouse_capture_window) // This is so you can capture mouse events in another window, as if it was this window (Set by capture_mouse())
				{
					Rect this_scr = client_area;
					Rect capture_scr = mouse_capture_window->client_area;

					event.xbutton.x += this_scr.left - capture_scr.left;
					event.xbutton.y += this_scr.top - capture_scr.top;
				}

				mouse_capture_window->get_mouse()->received_mouse_input(event.xbutton);
			}
			break;
		case MotionNotify:
			if (mouse_capture_window->get_mouse() && event.xany.send_event == 0)
			{
				if (this != mouse_capture_window) // This is so you can capture mouse events in another window, as if it was this window (Set by capture_mouse())
				{
					Rect this_scr = client_area;
					Rect capture_scr = mouse_capture_window->client_area;

					event.xmotion.x += this_scr.left - capture_scr.left;
					event.xmotion.y += this_scr.top - capture_scr.top;
				}

				mouse_capture_window->get_mouse()->received_mouse_move(event.xmotion);
			}
			break;
		case SelectionClear: // New clipboard selection owner
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

void X11Window::request_repaint(const Rect &cl_rect)
{
	XExposeEvent expose = {Expose, 0};
	expose.send_event = true;
	expose.display = handle.display;
	expose.window = handle.window;
	expose.x = cl_rect.left;
	expose.y = cl_rect.top;
	expose.width = cl_rect.get_width();
	expose.height = cl_rect.get_height();
	expose.count = 0;
	XSendEvent(handle.display, handle.window, False, 0, (XEvent *) &expose);
}

void X11Window::set_minimum_size(int width, int height, bool size_is_client_area)
{
	minimum_size = Size(width, height);

	if (!size_is_client_area)
	{
		minimum_size.width -= frame_extents.left + frame_extents.right;
		minimum_size.height -= frame_extents.top + frame_extents.bottom;
	}

	long user_hints;
	XGetWMNormalHints(handle.display, handle.window, size_hints, &user_hints);
	size_hints->min_width   = minimum_size.width;
	size_hints->min_height  = minimum_size.height;
	size_hints->flags |= PMinSize;
	XSetWMNormalHints(handle.display, handle.window, size_hints);
}

void X11Window::set_maximum_size(int width, int height, bool size_is_client_area)
{
	maximum_size = Size(width,height);

	if (!size_is_client_area)
	{
		maximum_size.width -= frame_extents.left + frame_extents.right;
		maximum_size.height -= frame_extents.top + frame_extents.bottom;
	}

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

void X11Window::process_window_sockets()
{
	for (auto & elem : joysticks)
	{
		InputDeviceProvider_LinuxJoystick *joystick_provider = dynamic_cast<InputDeviceProvider_LinuxJoystick *>(elem.get_provider());
		if (joystick_provider)
			joystick_provider->poll(false);
	}
}

}


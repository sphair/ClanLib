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
#include "API/Display/Window/display_window_message.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Window/timer.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/TargetProviders/timer_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "x11_window.h"
#include "input_device_provider_x11keyboard.h"
#include "input_device_provider_x11mouse.h"
#ifdef HAVE_LINUX_JOYSTICK_H
#include "input_device_provider_linuxjoystick.h"
#endif
#include "display_message_queue_x11.h"
#include "timer_provider_x11.h"
#include <X11/Xatom.h>

CL_X11Window::CL_X11Window(CL_DisplayMessageQueue_X11 *message_queue)
: window(0), window_last_focus(0), cmap(0), allow_resize(false), bpp(0), message_queue(message_queue), fullscreen(false),
  disp(0), system_cursor(0), hidden_cursor(0), cursor_bitmap(0), 
  keyboard(0), mouse(0), site(0), ignore_focus_events(false), clipboard(this)
{
	keyboard = new CL_InputDeviceProvider_X11Keyboard(this);
	mouse = new CL_InputDeviceProvider_X11Mouse(this);
	message_queue->add_client(this);
}

CL_X11Window::~CL_X11Window()
{
	message_queue->remove_client(this);

	if (is_fullscreen())
		restore_videomode();

	close_window();

	if (disp)
	{
		XCloseDisplay(disp);
		disp = 0;
	}

	if (keyboard) delete keyboard;
	if (mouse) delete mouse;
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

	if (!window_has_caption)
	{
		//TODO: Resize does not work on windows without a caption - May need to temporary enable it ... this is silly though!
		return;
	}

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
	// Not required?
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

void CL_X11Window::set_timer(CL_TimerProvider *timer)
{
	kill_timer(timer);
	CL_TimerProvider_X11 *xtimer = dynamic_cast<CL_TimerProvider_X11*>(timer);
	if (!xtimer)
		throw CL_Exception(cl_text("Unsupported timer"));

	timer_list.push_back(xtimer);
}

void CL_X11Window::kill_timer(CL_TimerProvider *timer)
{
	CL_TimerProvider_X11 *xtimer = dynamic_cast<CL_TimerProvider_X11*>(timer);
	if (!xtimer)
		throw CL_Exception(cl_text("Unsupported timer"));

	std::list<CL_TimerProvider_X11 *>::iterator it;
	// Find existing timer
	for (it = timer_list.begin(); it != timer_list.end(); ++it)
	{
		if (*it == xtimer)
		{
			timer_list.erase(it);
			break;
		}
	}
}

void CL_X11Window::close_window()
{
	bool focus = false;
	ic.clear();

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
	ignore_focus_events = false;

	if (!desc.get_owner().is_null())
	{
		// This is not required. When enabled (with a parent window) popup windows would be created, with an incorrect style
		//parent = provider_glx->get_window();
			
		if (!desc.has_caption())	// Support for popup window, as the parent window will loose input focus, for compatibility with Microsoft Windows platform, we ignore the first focus event
		{
			ignore_focus_events = true;
		}

		CL_Rect rect = get_screen_position();

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

//TODO: What is the policy of screen mode change? should switching to fullscreen change the display mode?
/*		win_width = DisplayWidth(disp, opengl_visual_info->screen);
		win_height = DisplayHeight(disp, opengl_visual_info->screen);
*/
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

	resize_enabled = desc.get_allow_resize();

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
		XSync(disp, True);
	}

	if (desc.is_fullscreen())
		set_videomode(desc.get_size().width, desc.get_size().height, desc.get_bpp(), desc.get_refresh_rate());
	else
		restore_videomode();

}

void CL_X11Window::process_timer_events()
{

	if (timer_list.empty()) return;

	unsigned int current_time = CL_System::get_time();
	std::list<CL_TimerProvider_X11 *>::iterator it;

	// Find existing timer
	for (it = timer_list.begin(); it != timer_list.end();)
	{
		CL_TimerProvider_X11 *xptr = *it;
		++it;	// Advance iterator now, as the current one may be destroyed

		xptr->process_timer(current_time);
	}
}

// Returns: true is a timer event would occur
bool CL_X11Window::check_timers(void)
{
	if (timer_list.empty()) return false;

	unsigned int current_time = CL_System::get_time();
	std::list<CL_TimerProvider_X11 *>::iterator it;
	// Find existing timer
	for (it = timer_list.begin(); it != timer_list.end(); ++it)
	{
		if ((*it)->check_timer(current_time))
			return true;
	}

	return false;
}

bool CL_X11Window::get_message(XEvent &clan_event)
{
	XEvent event;
	CL_Rect *rect;
	CL_DisplayWindowMessage message;
	bool clan_event_set = false;

	ic.poll(false);		// Check input devices

	// Dispatch all timer events
	process_timer_events();

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
						if (ignore_focus_events)
						{
							ignore_focus_events = false;
						}else
						{
							site->sig_lost_focus->invoke();
						}
					}
				}
				break;
			case KeyRelease:
			case KeyPress:
				keyboard->received_keyboard_input(event.xkey);
				break;
			//case KeymapNotify:
			//	break;
			case ButtonPress:
			case ButtonRelease:
				mouse->received_mouse_input(event.xbutton);
				break;
			case MotionNotify:
				mouse->received_mouse_move(event.xmotion);
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

		clan_event = event;
		clan_event_set = true;
	}

	return clan_event_set;
}

// This is called for each window by CL_DisplayMessageQueue_X11 to check for messages
bool CL_X11Window::has_messages()
{
	bool message_flag = false;

	if (XPending(disp) > 0)
	{
		message_flag = true;
	}

	if (check_timers())
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
				CL_InputDevice device(new CL_InputDeviceProvider_LinuxJoystick(this, cl_text(pathname)));
				ic.add_joystick(device);
			} 
			catch (CL_Exception error)
			{
				cl_log_event("debug", "Joystick Error: %1", error.message);
			}
		}
	}
#endif
}

void CL_X11Window::restore_videomode()
{
	if (!fullscreen)
		return;

	XF86VidModeSwitchToMode(disp, current_screen, &old_mode);
	XF86VidModeSetViewPort(disp, current_screen, old_x, old_y);
	old_x = old_y = -1;
	
	XUngrabPointer(disp, CurrentTime);

	fullscreen = false;
}

void CL_X11Window::set_videomode(int width, int height, int bpp_local, int refresh_rate)
{
	if(fullscreen)
		return;

	// Vid-mode Switching
	XF86VidModeModeLine cur_mode;

	XF86VidModeGetModeLine(disp, current_screen, &dotclock, &cur_mode);

	old_mode.dotclock   = dotclock;
	old_mode.hdisplay   = cur_mode.hdisplay;
	old_mode.hsyncstart = cur_mode.hsyncstart;
	old_mode.hsyncend   = cur_mode.hsyncend;
	old_mode.htotal     = cur_mode.htotal;
	old_mode.vdisplay   = cur_mode.vdisplay;
	old_mode.vsyncstart = cur_mode.vsyncstart;
	old_mode.vsyncend   = cur_mode.vsyncend;
	old_mode.vtotal     = cur_mode.vtotal;
	old_mode.flags      = cur_mode.flags;
	old_mode.privsize   = 0;

	int num_modes;
	XF86VidModeModeInfo **modes;
	XF86VidModeGetAllModeLines(disp, current_screen, &num_modes, &modes);

	std::list<XF86VidModeModeInfo *> usable_modes;
	for(int i = 0; i < num_modes; i++)
	{
		if(modes[i]->hdisplay == width && modes[i]->vdisplay == height)
		{
			cl_log_event("debug", "Useable fullscreen mode found: %1x%2", width, height);
			usable_modes.push_back(modes[i]);
		}
	}

	if (usable_modes.empty())
	{
		cl_log_event("debug", "No useable fullscreen modes available!");
	}
	else 
	{	
		if(!width)
			width = get_viewport().get_width();

		if(!height)
			height = get_viewport().get_height();
	
		if(!bpp_local)
			bpp_local = bpp;

		//Hide Window
		if (0)
		{  // FIXME: allow_override doesn't play together with
			// GrabPointer, not sure what is wrong but it simply doesn't
			// work.
			//
			// The code outside the 'if(0)' as it is now, works mostly,
			// however it doesn't work when the window or a part of it is
			// outside of the screen, since the window isn't moved
			// fullscreen will only show half the window, shouldn't be a
			// problem for most of the time, but will be tricky if the
			// window has equal size as the desktop.

			// Move the window into the right position, this must happen
			// BEFORE we remove control from the window manager
			XMoveResizeWindow(disp, window, 0, 0, width, height);

			// Move the mouse and switch moves
			XWarpPointer(disp, None, None, 0, 0, 0, 0, width/2, height/2);

			XUnmapWindow(disp, window);
			{ // Wait for window to disapear
				XEvent event;
				do {
					XMaskEvent(disp, StructureNotifyMask, &event);
				} while ( (event.type != UnmapNotify) || (event.xunmap.event != window) );
			}
			// Turn off WM control
			attributes.override_redirect = True;
			XChangeWindowAttributes(disp, window, CWBorderPixel | CWColormap | CWOverrideRedirect, &attributes);

			// Re-appear window
			XMapRaised(disp, window);
		}

		while (1) 
		{
			int result = XGrabPointer(disp, window, True, 0, GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
			if ( result == GrabSuccess ) {
				break;
			}	
			CL_System::sleep(100);
		}

		XF86VidModeGetViewPort(disp,current_screen, &old_x, &old_y);

		XF86VidModeSwitchToMode(disp, current_screen, *(usable_modes.begin()));
		Window child_window;
		int x, y;
		// Get the windows absolute position (aka relative to
		// the root window)
		XTranslateCoordinates(disp, window, DefaultRootWindow(disp), 0, 0, &x, &y, &child_window);
	
		XF86VidModeSetViewPort(disp, current_screen, x, y);
		
		XSync(disp, True);

		fullscreen = true;
	}
}


void CL_X11Window::set_clipboard_text(const CL_StringRef &text)
{
	clipboard.set_clipboard_text(text);
}

CL_String CL_X11Window::get_clipboard_text() const
{
	return clipboard.get_clipboard_text();
}

bool CL_X11Window::is_clipboard_text_available() const
{
	return clipboard.is_clipboard_text_available();
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

void CL_X11Window::invalidate_rect( const CL_Rect &cl_rect )
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
	return keyboard->get_keyboard_modifiers(key_shift, key_alt, key_ctrl);
}

CL_Point CL_X11Window::get_mouse_position() const
{
	return mouse->get_position();
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




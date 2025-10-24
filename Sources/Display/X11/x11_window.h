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

#pragma once


#include <list>
#include <map>
#include "API/Display/api_display.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/timer.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/display_window_message.h"
#include "API/Core/System/sharedptr.h"
#include "API/Core/Math/point.h"
#include "API/Core/Signals/callback_v0.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/XInput.h>
#include <sys/time.h>

class CL_InputDeviceProvider_X11Keyboard;
class CL_InputDeviceProvider_X11Mouse;
class CL_InputDeviceProvider_LinuxJoystick;
class CL_DisplayMessageQueue_X11;
class CL_TimerProvider;
class CL_Rect;
class CL_DisplayWindowSite;
class CL_DisplayWindowDescription;
class CL_CursorProvider_X11;


typedef struct _CL_XTimer
{
	CL_TimerProvider *timer;
	unsigned int start_time;	// The start time
}CL_XTimer;

class CL_X11Window
{
/// \name Construction
/// \{

public:
	CL_X11Window(CL_DisplayMessageQueue_X11 *message_queue);

	~CL_X11Window();


/// \}
/// \name Attributes
/// \{

public:
	CL_Rect get_geometry() const;

	CL_Rect get_viewport() const;

	bool has_focus() const;

	bool is_minimized() const;

	bool is_maximized() const;

	bool is_visible() const;

	bool is_fullscreen() const {return fullscreen;}

	/// \brief Returns the X11 display handle.
	Display *get_display() { return disp; }

	/// \brief Handle to X11 window handle.
	Window get_window() { return window; }

	CL_InputContext get_ic() const { return ic; }

	CL_InputContext ic;

	CL_Callback_v0 &func_on_resized() { return callback_on_resized; }

	bool is_clipboard_text_available() const;


/// \}
/// \name Operations
/// \{

public:
	CL_Point client_to_screen(const CL_Point &client);

	CL_Point screen_to_client(const CL_Point &screen);

	void open_screen();

	void create(XVisualInfo *visual, int screen_bpp, CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);

	void show_system_cursor();

	void set_cursor(CL_CursorProvider_X11 *cursor);

	void set_cursor(enum CL_StandardCursor type);

	void hide_system_cursor();

	void set_title(const CL_StringRef &new_title);

	void set_position(const CL_Rect &pos, bool client_area);

	void set_size(int width, int height, bool client_area);

	void set_minimum_size(int width, int height, bool client_area);

	void set_maximum_size(int width, int height, bool client_area);

	void set_enabled(bool enable);

	void minimize();

	void restore();

	void maximize();

	void show(bool activate);

	void hide();

	void bring_to_front();

	/// \brief Capture/Release the mouse.
	void capture_mouse(bool capture);

	bool get_message(CL_XEvent &clan_event);

	/// \brief Set the window timer
	void set_timer(CL_TimerProvider *timer);

	/// \brief Stop the window timer
	void kill_timer(CL_TimerProvider *timer);

	void set_clipboard_text(const CL_StringRef &text);

	CL_String get_clipboard_text() const;

	/// \brief Check for window messages
	/** \return true when there is a message*/
	bool has_messages();

	void restore_videomode();

	void set_videomode(int width, int height, int bpp_local, int refresh_rate);

	void invalidate_rect(const CL_Rect &rect);

/// \}
/// \name Implementation
/// \{

private:
	void wait_mapped();

	bool check_timers(void);

	CL_Rect get_screen_position() const;

	void setup_swap_interval_pointers();

	void setup_joysticks();

	void process_timer_events();

	// Close the window, freeing any allocated resources
	void close_window();

	/// \brief Creates the initial window based on the window description.
	void create_new_window(XVisualInfo *visual, int screen_bpp, const CL_DisplayWindowDescription &desc);

	void received_keyboard_input(XKeyEvent &event);

	void received_mouse_input(XButtonEvent &event);

	void received_mouse_move(XMotionEvent &event);

	void clear_structurenotify_events();

	CL_Rect get_window_frame_size();

	bool get_xevent( XEvent &event ) const;

	bool get_xevent( XEvent &event, int event_type, bool block ) const;

	unsigned char *get_property(Window use_window, Atom prop, unsigned long *number_items_ptr, int *actual_format_ptr, Atom *actual_type_ptr) const;

	void process_selection_request(XEvent &event);

	/// \brief Handle to X11 window.
	Window window;

	Window window_last_focus;

	int window_last_revert_return;

	Colormap cmap;

	CL_String title;

	bool allow_resize;

	bool layered;

	bool alt_down, shift_down, ctrl_down;

	int bpp;

	bool fullscreen;

	int current_screen;

	CL_Point mouse_pos;

	/// \brief Attributes used to create window.
	XSetWindowAttributes attributes;

	/// \brief X11 Display handle
	Display *disp;

	Cursor system_cursor;
	Cursor hidden_cursor;
	Pixmap cursor_bitmap;

	/// \brief Attributes to switch between windowed and fullscreen
	int dotclock;
	XF86VidModeModeInfo old_mode;
	int old_x, old_y;

	CL_InputDeviceProvider_X11Keyboard *keyboard;

	CL_InputDeviceProvider_X11Mouse *mouse;

	CL_DisplayWindowSite *site;

	CL_DisplayMessageQueue_X11 *message_queue;

	std::list<CL_XTimer> timer_list;

	CL_Callback_v0 callback_on_resized;

	bool ignore_focus_events;

	static CL_Rect frame_size;
	static bool frame_size_set;
	bool window_has_caption;
	bool resize_enabled;

	Atom atom_CLIPBOARD;
	CL_String clipboard_current;
	bool clipboard_available;

	friend class CL_InputDeviceProvider_X11Keyboard;

	friend class CL_InputDeviceProvider_X11Mouse;
#ifdef HAVE_LINUX_JOYSTICK_H
	friend class CL_InputDeviceProvider_LinuxJoystick;
#endif
/// \}
};




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

#pragma once

#include <cmath>
#include <list>
#include <map>
#include "API/Core/Signals/signal.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/cursor.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "clipboard_x11.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

//#include <X11/extensions/XInput.h>
#include <sys/time.h>

namespace clan
{

class InputDeviceProvider_X11Keyboard;
class InputDeviceProvider_X11Mouse;
class InputDeviceProvider_LinuxJoystick;
class DisplayMessageQueue_X11;
class Rect;
class DisplayWindowSite;
class DisplayWindowDescription;
class CursorProvider_X11;

class X11Window
{
/// \name Construction
/// \{

public:
	X11Window();

	~X11Window();

/// \}
/// \name Attributes
/// \{

public:
	Rect get_geometry() const;
	Rect get_viewport() const;

	float get_ppi() const { return ppi; }
	float get_pixel_ratio() const { return pixel_ratio; }

	bool has_focus() const;
	bool is_minimized() const;
	bool is_maximized() const;
	bool is_visible() const;
	bool is_fullscreen() const { return fullscreen; }

	Size get_minimum_size(bool client_area) const;
	Size get_maximum_size(bool client_area) const;

	std::string get_title() const;

	DisplayWindowHandle get_handle() const { return handle; }
	::Display *get_display() const { return handle.display; }
	::Window get_window() const { return handle.window; }

	InputContext get_ic() { return ic; } // Important: do not return by reference, so the shared pointer exists if this window is destroyed

	std::function<void()> &func_on_resized() { return callback_on_resized; }
	std::function<bool(XButtonEvent &)> &func_on_clicked() { return callback_on_clicked; }

	bool is_clipboard_text_available() const;
	bool is_clipboard_image_available() const;

	std::string get_clipboard_text() const;
	PixelBuffer get_clipboard_image() const;

	unsigned char *get_property(::Window use_window, Atom prop, unsigned long *number_items_ptr, int *actual_format_ptr, Atom *actual_type_ptr) const;

	const std::vector<int> &get_window_socket_messages() const;

/// \}
/// \name Operations
/// \{

public:
	Point client_to_screen(const Point &client);
	Point screen_to_client(const Point &screen);

	void capture_mouse(bool capture);

	void request_repaint(const Rect &rect);

	void create(XVisualInfo *visual, DisplayWindowSite *site, const DisplayWindowDescription &description);

	void show_system_cursor();
	void hide_system_cursor();
	void set_cursor(CursorProvider_X11 *cursor);
	void set_cursor(enum StandardCursor type);

	void set_title(const std::string &new_title);
	void set_position(const Rect &pos, bool client_area);
	void set_size(int width, int height, bool client_area);
	void set_minimum_size(int width, int height, bool client_area);
	void set_maximum_size(int width, int height, bool client_area);

	void set_pixel_ratio(float ratio);

	void set_enabled(bool enable);
	void minimize();
	void restore();
	void maximize();
	void show(bool activate);
	void hide();
	void bring_to_front();

	void set_clipboard_text(const std::string &text);
	void set_clipboard_image(const PixelBuffer &buf);

	void set_large_icon(const PixelBuffer &image);
	void set_small_icon(const PixelBuffer &image);

	void process_message(XEvent &event, X11Window *mouse_capture_window);
	void process_message_complete();

	void get_keyboard_modifiers(bool &key_shift, bool &key_alt, bool &key_ctrl) const;
	Point get_mouse_position() const;

	void process_queued_events();
	bool process_window_sockets(bool peek_only);

/// \}
/// \name Implementation
/// \{

private:

	void process_window_resize(const Rect &new_rect);
	void calculate_window_frame_size();
	void map_window();
	void unmap_window();
	Rect get_screen_position() const;
	void setup_swap_interval_pointers();
	void setup_joysticks();
	void close_window();
	void received_keyboard_input(XKeyEvent &event);
	void received_mouse_input(XButtonEvent &event);
	void received_mouse_move(XMotionEvent &event);
	void clear_structurenotify_events();
	bool check_net_wm_state(Atom atom1, Atom atom2 = None) const;
	bool modify_net_wm_state(bool add, Atom atom1, Atom atom2 = None);
	InputDeviceProvider_X11Keyboard *get_keyboard() const;
	InputDeviceProvider_X11Mouse *get_mouse() const;

	InputContext ic;
	DisplayWindowHandle handle;
	Colormap color_map;
	bool minimized;
	bool maximized;
	bool restore_to_maximized;
	bool fullscreen;
	int current_screen;
	::Cursor system_cursor;
	::Cursor hidden_cursor;
	Pixmap cursor_bitmap;
	InputDevice keyboard, mouse;
	std::vector<InputDevice> joysticks;
	DisplayWindowSite *site;
	std::function<void()> callback_on_resized;
	std::function<bool(XButtonEvent &)> callback_on_clicked;
	Size minimum_size;
	Size maximum_size;
	std::string window_title;
	bool resize_enabled;
	Clipboard_X11 clipboard;
	std::vector<int> current_window_events;
	bool is_window_mapped;
	XSizeHints *size_hints;

	Atom wm_protocols;
	Atom wm_delete_window;
	Atom net_wm_ping;
	Atom wm_state;
	Atom motif_wm_hints;
	Atom kwm_win_decoration;
	Atom net_wm_state;
	Atom net_wm_state_maximized_vert;
	Atom net_wm_state_maximized_horz;
	Atom net_wm_state_hidden;
	Atom net_wm_state_fullscreen;
	Atom net_frame_extents;
	Atom win_hints;

	bool frame_size_calculated; // This is set when the window is mapped
	bool requested_size_contains_frame; // true when requested_current_window_client_area contains the window frame (because the frame size is not yet known)
	int frame_size_left;
	int frame_size_right;
	int frame_size_top;
	int frame_size_bottom;
	int border_width;

	Rect client_area; // Current window client area. Does not contain window frame.

	/**
	 * Contains `Rect`s obtained from X11 window resize events.
	 * Elements stored are not used. Cleared on process_message_complete (once
	 * all X11 events have been polled. Before it is cleared, a repaint of the
	 * entire viewport is requested if it is not empty.
	 */
	std::vector<Rect> resize_event_rects;

	/**
	 * Contains `Rect`s obtained from repaint request events.
	 * Elements stored are used to call site->sig_paint().
	 * Cleared on process_queued_events(), after process_message_complete().
	 */
	std::vector<Rect> repaint_event_rects;

	float ppi           = 96.0f;
	float pixel_ratio   = 0.0f;	// 0.0f = Unset

/// \}
};

}

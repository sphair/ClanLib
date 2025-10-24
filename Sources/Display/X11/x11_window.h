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

#pragma once


#include <list>
#include <map>
#include "API/Display/api_display.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Display/Window/display_window.h"
#include "API/Core/System/sharedptr.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Signals/callback_v0.h"
#include "API/Core/Signals/callback_1.h"
#include "clipboard_x11.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

//#include <X11/extensions/XInput.h>
#include <sys/time.h>

class CL_InputDeviceProvider_X11Keyboard;
class CL_InputDeviceProvider_X11Mouse;
class CL_InputDeviceProvider_LinuxJoystick;
class CL_DisplayMessageQueue_X11;
class CL_Rect;
class CL_DisplayWindowSite;
class CL_DisplayWindowDescription;
class CL_CursorProvider_X11;
class CL_SocketMessage_X11;

class CL_X11Window
{
/// \name Construction
/// \{

public:
	CL_X11Window();

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
	CL_Size get_minimum_size(bool client_area) const;
	CL_Size get_maximum_size(bool client_area) const;
	CL_String get_title() const;
	bool is_fullscreen() const {return fullscreen;}
	Display *get_display() const { return display; }
	Window get_window() const { return window; }
	CL_InputContext &get_ic() { return ic; }		// WARNING: return by reference, so the shared pointer exists if this window is destroyed
	CL_Callback_v0 &func_on_resized() { return callback_on_resized; }
	CL_Callback_1<bool, XButtonEvent &> &func_on_clicked() { return callback_on_clicked; }
	bool is_clipboard_text_available() const;
	bool is_clipboard_image_available() const;
	CL_String get_clipboard_text() const;
	CL_PixelBuffer get_clipboard_image() const;
	unsigned char *get_property(Window use_window, Atom prop, unsigned long *number_items_ptr, int *actual_format_ptr, Atom *actual_type_ptr) const;
	const std::vector<CL_SocketMessage_X11> &get_window_socket_messages() const;

/// \}
/// \name Operations
/// \{

public:
	CL_Point client_to_screen(const CL_Point &client);
	CL_Point screen_to_client(const CL_Point &screen);
	void create(XVisualInfo *visual, CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);
	void show_system_cursor();
	void set_large_icon(const CL_PixelBuffer &image);
	void set_small_icon(const CL_PixelBuffer &image);
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
	void capture_mouse(bool capture);
	void get_message(CL_X11Window *mouse_capture_window);
	bool has_messages();
	void request_repaint(const CL_Rect &rect);
	void set_clipboard_text(const CL_StringRef &text);
	void set_clipboard_image(const CL_PixelBuffer &buf);
	bool get_xevent( XEvent &event ) const;
	bool get_xevent( XEvent &event, int event_type ) const;
	void get_keyboard_modifiers(bool &key_shift, bool &key_alt, bool &key_ctrl) const;
	CL_Point get_mouse_position() const;
	void open_screen();

	// The library will be opened / closed by this class
	// Returns 0 if the library could not be found
	// Currently, only supports a single library
	void *dlopen(const char *filename, int flag); 

	void process_queued_events();

/// \}
/// \name Implementation
/// \{

private:

	void calculate_window_frame_size();
	void map_window();
	void unmap_window();
	CL_Rect get_screen_position() const;
	void setup_swap_interval_pointers();
	void setup_joysticks();
	void close_window();
	void received_keyboard_input(XKeyEvent &event);
	void received_mouse_input(XButtonEvent &event);
	void received_mouse_move(XMotionEvent &event);
	void clear_structurenotify_events();
	bool check_net_wm_state(Atom atom1, Atom atom2 = None) const;
	bool modify_net_wm_state(bool add, Atom atom1, Atom atom2 = None);
	CL_InputDeviceProvider_X11Keyboard *get_keyboard() const;
	CL_InputDeviceProvider_X11Mouse *get_mouse() const;

	CL_InputContext ic;
	Window window;
	Colormap color_map;
	bool minimized;
	bool maximized;
	bool restore_to_maximized;
	bool fullscreen;
	int current_screen;
	XSetWindowAttributes attributes;
	Display *display;
	Cursor system_cursor;
	Cursor hidden_cursor;
	Pixmap cursor_bitmap;
	CL_InputDevice keyboard, mouse;
	std::vector<CL_InputDevice> joysticks;
	CL_DisplayWindowSite *site;
	CL_Callback_v0 callback_on_resized;
	CL_Callback_1<bool, XButtonEvent &> callback_on_clicked;
	CL_Size minimum_size;
	CL_Size maximum_size;
	CL_String window_title;
	bool resize_enabled;
	void *dlopen_lib_handle;
	CL_Clipboard_X11 clipboard;
	std::vector<CL_SocketMessage_X11> current_window_events;
	std::vector<CL_Rect> last_repaint_rect;
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

	bool frame_size_calculated;	// This is set when the window is mapped
	bool requested_size_contains_frame;	// true when requested_current_window_client_area contains the window frame (because the frame size is not yet known)
	int frame_size_left;
	int frame_size_right;
	int frame_size_top;
	int frame_size_bottom;

	CL_Rect current_window_client_area;		// Set by ConfigureNotify event. Excludes window frame
	CL_Rect requested_current_window_client_area;	// Excludes window frame. Requested from ClanLib before ConfigureNotify is called

	const static int max_allowable_expose_events = 8;

	bool always_send_window_position_changed_event;
	bool always_send_window_size_changed_event;

/// \}
};

